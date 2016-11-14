#ifndef CALLBACK_IO_H
#define CALLBACK_IO_H
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
#include "fume/value_representation.h"

namespace fume
{

class tx_stream;

MC_STATUS write_vr_data_from_callback( tx_stream&              stream,
                                       TRANSFER_SYNTAX         syntax,
                                       int                     message_id,
                                       uint32_t                tag,
                                       const callback_parms_t& callback );

MC_STATUS write_encapsulated_value_from_function( tx_stream&            stream,
                                                  TRANSFER_SYNTAX       syntax,
                                                  const set_func_parms& parms );

MC_STATUS write_next_encapsulated_value_from_function( tx_stream&            stream,
                                                       TRANSFER_SYNTAX       syntax,
                                                       const set_func_parms& parms );

MC_STATUS close_encapsulated_value( tx_stream& stream, TRANSFER_SYNTAX syntax );

}


#endif
