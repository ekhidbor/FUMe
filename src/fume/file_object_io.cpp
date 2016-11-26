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
#include <cstdint>
#include <array>
#include <string>

// local public
#include "mcstatus.h"
#include "diction.h"

// local private
#include "fume/file_object.h"
#include "fume/tx_stream.h"
#include "fume/rx_stream.h"
#include "fume/value_representation.h"
#include "fume/null_tx_stream.h"
#include "fume/file_tx_stream.h"
#include "fume/file_rx_stream.h"
#include "fume/library_context.h"
#include "fume/data_dictionary_io.h"
#include "fume/file_object_io.h"

using std::array;
using std::string;

namespace fume
{

static void write_meta_information_version( data_dictionary& obj );

static MC_STATUS fill_group_2_attributes( data_dictionary& dict );

static MC_STATUS update_file_group_length( data_dictionary& dict );

static MC_STATUS write_file_values( tx_stream&       stream,
                                    data_dictionary& dict,
                                    int              app_id );

static MC_STATUS read_file_header( rx_stream&   stream,
                                   file_object& file,
                                   int          app_id );

// This array is written after the preamble. It is not
// NULL-terminated and therefore should not be treated
// as a C-style string
static const array<char, 4> DICOM_PREFIX { { 'D', 'I', 'C', 'M' } };
static const char META_INFORMATION_VERSION[] = { 0, 1 };

MC_STATUS write_file( file_object&      file,
                      int               alignment,
                      void*             user_info,
                      WriteFileCallback callback )
{
    return write_file( file,
                       file.application_id(),
                       alignment,
                       user_info,
                       callback );
}

MC_STATUS write_file( file_object&      file,
                      int               app_id,
                      int               alignment,
                      void*             user_info,
                      WriteFileCallback callback )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( callback != nullptr )
    {
        const string& filename( file.get_filename() );

        file_tx_stream stream( filename.c_str(), callback, user_info );

        ret = write_file( stream, file, app_id );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = stream.finalize();
        }
        else
        {
            // Call finalize in case the callback function needs
            // to clean itself up.
            // TODO: figure out if this is consistent with the
            // reference implementation
            (void)stream.finalize();
        }
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS write_file( tx_stream& stream, file_object& file, int app_id )
{
    // Fill in required Group 2 attribute data
    MC_STATUS ret = fill_group_2_attributes( file );
    if( ret == MC_NORMAL_COMPLETION )
    {
        uint8_t preamble[128];
        // This function only fails if the pointer is NULL
        (void)file.get_preamble( preamble );

        ret = stream.write( preamble, sizeof(preamble) );
        if( ret == MC_NORMAL_COMPLETION )
        {
            // Write the 4-character prefix
            ret = stream.write( DICOM_PREFIX.data(), DICOM_PREFIX.size() );
            if( ret == MC_NORMAL_COMPLETION )
            {
                ret = write_file_values( stream, file, app_id );
            }
            else
            {
                // Do nothing. Will return error from stream::write
            }
        }
        else
        {
            // Do nothing. Will return error from stream::write
        }
    }
    else
    {
        // Do nothing. WIll return error from update_file_group_length
    }

    return ret;
}

MC_STATUS buffer_callback( int           CBMsgFileItemID,
                           unsigned long Cbtag,
                           int           CbisFirst,
                           void*         CbuserInfo,
                           int*          CbdataSizePtr,
                           void**        CbdataBufferPtr,
                           int*          CbisLastPtr )
{
    assert( CbdataBufferPtr != nullptr );
    assert( CbuserInfo != nullptr );
    assert( CbisLastPtr != nullptr );

    set_buf_parms& parms( *static_cast<set_buf_parms*>( CbuserInfo ) );

    *CbdataBufferPtr = const_cast<void*>( parms.first );
    *CbdataSizePtr = parms.second;
    *CbisLastPtr = 1;

    return MC_NORMAL_COMPLETION;
}

void write_meta_information_version( data_dictionary& dict )
{
    set_buf_parms buf_parms( META_INFORMATION_VERSION,
                             sizeof(META_INFORMATION_VERSION) );
    value_representation& element( dict[MC_ATT_FILE_META_INFORMATION_VERSION] );

    if( element.is_null() == true )
    {
        set_func_parms func_parms = { buffer_callback, &buf_parms, -1, 0 };
        element.set( func_parms );
    }
}

MC_STATUS fill_group_2_attributes( data_dictionary& dict )
{
    // This function should always succeed. If it does not then there is a
    // library error
    write_meta_information_version( dict );

    const string* implementation_version = nullptr;
    const string* implementation_uid = nullptr;

    assert( g_context != nullptr );
    g_context->get_string_config_value( IMPLEMENTATION_CLASS_UID,
                                        implementation_uid );
    g_context->get_string_config_value( IMPLEMENTATION_VERSION,
                                        implementation_version );

    if( dict.has_tag( MC_ATT_IMPLEMENTATION_VERSION_NAME ) == false &&
        implementation_version != nullptr )
    {
        dict[MC_ATT_IMPLEMENTATION_VERSION_NAME].set
        (
            implementation_version->c_str()
        );
    }

    if( dict.has_tag( MC_ATT_IMPLEMENTATION_CLASS_UID ) == false &&
        implementation_uid != nullptr )
    {
        dict[MC_ATT_IMPLEMENTATION_CLASS_UID].set
        (
            implementation_uid->c_str()
        );
    }

    return update_file_group_length( dict );
}

MC_STATUS write_file_values( tx_stream&       stream,
                             data_dictionary& dict,
                             int              app_id )
{
    TRANSFER_SYNTAX syntax = INVALID_TRANSFER_SYNTAX;
    MC_STATUS ret = dict.get_transfer_syntax( syntax );
    if( ret == MC_NORMAL_COMPLETION )
    {
        // Group 2 attributes are always written in Explicit Little Endian
        // transfer syntax
        MC_STATUS ret = write_values( stream,
                                      EXPLICIT_LITTLE_ENDIAN,
                                      dict,
                                      app_id,
                                      0x00020000u,
                                      0x0002FFFFu );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = write_values( stream,
                                syntax,
                                dict,
                                app_id,
                                0x00030000u,
                                0xFFFFFFFFu );
        }
        else
        {
            // Do nothing. Will return error from write_values
        }
    }
    else
    {
        // Do nothing. Will return error from get_transfer_syntax
    }

