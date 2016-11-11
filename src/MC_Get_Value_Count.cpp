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

// boost
#include "boost/numeric/conversion/cast.hpp"

// local public
#include "mcstatus.h"
#include "mc3msg.h"

// local private
#include "fume/library_context.h"
#include "fume/data_dictionary.h"
#include "fume/value_representation.h"

using boost::numeric_cast;
using boost::bad_numeric_cast;

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
            const data_dictionary* dict = g_context->get_object( MsgFileItemID );
            if( dict != nullptr )
            {
                const uint32_t tag_u32 = numeric_cast<uint32_t>( Tag );
                if( dict->has_tag( tag_u32) == true )
                {
                    const value_representation* element = dict->at( tag_u32 );
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
                    ret = MC_INVALID_TAG;
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
