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
#include <cstdint>

// local public
#include "mcstatus.h"
#include "mergecom.h"
#include "mc3msg.h"

// local private
#include "fume/rx_stream.h"
#include "fume/sink_callback_io.h"
#include "fume/application_callback_sink.h"
#include "fume/vr_data_parser.h"

namespace fume
{

MC_STATUS read_vr_data_to_callback( rx_stream&              source,
                                    TRANSFER_SYNTAX         syntax,
                                    int                     message_id,
                                    uint32_t                tag,
                                    const callback_parms_t& dest )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( dest.first != nullptr )
    {
        application_callback_sink dest_sink( dest, message_id, tag );
        ret = read_vr_data( source, syntax, dest_sink );
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

}
