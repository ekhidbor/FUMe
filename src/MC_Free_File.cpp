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

using fume::g_context;

MC_STATUS MC_Free_File( int* FileID )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context != nullptr &&
            FileID    != nullptr )
        {
            ret = g_context->free_file_object( *FileID );
            if( ret == MC_NORMAL_COMPLETION )
            {
                *FileID = -1;
            }
            else
            {
                // Do nothing. Will return error from free_file_object
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
