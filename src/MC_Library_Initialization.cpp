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
#include "mc3msg.h"

// local private
#include "fume/library_context.h"

using fume::library_context;
using fume::g_context;

MC_STATUS MC_Library_Initialization( CfgFunction        cfg,
                                     DictionaryFunction dict,
                                     FutureFunction     future )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context == nullptr )
        {
            g_context.reset( new library_context() );
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            ret = MC_LIBRARY_ALREADY_INITIALIZED;
        }
    }
    catch( ... )
    {
        ret = MC_SYSTEM_ERROR;
    }

    return ret;
}
