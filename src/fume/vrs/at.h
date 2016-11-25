#ifndef AT_H
#define AT_H
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
#include <cstdint>
#include <limits>

// local public
#include "mc3msg.h"

// local private
#include "fume/value_representation.h"
#include "fume/value_conversion.h"
#include "fume/vrs/vr_value_list.h"

namespace fume
{
namespace vrs
{

// Attribute value representations
// TODO: Derive from one of the base classes
// TODO: Clean up the base VR class hierarchy
class at final : public value_representation
{
public:
    at( unsigned int min_vals,
        unsigned int max_vals,
        unsigned int multiple )
        : value_representation( min_vals, max_vals, multiple )
    {
    }
    virtual ~at()
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
    virtual MC_STATUS set( long val ) override final
    {
        return cast_and_call_setter<uint32_t>( val,
                                               [this]( uint32_t dst_val )
                                               {
                                                   return m_values.set( dst_val );
                                               } );
    }
    virtual MC_STATUS set( unsigned long val ) override final
    {
        return cast_and_call_setter<uint32_t>( val,
                                               [this]( uint32_t dst_val )
                                               {
                                                   return m_values.set( dst_val );
                                               } );
    }
    virtual MC_STATUS set( const char* val ) override final
    {
        return cast_and_call_setter<uint32_t>( val,
                                               [this]( uint32_t dst_val )
                                               {
                                                   return m_values.set( dst_val );
                                               } );
    }

    // Sets the value of the data element to NULL (ie. zero length)
    virtual MC_STATUS set_null() override final
    {
        m_values.set_null();
        return MC_NORMAL_COMPLETION;
    }

    virtual MC_STATUS set_next( long val ) override final
    {
        return cast_and_call_setter<uint32_t>( val,
                                               [this]( uint32_t dst_val )
                                               {
                                                   return m_values.set_next( dst_val );
                                               } );
    }
    virtual MC_STATUS set_next( unsigned long val ) override final
    {
        return cast_and_call_setter<uint32_t>( val,
                                               [this]( uint32_t dst_val )
                                               {
                                                   return m_values.set_next( dst_val );
                                               } );
    }
    virtual MC_STATUS set_next( const char* val ) override final
    {
        return cast_and_call_setter<uint32_t>( val,
                                               [this]( uint32_t dst_val )
                                               {
                                                   return m_values.set_next( dst_val );
                                               } );
    }

    // Removes the "current" value
    virtual MC_STATUS delete_current() override final
    {
        return m_values.delete_current();
    }

// value_representation -- accessors
public:
    virtual MC_STATUS get( long& val ) override final
    {
        return cast_and_call_getter<uint32_t>( val,
                                               [this]( uint32_t& src_val )
                                               {
                                                   return m_values.get( src_val );
                                               } );
    }
    virtual MC_STATUS get( unsigned long& val ) override final
    {
        return cast_and_call_getter<uint32_t>( val,
                                               [this]( uint32_t& src_val )
                                               {
                                                   return m_values.get( src_val );
                                               } );
    }
    virtual MC_STATUS get( get_string_parms& val ) override final
    {
        return cast_and_call_getter<uint32_t>( val,
                                               [this]( uint32_t& src_val )
                                               {
                                                   return m_values.get( src_val );
                                               } );
    }

    virtual MC_STATUS get_next( long& val ) override final
    {
        return cast_and_call_getter<uint32_t>( val,
                                               [this]( uint32_t& src_val )
                                               {
                                                   return m_values.get_next( src_val );
                                               } );
    }
    virtual MC_STATUS get_next( unsigned long& val ) override final
    {
        return cast_and_call_getter<uint32_t>( val,
                                               [this]( uint32_t& src_val )
                                               {
                                                   return m_values.get_next( src_val );
                                               } );
    }
    virtual MC_STATUS get_next( get_string_parms& val ) override final
    {
        return cast_and_call_getter<uint32_t>( val,
                                               [this]( uint32_t& src_val )
                                               {
                                                   return m_values.get_next( src_val );
                                               } );
    }

    // Returns the number of elements
    virtual int count() const override final
    {
        return static_cast<int>( m_values.count() );
    }

    // Indicates whether or not the element is null
    virtual bool is_null() const override final
    {
        return m_values.is_null();
    }

    virtual MC_VR vr() const override final
    {
        return AT;
    }

    virtual std::unique_ptr<value_representation> clone() const override final
    {
        return std::unique_ptr<value_representation>( new at( *this ) );
    }

private:
    at( const at& rhs )
        : value_representation( rhs ),
          m_values( rhs.m_values )
    {
    }

private:
    static constexpr size_t MAX_SIZE = std::numeric_limits<uint16_t>::max() - 1u;
    typedef vr_value_list<uint32_t, MAX_SIZE> value_list_t;

private:
    value_list_t m_values;
};

} // namespace vrs
} // namespace fume

#endif
