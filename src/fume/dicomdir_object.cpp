/**
 * This file is a part of the FUMe project.
 *
 * To the extent possible under law, the person who associated CC0 with
 * FUMe has waived all copyright and related or neighboring rights
 * to FUMe.
 *
 * You should have received a copy of the CC0 legalcode along with this
 * work.  If not, see http://creativecommons.org/publicdomain/zero/1.0/.
 */

// std
#include <cassert>
#include <cstdio>
#include <unordered_map>

// local public
#include "mc3msg.h"
#include "mc3media.h"
#include "diction.h"

// local private
#include "fume/library_context.h"
#include "fume/dicomdir_object.h"
#include "fume/null_tx_stream.h"
#include "fume/value_representation.h"
#include "fume/record_object.h"
#include "fume/data_dictionary_search.h"
#include "fume/file_object_io.h"

using std::unordered_map;

namespace fume
{

typedef unordered_map<int, uint32_t> offset_map_t;

struct root_record_offet_context
{
    const dicomdir_object& dicomdir;
    const offset_map_t&    offsets;
};

static MC_STATUS write_using_stdio( char* Cbfilename,
                                    void* CbuserInfo,
                                    int   CbdataSize,
                                    void* CbdataBuffer,
                                    int   CbisFirst,
                                    int   CbisLast )
{
    assert( CbuserInfo != nullptr );
    FILE** f_ptr = static_cast<FILE**>( CbuserInfo );

    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( CbisFirst != 0 )
    {
        *f_ptr = fopen( Cbfilename, "wb" );
        if( *f_ptr != nullptr )
        {
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            ret = MC_CANNOT_COMPLY;
        }
    }
    else
    {
        // Do nothing. File pointer already initialized
    }

    if( *f_ptr != nullptr )
    {
        if( CbdataSize > 0 && CbdataBuffer != nullptr )
        {
            if( fwrite( CbdataBuffer, CbdataSize, 1, *f_ptr ) == 1u )
            {
                ret = MC_NORMAL_COMPLETION;
            }
            else
            {
                ret = MC_CANNOT_COMPLY;
            }
        }
        else
        {
            // Nothing to write. Not an error
            ret = MC_NORMAL_COMPLETION;
        }
    }
    else
    {
        // No file pointer. Error
        ret = MC_CANNOT_COMPLY;
    }

    if( CbisLast != 0 && *f_ptr != nullptr )
    {
        fflush( *f_ptr );
        fclose( *f_ptr );
        *f_ptr = nullptr;
    }
    else
    {
        // Do nothing
    }

    return ret;
}

static MC_STATUS write_record_offset( int                 parent_id,
                                      uint32_t            tag,
                                      int                 record_id_to_write,
                                      const offset_map_t& offsets )
{
    const offset_map_t::const_iterator itr = offsets.find( record_id_to_write );
    const bool has_record = record_id_to_write > 0 && itr != offsets.cend();
    const uint32_t offset = has_record ? itr->second : 0u;

    return MC_Set_Value_From_ULongInt( parent_id, tag, offset );
}

static MC_TRAVERSAL_STATUS update_record_offsets( int   CurrentRecID,
                                                  void* UserData )
{
    MC_TRAVERSAL_STATUS ret = MC_TS_CONTINUE;

    assert( UserData != nullptr );
    const offset_map_t& offset_map( *static_cast<offset_map_t*>( UserData ) );
    record_object* record =
        dynamic_cast<record_object*>( g_context->get_object( CurrentRecID ) );
    if( record != nullptr )
    {
        MC_STATUS stat = write_record_offset
                         (
                             CurrentRecID,
                             MC_ATT_OFFSET_OF_THE_NEXT_DIRECTORY_RECORD,
                             record->get_next_record(),
                             offset_map
                         );
        if( stat == MC_NORMAL_COMPLETION )
        {
            stat = write_record_offset
                   (
                       CurrentRecID,
                       MC_ATT_OFFSET_OF_REFERENCED_LOWER_LEVEL_DIRECTORY_ENTITY,
                       record->get_child_record(),
                       offset_map
                   );
            ret = stat == MC_NORMAL_COMPLETION ? MC_TS_CONTINUE : MC_TS_ERROR;
        }
        else
        {
            ret = MC_TS_ERROR;
        }
    }
    else
    {
        ret = MC_TS_ERROR;
    }

    return ret;
}

static MC_TRAVERSAL_STATUS update_root_offsets( int   CurrentRecID,
                                                void* UserData )
{
    assert( UserData != nullptr );

    const root_record_offet_context& context
    (
        *static_cast<root_record_offet_context*>( UserData )
    );
    const bool is_first = context.dicomdir.get_child_record() == CurrentRecID;

    MC_STATUS stat = MC_CANNOT_COMPLY;
    if( is_first == true )
    {
        stat = write_record_offset
               (
                   context.dicomdir.id(),
                   MC_ATT_OFFSET_OF_THE_FIRST_DIRECTORY_RECORD_OF_THE_ROOT_DIRECTORY_ENTITY,
                   CurrentRecID,
                   context.offsets
               );
    }
    else
    {
        // Not first record. Write to last record item but not first
        stat = MC_NORMAL_COMPLETION;
    }

    if( stat == MC_NORMAL_COMPLETION )
    {
        stat = write_record_offset
               (
                   context.dicomdir.id(),
                   MC_ATT_OFFSET_OF_THE_LAST_DIRECTORY_RECORD_OF_THE_ROOT_DIRECTORY_ENTITY,
                   CurrentRecID,
                   context.offsets
               );
    }
    else
    {
        // Do nothing. Return error
    }

    // Either traverse the next item or report an error
    return stat == MC_NORMAL_COMPLETION ? MC_TS_STOP_LOWER : MC_TS_ERROR;
}

static MC_STATUS update_offset_values( dicomdir_object&    dicomdir,
                                       const offset_map_t& offsets )
{
    root_record_offet_context root_context = { dicomdir, offsets };
    // Update the record offsets pointing to the root records
    MC_STATUS ret = MC_DDH_Traverse_Records( dicomdir.id(),
                                             static_cast<void*>( &root_context ),
                                             update_root_offsets );

    if( ret == MC_NORMAL_COMPLETION )
    {
        // Update the record offsets in the records themselves
        ret = MC_DDH_Traverse_Records( dicomdir.id(),
                                       const_cast<offset_map_t*>( &offsets ),
                                       update_record_offsets );
    }

    return ret;
}

static MC_STATUS get_record_offsets( value_representation& record_sequence,
                                     offset_map_t&         record_offsets )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;
    offset_map_t tmp_offsets;

