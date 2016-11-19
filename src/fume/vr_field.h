#ifndef VR_FIELD_H
#define VR_FIELD_H
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

// boost

// local public
#include "mc3msg.h"

namespace fume
{

typedef std::array<char, 2u> vr_value_t;

MC_STATUS get_vr_field_size( MC_VR vr, uint8_t& field_size );
MC_STATUS get_vr_field_value( MC_VR vr, vr_value_t& value );
MC_STATUS get_vr_code( const vr_value_t value, MC_VR& vr );

inline bool vr_is_valid( MC_VR vr )
{
    return vr >= AE && vr <= OL;
}

} // namespace fume


#endif
