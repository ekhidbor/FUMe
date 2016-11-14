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
#include "fume/data_dictionary.h"

using fume::g_context;
using fume::data_dictionary;

MC_STATUS MC_DDH_Copy_Values( int SourceID, int DestID, unsigned long* TagList )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context != nullptr )
        {
            const data_dictionary* src = g_context->get_object( SourceID );
            data_dictionary* dst = g_context->get_object( DestID );
            if( src != nullptr && dst != nullptr )
            {
                ret = dst->copy_values( *src, TagList );
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
