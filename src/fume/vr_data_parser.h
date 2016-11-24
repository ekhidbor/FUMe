#ifndef VR_DATA_PARSER_H
#define VR_DATA_PARSER_H
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
#include "mc3msg.h"

// local private

namespace fume
{

class rx_stream;
class encapsulated_value_sink;

MC_STATUS read_vr_data( rx_stream&               source,
                        TRANSFER_SYNTAX          syntax,
                        encapsulated_value_sink& dest );

}


#endif
