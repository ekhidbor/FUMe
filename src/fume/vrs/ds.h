#ifndef DS_H
#define DS_H
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

bool validate_ds( const std::string& val );

// Decimal String value representation
class ds final : public string_vr
{
public:
    ds( unsigned int min_vals, unsigned int max_vals, unsigned int multiple )
        : string_vr( min_vals, max_vals, multiple )
    {
    }
    ~ds()
    {
    }

// value_representation -- modifiers
public:
    virtual MC_STATUS set( double val ) override final
    {
        return cast_and_set_string( val );
    }
    virtual MC_STATUS set( float val ) override final
    {
        return cast_and_set_string( val );
    }
    virtual MC_STATUS set( int val ) override final
    {
        return cast_and_set_string( val );
    }
    virtual MC_STATUS set( short val ) override final
    {
        return cast_and_set_string( val );
    }
    virtual MC_STATUS set( long val ) override final
    {
        return cast_and_set_string( val );
    }
    virtual MC_STATUS set( unsigned int val ) override final
    {
        return cast_and_set_string( val );
    }
    virtual MC_STATUS set( unsigned short val ) override final
    {
        return cast_and_set_string( val );
    }
    virtual MC_STATUS set( unsigned long val ) override final
    {
        return cast_and_set_string( val );
    }

    virtual MC_STATUS set_next( double val ) override final
    {
        return cast_and_set_next_string( val );
    }
    virtual MC_STATUS set_next( float val ) override final
    {
        return cast_and_set_next_string( val );
    }
    virtual MC_STATUS set_next( int val ) override final
    {
        return cast_and_set_next_string( val );
    }
    virtual MC_STATUS set_next( short val ) override final
    {
        return cast_and_set_next_string( val );
    }
    virtual MC_STATUS set_next( long val ) override final
    {
        return cast_and_set_next_string( val );
    }
    virtual MC_STATUS set_next( unsigned int val ) override final
    {
        return cast_and_set_next_string( val );
    }
    virtual MC_STATUS set_next( unsigned short val ) override final
    {
        return cast_and_set_next_string( val );
    }
    virtual MC_STATUS set_next( unsigned long val ) override final
    {
        return cast_and_set_next_string( val );
    }

// value_representation -- accessors
public:
    virtual MC_STATUS get( double& val ) const override final
    {
        return cast_and_get_string( val );
    }
    virtual MC_STATUS get( float& val ) const override final
    {
        return cast_and_get_string( val );
    }
    virtual MC_STATUS get( int& val ) const override final
    {
        return cast_and_get_string( val );
    }
    virtual MC_STATUS get( short& val ) const override final
    {
        return cast_and_get_string( val );
    }
    virtual MC_STATUS get( long& val ) const override final
    {
        return cast_and_get_string( val );
    }
    virtual MC_STATUS get( unsigned int& val ) const override final
    {
        return cast_and_get_string( val );
    }
    virtual MC_STATUS get( unsigned short& val ) const override final
    {
        return cast_and_get_string( val );
    }
    virtual MC_STATUS get( unsigned long& val ) const override final
    {
        return cast_and_get_string( val );
    }

    virtual MC_STATUS get_next( double& val ) const override final
    {
        return cast_and_get_next_string( val );
    }
    virtual MC_STATUS get_next( float& val ) const override final
    {
        return cast_and_get_next_string( val );
    }
    virtual MC_STATUS get_next( int& val ) const override final
    {
        return cast_and_get_next_string( val );
    }
    virtual MC_STATUS get_next( short& val ) const override final
    {
        return cast_and_get_next_string( val );
    }
    virtual MC_STATUS get_next( long& val ) const override final
    {
        return cast_and_get_next_string( val );
    }
    virtual MC_STATUS get_next( unsigned int& val ) const override final
    {
        return cast_and_get_next_string( val );
    }
    virtual MC_STATUS get_next( unsigned short& val ) const override final
    {
        return cast_and_get_next_string( val );
    }
    virtual MC_STATUS get_next( unsigned long& val ) const override final
    {
        return cast_and_get_next_string( val );
    }

    virtual MC_VR vr() const override final
    {
        return DS;
    }

    virtual std::unique_ptr<value_representation> clone() const override final
    {
        return std::unique_ptr<value_representation>( new ds( *this ) );
    }

protected:
    virtual MC_STATUS validate_value( const std::string& val ) const override final
    {
        return validate_ds( val ) ? MC_NORMAL_COMPLETION : MC_INVALID_VALUE_FOR_VR;
    }

private:
    ds( const ds& rhs )
        : string_vr( rhs )
    {
    }
};

} // namespace vrs
} // namespace fume

#endif
