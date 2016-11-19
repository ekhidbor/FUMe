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

// boost

// local public
#include "mc3media.h"
#include "mcstatus.h"

namespace fume
{

MC_STATUS get_record_type_from_enum( MC_DIR_RECORD_TYPE record_type,
                                     char*              val,
                                     int                val_length );

MC_STATUS get_enum_from_record_type( const char*         val,
                                     MC_DIR_RECORD_TYPE& record_type );

} // namespace fume


#endif
