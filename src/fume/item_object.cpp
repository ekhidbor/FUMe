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

// local private
#include "fume/item_object.h"
#include "fume/tx_stream.h"

namespace fume
{

static MC_STATUS write_item_tag( tx_stream& stream, TRANSFER_SYNTAX syntax )
{
    MC_STATUS ret = stream.write_tag( 0xFFFEE000u, syntax );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = stream.write_val( static_cast<uint32_t>( 0xFFFFFFFFu ), syntax );
    }
    else
    {
        // Do nothing. Will return error from write_vr
    }

    return ret;
}

static MC_STATUS write_item_delimitation( tx_stream&      stream,
                                          TRANSFER_SYNTAX syntax )
{
    MC_STATUS ret = stream.write_tag( 0xFFFEE00Du, syntax );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = stream.write_val( static_cast<uint32_t>( 0x00000000u ), syntax );
    }
    else
    {
        // Do nothing. Will return error from write_vr
    }

    return ret;
}

MC_STATUS item_object::to_stream( tx_stream&      stream,
                                  TRANSFER_SYNTAX syntax ) const
{
    MC_STATUS ret = write_item_tag( stream, syntax );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = write_values( stream, syntax, begin(), end() );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = write_item_delimitation( stream, syntax );
        }
        else
        {
            // Do nothing. Will return error from to_stream
        }
    }
    else
    {
        // Do nothing. Will return error from write_item_tag
    }

    return ret;
}

}
