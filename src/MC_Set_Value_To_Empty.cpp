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
#include "fume/data_dictionary_search.h"

using boost::numeric_cast;
using boost::bad_numeric_cast;

using fume::g_context;
using fume::data_dictionary;
using fume::erase_tag;

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
                const bool value_erased =
                    erase_tag( *dict, numeric_cast<uint32_t>( Tag ) );
                ret = value_erased ? MC_NORMAL_COMPLETION : MC_INVALID_TAG;
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
