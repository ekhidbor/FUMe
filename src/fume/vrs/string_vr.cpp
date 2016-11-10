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
#include <cassert>
#include <algorithm>
#include <numeric>
#include <limits>

// local public
#include "mc3msg.h"

// local private
#include "fume/vrs/string_vr.h"
#include "fume/tx_stream.h"

using std::accumulate;
using std::min;
using std::deque;
using std::string;
using std::numeric_limits;
using std::move;

namespace fume
{
namespace vrs
{

static uint32_t get_total_length( const deque<string>& values )
{
    // Calculate the total length of all the strings
    const uint32_t str_size = accumulate( values.cbegin(),
                                          values.cend(),
                                          static_cast<uint32_t>(0u),
                                          []( uint32_t sum, const string& val )
                                          {
                                              return sum + val.size();
                                          } );
    // There are delimiters between each value. Therefore n - 1 delimiters
    // for n values
    const uint32_t delim_size = values.size() <= 1u ? 0 : values.size() - 1u;
    const uint32_t total_size = str_size + delim_size;

    return total_size;
}

static MC_STATUS write_string( tx_stream& stream, const string& value )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( value.empty() == false )
    {
        ret = stream.write( value.data(), value.size() );
    }
    else
    {
        ret = MC_NORMAL_COMPLETION;
    }

    return ret;
}

string_vr::string_vr( unsigned int min_vals,
                      unsigned int max_vals,
                      unsigned int multiple,
                      char         pad )
    : value_representation( min_vals, max_vals, multiple ),
      m_current_idx( 0 ),
      m_pad( pad )
{
}

MC_STATUS string_vr::to_stream( tx_stream& stream ) const
{
    const uint32_t value_length = get_total_length( m_values );
    const uint32_t value_length_even = value_length + (value_length % 2u);

    MC_STATUS ret = MC_CANNOT_COMPLY;
    // UC, UR, UT are 32-bit length
    const MC_VR this_vr = vr();
    if( this_vr == UC ||
        this_vr == UR ||
        this_vr == UT ||
        stream.transfer_syntax() == IMPLICIT_LITTLE_ENDIAN )
    {
        ret = stream.write_val( value_length_even );
    }
    else
    {
        // TODO: proper length validation
        ret = stream.write_val( static_cast<uint16_t>( value_length_even ) );
    }

    if( ret == MC_NORMAL_COMPLETION && m_values.empty() == false )
    {
        ret = write_string( stream, m_values.front() );
        for( deque<string>::const_iterator itr = m_values.cbegin() + 1u;
             ret == MC_NORMAL_COMPLETION && itr != m_values.cend();
             ++itr )
        {
            static const char DELIM = '\\';
            ret = stream.write_val( static_cast<uint8_t>( DELIM ) );
            if( ret == MC_NORMAL_COMPLETION )
            {
                ret = write_string( stream, *itr );
            }
        }

        // If there were no errors and we need to write a pad byte
        if( ret == MC_NORMAL_COMPLETION && value_length != value_length_even )
        {
            // Do so
            ret = stream.write_val( static_cast<uint8_t>( m_pad ) );
        }
        else
        {
            // Do nothing. Will return status from the last write
        }
    }
    else
    {
        // Do nothing. Will return error if write_element_length failed or
        // MC_NORMAL_COMPLETION if zero length value
    }

    return ret;
}

MC_STATUS string_vr::set( const char* val )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( val != nullptr )
    {
        ret = set( string( val ) );
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS string_vr::set( string&& val )
{
    // According to the docs, failing validation doesn't cause the value
    // to not be saved
    // Must validate before push_back
    MC_STATUS ret = validate_value( val );

    m_values.clear();
    m_current_idx = 0;
    m_values.push_back( move( val ) );

    return ret;
}

MC_STATUS string_vr::set_next( const char* val )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( val != nullptr )
    {
        ret = set_next( string( val ) );
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS string_vr::set_next( string&& val )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( m_values.size() < numeric_limits<uint16_t>::max() )
    {
        // Validate before push_back
        ret = validate_value( val );

        m_current_idx = 0;
        m_values.push_back( move( val ) );
    }
    else
    {
        ret = MC_TOO_MANY_VALUES;
    }

    return ret;
}

MC_STATUS string_vr::get( get_string_parms& val ) const
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( val.first != nullptr )
    {
        const string* cur_val = nullptr;
        ret = get( cur_val );
        if( ret == MC_NORMAL_COMPLETION )
        {
            assert( cur_val != nullptr );
            if( val.second > cur_val->size() )
            {
                strncpy( val.first, cur_val->c_str(), val.second );
                val.second = static_cast<MC_size_t>( cur_val->size() );
                ret = MC_NORMAL_COMPLETION;
            }
            else
            {
                ret = MC_BUFFER_TOO_SMALL;
            }
        }
        else
        {
            // Do nothing. Will return error from string get
        }
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS string_vr::get( const string*& val ) const
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( m_values.empty() == false )
    {
        m_current_idx = 0;
        val = &m_values[m_current_idx];
        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_NULL_VALUE;
    }

    return ret;
}

MC_STATUS string_vr::get_next( get_string_parms& val ) const
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( val.first != nullptr )
    {
        const string* cur_val = nullptr;
        ret = get_next( cur_val );
        if( ret == MC_NORMAL_COMPLETION )
        {
            assert( cur_val != nullptr );
            if( val.second > cur_val->size() )
            {
                strncpy( val.first, cur_val->c_str(), val.second );
                val.second = static_cast<MC_size_t>( cur_val->size() );
                ret = MC_NORMAL_COMPLETION;
            }
            else
            {
                ret = MC_BUFFER_TOO_SMALL;
            }
        }
        else
        {
            // Do nothing. Will return error from get_next
        }
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS string_vr::get_next( const string*& val ) const
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( m_values.empty() == false )
    {
        m_current_idx = min( m_values.size(), m_current_idx + 1 );
        if( m_current_idx < m_values.size() )
        {
            val = &m_values[m_current_idx];
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

MC_STATUS string_vr::delete_current()
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
