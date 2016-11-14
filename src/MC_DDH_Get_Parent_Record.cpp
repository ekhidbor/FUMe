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
#include "fume/record_object.h"

using fume::g_context;
using fume::dicomdir_object;
using fume::record_object;
using fume::data_dictionary;

MC_STATUS MC_DDH_Get_Parent_Record( int RecordID, int* ParentID )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context != nullptr && ParentID != nullptr )
        {
            data_dictionary* dict = g_context->get_object( RecordID );
            if( dict != nullptr )
            {
                dicomdir_object* dicomdir = dynamic_cast<dicomdir_object*>( dict );
                record_object* record = dynamic_cast<record_object*>( dict );

                if( dicomdir != nullptr )
                {
                    *ParentID = 0;
                    ret = MC_NORMAL_COMPLETION;
                }
                else if( record != nullptr )
                {
                    *ParentID = record->get_parent_record();
                    ret = MC_NORMAL_COMPLETION;
                }
                else
                {
                    ret = MC_INVALID_RECORD_ID;
                }
            }
            else
            {
                ret = MC_INVALID_RECORD_ID;
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
