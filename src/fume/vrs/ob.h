#ifndef OB_H
#define OB_H
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

// Other Byte value representations
// TODO: derive ob from other_vr<uint8_t, OB> and also make it implemente
// tx_stream and rx_stream to enable implementation of
// MC_Set_(Next)?_Encapsulated_Value_From_Function and
// MC_Get_(Next)?_Encapsulated_Value_From_Function
typedef other_vr<uint8_t, OB> ob;

} // namespace vrs
} // namespace fume

#endif
