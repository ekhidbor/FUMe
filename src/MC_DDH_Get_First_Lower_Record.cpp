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
#include <algorithm>

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
using std::max;

MC_STATUS MC_DDH_Get_First_Lower_Record( int ParentID, int* LowerID )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context != nullptr && LowerID != nullptr )
        {
            data_dictionary* dict = g_context->get_object( ParentID );
            if( dict != nullptr )
            {
                dicomdir_object* dicomdir = dynamic_cast<dicomdir_object*>( dict );
                record_object* record = dynamic_cast<record_object*>( dict );

                if( dicomdir != nullptr )
                {
                    // Return 0 if no next record (get_next_record will return
                    // negaitve value)
                    *LowerID = max( 0, dicomdir->get_child_record() );
                    ret = MC_NORMAL_COMPLETION;
                }
                else if( record != nullptr )
                {
                    // Return 0 if no next record (get_next_record will return
                    // negaitve value)
                    *LowerID = max( 0, record->get_child_record() );
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
