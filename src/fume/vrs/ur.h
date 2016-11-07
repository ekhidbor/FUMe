#ifndef UR_H
#define UR_H
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

// Universal Resource Identifier value representation
class ur final : public string_vr
{
public:
    ur()
        : string_vr( 1u, 1u, 1u )
    {
    }
    ~ur()
    {
    }

    virtual MC_VR vr() const override final
    {
        return UR;
    }

protected:
    virtual MC_STATUS validate_value( const char* val ) const override final;
};

} // namespace vrs
} // namespace fume

#endif
