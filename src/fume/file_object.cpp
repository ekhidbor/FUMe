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

// boost
#include "boost/numeric/conversion/cast.hpp"

// local public
#include "diction.h"

// local private
#include "fume/file_object.h"
#include "fume/tx_stream.h"
#include "fume/file_tx_stream.h"
#include "fume/null_tx_stream.h"
#include "fume/value_representation.h"
#include "fume/vrs/ob.h"
#include "fume/library_context.h"

using std::memcpy;
using std::strncpy;
using std::find_if;
using std::string;

using boost::numeric_cast;
using boost::bad_numeric_cast;

using fume::vrs::ob;

namespace fume
{

// This array is written after the preamble. It is not
// NULL-terminated and therefore should not be treated
// as a C-style string
static const char DICOM_PREFIX[] = { 'D', 'I', 'C', 'M' };
static const char META_INFORMATION_VERSION[] = { 0, 1 };

file_object::file_object( int id, const char* filename, bool created_empty )
    : data_dictionary( id, created_empty ),
      m_filename( filename )
{
    m_preamble.fill( 0 );
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
    ob& meta_version
    (
        dynamic_cast<ob&>((*this)[MC_ATT_FILE_META_INFORMATION_VERSION])
    );

    if( meta_version.is_null() == true )
    {
        meta_version.write( META_INFORMATION_VERSION,
                            sizeof(META_INFORMATION_VERSION) );
    }

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

MC_STATUS file_object::open( void*            user_info,
                             ReadFileCallback callback )
{
    // TODO: implement
    return MC_CANNOT_COMPLY;
}

MC_STATUS file_object::write_file( tx_stream& stream, int app_id )
{
    // Fill in required Group 2 attribute data
    MC_STATUS ret = fill_group_2_attributes();
    if( ret == MC_NORMAL_COMPLETION )
    {
        stream.write( m_preamble.data(), m_preamble.size() );
        if( ret == MC_NORMAL_COMPLETION )
        {
            // Write the 4-character prefix
            ret = stream.write( DICOM_PREFIX, sizeof(DICOM_PREFIX) );
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

void file_object::empty_file()
{
    set_all_empty();
    m_preamble.fill( 0u );
}

MC_STATUS file_object::set_transfer_syntax( TRANSFER_SYNTAX syntax )
{
    // Should not be possible
    assert( g_context != nullptr );

    char syntax_uid[65] = { '\0' };
    MC_STATUS ret = g_context->get_transfer_syntax_from_enum
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

MC_STATUS file_object::get_transfer_syntax( TRANSFER_SYNTAX& syntax ) const
{
    // Should not be possible
    assert( g_context != nullptr );

    char syntax_uid[65] = { '\0' };

    get_string_parms syntax_uid_parms =
    {
        syntax_uid,
        static_cast<MC_size_t>( sizeof(syntax_uid) )
    };

    MC_STATUS ret = MC_CANNOT_COMPLY;

    const value_representation* syntax_vr = at( MC_ATT_TRANSFER_SYNTAX_UID );
    if( syntax_vr != nullptr )
    {
        ret = syntax_vr->get( syntax_uid_parms );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = g_context->get_enum_from_transfer_syntax( syntax_uid, syntax );
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

MC_STATUS file_object::write_values( tx_stream& stream, int app_id ) const
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
            stream.bytes_written()
        );
    }
    else
    {
        // Do nothing. Will return error from write_values
    }

    return ret;
}

}
