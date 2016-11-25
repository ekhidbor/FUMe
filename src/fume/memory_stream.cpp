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
#include <algorithm>

// local public

// local private
#include "fume/memory_stream.h"

using std::deque;
using std::copy;
using std::min;
using std::max;

namespace fume
{

MC_STATUS memory_stream::write( const void* buffer,
                                uint32_t    buffer_bytes )
{
    const uint64_t new_size = max( m_offset + buffer_bytes,
                                  static_cast<uint64_t>( m_data.size() ) );
    m_data.resize( new_size );

    const deque<uint8_t>::iterator dst_begin = m_data.begin() + m_offset;

    const uint8_t* src_begin = static_cast<const uint8_t*>( buffer );
    const uint8_t* src_end = src_begin + buffer_bytes;

    copy( src_begin, src_end, dst_begin );
    m_offset += buffer_bytes;

    return MC_NORMAL_COMPLETION;
}

MC_STATUS memory_stream::peek( void* buffer, uint32_t buffer_bytes )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    const uint64_t new_offset = m_offset + buffer_bytes;
    if( m_offset < m_data.size() && new_offset <= m_data.size() )
    {
        const deque<uint8_t>::const_iterator begin = m_data.cbegin() + m_offset;
        const deque<uint8_t>::const_iterator end = begin + buffer_bytes;

        copy( begin, end, static_cast<uint8_t*>( buffer ) );
        ret = MC_NORMAL_COMPLETION;
    }
    else if( m_offset == m_data.size() )
    {
        ret = MC_END_OF_DATA;
    }
    else
    {
        ret = MC_UNEXPECTED_EOD;
    }

    return ret;
}

MC_STATUS memory_stream::read( void* buffer, uint32_t buffer_bytes )
{
    MC_STATUS ret = peek( buffer, buffer_bytes );
    if( ret == MC_NORMAL_COMPLETION )
    {
        m_offset += buffer_bytes;
        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        // Do nothing. Will return error
    }

    return ret;
}


MC_STATUS memory_stream::clear()
{
    m_data.clear();
    m_offset = 0;

    return MC_NORMAL_COMPLETION;
}

MC_STATUS memory_stream::seek( uint64_t pos )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    // seek is allowed to go past the end of the stream, but data
    // cannot be read
    m_offset = pos;
    ret = MC_NORMAL_COMPLETION;

    return ret;
}

}