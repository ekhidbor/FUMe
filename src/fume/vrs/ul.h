#ifndef UL_H
#define UL_H
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

// local private
#include "fume/vrs/binary_vr.h"

namespace fume
{
namespace vrs
{

// Signed Short value representation
typedef binary_vr<uint32_t, UL> ul;

} // namespace vrs
} // namespace fume

#endif
