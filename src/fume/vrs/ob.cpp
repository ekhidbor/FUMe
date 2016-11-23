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
#include <limits>

// local private
#include "fume/vrs/ob.h"

using std::numeric_limits;

namespace fume
{
namespace vrs
{

MC_STATUS ob::write( const void* buffer, uint32_t buffer_bytes )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( data_size() + buffer_bytes < numeric_limits<uint32_t>::max() )
    {
        const uint8_t* data = static_cast<const uint8_t*>( buffer );
        append( data, data + buffer_bytes );

        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_VALUE_TOO_LARGE;
    }

    return ret;
}

} // namespace vrs
} // namespace fume
