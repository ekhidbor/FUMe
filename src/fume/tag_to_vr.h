#ifndef TAG_TO_DEFAULT_VR_H
#define TAG_TO_DEFAULT_VR_H
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
#include <unordered_map>

// local public
#include "mc3msg.h"

namespace fume
{

class data_dictionary;

struct tag_vr_packed
{
    uint32_t vr       :  6;
    uint32_t min_vals :  5;
    uint32_t max_vals : 16;
    uint32_t multiple :  5;
};

typedef std::unordered_map<uint32_t, tag_vr_packed> tag_to_vr_map;

tag_to_vr_map create_default_tag_vr_dict();

// For tags with conditional VR, return a tag_vr_packed struct. If tag is
// not a conditional VR, returns false
bool get_conditional_tag_vr( uint32_t               tag,
                             const data_dictionary& dict,
                             MC_VR&                 tag_vr );

} // namespace fume


#endif
