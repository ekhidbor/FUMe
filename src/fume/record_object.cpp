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

// local public
#include "diction.h"

// local private
#include "fume/library_context.h"
#include "fume/record_object.h"
#include "fume/tx_stream.h"
#include "fume/value_representation.h"

namespace fume
{

record_object::record_object( int         dicomdir_file_id,
                              int         parent_id,
                              int         id,
                              const char* record_type,
                              bool        created_empty )
    : item_object( id, created_empty ),
      m_dicomdir_file_id( dicomdir_file_id ),
      m_parent_id( parent_id ),
      m_next_record( -1 ),
      m_child_record( -1 )
{
    (*this)[MC_ATT_OFFSET_OF_THE_NEXT_DIRECTORY_RECORD].set
    (
        static_cast<unsigned long>( 0x00000000u )
    );
    (*this)[MC_ATT_OFFSET_OF_REFERENCED_LOWER_LEVEL_DIRECTORY_ENTITY].set
    (
        static_cast<unsigned long>( 0x00000000u )
    );
    (*this)[MC_ATT_RECORD_IN_USE_FLAG].set( 0xFFFFu );
    if( record_type != nullptr )
    {
        (*this)[MC_ATT_DIRECTORY_RECORD_TYPE].set( record_type );
    }
}

MC_STATUS record_object::to_stream( tx_stream&      stream,
                                    TRANSFER_SYNTAX syntax ) const
{
    // Save off the byte offset of this record
    m_offset = stream.bytes_written();
    return item_object::to_stream( stream, syntax );
}

MC_STATUS record_object::get_record_type( MC_DIR_RECORD_TYPE& type ) const
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    char record_type_str[64] = { '\0' };
    get_string_parms parms = { record_type_str, sizeof(record_type_str) };

    const value_representation* record_type_val = at( MC_ATT_DIRECTORY_RECORD_TYPE );
    if( record_type_val != nullptr )
    {
        ret = record_type_val->get( parms );
        if( ret == MC_NORMAL_COMPLETION )
        {
            assert( g_context != nullptr );
            ret = g_context->get_enum_from_record_type( record_type_str, type );
        }
        else
        {
            type = MC_REC_TYPE_UNKNOWN;
            ret = MC_NORMAL_COMPLETION;
        }
    }
    else
    {
        type = MC_REC_TYPE_UNKNOWN;
        ret = MC_NORMAL_COMPLETION;
    }

    return ret;
}

}
