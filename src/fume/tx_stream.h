#ifndef TX_STREAM_H
#define TX_STREAM_H
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
#include <cstdlib>
#include <cstdint>

// local public
#include "mcstatus.h"
#include "mc3msg.h"

namespace fume
{

class tx_stream
{
public:
    tx_stream()
    {
    }
    virtual ~tx_stream()
    {
    }

    // returns MC_NORMAL_COMPLETION if buffer_bytes have been successfully
    // written and some other error code otherwise
    virtual MC_STATUS write( const void* buffer, uint32_t buffer_bytes ) = 0;
    virtual uint64_t tell_write() const = 0;

    MC_STATUS write_vr( MC_VR vr, TRANSFER_SYNTAX syntax );

    MC_STATUS write_tag( uint32_t tag, TRANSFER_SYNTAX syntax );

    MC_STATUS write_val( int8_t val, TRANSFER_SYNTAX syntax );
    MC_STATUS write_val( uint8_t val, TRANSFER_SYNTAX syntax );
    MC_STATUS write_val( int16_t val, TRANSFER_SYNTAX syntax );
    MC_STATUS write_val( uint16_t val, TRANSFER_SYNTAX syntax );
    MC_STATUS write_val( int32_t val, TRANSFER_SYNTAX syntax );
    MC_STATUS write_val( uint32_t val, TRANSFER_SYNTAX syntax );
    MC_STATUS write_val( float val, TRANSFER_SYNTAX syntax );
    MC_STATUS write_val( double val, TRANSFER_SYNTAX syntax );
};


}

#endif
