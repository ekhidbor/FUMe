#ifndef DATA_DICTIONARY_IO_H
#define DATA_DICTIONARY_IO_H
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

// local private
#include "fume/data_dictionary_types.h"

namespace fume
{

class tx_stream;
class rx_stream;

MC_STATUS write_values( tx_stream&       stream,
                        TRANSFER_SYNTAX  syntax,
                        data_dictionary& dict,
                        int              app_id,
                        uint32_t         start_tag,
                        uint32_t         end_tag );

MC_STATUS write_values( tx_stream&           stream,
                        TRANSFER_SYNTAX      syntax,
                        data_dictionary&     dict,
                        uint32_t             start_tag,
                        uint32_t             end_tag );

MC_STATUS write_values( tx_stream&           stream,
                        TRANSFER_SYNTAX      syntax,
                        data_dictionary&     dict,
                        int                  app_id );

MC_STATUS read_values_from_item( rx_stream&       stream,
                                 TRANSFER_SYNTAX  syntax,
                                 data_dictionary& dict,
                                 int              app_id,
                                 uint32_t         size );

MC_STATUS read_values( rx_stream&       stream,
                       TRANSFER_SYNTAX  syntax,
                       data_dictionary& dict,
                       int              app_id );

MC_STATUS read_values_upto( rx_stream&       stream,
                            TRANSFER_SYNTAX  syntax,
                            data_dictionary& dict,
                            int              app_id,
                            uint32_t         end_tag );

}

#endif
