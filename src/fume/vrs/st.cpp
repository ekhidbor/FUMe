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
#include <cstdlib>
#include <cstring>

// local private
#include "fume/vrs/st.h"

static constexpr size_t MAX_LT_LEN = 10240u;

namespace fume
{
namespace vrs
{

MC_STATUS st::validate_value( const char* val ) const
{
    MC_STATUS ret = MC_NORMAL_COMPLETION;

    const size_t val_len = strlen( val );
    if( val_len <= MAX_LT_LEN )
    {
        // TODO: better validation
        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_INVALID_VALUE_FOR_VR;
    }

    return ret;
}

} // namespace vrs
} // namespace fume
