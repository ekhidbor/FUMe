#ifndef CONFIGURATION_MAPS_H
#define CONFIGURATION_MAPS_H
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
#include <unordered_map>
#include <memory>

// local public
#include "mc3msg.h"

namespace fume
{

typedef std::unordered_map<int, std::string> string_parm_map_t;
typedef std::unordered_map<int, int        > int_parm_map_t;
typedef std::unordered_map<int, long       > long_parm_map_t;
typedef std::unordered_map<int, bool       > bool_parm_map_t;

struct config_maps
{
    string_parm_map_t strings;
    int_parm_map_t    ints;
    long_parm_map_t   longs;
    bool_parm_map_t   bools;

    config_maps( config_maps&& rhs )
        : strings( std::move( rhs.strings ) ),
          ints( std::move( rhs.ints ) ),
          longs( std::move( rhs.longs ) ),
          bools( std::move( rhs.bools ) )
    {
    }

    config_maps() = default;
};

config_maps create_config_maps();

} // namespace fume


#endif
