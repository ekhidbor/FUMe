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

/// local private
#include "fume/library_context.h"
#include "fume/file_object.h"
#include "fume/file_object_io.h"

using fume::g_context;
using fume::file_object;
using fume::write_file;

MC_STATUS MC_Write_File_By_Callback( int               ApplicationID,
                                     int               FileID,
                                     int               NumBytes,
                                     void*             UserInfo,
                                     WriteFileCallback YourToMediaFunction )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context != nullptr )
        {
            file_object* file =
                dynamic_cast<file_object*>( g_context->get_object( FileID ) );
            if( file != nullptr )
            {
                ret = write_file( *file,
                                   ApplicationID,
                                   NumBytes,
                                   UserInfo,
                                   YourToMediaFunction );
            }
            else
            {
                ret = MC_INVALID_FILE_ID;
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
