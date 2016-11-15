#ifndef NULL_TX_STREAM_H
#define NULL_TX_STREAM_H
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
#include "fume/tx_stream.h"

namespace fume
{

class null_tx_stream final : public tx_stream
{
public:
    null_tx_stream()
        : m_bytes_written( 0u )
    {
    }
    ~null_tx_stream()
    {
    }

// tx_stream
public:
    virtual MC_STATUS write( const void* buffer,
                             size_t      buffer_bytes ) override final
    {
        m_bytes_written += static_cast<uint32_t>( buffer_bytes );
        return MC_NORMAL_COMPLETION;
    }

    virtual uint32_t bytes_written() const override final
    {
        return m_bytes_written;
    }

private:
    null_tx_stream( const null_tx_stream& );
    null_tx_stream& operator=( const null_tx_stream& );

private:
    uint32_t m_bytes_written;
};

}

#endif
