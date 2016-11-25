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
        : m_offset( 0 )
    {
    }

    memory_stream( const memory_stream& rhs )
        : m_offset( rhs.m_offset ),
          m_data( rhs.m_data )
    {
    }

    virtual ~memory_stream()
    {
    }

    virtual MC_STATUS write( const void* buffer,
                             uint32_t    buffer_bytes ) override final;
    virtual uint64_t tell_write() const override final
    {
        return m_offset;
    }

    virtual MC_STATUS read( void* buffer, uint32_t buffer_bytes ) override final;
    virtual MC_STATUS peek( void* buffer, uint32_t buffer_bytes ) override final;
    virtual uint64_t tell_read() const override final
    {
        return m_offset;
    }

    virtual MC_STATUS clear() override final;
    virtual MC_STATUS seek( uint64_t pos ) override final;

    virtual uint64_t size() const override final
    {
        return m_data.size();
    }

    virtual std::unique_ptr<seekable_stream> clone() override
    {
        return std::unique_ptr<seekable_stream>( new memory_stream( *this ) );
    }

private:
    std::deque<uint8_t> m_data;
    uint64_t m_offset = 0;
};


}

#endif
