#ifndef OW_H
#define OW_H
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
#include "fume/vrs/other_vr.h"

namespace fume
{
namespace vrs
{

// Other Word value representations
typedef other_vr<uint16_t, OW> ow;

} // namespace vrs
} // namespace fume

#endif
