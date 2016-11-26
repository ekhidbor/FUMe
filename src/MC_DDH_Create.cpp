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
#include "fume/file_object.h"

using fume::g_context;
using fume::file_object;

MC_STATUS MC_DDH_Create( const char* FilePath,
                         const char* FileSetID,
                         int         TemplateFileID,
                         int*        DirMsgIDPtr )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context   != nullptr &&
            DirMsgIDPtr != nullptr )
        {
            file_object* template_dict =
                dynamic_cast<file_object*>( g_context->get_object( TemplateFileID ) );
            // Create DICOMDIR object if the caller didn't specify a template ID
            // or they did and it's a valid file object
            if( TemplateFileID == 0 || template_dict != nullptr )
            {
                const int dicomdir_id =
                    g_context->create_dicomdir_object( FilePath,
                                                       FileSetID,
                                                       template_dict );
                if( dicomdir_id > 0 )
                {
                    *DirMsgIDPtr = dicomdir_id;
                    ret = MC_NORMAL_COMPLETION;
                }
                else
                {
                    ret = static_cast<MC_STATUS>( -dicomdir_id );
                }
            }
            else
            {
                ret = MC_INVALID_MESSAGE_ID;
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
