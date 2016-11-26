#ifndef FILE_OBJECT_IO_H
#define FILE_OBJECT_IO_H
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
#include "mcstatus.h"
#include "mc3media.h"

namespace fume
{

class tx_stream;
class rx_stream;
class file_object;

MC_STATUS write_file( file_object&      file,
                      int               app_id,
                      int               alignment,
                      void*             user_info,
                      WriteFileCallback callback );

MC_STATUS write_file( file_object&      file,
                      int               alignment,
                      void*             user_info,
                      WriteFileCallback callback );

MC_STATUS write_file( tx_stream& stream, file_object& file, int app_id );

MC_STATUS open_file( file_object&     file,
                     int              app_id,
                     void*            user_info,
                     ReadFileCallback callback );

MC_STATUS open_file_upto( file_object&     file,
                          int              app_id,
                          uint32_t         end_tag,
                          uint64_t&        offset,
                          void*            user_info,
                          ReadFileCallback callback );

}

#endif
