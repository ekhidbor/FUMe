#ifndef CALLBACK_IO_H
#define CALLBACK_IO_H
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

// local public
#include "mc3msg.h"

// local private

namespace fume
{

inline bool is_encapsulated( TRANSFER_SYNTAX syntax )
{
    return syntax != IMPLICIT_LITTLE_ENDIAN &&
           syntax != EXPLICIT_LITTLE_ENDIAN &&
           syntax != EXPLICIT_BIG_ENDIAN    &&
           syntax != DEFLATED_EXPLICIT_LITTLE_ENDIAN;
}

MC_STATUS validate_callback_output( const void* buffer, int size );

}


#endif
