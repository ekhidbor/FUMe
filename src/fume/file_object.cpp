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
#include <string>

// boost
#include "boost/numeric/conversion/cast.hpp"

// local public
#include "diction.h"

// local private
#include "fume/file_object.h"
#include "fume/file_object_io.h"
//#include "fume/data_dictionary_io.h"
#include "fume/value_representation.h"
#include "fume/transfer_syntax_to_uid.h"

using std::memcpy;
using std::strncpy;
using std::string;

using boost::numeric_cast;
using boost::bad_numeric_cast;

namespace fume
{

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

void file_object::empty_file()
{
    clear();
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

}
