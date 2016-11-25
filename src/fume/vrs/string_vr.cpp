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
#include <numeric>
#include <limits>

// local public
#include "mc3msg.h"

// local private
#include "fume/vrs/string_vr.h"
#include "fume/tx_stream.h"
#include "fume/rx_stream.h"

using std::min;
using std::deque;
using std::string;
using std::numeric_limits;
using std::move;

namespace fume
{
namespace vrs
{

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
      m_pad( pad )
{
}

MC_STATUS string_vr::from_stream( rx_stream&      stream,
                                  TRANSFER_SYNTAX syntax )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;
    uint32_t value_length = 0;

    value_list_t tmp_values;

    // UC, UR, UT are 32-bit length
    const MC_VR this_vr = vr();
    if( this_vr == UC ||
        this_vr == UR ||
        this_vr == UT ||
        syntax == IMPLICIT_LITTLE_ENDIAN )
    {
        ret = stream.read_val( value_length, syntax );
    }
    else
    {
        uint16_t value_length_16u;
        ret = stream.read_val( value_length_16u, syntax );
        value_length = value_length_16u;
    }

    string cur_string;
    for( uint32_t i = 0; i < value_length && ret == MC_NORMAL_COMPLETION; ++i )
    {
        char cur = '\0';

        ret = stream.read_val( cur, syntax );
        if( ret == MC_NORMAL_COMPLETION )
        {
            if( cur == '\\' )
            {
                ret = tmp_values.set_next( cur_string );
                cur_string.clear();
            }
            else if( cur != '\0' )
            {
                cur_string.push_back( cur );
            }
            else
            {
                // Discard NULL characters
            }
        }
        else
        {
            // Do nothing. Will terminate loop and return error
        }
    }

    if( ret == MC_NORMAL_COMPLETION )
    {
        // if there is "leftover" string data which was not delimited
        if( cur_string.empty() == false )
        {
            // Add it to the value list
            tmp_values.set_next( cur_string );
        }
        else
        {
            // All data already in the value list
        }

        // Only Update our values list if everything succeeded
        m_values.swap( tmp_values );
    }

    return ret;
}

MC_STATUS string_vr::to_stream( tx_stream& stream, TRANSFER_SYNTAX syntax )
{
    const uint32_t value_length = m_values.size();
    const uint32_t value_length_even = value_length + (value_length % 2u);

    MC_STATUS ret = MC_CANNOT_COMPLY;
    // UC, UR, UT are 32-bit length
    const MC_VR this_vr = vr();
    if( this_vr == UC ||
        this_vr == UR ||
        this_vr == UT ||
        syntax == IMPLICIT_LITTLE_ENDIAN )
    {
        ret = stream.write_val( value_length_even, syntax );
    }
    else
    {
        // TODO: proper length validation
        ret = stream.write_val( static_cast<uint16_t>( value_length_even ),
                                syntax );
    }

    if( ret == MC_NORMAL_COMPLETION && m_values.is_null() == false )
    {
        value_list_t::const_iterator itr = m_values.cbegin();
        ret = write_string( stream, *itr );
        ++itr;
        while( ret == MC_NORMAL_COMPLETION && itr != m_values.cend() )
        {
            static const char DELIM = '\\';
            ret = stream.write_val( static_cast<uint8_t>( DELIM ), syntax );
            if( ret == MC_NORMAL_COMPLETION )
            {
                ret = write_string( stream, *itr );
            }
            else
            {
                // Do nothing. WIll return error
            }

            ++itr;
        }

        // If there were no errors and we need to write a pad byte
        if( ret == MC_NORMAL_COMPLETION && value_length != value_length_even )
        {
            // Do so
            ret = stream.write_val( static_cast<uint8_t>( m_pad ), syntax );
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

MC_STATUS string_vr::set( string&& val )
{
    // According to the docs, failing validation doesn't cause the value
    // to not be saved
    // Must validate before push_back
    MC_STATUS validate_stat = validate_value( val );

    MC_STATUS ret = m_values.set( std::move( val ) );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = validate_stat;
    }
    else
    {
        // Do nothing. Will return set error
    }

    return ret;
}

MC_STATUS string_vr::set_next( string&& val )
{
    // According to the docs, failing validation doesn't cause the value
    // to not be saved
    // Must validate before push_back
    MC_STATUS validate_stat = validate_value( val );

    MC_STATUS ret = m_values.set_next( std::move( val) );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = validate_stat;
    }
    else
    {
        // Do nothing. Will return set error
    }

    return ret;
}

MC_STATUS string_vr::get( get_string_parms& val )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( val.first != nullptr )
    {
        const string* cur_val = nullptr;
        ret = m_values.get( cur_val );
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

MC_STATUS string_vr::get( const string*& val )
{
    return m_values.get( val );
}

MC_STATUS string_vr::get_next( get_string_parms& val )
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

MC_STATUS string_vr::get_next( const string*& val )
{
    return m_values.get_next( val );
}

} // namespace vrs
} // namespace fume
