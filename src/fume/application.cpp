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

// local public
#include "mc3msg.h"

// local private
#include "fume/application.h"
#include "fume/library_context.h"

using boost::numeric_cast;
using boost::bad_numeric_cast;

namespace fume
{

MC_STATUS application::register_callback_function( uint32_t         tag,
                                                   CallbackFunction function,
                                                   void*            function_context )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( function != nullptr )
    {
        assert( g_context != nullptr );

        MC_VR tag_vr = UNKNOWN_VR;
        ret = g_context->get_vr_type( tag, nullptr, tag_vr );
        if( ret == MC_NORMAL_COMPLETION )
        {
            if( tag_vr == OB || tag_vr == OW ||
                tag_vr == OL || tag_vr == OF || tag_vr == OD )
            {
                m_callbacks[tag] = callback_parms_t( function, function_context );
                ret = MC_NORMAL_COMPLETION;
            }
            else
            {
                ret = MC_INVALID_VALUE_FOR_VR;
            }
        }
        else
        {
            // Do nothing. Return error from get_vr_type
        }
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS application::release_callback_function( uint32_t tag )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( m_callbacks.erase( tag ) != 0 )
    {
        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_INVALID_TAG;
    }

    return ret;
}

callback_parms_t application::get_callback_function( uint32_t tag ) const
{
    callback_parms_t ret( nullptr, nullptr );

    const callback_map_t::const_iterator itr = m_callbacks.find( tag );
    if( itr != m_callbacks.cend() )
    {
        ret = itr->second;
    }
    else
    {
        ret.first = nullptr;
        ret.second = nullptr;
    }

    return ret;
}

}
