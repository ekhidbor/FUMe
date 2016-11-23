#ifndef STRING_VR_H
#define STRING_VR_H
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
#include <string>
#include <utility>

// local private
#include "fume/value_representation.h"
#include "fume/value_conversion.h"
#include "fume/vrs/vr_value_list.h"

namespace fume
{
namespace vrs
{

// Base class for string-based value representations
class string_vr : public value_representation
{
public:
    string_vr( unsigned int min_vals,
               unsigned int max_vals,
               unsigned int multiple,
               char         pad = ' ' );
    virtual ~string_vr()
    {

    }

// serializable (value_representation)
public:
    virtual MC_STATUS to_stream( tx_stream&      stream,
                                 TRANSFER_SYNTAX syntax ) override final;
    virtual MC_STATUS from_stream( rx_stream&      stream,
                                   TRANSFER_SYNTAX syntax ) override final;

// value_representation -- modifiers
public:
    using value_representation::set;
    virtual MC_STATUS set( const char* val ) override
    {
        return val != nullptr ? set( std::string( val ) ) :
                                MC_NULL_POINTER_PARM;
    }

    // Sets the value of the data element to NULL (ie. zero length)
    virtual MC_STATUS set_null() override final
    {
        m_values.set_null();
        return MC_NORMAL_COMPLETION;
    }

    using value_representation::set_next;
    virtual MC_STATUS set_next( const char* val ) override final
    {
        return val != nullptr ? set_next( std::string( val ) ) :
                                MC_NULL_POINTER_PARM;
    }

    // For string value representations, sets the next value of
    // the data element to NULL (ie zero length string)
    virtual MC_STATUS set_next_null() override final
    {
        return m_values.set_next( std::string() );
    }

    // Removes the "current" value
    virtual MC_STATUS delete_current() override final
    {
        return m_values.delete_current();
    }

// value_representation -- accessors
public:
    using value_representation::get;
    virtual MC_STATUS get( get_string_parms& val ) override final;

    using value_representation::get_next;
    virtual MC_STATUS get_next( get_string_parms& val ) override final;

    // Returns the number of elements
    virtual int count() const override final
    {
        // TODO: fix this to return uint32_t
        return static_cast<int>( m_values.count() );
    }

    // Indicates whether or not the element is null
    virtual bool is_null() const override final
    {
        return m_values.is_null();
    }

protected:
    virtual MC_STATUS validate_value( const std::string& val ) const = 0;

    MC_STATUS set( std::string&& val );
    MC_STATUS set_next( std::string&& val );

    MC_STATUS get( const std::string*& val );
    MC_STATUS get_next( const std::string*& val );


    template <class Src>
    MC_STATUS cast_and_set_string( Src val )
    {
        return cast_and_call_string_setter( val,
                                            [this]( std::string&& str_val )
                                            {
                                                return set( std::move( str_val ) );
                                            } );
    }

    template <class Src>
    MC_STATUS cast_and_set_next_string( Src val )
    {
        return cast_and_call_string_setter( val,
                                            [this]( std::string&& str_val )
                                            {
                                                return set_next( std::move( str_val ) );
                                            } );
    }

    template <class Dst>
    MC_STATUS cast_and_get_string( Dst& val )
    {
        return cast_and_call_string_getter( val,
                                            [this]( const std::string*& str_val )
                                            {
                                                return get( str_val );
                                            } );
    }

    template <class Dst>
    MC_STATUS cast_and_get_next_string( Dst& val )
    {
        return cast_and_call_string_getter( val,
                                            [this]( const std::string*& str_val )
                                            {
                                                return get_next( str_val );
                                            } );
    }

protected:
    string_vr( const string_vr& rhs )
        : value_representation( rhs ),
          m_values( rhs.m_values ),
          m_pad( rhs.m_pad )
    {
    }

private:
    typedef vr_value_list<std::string, 0xFFFFFFFE> value_list_t;

private:
    value_list_t m_values;
    const char                 m_pad;
};

} // namespace vrs
} // namespace fume

#endif
