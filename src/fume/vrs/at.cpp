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
#include <numeric>
#include <limits>

// local private
#include "fume/vrs/at.h"
#include "fume/tx_stream.h"

using std::min;
using std::deque;
using std::numeric_limits;
using std::move;

namespace fume
{
namespace vrs
{

MC_STATUS at::to_stream( tx_stream& stream ) const
{
    MC_STATUS ret =
        stream.write_val( static_cast<uint16_t>( m_values.size() *
                                                 sizeof(uint32_t) ) );
    if( ret == MC_NORMAL_COMPLETION && m_values.empty() == false )
    {
        for( typename deque<uint32_t>::const_iterator itr = m_values.cbegin();
             ret == MC_NORMAL_COMPLETION && itr != m_values.cend();
             ++itr )
        {
            ret = stream.write_tag( *itr );
        }
    }
    else
    {
        // Do nothing. Will return error if write_val failed or
        // MC_NORMAL_COMPLETION if zero length value
    }

    return ret;
}

MC_STATUS at::set_native( uint32_t val )
{
    m_values.clear();
    m_current_idx = 0;
    m_values.push_back( val );

    return MC_NORMAL_COMPLETION;
}

MC_STATUS at::set_next_native( uint32_t val )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( m_values.size() < numeric_limits<uint16_t>::max() )
    {
        m_current_idx = 0;
        m_values.push_back( val );

        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_TOO_MANY_VALUES;
    }

    return ret;
}

MC_STATUS at::get_native( uint32_t& val ) const
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( m_values.empty() == false )
    {
        m_current_idx = 0;
        val = m_values[m_current_idx];
        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_NULL_VALUE;
    }

    return ret;
}

MC_STATUS at::get_next_native( uint32_t& val ) const
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( m_values.empty() == false )
    {
        m_current_idx = min( m_values.size(), m_current_idx + 1 );
        if( m_current_idx < m_values.size() )
        {
            val = m_values[m_current_idx];
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            ret = MC_NO_MORE_VALUES;
        }
    }
    else
    {
        ret = MC_NULL_VALUE;
    }

    return ret;
}

MC_STATUS at::delete_current()
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( m_values.empty() == false )
    {
        if( m_current_idx < m_values.size() )
        {
            m_values.erase( m_values.cbegin() + m_current_idx );
            // Adjust the index to continue to be valid if we removed
            // the last element of a multi-element list
            if( m_current_idx > 0 && m_current_idx >= m_values.size() )
            {
                --m_current_idx;
            }
            else
            {
                // Do nothing. Index is still valid
            }
        }
        else
        {
            ret = MC_NO_MORE_VALUES;
        }
    }
    else
    {
        ret = MC_NULL_VALUE;
    }

    return ret;
}

} // namespace vrs
} // namespace fume
