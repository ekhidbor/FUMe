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

using boost::endian::native_to_big;
using boost::endian::native_to_big_inplace;
using boost::endian::native_to_little_inplace;

namespace fume
{

typedef char vr_field_val[4];
struct vr_field
{
    unsigned int size;
    vr_field_val field;
};

static const vr_field VR_FIELDS[] =
{
    // AE
    { 2u, { 'A', 'E', '\0', '\0' } },
    // AS
    { 2u, { 'A', 'S', '\0', '\0' } },
    // CS,
    { 2u, { 'C', 'S', '\0', '\0' } },
    // DA,
    { 2u, { 'D', 'A', '\0', '\0' } },
    // DS,
    { 2u, { 'D', 'S', '\0', '\0' } },
    // DT,
    { 2u, { 'D', 'T', '\0', '\0' } },
    // IS,
    { 2u, { 'I', 'S', '\0', '\0' } },
    // LO,
    { 2u, { 'L', 'O', '\0', '\0' } },
    // LT,
    { 2u, { 'L', 'T', '\0', '\0' } },
    // PN,
    { 2u, { 'P', 'N', '\0', '\0' } },
    // SH,
    { 2u, { 'S', 'H', '\0', '\0' } },
    // ST,
    { 2u, { 'S', 'T', '\0', '\0' } },
    // TM,
    { 2u, { 'T', 'M', '\0', '\0' } },
    // UC,
    { 4u, { 'U', 'C', '\0', '\0' } },
    // UR,
    { 4u, { 'U', 'R', '\0', '\0' } },
    // UT,
    { 4u, { 'U', 'T', '\0', '\0' } },
    // UI,
    { 2u, { 'U', 'I', '\0', '\0' } },
    // SS,
    { 2u, { 'S', 'S', '\0', '\0' } },
    // US,
    { 2u, { 'U', 'S', '\0', '\0' } },
    // AT,
    { 2u, { 'A', 'T', '\0', '\0' } },
    // SL,
    { 2u, { 'S', 'L', '\0', '\0' } },
    // UL,
    { 2u, { 'U', 'L', '\0', '\0' } },
    // FL,
    { 2u, { 'F', 'L', '\0', '\0' } },
    // FD,
    { 2u, { 'F', 'D', '\0', '\0' } },
    // UNKNOWN_VR,
    { 4u, { 'U', 'N', '\0', '\0' } },
    // OB,
    { 4u, { 'O', 'B', '\0', '\0' } },
    // OW,
    { 4u, { 'O', 'W', '\0', '\0' } },
    // OD,
    { 4u, { 'O', 'D', '\0', '\0' } },
    // OF,
    { 4u, { 'O', 'F', '\0', '\0' } },
    // SQ,
    { 4u, { 'S', 'Q', '\0', '\0' } },
    // OL
    { 4u, { 'O', 'L', '\0', '\0' } }
};

template<class T>
static MC_STATUS swap_and_write( tx_stream& stream, T val )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    switch( stream.transfer_syntax() )
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

MC_STATUS tx_stream::write_vr( MC_VR vr )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( vr_is_valid( vr ) == true )
    {
        const vr_field& field( VR_FIELDS[vr] );
        ret = write( field.field, field.size );
    }
    else
    {
        ret = MC_INVALID_VR_CODE;
    }

    return ret;
}

MC_STATUS tx_stream::write_tag( uint32_t tag )
{
    const uint16_t group = static_cast<uint16_t>( tag >> 16u );
    const uint16_t element = static_cast<uint16_t>( tag & 0xFFFFu );

    // Write the group number first
    MC_STATUS ret = write_val( group );
    if( ret == MC_NORMAL_COMPLETION )
    {
        // Then write the element number
        ret = write_val( element );
    }
    else
    {
        // Do nothing. Will return error from write_val
    }

    return ret;
}

MC_STATUS tx_stream::write_val( int8_t val )
{
    return swap_and_write( *this, val );
}

MC_STATUS tx_stream::write_val( uint8_t val )
{
    return swap_and_write( *this, val );
}

MC_STATUS tx_stream::write_val( int16_t val )
{
    return swap_and_write( *this, val );
}

MC_STATUS tx_stream::write_val( uint16_t val )
{
    return swap_and_write( *this, val );
}

MC_STATUS tx_stream::write_val( int32_t val )
{
    return swap_and_write( *this, val );
}

MC_STATUS tx_stream::write_val( uint32_t val )
{
    return swap_and_write( *this, val );
}

MC_STATUS tx_stream::write_val( float val )
{
    // TODO: endian swap
    static_assert( sizeof(float) == 4, "float must be 32-bits" );
    return write( &val, sizeof(val) );
}

MC_STATUS tx_stream::write_val( double val )
{
    // TODO: endian swap
    static_assert( sizeof(double) == 8, "double must be 64-bits" );
    return write( &val, sizeof(val) );
}

}
