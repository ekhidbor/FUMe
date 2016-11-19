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
#include "fume/tx_stream.h"
#include "fume/library_context.h"
#include "fume/vr_field.h"

using boost::endian::native_to_big;
using boost::endian::native_to_big_inplace;
using boost::endian::native_to_little_inplace;

namespace fume
{

template<class T>
static MC_STATUS swap_and_write( tx_stream&      stream,
                                 TRANSFER_SYNTAX syntax,
                                 T               val )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    switch( syntax )
    {
        case EXPLICIT_BIG_ENDIAN:
        case IMPLICIT_BIG_ENDIAN:
            native_to_big_inplace( val );
            ret = stream.write( &val, sizeof(val) );
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
            native_to_little_inplace( val );
            ret = stream.write( &val, sizeof(val) );
            break;
        case INVALID_TRANSFER_SYNTAX:
        default:
            ret = MC_INVALID_TRANSFER_SYNTAX;
            break;
    }

    return ret;
}

MC_STATUS tx_stream::write_vr( MC_VR vr, TRANSFER_SYNTAX syntax )
{
    vr_value_t vr_value;

    MC_STATUS ret = get_vr_field_value( vr, vr_value );
    if( ret == MC_NORMAL_COMPLETION )
    {
        // Don't write transfer syntax if implicit little endian
        if( syntax != IMPLICIT_LITTLE_ENDIAN )
        {
            uint8_t vr_size = 0;
            ret = get_vr_field_size( vr, vr_size );
            if( ret == MC_NORMAL_COMPLETION )
            {
                ret = write( vr_value.data(), vr_value.size() );
                if( vr_size > vr_value.size() )
                {
                    uint16_t extra = 0;
                    write_val( extra, syntax );
                }
                else
                {
                    // Do nothing
                }
            }
            else
            {
                // Do nothing. Will return error
            }
        }
        else
        {
            ret = MC_NORMAL_COMPLETION;
        }
    }
    else
    {
        // Do nothing. Will return error
    }

    return ret;
}

MC_STATUS tx_stream::write_tag( uint32_t tag, TRANSFER_SYNTAX syntax )
{
    const uint16_t group = static_cast<uint16_t>( tag >> 16u );
    const uint16_t element = static_cast<uint16_t>( tag & 0xFFFFu );

    // Write the group number first
    MC_STATUS ret = write_val( group, syntax );
    if( ret == MC_NORMAL_COMPLETION )
    {
        // Then write the element number
        ret = write_val( element, syntax );
    }
    else
    {
        // Do nothing. Will return error from write_val
    }

    return ret;
}

MC_STATUS tx_stream::write_val( int8_t val, TRANSFER_SYNTAX syntax )
{
    return swap_and_write( *this, syntax, val );
}

MC_STATUS tx_stream::write_val( uint8_t val, TRANSFER_SYNTAX syntax )
{
    return swap_and_write( *this, syntax, val );
}

MC_STATUS tx_stream::write_val( int16_t val, TRANSFER_SYNTAX syntax )
{
    return swap_and_write( *this, syntax, val );
}

MC_STATUS tx_stream::write_val( uint16_t val, TRANSFER_SYNTAX syntax )
{
    return swap_and_write( *this, syntax, val );
}

MC_STATUS tx_stream::write_val( int32_t val, TRANSFER_SYNTAX syntax )
{
    return swap_and_write( *this, syntax, val );
}

MC_STATUS tx_stream::write_val( uint32_t val, TRANSFER_SYNTAX syntax )
{
    return swap_and_write( *this, syntax, val );
}

MC_STATUS tx_stream::write_val( float val, TRANSFER_SYNTAX syntax )
{
    // TODO: endian swap
    static_assert( sizeof(float) == 4, "float must be 32-bits" );
    return write( &val, sizeof(val) );
}

MC_STATUS tx_stream::write_val( double val, TRANSFER_SYNTAX syntax )
{
    // TODO: endian swap
    static_assert( sizeof(double) == 8, "double must be 64-bits" );
    return write( &val, sizeof(val) );
}

}
