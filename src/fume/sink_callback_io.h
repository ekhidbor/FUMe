#ifndef SINK_CALLBACK_IO_H
#define SINK_CALLBACK_IO_H
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
#include "fume/application.h"
#include "fume/value_representation_types.h"

namespace fume
{

class rx_stream;

MC_STATUS read_vr_data_to_callback( rx_stream&              source,
                                    TRANSFER_SYNTAX         syntax,
                                    int                     message_id,
                                    uint32_t                tag,
                                    const callback_parms_t& dest );

MC_STATUS read_encapsulated_value_to_function( rx_stream&            stream,
                                               TRANSFER_SYNTAX       syntax,
                                               const get_func_parms& dest );

MC_STATUS read_next_encapsulated_value_from_function( rx_stream&            stream,
                                                      TRANSFER_SYNTAX       syntax,
                                                      const get_func_parms& dest );

MC_STATUS read_offset_table_to_function( rx_stream&            stream, 
                                         TRANSFER_SYNTAX       syntax,
                                         const get_func_parms& dest );

}


#endif