    return ret;
}

MC_STATUS update_file_group_length( data_dictionary& dict )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    null_tx_stream stream;

    // Compute length for all Group 2 attributes except group length
    ret = write_values( stream,
                        EXPLICIT_LITTLE_ENDIAN,
                        dict,
                        -1,
                        0x00020001u,
                        0x0002FFFFu );
    if( ret == MC_NORMAL_COMPLETION )
    {
        dict[MC_ATT_FILE_META_INFORMATION_GROUP_LENGTH].set
        (
            static_cast<uint32_t>( stream.tell_write() )
        );
    }
    else
    {
        // Do nothing. Will return error from write_values
    }

    return ret;
}

MC_STATUS read_file_header( rx_stream& stream, file_object& file, int app_id )
{
    uint8_t preamble[128];
    MC_STATUS ret = stream.read( preamble, sizeof(preamble) );
    if( ret == MC_NORMAL_COMPLETION )
    {
        // Read the 4-character prefix
        array<char, 4> prefix;
        ret = stream.read( prefix.data(), prefix.size() );
        if( ret == MC_NORMAL_COMPLETION )
        {
            if( prefix == DICOM_PREFIX )
            {
                // Clear the file object
                file.empty_file();
                // This function only fails if the pointer is NULL
                (void)file.set_preamble( preamble );

                // Read the Group 2 elements in explicit little endian
                ret = read_values_upto( stream,
                                        EXPLICIT_LITTLE_ENDIAN,
                                        file,
                                        app_id,
                                        0x0002FFFFu );
            }
            else
            {
                ret = MC_INVALID_FILE;
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

MC_STATUS open_file( file_object&     file,
                     int              app_id,
                     void*            user_info,
                     ReadFileCallback callback )
{
    uint64_t offset = 0;

    return open_file_upto( file, app_id, 0xFFFFFFFFu, offset, user_info, callback );
}

MC_STATUS open_file_upto( file_object&     file,
                          int              app_id,
                          uint32_t         end_tag,
                          uint64_t&        offset,
                          void*            user_info,
                          ReadFileCallback callback )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( callback != nullptr )
    {
        // We're reading the entire file in, so no need to save
        // to the member variable
        file_rx_stream stream( file.get_filename(), callback, user_info );
        ret = read_file_header( stream, file, app_id );
        if( ret == MC_NORMAL_COMPLETION )
        {
            TRANSFER_SYNTAX syntax = INVALID_TRANSFER_SYNTAX;
            ret = file.get_transfer_syntax( syntax );
            if( ret == MC_NORMAL_COMPLETION )
            {
                ret = read_values_upto( stream, syntax, file, app_id, end_tag );
                if( ret == MC_NORMAL_COMPLETION )
                {
                    offset = stream.tell_read();
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
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

}