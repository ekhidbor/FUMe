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
#include "fume/item_object.h"

using fume::g_context;
using fume::item_object;

MC_STATUS MC_Empty_Item( int ItemID )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context != nullptr )
        {
            item_object* item =
                dynamic_cast<item_object*>( g_context->get_object( ItemID ) );
            if( item != nullptr )
            {
                item->empty_item();
                ret = MC_NORMAL_COMPLETION;
            }
            else
            {
                ret = MC_INVALID_ITEM_ID;
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
