#ifndef VALUE_REPRESENTATION_TYPES_H
#define VALUE_REPRESENTATION_TYPES_H
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
#include <utility>

// local public
#include "mc3msg.h"

namespace fume
{

typedef std::pair<const void*, unsigned long> set_buf_parms;
typedef std::pair<      void*, unsigned long> get_buf_parms;

typedef std::pair<MC_UChar*, MC_size_t> get_ustring_parms;
typedef std::pair<char*,     MC_size_t> get_string_parms;

struct set_func_parms
{
    SetValueCallback callback;
    void*            callback_parm;
    int              message_id;
    unsigned long    tag;
};

struct get_func_parms
{
    GetValueCallback callback;
    void*            callback_parm;
    int              message_id;
    unsigned long    tag;
};

class value_representation;

} // namespace fume

#endif
