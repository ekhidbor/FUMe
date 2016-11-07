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

MC_STATUS MC_Register_Application( int* ApplicationID, char* AEtitle )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context     != nullptr &&
            ApplicationID != nullptr )
        {
            const int app_id = g_context->register_application( AEtitle );
            if( app_id < 0 )
            {
                ret = static_cast<MC_STATUS>( -app_id );
            }
            else
            {
                *ApplicationID = app_id;
                ret = MC_NORMAL_COMPLETION;
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
