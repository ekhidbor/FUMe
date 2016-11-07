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
#include <utility>

// boost
#include "boost/numeric/conversion/cast.hpp"

// local private
#include "fume/application.h"

using boost::numeric_cast;
using boost::bad_numeric_cast;

namespace fume
{

MC_STATUS application::register_callback_function( unsigned long    tag,
                                                   CallbackFunction function,
                                                   void*            function_context )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( function != nullptr )
        {
            m_callbacks[numeric_cast<uint32_t>( tag )] =
                callback_parms_t( function, function_context );
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            ret = MC_NULL_POINTER_PARM;
        }
    }
    catch( const bad_numeric_cast& )
    {
        ret = MC_INVALID_TAG;
    }

    return ret;
}

MC_STATUS application::release_callback_function( unsigned long tag )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( m_callbacks.erase( numeric_cast<uint32_t>( tag ) ) != 0 )
        {
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            ret = MC_INVALID_TAG;
        }
    }
    catch( const bad_numeric_cast& )
    {
        ret = MC_INVALID_TAG;
    }

    return ret;
}

callback_parms_t application::get_callback_function( unsigned long tag ) const
{
    callback_parms_t ret( nullptr, nullptr );

    try
    {
        const callback_map_t::const_iterator itr =
            m_callbacks.find( numeric_cast<uint32_t>( tag ) );
        if( itr != m_callbacks.cend() )
        {
            ret = itr->second;
        }
        else
        {
            ret.first = nullptr;
            ret.second = nullptr;
        }
    }
    catch( const bad_numeric_cast& )
    {
        ret.first = nullptr;
        ret.second = nullptr;
    }

    return ret;
}

}
