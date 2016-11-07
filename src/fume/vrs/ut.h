#ifndef UT_H
#define UT_H
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

// Unlimited Text value representation
// TODO: support for Unicode
class ut final : public string_vr
{
public:
    ut()
        : string_vr( 1u, 1u, 1u )
    {
    }
    ~ut()
    {
    }

    virtual MC_VR vr() const override final
    {
        return UT;
    }

    // TODO: set_next needs to do something different since this is
    // a single-valed VR

protected:
    virtual MC_STATUS validate_value( const char* val ) const override final;
};

} // namespace vrs
} // namespace fume

#endif
