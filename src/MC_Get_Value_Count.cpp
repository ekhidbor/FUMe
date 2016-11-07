/**
 * This file is a part of the FUMe project.
 *
 * To the extent possible under law, the person who associated CC0 with
 * FUMe has waived all copyright and related or neighboring rights
 * to FUMe.
 *
 * You should have received a copy of the CC0 legalcode along with this
 * work.  If not, see http://creativecommons.org/publicdomain/zero/1.0/.
**/

// std

// local public
#include "mcstatus.h"
#include "mc3msg.h"

// local private
#include "fume/library_context.h"
#include "fume/data_dictionary.h"
#include "fume/value_representation.h"

using fume::g_context;
using fume::data_dictionary;
using fume::value_representation;

MC_STATUS MC_Get_Value_Count( int           MsgFileItemID,
                              unsigned long Tag,
                              int*          CountPtr )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context != nullptr && CountPtr != nullptr )
        {
            const data_dictionary* dict = g_context->get_object( Tag );
            if( dict != nullptr )
            {
                ret = dict->check_tag_const( Tag );
                if( ret == MC_NORMAL_COMPLETION )
                {
                    const value_representation* element = dict->at( Tag );
                    if( element != nullptr )
                    {
                        if( element->is_null() == false )
                        {
                            *CountPtr = element->count();
                            ret = MC_NORMAL_COMPLETION;
                        }
                        else
                        {
                            ret = MC_NULL_VALUE;
                        }
                    }
                    else
                    {
                        ret = MC_EMPTY_VALUE;
                    }
                }
                else
                {
                    // Do nothing. Returns value from check_tag
                }
            }
            else
            {
                ret = MC_INVALID_MESSAGE_ID;
            }
        }
        else if( CountPtr == nullptr )
        {
            ret = MC_NULL_POINTER_PARM;
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
