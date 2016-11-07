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

// local public
#include "mcstatus.h"
#include "mergecom.h"

// local private
#include "fume/library_context.h"

using fume::g_context;

MC_STATUS MC_Release_Application( int* ApplicationID )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context     != nullptr &&
            ApplicationID != nullptr )
        {
            ret = g_context->release_application( *ApplicationID );
            if( ret == MC_NORMAL_COMPLETION )
            {
                *ApplicationID = -1;
            }
            else
            {
                // Do nothing. Will return error from release_application
            }
        }
        else if( g_context == nullptr )
        {
            ret = MC_LIBRARY_NOT_INITIALIZED;
        }
        else
        {
            ret = MC_NULL_POINTER_PARM;
        }
    }
    catch( ... )
    {
        ret = MC_SYSTEM_ERROR;
    }

    return ret;
}
