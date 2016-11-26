#ifndef DATA_DICTIONARY_TYPES_H
#define DATA_DICTIONARY_TYPES_H
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
#include <memory>
#include <map>
#include <unordered_map>

// local public
#include "mcstatus.h"
#include "mc3msg.h"

// local private

namespace fume
{

class value_representation;

typedef std::unique_ptr<value_representation> unique_vr_ptr;
typedef std::map<uint32_t, unique_vr_ptr>     value_dict;
typedef value_dict::value_type                value_dict_item;
typedef value_dict::const_iterator            dictionary_iter;

typedef std::unordered_map<uint32_t, MC_VR> nonstandard_vr_dict;

class data_dictionary;

}

#endif
