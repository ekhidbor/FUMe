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

// local private
#include "fume/dicomdir_object.h"

namespace fume
{

dicomdir_object::dicomdir_object( int         id,
                                  const char* filename,
                                  bool        created_empty )
    : file_object( id, filename, created_empty )
{

}

}
