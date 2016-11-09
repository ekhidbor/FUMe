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

MC_STATUS MC_Cleanup_Memory( int Timeout )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( g_context != nullptr && Timeout >= 0 )
    {
        // This function deliberately does nothing as we just
        // use the standard C++ memory management system
        ret = MC_NORMAL_COMPLETION;
    }
    else if( g_context != nullptr )
    {
        ret = MC_LIBRARY_NOT_INITIALIZED;
    }
    else
    {
        ret = MC_VALUE_OUT_OF_RANGE;
    }

    return ret;
}
