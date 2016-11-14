#ifndef PN_H
#define PN_H
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

bool validate_pn( const std::string& val );

// Person Name value representation
// TODO: support for Unicode
class pn final : public string_vr
{
public:
    pn( unsigned int min_vals, unsigned int max_vals, unsigned int multiple )
        : string_vr( min_vals, max_vals, multiple )
    {
    }
    ~pn()
    {
    }

    virtual MC_VR vr() const override final
    {
        return PN;
    }

    virtual std::unique_ptr<value_representation> clone() const override final
    {
        return std::unique_ptr<value_representation>( new pn( *this ) );
    }

protected:
    virtual MC_STATUS validate_value( const std::string& val ) const override final
    {
        return validate_pn( val ) ? MC_NORMAL_COMPLETION : MC_INVALID_VALUE_FOR_VR;
    }

private:
    pn( const pn& rhs )
        : string_vr( rhs )
    {
    }
};

} // namespace vrs
} // namespace fume

#endif
