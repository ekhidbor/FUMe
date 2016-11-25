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
#include "fume/record_object.h"

using fume::g_context;
using fume::record_object;

MC_STATUS MC_DDH_Get_Record_Type( int RecordID, MC_DIR_RECORD_TYPE* RecType )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context != nullptr && RecType != nullptr )
        {
            record_object* record =
                dynamic_cast<record_object*>( g_context->get_object( RecordID ) );
            if( record != nullptr )
            {
                ret = record->get_record_type( *RecType );
            }
            else
            {
                ret = MC_INVALID_RECORD_ID;
            }
        }
        else if( g_context != nullptr )
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
