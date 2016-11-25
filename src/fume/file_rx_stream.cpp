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
#include <cassert>
#include <algorithm>

// local private
#include "fume/file_rx_stream.h"

using std::string;
using std::deque;

namespace fume
{

file_rx_stream::file_rx_stream( const string&    filename,
                                ReadFileCallback callback,
                                void*            user_info )
    : m_filename( filename ),
      m_bytes_read( 0 ),
      m_callback( callback ),
      m_user_info( user_info ),
      m_first( true ),
      m_last( false )
{
    // Checked by caller
    assert( m_callback != nullptr );
}

MC_STATUS file_rx_stream::peek( void* buffer, uint32_t buffer_bytes )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( buffer != nullptr )
    {
        // If available_bytes == 0, returns MC_END_OF_DATA
        // If 0 < available_bytes < buffer_bytes, returns MC_UNEXPECTED_EOD
        // If available_bytes >= buffer_bytes, returns MC_NORMAL_COMPLETION
        // If callback error, returns MC_CALLBACK_CANNOT_COMPLY
        ret = ensure_bytes_available( buffer_bytes );
        if( ret == MC_NORMAL_COMPLETION )
        {
            assert( m_data_buffer.size() >= buffer_bytes );
            const deque<uint8_t>::const_iterator begin = m_data_buffer.begin();
            const deque<uint8_t>::const_iterator end = begin + buffer_bytes;

            copy( begin, end, static_cast<uint8_t*>( buffer ) );

            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            // Do nothing. Will return status as indicated above
        }
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS file_rx_stream::read( void* buffer, uint32_t buffer_bytes )
{
    MC_STATUS ret = peek( buffer, buffer_bytes );

    if( ret == MC_NORMAL_COMPLETION )
    {
        const deque<uint8_t>::const_iterator begin = m_data_buffer.begin();
        const deque<uint8_t>::const_iterator end = begin + buffer_bytes;

        m_data_buffer.erase( begin, end );
        m_bytes_read += buffer_bytes;
    }
    else
    {
        // Do nothing. Return error
    }

    return ret;
}

MC_STATUS file_rx_stream::ensure_bytes_available( uint32_t min_bytes_available )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    // If we already have enough data in the buffer, do nothing and
    // return success
    if( m_data_buffer.size() >= min_bytes_available )
    {
        ret = MC_NORMAL_COMPLETION;
    }
    // If we don't have enough data and we've already been signaled there's no
    // more data, return the appropriate EOD return code
    else if( m_last == true )
    {
        ret = m_data_buffer.empty() ? MC_END_OF_DATA : MC_UNEXPECTED_EOD;
    }
    else
    {
        assert( m_callback != nullptr );
        ret = MC_NORMAL_COMPLETION;
        while( ret == MC_NORMAL_COMPLETION &&
               m_last == false             &&
               m_data_buffer.size() < min_bytes_available )
        {
            int cb_size = 0;
            void* cb_data = nullptr;
            int cb_last = 0;

            const MC_STATUS stat =
                m_callback( const_cast<char*>( m_filename.c_str() ),
                            m_user_info,
                            &cb_size,
                            &cb_data,
                            static_cast<int>( m_first ),
                            &cb_last );
            if( (stat == MC_NORMAL_COMPLETION) &&
                (cb_size > 0)                  &&
                ((cb_size % 2 == 0 ))          &&
                (cb_data != nullptr) )
            {
                const uint8_t* data = static_cast<uint8_t*>( cb_data );

                m_data_buffer.insert( m_data_buffer.cend(),
                                      data,
                                      data + cb_size );
                m_last = cb_last != 0;
                m_first = false;

                ret = MC_NORMAL_COMPLETION;
            }
            else if( stat != MC_NORMAL_COMPLETION )
            {
                ret = MC_CALLBACK_CANNOT_COMPLY;
            }
            else if( cb_data == nullptr )
            {
                ret = MC_CALLBACK_PARM_ERROR;
            }
            else
            {
                ret = MC_CALLBACK_DATA_SIZE_UNEVEN;
            }
        }

        // If we've read in all the data, make sure we've read in enough
        if( ret == MC_NORMAL_COMPLETION && m_last == true )
        {
            if( m_data_buffer.size() >= min_bytes_available )
            {
                ret = MC_NORMAL_COMPLETION;
            }
            else if( m_data_buffer.empty() == true )
            {
                ret = MC_END_OF_DATA;
            }
            else
            {
                ret = MC_UNEXPECTED_EOD;
            }
        }
        else
        {
            // Do nothing. Either error is indicated or enough bytes
            // are in the buffer
        }
    }

    return ret;
}

}
