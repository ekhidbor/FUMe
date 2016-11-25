#ifndef RX_STREAM_H
#define RX_STREAM_H
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

class rx_stream
{
public:
    rx_stream()
    {
    }
    virtual ~rx_stream()
    {
    }

    // returns MC_NORMAL_COMPLETION if buffer_bytes have been successfully
    // returns MC_END_OF_DATA if the function is called and zero bytes
    //                        are available
    // returns MC_UNEXPECTED_EOD if the function is called and an insufficient
    //                           but non-zero number of bytes are available
    virtual MC_STATUS read( void* buffer, uint32_t buffer_bytes ) = 0;
    virtual MC_STATUS peek( void* buffer, uint32_t buffer_bytes ) = 0;
    virtual uint64_t tell_read() const = 0;

    MC_STATUS read_vr( MC_VR& vr, TRANSFER_SYNTAX syntax );

    MC_STATUS read_tag( uint32_t& tag, TRANSFER_SYNTAX syntax );
    MC_STATUS peek_tag( uint32_t& tag, TRANSFER_SYNTAX syntax );

    MC_STATUS read_val( int8_t& val, TRANSFER_SYNTAX syntax );
    MC_STATUS read_val( uint8_t& val, TRANSFER_SYNTAX syntax );
    MC_STATUS read_val( int16_t& val, TRANSFER_SYNTAX syntax );
    MC_STATUS read_val( uint16_t& val, TRANSFER_SYNTAX syntax );
    MC_STATUS read_val( int32_t& val, TRANSFER_SYNTAX syntax );
    MC_STATUS read_val( uint32_t& val, TRANSFER_SYNTAX syntax );
    MC_STATUS read_val( float& val, TRANSFER_SYNTAX syntax );
    MC_STATUS read_val( double& val, TRANSFER_SYNTAX syntax );
    MC_STATUS read_val( char& val, TRANSFER_SYNTAX syntax );

    MC_STATUS read_vals( int16_t* vals, uint32_t num_vals, TRANSFER_SYNTAX syntax );
    MC_STATUS read_vals( uint16_t* vals, uint32_t num_vals, TRANSFER_SYNTAX syntax );
    MC_STATUS read_vals( int32_t* vals, uint32_t num_vals, TRANSFER_SYNTAX syntax );
    MC_STATUS read_vals( uint32_t* vals, uint32_t num_vals, TRANSFER_SYNTAX syntax );
    MC_STATUS read_vals( float* vals, uint32_t num_vals, TRANSFER_SYNTAX syntax );
    MC_STATUS read_vals( double* vals, uint32_t num_vals, TRANSFER_SYNTAX syntax );
};


}

#endif
