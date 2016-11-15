#ifndef RECORD_TYPE_TO_STRING
#define RECORD_TYPE_TO_STRING
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
#include <string>

// boost
#include "boost/bimap.hpp"

// local public
#include "mc3media.h"

namespace fume
{

typedef boost::bimap<MC_DIR_RECORD_TYPE, std::string> record_type_map_t;
record_type_map_t create_record_type_to_string_map();

} // namespace fume


#endif
