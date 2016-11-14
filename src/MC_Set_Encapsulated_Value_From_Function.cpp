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
#include "mc3msg.h"

// local private
#include "fume/library_context.h"
#include "fume/data_dictionary.h"
#include "fume/callback_io.h"
#include "fume/vrs/ob.h"

using boost::numeric_cast;
using boost::bad_numeric_cast;

using fume::g_context;
using fume::data_dictionary;
using fume::set_func_parms;
using fume::vrs::ob;
using fume::write_encapsulated_value_from_function;

MC_STATUS MC_Set_Encapsulated_Value_From_Function( int              MsgFileItemID,
                                                   unsigned long    Tag,
                                                   void*            UserInfo,
                                                   SetValueCallback YourSetFunction )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context != nullptr )
        {
            data_dictionary* dict = g_context->get_object( MsgFileItemID );
            if( dict != nullptr )
            {
                const uint32_t tag_u32 = numeric_cast<uint32_t>( Tag );
                if( dict->has_tag( tag_u32 ) == true )
                {
                    ob* element = dynamic_cast<ob*>( dict->at( tag_u32) );
                    if( element != nullptr )
                    {
                        TRANSFER_SYNTAX syntax = INVALID_TRANSFER_SYNTAX;
                        ret = dict->get_transfer_syntax( syntax );
                        if( ret == MC_NORMAL_COMPLETION )
                        {
                            set_func_parms parms =
                            {
                                YourSetFunction,
                                UserInfo,
                                MsgFileItemID,
                                Tag
                            };
                            ret = write_encapsulated_value_from_function( *element,
                                                                          syntax,
                                                                          parms );
                        }
                        else
                        {
                            // Do nothing. Returns error from get_transfer_syntax
                        }
                    }
                    else
                    {
                        ret = MC_INCOMPATIBLE_VR;
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
