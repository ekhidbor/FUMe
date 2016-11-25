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
#include <cstring>
#include <cassert>
#include <algorithm>
#include <string>
#include <array>

// boost
#include "boost/numeric/conversion/cast.hpp"

// local public
#include "diction.h"

// local private
#include "fume/file_object.h"
#include "fume/tx_stream.h"
#include "fume/file_tx_stream.h"
#include "fume/null_tx_stream.h"
#include "fume/file_rx_stream.h"
#include "fume/value_representation.h"
#include "fume/vrs/ob.h"
#include "fume/library_context.h"
#include "fume/transfer_syntax_to_uid.h"

using std::memcpy;
using std::strncpy;
using std::find_if;
using std::string;
using std::array;

using boost::numeric_cast;
using boost::bad_numeric_cast;

using fume::vrs::ob;

namespace fume
{

// This array is written after the preamble. It is not
// NULL-terminated and therefore should not be treated
// as a C-style string
static const array<char, 4> DICOM_PREFIX { 'D', 'I', 'C', 'M' };
static const char META_INFORMATION_VERSION[] = { 0, 1 };

static MC_STATUS buffer_callback( int           CBMsgFileItemID,
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

static void write_meta_information_version( file_object& obj )
{
    set_buf_parms buf_parms( META_INFORMATION_VERSION,
                             sizeof(META_INFORMATION_VERSION) );
    value_representation* element = obj.at( MC_ATT_FILE_META_INFORMATION_VERSION );
    assert( element != nullptr );

    if( element->is_null() == true )
    {
        set_func_parms func_parms = { buffer_callback, &buf_parms, -1, 0 };
        element->set( func_parms );
    }
}

file_object::file_object( int id, const char* filename, bool created_empty )
    : data_dictionary( id, created_empty ),
      m_filename( filename )
{
    m_preamble.fill( 0 );
}

file_object::~file_object()
{
}


MC_STATUS file_object::set_preamble( const void* preamble )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( preamble != nullptr )
    {
        memcpy( m_preamble.data(), preamble, m_preamble.size() );
        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS file_object::get_preamble( void* preamble ) const
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( preamble != nullptr )
    {
        memcpy( preamble, m_preamble.data(), m_preamble.size() );
        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS file_object::reset_filename( const char* filename )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( filename != nullptr )
    {
        m_filename = filename;
        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS file_object::get_filename( char* filename, int filesize ) const
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( filename != nullptr &&
            filesize > numeric_cast<int>( m_filename.size() ) )
        {
            // filesize must be greater than m_filename.size(), which
            // has a minimum value of 0. Therefore filesize is always
            // positive and the static_cast is safe
            strncpy( filename,
                     m_filename.c_str(),
                     static_cast<size_t>( filesize ) );
            ret = MC_NORMAL_COMPLETION;
        }
        else if( filename == nullptr )
        {
            ret = MC_NULL_POINTER_PARM;
        }
        else
        {
            ret = MC_BUFFER_TOO_SMALL;
        }
    }
    catch( const bad_numeric_cast& )
    {
        ret = MC_BUFFER_TOO_SMALL;
    }

    return ret;
}

MC_STATUS file_object::write( int               app_id,
                              int               alignment,
                              void*             user_info,
                              WriteFileCallback callback )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( callback != nullptr )
    {
        // TODO: fill in Group 2 attributes if not provided
        file_tx_stream stream( m_filename.c_str(), callback, user_info );

        ret = write_file( stream, app_id );
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

MC_STATUS file_object::fill_group_2_attributes()
{
    // This function should always succeed. If it does not then there is a
    // library error
    write_meta_information_version( *this );

    const string* implementation_version = nullptr;
    const string* implementation_uid = nullptr;

    assert( g_context != nullptr );
    g_context->get_string_config_value( IMPLEMENTATION_CLASS_UID,
                                        implementation_uid );
    g_context->get_string_config_value( IMPLEMENTATION_VERSION,
                                        implementation_version );

    if( has_tag( MC_ATT_IMPLEMENTATION_VERSION_NAME ) == false &&
        implementation_version != nullptr )
    {
        (*this)[MC_ATT_IMPLEMENTATION_VERSION_NAME].set
        (
            implementation_version->c_str()
        );
    }

    if( has_tag( MC_ATT_IMPLEMENTATION_CLASS_UID ) == false &&
        implementation_uid != nullptr )
    {
        (*this)[MC_ATT_IMPLEMENTATION_CLASS_UID].set
        (
            implementation_uid->c_str()
        );
    }

    return update_file_group_length();
}

MC_STATUS file_object::open( int              app_id,
                             void*            user_info,
                             ReadFileCallback callback )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( callback != nullptr )
    {
        // There is not currently an open stream. So just read the entire
        // file in
        if( m_rx_stream == nullptr )
        {
            // We're reading the entire file in, so no need to save
            // to the member variable
            file_rx_stream stream( m_filename, callback, user_info );
            ret = read_file_header( stream, app_id );
            if( ret == MC_NORMAL_COMPLETION )
            {
                TRANSFER_SYNTAX syntax = INVALID_TRANSFER_SYNTAX;
                ret = get_transfer_syntax( syntax );
                if( ret == MC_NORMAL_COMPLETION )
                {
                    ret = read_values( stream, syntax, app_id );
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
            TRANSFER_SYNTAX syntax = INVALID_TRANSFER_SYNTAX;
            ret = get_transfer_syntax( syntax );
            if( ret == MC_NORMAL_COMPLETION )
            {
                ret = read_values( *m_rx_stream, syntax, app_id );
            }
            else
            {
                // Do nothing. Will return error
            }
        }
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS file_object::write_file( tx_stream& stream, int app_id )
{
    // Fill in required Group 2 attribute data
    MC_STATUS ret = fill_group_2_attributes();
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = stream.write( m_preamble.data(), m_preamble.size() );
        if( ret == MC_NORMAL_COMPLETION )
        {
            // Write the 4-character prefix
            ret = stream.write( DICOM_PREFIX.data(), DICOM_PREFIX.size() );
            if( ret == MC_NORMAL_COMPLETION )
            {
                ret = write_values( stream, app_id );
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

MC_STATUS file_object::read_file_header( rx_stream& stream, int app_id )
{
    MC_STATUS ret = stream.read( m_preamble.data(), m_preamble.size() );
    if( ret == MC_NORMAL_COMPLETION )
    {
        // Read the 4-character prefix
        array<char, 4> prefix;
        ret = stream.read( prefix.data(), prefix.size() );
        if( ret == MC_NORMAL_COMPLETION )
        {
            if( prefix == DICOM_PREFIX )
            {
                // Read the Group 2 elements in explicit little endian
                ret = read_values_upto( stream,
                                        EXPLICIT_LITTLE_ENDIAN,
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

void file_object::empty_file()
{
    set_all_empty();
    m_preamble.fill( 0u );
}

MC_STATUS file_object::set_transfer_syntax( TRANSFER_SYNTAX syntax )
{
    char syntax_uid[65] = { '\0' };
    MC_STATUS ret = get_transfer_syntax_from_enum
                    (
                        syntax,
                        syntax_uid,
                        static_cast<int>( sizeof(syntax_uid) )
                    );
    if( ret == MC_NORMAL_COMPLETION )
    {
        value_representation& syntax_vr( (*this)[MC_ATT_TRANSFER_SYNTAX_UID] );
        ret = syntax_vr.set( syntax_uid );
    }
    else
    {
        // Do nothing. Will return error from get_transfer_syntax_from_enum
    }

    return ret;
}

MC_STATUS file_object::get_transfer_syntax( TRANSFER_SYNTAX& syntax )
{
    char syntax_uid[65] = { '\0' };

    get_string_parms syntax_uid_parms =
    {
        syntax_uid,
        static_cast<MC_size_t>( sizeof(syntax_uid) )
    };

    MC_STATUS ret = MC_CANNOT_COMPLY;

    value_representation* syntax_vr = at( MC_ATT_TRANSFER_SYNTAX_UID );
    if( syntax_vr != nullptr )
    {
        ret = syntax_vr->get( syntax_uid_parms );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = get_enum_from_transfer_syntax( syntax_uid, syntax );
        }
        else
        {
            // Do nothing. Will return error from get
        }
    }
    else
    {
        ret = MC_INVALID_TAG;
    }

    return ret;
}

MC_STATUS file_object::write_values( tx_stream& stream, int app_id )
{
    // Get iterators for all Group 2 attributes.
    const const_value_range& group2_range( get_value_range( 0x00020000u,
                                                            0x0002FFFFu ) );

    TRANSFER_SYNTAX syntax = INVALID_TRANSFER_SYNTAX;
    MC_STATUS ret = get_transfer_syntax( syntax );
    if( ret == MC_NORMAL_COMPLETION )
    {
        // Group 2 attributes are always written in Explicit Little Endian
        // transfer syntax
        MC_STATUS ret = write_values( stream,
                                      EXPLICIT_LITTLE_ENDIAN,
                                      app_id,
                                      group2_range.begin(),
                                      group2_range.end() );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = write_values( stream,
                                syntax,
                                app_id,
                                group2_range.end(),
                                end() );
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

MC_STATUS file_object::update_file_group_length()
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    null_tx_stream stream;
    // Get iterators for all Group 2 attributes except group length
    const const_value_range& group2_range( get_value_range( 0x00020001,
                                                            0x0002FFFFu ) );

    ret = write_values( stream,
                        EXPLICIT_LITTLE_ENDIAN,
                        -1,
                        group2_range.begin(),
                        group2_range.end() );
    if( ret == MC_NORMAL_COMPLETION )
    {
        (*this)[MC_ATT_FILE_META_INFORMATION_GROUP_LENGTH].set
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

}
