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
#include <array>
#include <unordered_map>
#include <algorithm>
#include <iterator>

// boost
#include "boost/bimap.hpp"

// local public
#include "mc3msg.h"
#include "mcstatus.h"

// local private
#include "fume/vr_field.h"

using std::array;
using std::unordered_map;
using std::copy;

using boost::bimap;

namespace std
{

template<>
struct hash<std::array<char, 2u> >
{
    typedef size_t result_type;
    typedef std::array<char, 2u> argument_type;

    result_type operator()( const argument_type& val )
    {
        const size_t v1 = val[0];
        const size_t v2 = val[1];

        return (v2 << 8) | v1;
    }
};

}

namespace fume
{

typedef unordered_map<int, uint8_t> vr_size_map_t;
typedef bimap<int, vr_value_t> vr_value_map_t;

static const vr_size_map_t::value_type VR_SIZE_VALUES[] =
{
    { AE,         2u },
    { AS,         2u },
    { CS,         2u },
    { DA,         2u },
    { DS,         2u },
    { DT,         2u },
    { IS,         2u },
    { LO,         2u },
    { LT,         2u },
    { PN,         2u },
    { SH,         2u },
    { ST,         2u },
    { TM,         2u },
    { UC,         4u },
    { UR,         4u },
    { UT,         4u },
    { UI,         2u },
    { SS,         2u },
    { US,         2u },
    { AT,         2u },
    { SL,         2u },
    { UL,         2u },
    { FL,         2u },
    { FD,         2u },
    { UNKNOWN_VR, 4u },
    { OB,         4u },
    { OW,         4u },
    { OD,         4u },
    { OF,         4u },
    { SQ,         4u },
    { OL,         4u }
};

// Disable "extra braces" warning message. The extra braces
// are unnecessary
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"

static const vr_value_map_t::value_type VR_VALUES[] =
{
    { AE,         { 'A', 'E' } },
    { AS,         { 'A', 'S' } },
    { CS,         { 'C', 'S' } },
    { DA,         { 'D', 'A' } },
    { DS,         { 'D', 'S' } },
    { DT,         { 'D', 'T' } },
    { IS,         { 'I', 'S' } },
    { LO,         { 'L', 'O' } },
    { LT,         { 'L', 'T' } },
    { PN,         { 'P', 'N' } },
    { SH,         { 'S', 'H' } },
    { ST,         { 'S', 'T' } },
    { TM,         { 'T', 'M' } },
    { UC,         { 'U', 'C' } },
    { UR,         { 'U', 'R' } },
    { UT,         { 'U', 'T' } },
    { UI,         { 'U', 'I' } },
    { SS,         { 'S', 'S' } },
    { US,         { 'U', 'S' } },
    { AT,         { 'A', 'T' } },
    { SL,         { 'S', 'L' } },
    { UL,         { 'U', 'L' } },
    { FL,         { 'F', 'L' } },
    { FD,         { 'F', 'D' } },
    { UNKNOWN_VR, { 'U', 'N' } },
    { OB,         { 'O', 'B' } },
    { OW,         { 'O', 'W' } },
    { OD,         { 'O', 'D' } },
    { OF,         { 'O', 'F' } },
    { SQ,         { 'S', 'Q' } },
    { OL,         { 'O', 'L' } }
};

#pragma GCC diagnostic pop

static const vr_value_map_t& vr_value_map()
{
    static const vr_value_map_t ret( begin( VR_VALUES ), end( VR_VALUES ) );
    return ret;
}

static const vr_size_map_t& vr_size_map()
{
    static const vr_size_map_t ret( begin( VR_SIZE_VALUES ),
                                    end( VR_SIZE_VALUES ) );
    return ret;
}

MC_STATUS get_vr_field_size( MC_VR vr, uint8_t& field_size )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    const vr_size_map_t::const_iterator itr = vr_size_map().find( vr );
    if( itr != vr_size_map().cend() )
    {
        field_size = itr->second;
        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_INVALID_VR_CODE;
    }

    return ret;
}

MC_STATUS get_vr_field_value( MC_VR vr, vr_value_t& value )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    const vr_value_map_t::left_map::const_iterator itr =
        vr_value_map().left.find( vr );
    if( itr != vr_value_map().left.end() )
    {
        copy( itr->second.cbegin(), itr->second.cend(), value.begin() );
        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_INVALID_VR_CODE;
    }

    return ret;
}

MC_STATUS get_vr_code( const vr_value_t value, MC_VR& vr )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    const vr_value_map_t::right_map::const_iterator itr =
        vr_value_map().right.find( value );
    if( itr != vr_value_map().right.end() )
    {
        vr = static_cast<MC_VR>( itr->second );
        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_INVALID_VR_CODE;
    }

    return ret;
}

}
