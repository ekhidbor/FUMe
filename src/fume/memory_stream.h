#ifndef MEMORY_STREAM_H
#define MEMORY_STREAM_H
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
#include <deque>
#include <memory>

// local public

// local private
#include "fume/seekable_stream.h"

namespace fume
{

class memory_stream final : public seekable_stream
{
public:
    memory_stream()
        : m_read_offset( 0 )
    {
    }
    virtual ~memory_stream()
    {
    }

    virtual MC_STATUS write( const void* buffer,
                             uint32_t    buffer_bytes ) override final;
    virtual uint64_t bytes_written() const override final
    {
        return m_data.size();
    }

    virtual MC_STATUS read( void* buffer, uint32_t buffer_bytes ) override final;
    virtual MC_STATUS peek( void* buffer, uint32_t buffer_bytes ) override final;
    virtual uint64_t bytes_read() const override final
    {
        return m_read_offset;
    }

    virtual MC_STATUS clear() override final;
    virtual MC_STATUS rewind_read() override final;

    virtual std::unique_ptr<seekable_stream> clone() override
    {
        std::unique_ptr<memory_stream> ret( new memory_stream() );
        ret->m_data = m_data;
        // Keep the read offset initialized to zero

        return std::move( ret );
    }

private:
    std::deque<uint8_t> m_data;
    uint64_t m_read_offset = 0;
};


}

#endif
