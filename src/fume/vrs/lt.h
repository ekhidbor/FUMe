#ifndef LT_H
#define LT_H
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

// local private
#include "fume/vrs/string_vr.h"

namespace fume
{
namespace vrs
{

bool validate_lt( const std::string& val );

// Long Text value representation
// TODO: support for Unicode
class lt final : public string_vr
{
public:
    lt()
        : string_vr( 1u, 1u, 1u )
    {
    }
    ~lt()
    {
    }

    virtual MC_VR vr() const override final
    {
        return LT;
    }

    // TODO: set_next needs to do something different since this is
    // a single-valed VR

protected:
    virtual MC_STATUS validate_value( const std::string& val ) const override final
    {
        return validate_lt( val ) ? MC_NORMAL_COMPLETION : MC_INVALID_VALUE_FOR_VR;
    }
};

} // namespace vrs
} // namespace fume

#endif
