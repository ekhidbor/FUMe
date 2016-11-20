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
#include <deque>

// local public
#include "mc3msg.h"

// local private
#include "fume/value_representation.h"
#include "fume/value_conversion.h"

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
        : value_representation( min_vals, max_vals, multiple ),
          m_current_idx( 0 )
    {
    }
    virtual ~at()
    {

    }

// serializable (value_representation)
public:
    virtual MC_STATUS to_stream( tx_stream&      stream,
                                 TRANSFER_SYNTAX syntax ) const override final;
    virtual MC_STATUS from_stream( rx_stream&      stream,
                                   TRANSFER_SYNTAX syntax ) override final;

// value_representation -- modifiers
public:
    virtual MC_STATUS set( int val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( float val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( double val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( short val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( long val ) override final
    {
        return cast_and_call_setter<uint32_t>( val,
                                               [this]( uint32_t dst_val )
                                               {
                                                   return set_native( dst_val );
                                               } );
    }
    virtual MC_STATUS set( unsigned int val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( unsigned short val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( unsigned long val ) override final
    {
        return cast_and_call_setter<uint32_t>( val,
                                               [this]( uint32_t dst_val )
                                               {
                                                   return set_native( dst_val );
                                               } );
    }
    virtual MC_STATUS set( const char* val ) override final
    {
        return cast_and_call_setter<uint32_t>( val,
                                               [this]( uint32_t dst_val )
                                               {
                                                   return set_native( dst_val );
                                               } );
    }
    virtual MC_STATUS set( const MC_UChar* val ) override final
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

    virtual MC_STATUS set_next( int val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( float val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( double val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( short val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( long val ) override final
    {
        return cast_and_call_setter<uint32_t>( val,
                                               [this]( uint32_t dst_val )
                                               {
                                                   return set_next_native( dst_val );
                                               } );
    }
    virtual MC_STATUS set_next( unsigned int val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( unsigned short val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( unsigned long val ) override final
    {
        return cast_and_call_setter<uint32_t>( val,
                                               [this]( uint32_t dst_val )
                                               {
                                                   return set_next_native( dst_val );
                                               } );
    }
    virtual MC_STATUS set_next( const char* val ) override final
    {
        return cast_and_call_setter<uint32_t>( val,
                                               [this]( uint32_t dst_val )
                                               {
                                                   return set_next_native( dst_val );
                                               } );
    }
    virtual MC_STATUS set_next( const MC_UChar* val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( const set_buf_parms& val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next_null() override final
    {
        return MC_INCOMPATIBLE_VR;
    }

    // Removes the "current" value
    virtual MC_STATUS delete_current() override final;

// value_representation -- accessors
public:
    virtual MC_STATUS get( int& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( double& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( float& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( short& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( long& val ) const override final
    {
        return cast_and_call_getter<uint32_t>( val,
                                               [this]( uint32_t& src_val )
                                               {
                                                   return get_native( src_val );
                                               } );
    }
    virtual MC_STATUS get( unsigned int& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( unsigned short& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( unsigned long& val ) const override final
    {
        return cast_and_call_getter<uint32_t>( val,
                                               [this]( uint32_t& src_val )
                                               {
                                                   return get_native( src_val );
                                               } );
    }
    virtual MC_STATUS get( get_string_parms& val ) const override final
    {
        return cast_and_call_getter<uint32_t>( val,
                                               [this]( uint32_t& src_val )
                                               {
                                                   return get_native( src_val );
                                               } );
    }
    virtual MC_STATUS get( get_ustring_parms& val ) const override final
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


    virtual MC_STATUS get_next( int& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( double& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( float& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( short& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( long& val ) const override final
    {
        return cast_and_call_getter<uint32_t>( val,
                                               [this]( uint32_t& src_val )
                                               {
                                                   return get_next_native( src_val );
                                               } );
    }
    virtual MC_STATUS get_next( unsigned int& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( unsigned short& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( unsigned long& val ) const override final
    {
        return cast_and_call_getter<uint32_t>( val,
                                               [this]( uint32_t& src_val )
                                               {
                                                   return get_next_native( src_val );
                                               } );
    }
    virtual MC_STATUS get_next( get_string_parms& val ) const override final
    {
        return cast_and_call_getter<uint32_t>( val,
                                               [this]( uint32_t& src_val )
                                               {
                                                   return get_next_native( src_val );
                                               } );
    }
    virtual MC_STATUS get_next( get_ustring_parms& val ) const override final
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

    virtual MC_VR vr() const override final
    {
        return AT;
    }

    virtual std::unique_ptr<value_representation> clone() const override final
    {
        return std::unique_ptr<value_representation>( new at( *this ) );
    }

private:
    MC_STATUS set_native( uint32_t val );
    MC_STATUS set_next_native( uint32_t val );
    MC_STATUS get_native( uint32_t& val ) const;
    MC_STATUS get_next_native( uint32_t& val ) const;

private:
    at( const at& rhs )
        : value_representation( rhs ),
          m_values( rhs.m_values ),
          m_current_idx( rhs.m_current_idx )
    {
    }

private:
    std::deque<uint32_t> m_values;

    // Used for get_next and delete_current
    mutable size_t m_current_idx;
};

} // namespace vrs
} // namespace fume

#endif
