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

// boost
#include "boost/endian/conversion.hpp"

// local public
#include "mc3msg.h"

// local private
#include "fume/rx_stream.h"
#include "fume/vr_field.h"

using boost::endian::big_to_native_inplace;
using boost::endian::little_to_native_inplace;

namespace fume
{

template<class T>
static MC_STATUS wire_to_host( TRANSFER_SYNTAX syntax,
                               T&              val )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    switch( syntax )
    {
        case EXPLICIT_BIG_ENDIAN:
        case IMPLICIT_BIG_ENDIAN:
            big_to_native_inplace( val );
            ret = MC_NORMAL_COMPLETION;
            break;
        // All the encapsulated transfer syntaxes are
        // in little endian
        case IMPLICIT_LITTLE_ENDIAN:
        case EXPLICIT_LITTLE_ENDIAN:
        case DEFLATED_EXPLICIT_LITTLE_ENDIAN:
        case RLE:
        case JPEG_BASELINE:
        case JPEG_EXTENDED_2_4:
        case JPEG_EXTENDED_3_5:
        case JPEG_SPEC_NON_HIER_6_8:
        case JPEG_SPEC_NON_HIER_7_9:
        case JPEG_FULL_PROG_NON_HIER_10_12:
        case JPEG_FULL_PROG_NON_HIER_11_13:
        case JPEG_LOSSLESS_NON_HIER_14:
        case JPEG_LOSSLESS_NON_HIER_15:
        case JPEG_EXTENDED_HIER_16_18:
        case JPEG_EXTENDED_HIER_17_19:
        case JPEG_SPEC_HIER_20_22:
        case JPEG_SPEC_HIER_21_23:
        case JPEG_FULL_PROG_HIER_24_26:
        case JPEG_FULL_PROG_HIER_25_27:
        case JPEG_LOSSLESS_HIER_28:
        case JPEG_LOSSLESS_HIER_29:
        case JPEG_LOSSLESS_HIER_14:
        case JPEG_2000_LOSSLESS_ONLY:
        case JPEG_2000:
        case JPEG_LS_LOSSLESS:
        case JPEG_LS_LOSSY:
        case MPEG2_MPML:
        case PRIVATE_SYNTAX_1:
        case PRIVATE_SYNTAX_2:
        case JPEG_2000_MC_LOSSLESS_ONLY:
        case JPEG_2000_MC:
        case MPEG2_MPHL:
        case MPEG4_AVC_H264_HP_LEVEL_4_1:
        case MPEG4_AVC_H264_BDC_HP_LEVEL_4_1:
        case JPIP_REFERENCED:
        case JPIP_REFERENCED_DEFLATE:
            little_to_native_inplace( val );
            ret = MC_NORMAL_COMPLETION;
        case INVALID_TRANSFER_SYNTAX:
        default:
            ret = MC_INVALID_TRANSFER_SYNTAX;
            break;
    }

    return ret;
}

template<class T>
static MC_STATUS read_and_swap( rx_stream&      stream,
                                TRANSFER_SYNTAX syntax,
                                T&              val )
{
    MC_STATUS ret = stream.read( &val, sizeof(val) );

    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = wire_to_host( syntax, val );
    }
    else
    {
        // Do nothing. Will return error
    }

    return ret;
}

MC_STATUS rx_stream::read_vr( MC_VR& vr, TRANSFER_SYNTAX syntax )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    // Don't read transfer syntax if implicit little endian
    if( syntax != IMPLICIT_LITTLE_ENDIAN )
    {
        vr_value_t vr_value;
        ret = read( vr_value.data(), vr_value.size() );
        if( ret == MC_NORMAL_COMPLETION )
        {
            MC_VR tmp_vr = UNKNOWN_VR;
            uint8_t vr_size = 0;
            ret = get_vr_code( vr_value, tmp_vr, vr_size );
            if( ret == MC_NORMAL_COMPLETION )
            {
                if( vr_size > vr_value.size() )
                {
                    uint16_t extra = 0;
                    ret = read_val( extra, syntax );
                }
                else
                {
                    // Do nothing. 2-byte VR field
                }
            }
            else
            {
                // Do nothing. WIll return get_vr_code error
            }
        }
        else
        {
            // Do nothing. Will return read error
        }
    }
    else
    {
        // Indicate error since this functino shouldn't be called for
        // implicit transfer syntaxes
        ret = MC_INVALID_TRANSFER_SYNTAX;
    }

    return ret;
}

MC_STATUS rx_stream::read_tag( uint32_t& tag, TRANSFER_SYNTAX syntax )
{
    uint16_t vals[2] = { 0 };

    // Read the group number first
    MC_STATUS ret = read( vals, sizeof(vals) );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = wire_to_host( syntax, vals[0] );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = wire_to_host( syntax, vals[1] );
            if( ret == MC_NORMAL_COMPLETION )
            {
                tag = (static_cast<uint32_t>( vals[0] ) << 16u) |
                      static_cast<uint32_t>( vals[1] );
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
    else
    {
        // Do nothing. Will return error from write_val
    }

    return ret;
}

MC_STATUS rx_stream::peek_tag( uint32_t& tag, TRANSFER_SYNTAX syntax )
{
    uint16_t vals[2] = { 0 };

    // Read the group number first
    MC_STATUS ret = peek( vals, sizeof(vals) );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = wire_to_host( syntax, vals[0] );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = wire_to_host( syntax, vals[1] );
            if( ret == MC_NORMAL_COMPLETION )
            {
                tag = (static_cast<uint32_t>( vals[0] ) << 16u) |
                      static_cast<uint32_t>( vals[1] );
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
    else
    {
        // Do nothing. Will return error from write_val
    }

    return ret;
}

MC_STATUS rx_stream::read_val( int8_t& val, TRANSFER_SYNTAX syntax )
{
    // Single byte -- no need to try byte swap
    return read( &val, sizeof(val) );
}

MC_STATUS rx_stream::read_val( uint8_t& val, TRANSFER_SYNTAX syntax )
{
    // Single byte -- no need to try byte swap
    return read( &val, sizeof(val) );
}

MC_STATUS rx_stream::read_val( char& val, TRANSFER_SYNTAX syntax )
{
    // Single byte -- no need to try byte swap
    return read( &val, sizeof(val) );
}

MC_STATUS rx_stream::read_val( int16_t& val, TRANSFER_SYNTAX syntax )
{
    return read_and_swap( *this, syntax, val );
}

MC_STATUS rx_stream::read_val( uint16_t& val, TRANSFER_SYNTAX syntax )
{
    return read_and_swap( *this, syntax, val );
}

MC_STATUS rx_stream::read_val( int32_t& val, TRANSFER_SYNTAX syntax )
{
    return read_and_swap( *this, syntax, val );
}

MC_STATUS rx_stream::read_val( uint32_t& val, TRANSFER_SYNTAX syntax )
{
    return read_and_swap( *this, syntax, val );
}

MC_STATUS rx_stream::read_val( float& val, TRANSFER_SYNTAX syntax )
{
    // TODO: endian swap
    static_assert( sizeof(float) == 4, "float must be 32-bits" );
    return read( &val, sizeof(val) );
}

MC_STATUS rx_stream::read_val( double& val, TRANSFER_SYNTAX syntax )
{
    // TODO: endian swap
    static_assert( sizeof(double) == 8, "double must be 64-bits" );
    return read( &val, sizeof(val) );
}

}
