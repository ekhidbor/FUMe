#ifndef TRANSFER_SYNTAX_TO_UID
#define TRANSFER_SYNTAX_TO_UID
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
#include "mc3msg.h"

namespace fume
{

typedef boost::bimap<TRANSFER_SYNTAX, std::string> transfer_syntax_map_t;
transfer_syntax_map_t create_transfer_syntax_to_uid_map();

} // namespace fume


#endif