    int cur_record_id = 0;
    if( record_sequence.is_null() == false )
    {
        ret = record_sequence.get( cur_record_id );
        while( ret == MC_NORMAL_COMPLETION )
        {
            assert( g_context != nullptr );
            const record_object* record =
                dynamic_cast<record_object*>( g_context->get_object( cur_record_id ) );
            if( record != nullptr )
            {
                tmp_offsets[cur_record_id] = record->get_offset();
                ret = record_sequence.get_next( cur_record_id );
            }
            else
            {
                ret = MC_INVALID_RECORD_ID;
            }
        }

        if( ret == MC_NO_MORE_VALUES )
        {
            // Only update output upon successful completion
            record_offsets.swap( tmp_offsets );
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            // Do nothing. Will return error
        }
    }
    else
    {
        record_offsets.swap( tmp_offsets );
        ret = MC_NORMAL_COMPLETION;
    }

    return ret;
}

dicomdir_object::dicomdir_object( int                    id,
                                  const char*            filename,
                                  data_dictionary* source,
                                  bool                   created_empty )
    : file_object( id, filename, created_empty )
{
    if( source != nullptr )
    {
        // Just copy tags from groups 2 through 4
        // TODO: remove when we have message templates based on IOD type
        // Do before we assign DICOMDIR values to prevent these values from
        // overwriting ours
        copy_values( *this, *source, 0x00020000u, 0x0004FFFFu );
    }

    (*this)[MC_ATT_OFFSET_OF_THE_FIRST_DIRECTORY_RECORD_OF_THE_ROOT_DIRECTORY_ENTITY].set( 0u );
    (*this)[MC_ATT_OFFSET_OF_THE_LAST_DIRECTORY_RECORD_OF_THE_ROOT_DIRECTORY_ENTITY].set( 0u );
    (*this)[MC_ATT_FILE_SET_CONSISTENCY_FLAG].set( 0u );
    (*this)[MC_ATT_MEDIA_STORAGE_SOP_CLASS_UID].set( "1.2.840.10008.1.3.10" );

    set_transfer_syntax( EXPLICIT_LITTLE_ENDIAN );
}

MC_STATUS dicomdir_object::update()
{
    null_tx_stream null_stream;

    // Update all offset values using the NULL stream.
    // Note we don't use the callback function. There shouldn't
    // be anything in a DICOMDIR which would use a callback
    MC_STATUS ret = write_file( null_stream, *this, -1 );
    if( ret == MC_NORMAL_COMPLETION )
    {
        offset_map_t record_offsets;
        ret = get_record_offsets( (*this)[MC_ATT_DIRECTORY_RECORD_SEQUENCE],
                                  record_offsets );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = update_offset_values( *this, record_offsets );
            if( ret == MC_NORMAL_COMPLETION )
            {
                FILE* f = nullptr;
                ret = write_file( *this,
                                  0,
                                  static_cast<void*>( &f ),
                                  write_using_stdio );
            }
            else
            {
                // Do nothing. Will return error
            }
        }
        else
        {
            // Do nothing. Will return error
        }
    }
    else
    {
        // Do nothing. Will return error
    }

    return ret;
}

}
