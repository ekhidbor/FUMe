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
#include <vector>
#include <limits>
#include <algorithm>

// local public
#include "mcstatus.h"
#include "mc3msg.h"
#include "diction.h"

// local private
#include "fume/rx_stream.h"
#include "fume/vr_data_parser.h"
#include "fume/encapsulated_value_sink.h"

using std::vector;
using std::numeric_limits;
using std::min;

namespace fume
{

static MC_STATUS read_raw_value( rx_stream&               source,
                                 encapsulated_value_sink& dest,
                                 uint32_t                 vr_length );

static MC_STATUS read_encapsulated_frames( rx_stream&               source,
                                           TRANSFER_SYNTAX          syntax,
                                           encapsulated_value_sink& dest );

static MC_STATUS read_offset_table( rx_stream&               source,
                                    TRANSFER_SYNTAX          syntax,
                                    encapsulated_value_sink& dest );

static MC_STATUS read_encapsulated_value( rx_stream&               source,
                                          TRANSFER_SYNTAX          syntax,
                                          encapsulated_value_sink& dest );

MC_STATUS read_vr_data( rx_stream&               source,
                        TRANSFER_SYNTAX          syntax,
                        encapsulated_value_sink& dest )
{
    uint32_t vr_length = 0;
    MC_STATUS ret = source.read_val( vr_length, syntax );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = dest.provide_value_length( vr_length, syntax );
        if( ret == MC_NORMAL_COMPLETION )
        {
            if( vr_length == numeric_limits<uint32_t>::max() )
            {
                ret = read_encapsulated_value( source, syntax, dest );
            }
            else
            {
                ret = read_raw_value( source, dest, vr_length );
            }

            if( ret == MC_NORMAL_COMPLETION )
            {
                ret = dest.finalize();
            }
            else
            {
                // Do not finalize. Will return error
            }
        }
    }
    else
    {
        // Do nothing. Will return error from get_value_length
    }

    return ret;
}

MC_STATUS read_raw_value( rx_stream&               source,
                          encapsulated_value_sink& dest,
                          uint32_t                 vr_length )
{
    MC_STATUS ret = MC_NORMAL_COMPLETION;

    uint8_t buffer[1024];
    uint32_t bytes_written = 0;
    while( ret == MC_NORMAL_COMPLETION && bytes_written < vr_length )
    {
        const uint32_t bytes_remaining = vr_length - bytes_written;
        const uint32_t bytes_read = min( static_cast<uint32_t>( sizeof(buffer) ),
                                         bytes_remaining );
        ret = source.read( buffer, bytes_read );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = dest.write( buffer, bytes_read );
        }
        else
        {
            // Do nothing. Will return error
        }
    }

    return ret;
}

MC_STATUS read_encapsulated_frame( rx_stream&               source,
                                   TRANSFER_SYNTAX          syntax,
                                   encapsulated_value_sink& dest )
{
    uint32_t tag = 0;
    MC_STATUS ret = source.read_tag( tag, syntax );
    if( ret == MC_NORMAL_COMPLETION )
    {
        if( tag == MC_ATT_ITEM )
        {
            uint32_t length = 0;
            ret = source.read_val( length, syntax );
            if( ret == MC_NORMAL_COMPLETION )
            {
                ret = dest.start_of_frame( length, syntax );
                if( ret == MC_NORMAL_COMPLETION )
                {
                    ret = read_raw_value( source, dest, length );
                }
                else
                {
                    // Do nothing. Will return error
                }
            }
            else
            {
                // Do nothing. Will return error
            }
        }
        else if( tag == MC_ATT_SEQUENCE_DELIMITATION_ITEM )
        {
            uint32_t length = 0;
            ret = source.read_val( length, syntax );
            if( ret == MC_NORMAL_COMPLETION )
            {
                ret = dest.end_of_sequence( syntax );
                if( ret == MC_NORMAL_COMPLETION )
                {
                    ret = MC_NO_MORE_VALUES;
                }
                else
                {
                    // Do nothing. Will return error
                }
            }
            else
            {
                // Do nothing. WIll return error
            }
        }
        else
        {
            ret = MC_MISSING_DELIMITER;
        }
    }
    else
    {
        // Do nothing. Will return error
    }

    return ret;
}

MC_STATUS read_encapsulated_frames( rx_stream&               source,
                                    TRANSFER_SYNTAX          syntax,
                                    encapsulated_value_sink& dest )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    do
    {
        ret = read_encapsulated_frame( source, syntax, dest );
    }
    while( ret == MC_NORMAL_COMPLETION );

    return ret;
}

MC_STATUS read_encapsulated_value( rx_stream&               source,
                                   TRANSFER_SYNTAX          syntax,
                                   encapsulated_value_sink& dest )
{
    MC_STATUS ret = read_offset_table( source, syntax, dest );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = read_encapsulated_frames( source, syntax, dest );
    }
    else
    {
        // Do nothing. Will return error
    }

    return ret;
}

MC_STATUS read_offset_table( rx_stream&               source,
                             TRANSFER_SYNTAX          syntax,
                             encapsulated_value_sink& dest )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    uint32_t tag = 0;
    ret = source.read_tag( tag, syntax );
    if( ret == MC_NORMAL_COMPLETION )
    {
        if( tag == MC_ATT_ITEM )
        {
            uint32_t length = 0;
            ret = source.read_val( length, syntax );
            if( ret == MC_NORMAL_COMPLETION )
            {
                if( length % sizeof(uint32_t) == 0 )
                {
                    const uint32_t num_elems = length / sizeof(uint32_t);
                    // Make sure the vector has at least one element so the
                    // buffer we pass into the function is valid even if there
                    // is no offset table
                    vector<uint32_t> offset_table( min( 1u, num_elems ) );
                    ret = MC_NORMAL_COMPLETION;
                    for( uint32_t i = 0;
                         (ret == MC_NORMAL_COMPLETION) && (i < num_elems);
                         ++i )
                    {
                        ret = source.read_val( offset_table[i], syntax );
                    }

                    if( ret == MC_NORMAL_COMPLETION )
                    {
                        ret = dest.provide_offset_table( offset_table.data(),
                                                         length,
                                                         syntax );
                    }
                    else
                    {
                        // Do nothing. Will return error
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
        }
        else
        {
            ret = MC_MISSING_DELIMITER;
        }
    }
    else
    {
        // Do nothing. Will return error
    }

    return ret;
}

}
