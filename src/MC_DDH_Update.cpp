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
#include "mc3media.h"

// local private
#include "fume/library_context.h"
#include "fume/dicomdir_object.h"

using fume::g_context;
using fume::dicomdir_object;

MC_STATUS MC_DDH_Update( int DirMsgID )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context != nullptr )
        {
            dicomdir_object* dicomdir =
                dynamic_cast<dicomdir_object*>( g_context->get_object( DirMsgID ) );
            if( dicomdir != nullptr )
            {
                ret = dicomdir->update();
            }
            else
            {
                ret = MC_INVALID_DICOMDIR_ID;
            }
        }
        else
        {
            ret = MC_LIBRARY_NOT_INITIALIZED;
        }
    }
    catch( ... )
    {
        ret = MC_SYSTEM_ERROR;
    }

    return ret;
}
