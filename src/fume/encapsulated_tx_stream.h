#ifndef ENCAPSULATED_TX_STREAM_H
#define ENCAPSULATED_TX_STREAM_H
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
#include "fume/tx_stream.h"

namespace fume
{

class encapsulated_tx_stream : public tx_stream
{
public:
    encapsulated_tx_stream()
    {
    }
    virtual ~encapsulated_tx_stream()
    {
    }

    virtual MC_STATUS provide_value_length( uint32_t        length,
                                            TRANSFER_SYNTAX syntax ) = 0;

    virtual MC_STATUS provide_offset_table( const uint32_t* table,
                                            uint32_t        table_elements,
                                            TRANSFER_SYNTAX syntax ) = 0;

    virtual MC_STATUS start_of_frame( uint32_t size, TRANSFER_SYNTAX syntax ) = 0;

    virtual MC_STATUS end_of_sequence( TRANSFER_SYNTAX syntax ) = 0;

    virtual MC_STATUS finalize() = 0;
};


}

#endif
