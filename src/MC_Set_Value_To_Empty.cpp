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
#include "fume/data_dictionary.h"

using fume::g_context;
using fume::data_dictionary;

MC_STATUS MC_Set_Value_To_Empty( int MsgFileItemID, unsigned long Tag )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context != nullptr )
        {
            data_dictionary* dict = g_context->get_object( MsgFileItemID );
            if( dict != nullptr )
            {
                ret = dict->set_empty( Tag );
            }
            else
            {
                ret = MC_INVALID_MESSAGE_ID;
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
