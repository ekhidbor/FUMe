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

namespace fume
{

MC_STATUS memory_stream::write( const void* buffer,
                                uint32_t    buffer_bytes )
{
    const uint8_t* data = static_cast<const uint8_t*>( buffer );
    m_data.insert( m_data.cend(), data, data + buffer_bytes );

    return MC_NORMAL_COMPLETION;
}

MC_STATUS memory_stream::peek( void* buffer, uint32_t buffer_bytes )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    const uint64_t new_read_offset = m_read_offset + buffer_bytes;
    if( new_read_offset <= m_data.size() )
    {
        const deque<uint8_t>::const_iterator begin = m_data.cbegin() +
                                                     m_read_offset;
        const deque<uint8_t>::const_iterator end = begin + buffer_bytes;

        copy( begin, end, static_cast<uint8_t*>( buffer ) );
        ret = MC_NORMAL_COMPLETION;
    }
    if( m_read_offset == m_data.size() )
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
        m_read_offset += buffer_bytes;
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
    m_read_offset = 0;

    return MC_NORMAL_COMPLETION;
}

MC_STATUS memory_stream::rewind_read()
{
    m_read_offset = 0;

    return MC_NORMAL_COMPLETION;
}

}