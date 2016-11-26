#ifndef VR_FACTORY_H
#define VR_FACTORY_H
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
#include <memory>

// local public
#include "mc3msg.h"

namespace fume
{

class value_representation;

std::unique_ptr<value_representation> create_vr( MC_VR        vr,
                                                 unsigned int min_vals,
                                                 unsigned int max_vals,
                                                 unsigned int multiple );

} // namespace fume


#endif
