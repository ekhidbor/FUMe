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
#include "mc3media.h"

/// local private
#include "fume/library_context.h"
#include "fume/file_object.h"
#include "fume/file_object_io.h"

using boost::numeric_cast;
using boost::bad_numeric_cast;

using fume::g_context;
using fume::file_object;
using fume::open_file_upto;

MC_STATUS MC_Open_File_Upto_Tag( int              ApplicationID,
                                 int              FileID,
                                 void*            UserInfo,
                                 unsigned long    Tag,
                                 long*            Offset,
                                 ReadFileCallback YourFromMediaFunction )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context != nullptr && Offset != nullptr )
        {
            file_object* file =
                dynamic_cast<file_object*>( g_context->get_object( FileID ) );
            if( file != nullptr )
            {
                uint64_t offset = 0;
                ret = open_file_upto( *file,
                                      ApplicationID,
                                      numeric_cast<uint32_t>( Tag ),
                                      offset,
                                      UserInfo,
                                      YourFromMediaFunction );
                if( ret == MC_NORMAL_COMPLETION )
                {
                    *Offset = numeric_cast<long>( offset );
                }
                else
                {
                    // Leave Offset unmodified on error
                }
            }
            else
            {
                ret = MC_INVALID_FILE_ID;
            }
        }
        else if( Offset == nullptr )
        {
            ret = MC_NULL_POINTER_PARM;
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
