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
#include <cassert>
#include <deque>
#include <string>
#include <utility>

// local private
#include "fume/value_representation.h"
#include "fume/value_conversion.h"

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
                                 TRANSFER_SYNTAX syntax ) const override final;
    virtual MC_STATUS from_stream( rx_stream& stream ) override final
    {
        // TODO: implement
        return MC_CANNOT_COMPLY;
    }

// value_representation -- modifiers
public:
    virtual MC_STATUS set( const char* val ) override;

    virtual MC_STATUS set( double val ) override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( float val ) override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( int val ) override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( short val ) override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( long val ) override
    {
        return MC_INCOMPATIBLE_VR;
    }

    virtual MC_STATUS set( const MC_UChar* val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( unsigned int val ) override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( unsigned short val ) override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( unsigned long val ) override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( const set_buf_parms& val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( const set_func_parms& val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }

    // Sets the value of the data element to NULL (ie. zero length)
    virtual MC_STATUS set_null() override final
    {
        m_values.clear();
        return MC_NORMAL_COMPLETION;
    }

    virtual MC_STATUS set_next( const char* val ) override final;

    virtual MC_STATUS set_next( double val ) override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( float val ) override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( int val ) override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( short val ) override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( long val ) override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( const MC_UChar* val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( unsigned int val ) override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( unsigned short val ) override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( unsigned long val ) override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( const set_buf_parms& val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }

    // For string value representations, sets the next value of
    // the data element to NULL (ie zero length string)
    virtual MC_STATUS set_next_null() override final
    {
        m_values.push_back( std::string() );
        return MC_NORMAL_COMPLETION;
    }

    // Removes the "current" value
    virtual MC_STATUS delete_current() override final;

// value_representation -- accessors
public:
    virtual MC_STATUS get( get_string_parms& val ) const override final;

    virtual MC_STATUS get( double& val ) const override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( float& val ) const override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( int& val ) const override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( short& val ) const override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( long& val ) const override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( get_ustring_parms& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( unsigned int& val ) const override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( unsigned short& val ) const override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( unsigned long& val ) const override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( get_buf_parms& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( const get_func_parms& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }

    virtual MC_STATUS get_next( get_string_parms& val ) const override final;

    virtual MC_STATUS get_next( double& val ) const override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( float& val ) const override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( int& val ) const override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( short& val ) const override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( long& val ) const override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( get_ustring_parms& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( unsigned int& val ) const override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( unsigned short& val ) const override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( unsigned long& val ) const override
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( get_buf_parms& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }

    // Returns the number of elements
    virtual int count() const override final
    {
        return static_cast<int>( m_values.size() );
    }

    // Indicates whether or not the element is null
    virtual bool is_null() const override final
    {
        return m_values.empty();
    }

protected:
    virtual MC_STATUS validate_value( const std::string& val ) const = 0;

    MC_STATUS set( std::string&& val );
    MC_STATUS set_next( std::string&& val );

    MC_STATUS get( const std::string*& val ) const;
    MC_STATUS get_next( const std::string*& val ) const;


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
    MC_STATUS cast_and_get_string( Dst& val ) const
    {
        return cast_and_call_string_getter( val,
                                            [this]( const std::string*& str_val )
                                            {
                                                return get( str_val );
                                            } );
    }

    template <class Dst>
    MC_STATUS cast_and_get_next_string( Dst& val ) const
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
          m_current_idx( rhs.m_current_idx ),
          m_pad( rhs.m_pad )
    {
    }

private:
    std::deque<std::string> m_values;

    // Used for get_next and delete_current
    mutable size_t m_current_idx;
    const char m_pad;
};

} // namespace vrs
} // namespace fume

#endif
