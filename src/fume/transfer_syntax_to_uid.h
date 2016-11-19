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

// local public
#include "mcstatus.h"
#include "mc3msg.h"

namespace fume
{

MC_STATUS get_transfer_syntax_from_enum( TRANSFER_SYNTAX syntax,
                                         char*           uid,
                                         int             uid_length );

MC_STATUS get_enum_from_transfer_syntax( const char*      uid,
                                         TRANSFER_SYNTAX& syntax );

} // namespace fume


#endif
