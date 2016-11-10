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

// boost
#include "boost/numeric/conversion/cast.hpp"

// local public
#include "mcstatus.h"
#include "mergecom.h"

// local private
#include "fume/library_context.h"
#include "fume/application.h"

using boost::numeric_cast;
using boost::bad_numeric_cast;

using fume::g_context;
using fume::application;

MC_STATUS MC_Release_Callback_Function ( int ApplicationID, unsigned long Tag )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context != nullptr )
        {
            application* app = g_context->get_application( ApplicationID );
            if( app != nullptr )
            {
                ret = app->release_callback_function( numeric_cast<uint32_t>( Tag ) );
            }
            else
            {
                ret = MC_INVALID_APPLICATION_ID;
            }
        }
        else
        {
            ret = MC_LIBRARY_NOT_INITIALIZED;
        }
    }
    catch( const bad_numeric_cast& )
    {
        ret = MC_INVALID_TAG;
    }
    catch( ... )
    {
        ret = MC_SYSTEM_ERROR;
    }

    return ret;
}
