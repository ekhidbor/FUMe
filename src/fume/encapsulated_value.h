#ifndef ENCAPSULATED_VALUE_H
#define ENCAPSULATED_VALUE_H
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
#include <memory>

// local public
#include "mcstatus.h"

// local private
#include "fume/encapsulated_value_sink.h"

namespace fume
{

class tx_stream;

class encapsulated_value : public encapsulated_value_sink
{
public:
    encapsulated_value()
    {
    }
    virtual ~encapsulated_value()
    {
    }

    virtual MC_STATUS write_vr_data_to_stream( tx_stream&      stream,
                                               TRANSFER_SYNTAX syntax ) = 0;

    virtual MC_STATUS write_data_to_stream( tx_stream&      stream,
                                            TRANSFER_SYNTAX syntax ) = 0;

    virtual MC_STATUS write_first_frame_to_stream
    (
        encapsulated_value_sink& stream,
        TRANSFER_SYNTAX          syntax
    ) = 0;

    virtual MC_STATUS write_next_frame_to_stream
    (
        encapsulated_value_sink& stream,
        TRANSFER_SYNTAX          syntax
    ) = 0;

    virtual MC_STATUS write_frame_to_stream
    (
        unsigned int             idx,
        encapsulated_value_sink& stream,
        TRANSFER_SYNTAX          syntax
    ) = 0;

    virtual MC_STATUS clear() = 0;
    virtual uint64_t size() const = 0;

    virtual std::unique_ptr<encapsulated_value> clone() const = 0;
};


}

#endif
