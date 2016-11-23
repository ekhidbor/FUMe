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
#include <limits>

// local private
#include "fume/vrs/at.h"
#include "fume/tx_stream.h"
#include "fume/rx_stream.h"

using std::numeric_limits;

namespace fume
{
namespace vrs
{

MC_STATUS at::from_stream( rx_stream& stream, TRANSFER_SYNTAX syntax )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;
    uint32_t value_length = 0;
    value_list_t tmp_values;

    if( syntax == IMPLICIT_LITTLE_ENDIAN )
    {
        ret = stream.read_val( value_length, syntax );
    }
    else
    {
        uint16_t value_length_16u;
        ret = stream.read_val( value_length_16u, syntax );
        value_length = value_length_16u;
    }

    if( ret == MC_NORMAL_COMPLETION )
    {
        if( value_length % sizeof(uint32_t) == 0 )
        {
            const uint32_t num_items = value_length / sizeof(uint32_t);
            for( uint32_t i = 0; i < num_items && ret == MC_NORMAL_COMPLETION; ++i )
            {
                uint32_t val = 0;
                ret = stream.read_val( val, syntax );

                if( ret == MC_NORMAL_COMPLETION )
                {
                    ret = tmp_values.set_next( val );
                }
            }

            if( ret == MC_NORMAL_COMPLETION )
            {
                // Only update values if everything succeeded
                m_values.swap( tmp_values );
            }
        }
        else
        {
            ret = MC_INVALID_LENGTH_FOR_VR;
        }
    }
    else
    {
        // Do nothing. Will return error
    }

    return ret;
}

MC_STATUS at::to_stream( tx_stream& stream, TRANSFER_SYNTAX syntax )
{
    const uint32_t value_size = m_values.size();
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( syntax != IMPLICIT_LITTLE_ENDIAN )
    {
        // vr_value_list ensures size stays under this limit
        assert( value_size < numeric_limits<uint16_t>::max() );
        ret = stream.write_val( static_cast<uint16_t>( value_size ), syntax );
    }
    else
    {
        ret = stream.write_val( value_size, syntax );
    }

    if( ret == MC_NORMAL_COMPLETION && m_values.is_null() == false )
    {
        for( typename value_list_t::const_iterator itr = m_values.cbegin();
             ret == MC_NORMAL_COMPLETION && itr != m_values.cend();
             ++itr )
        {
            ret = stream.write_tag( *itr, syntax );
        }
    }
    else
    {
        // Do nothing. Will return error if write_val failed or
        // MC_NORMAL_COMPLETION if zero length value
    }

    return ret;
}

} // namespace vrs
} // namespace fume
