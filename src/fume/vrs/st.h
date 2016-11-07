#ifndef ST_H
#define ST_H
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

// Short Text value representation
// TODO: support for Unicode
class st final : public string_vr
{
public:
    st( unsigned int min_vals, unsigned int max_vals, unsigned int multiple )
        : string_vr( min_vals, max_vals, multiple )
    {
    }
    ~st()
    {
    }

    virtual MC_VR vr() const override final
    {
        return ST;
    }

    // TODO: set_next needs to do something different since this is
    // a single-valed VR

protected:
    virtual MC_STATUS validate_value( const char* val ) const override final;
};

} // namespace vrs
} // namespace fume

#endif
