#ifndef BINARY_VR_H
#define BINARY_VR_H
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
#include <cassert>
#include <limits>

// local public
#include "mc3msg.h"

// local private
#include "fume/value_representation.h"
#include "fume/value_conversion.h"
#include "fume/tx_stream.h"
#include "fume/rx_stream.h"
#include "fume/vrs/vr_value_list.h"

namespace fume
{
namespace vrs
{

// Base class for binary data-based value representations
template<class T, MC_VR VR>
class binary_vr final : public value_representation
{
public:
    binary_vr( unsigned int min_vals,
               unsigned int max_vals,
               unsigned int multiple )
        : value_representation( min_vals, max_vals, multiple )
    {
    }
    virtual ~binary_vr()
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
    virtual MC_STATUS set( int val ) override final
    {
        return cast_and_call_setter<T>( val, [this]( T dst_val )
                                             {
                                                 return m_values.set( dst_val );
                                             } );
    }
    virtual MC_STATUS set( float val ) override final
    {
        return cast_and_call_setter<T>( val, [this]( T dst_val )
                                             {
                                                 return m_values.set( dst_val );
                                             } );
    }
    virtual MC_STATUS set( double val ) override final
    {
        return cast_and_call_setter<T>( val, [this]( T dst_val )
                                             {
                                                 return m_values.set( dst_val );
                                             } );
    }
    virtual MC_STATUS set( short val ) override final
    {
        return cast_and_call_setter<T>( val, [this]( T dst_val )
                                             {
                                                 return m_values.set( dst_val );
                                             } );
    }
    virtual MC_STATUS set( long val ) override final
    {
        return cast_and_call_setter<T>( val, [this]( T dst_val )
                                             {
                                                 return m_values.set( dst_val );
                                             } );
    }
    virtual MC_STATUS set( unsigned int val ) override final
    {
        return cast_and_call_setter<T>( val, [this]( T dst_val )
                                             {
                                                 return m_values.set( dst_val );
                                             } );
    }
    virtual MC_STATUS set( unsigned short val ) override final
    {
        return cast_and_call_setter<T>( val, [this]( T dst_val )
                                             {
                                                 return m_values.set( dst_val );
                                             } );
    }
    virtual MC_STATUS set( unsigned long val ) override final
    {
        return cast_and_call_setter<T>( val, [this]( T dst_val )
                                             {
                                                 return m_values.set( dst_val );
                                             } );
    }
    virtual MC_STATUS set( const char* val ) override final
    {
        return cast_and_call_setter<T>( val, [this]( T dst_val )
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

    virtual MC_STATUS set_next( int val ) override final
    {
        return cast_and_call_setter<T>( val, [this]( T dst_val )
                                             {
                                                 return m_values.set_next( dst_val );
                                             } );
    }
    virtual MC_STATUS set_next( float val ) override final
    {
        return cast_and_call_setter<T>( val, [this]( T dst_val )
                                             {
                                                 return m_values.set_next( dst_val );
                                             } );
    }
    virtual MC_STATUS set_next( double val ) override final
    {
        return cast_and_call_setter<T>( val, [this]( T dst_val )
                                             {
                                                 return m_values.set_next( dst_val );
                                             } );
    }
    virtual MC_STATUS set_next( short val ) override final
    {
        return cast_and_call_setter<T>( val, [this]( T dst_val )
                                             {
                                                 return m_values.set_next( dst_val );
                                             } );
    }
    virtual MC_STATUS set_next( long val ) override final
    {
        return cast_and_call_setter<T>( val, [this]( T dst_val )
                                             {
                                                 return m_values.set_next( dst_val );
                                             } );
    }
    virtual MC_STATUS set_next( unsigned int val ) override final
    {
        return cast_and_call_setter<T>( val, [this]( T dst_val )
                                             {
                                                 return m_values.set_next( dst_val );
                                             } );
    }
    virtual MC_STATUS set_next( unsigned short val ) override final
    {
        return cast_and_call_setter<T>( val, [this]( T dst_val )
                                             {
                                                 return m_values.set_next( dst_val );
                                             } );
    }
    virtual MC_STATUS set_next( unsigned long val ) override final
    {
        return cast_and_call_setter<T>( val, [this]( T dst_val )
                                             {
                                                 return m_values.set_next( dst_val );
                                             } );
    }
    virtual MC_STATUS set_next( const char* val ) override final
    {
        return cast_and_call_setter<T>( val, [this]( T dst_val )
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
    virtual MC_STATUS get( int& val ) override final
    {
        return cast_and_call_getter<T>( val, [this]( T& src_val )
                                             {
                                                 return m_values.get( src_val );
                                             } );
    }
    virtual MC_STATUS get( double& val ) override final
    {
        return cast_and_call_getter<T>( val, [this]( T& src_val )
                                             {
                                                 return m_values.get( src_val );
                                             } );
    }
    virtual MC_STATUS get( float& val ) override final
    {
        return cast_and_call_getter<T>( val, [this]( T& src_val )
                                             {
                                                 return m_values.get( src_val );
                                             } );
    }
    virtual MC_STATUS get( short& val ) override final
    {
        return cast_and_call_getter<T>( val, [this]( T& src_val )
                                             {
                                                 return m_values.get( src_val );
                                             } );
    }
    virtual MC_STATUS get( long& val ) override final
    {
        return cast_and_call_getter<T>( val, [this]( T& src_val )
                                             {
                                                 return m_values.get( src_val );
                                             } );
    }
    virtual MC_STATUS get( unsigned int& val ) override final
    {
        return cast_and_call_getter<T>( val, [this]( T& src_val )
                                             {
                                                 return m_values.get( src_val );
                                             } );
    }
    virtual MC_STATUS get( unsigned short& val ) override final
    {
        return cast_and_call_getter<T>( val, [this]( T& src_val )
                                             {
                                                 return m_values.get( src_val );
                                             } );
    }
    virtual MC_STATUS get( unsigned long& val ) override final
    {
        return cast_and_call_getter<T>( val, [this]( T& src_val )
                                             {
                                                 return m_values.get( src_val );
                                             } );
    }
    virtual MC_STATUS get( get_string_parms& val ) override final
    {
        return cast_and_call_getter<T>( val, [this]( T& src_val )
                                             {
                                                 return m_values.get( src_val );
                                             } );
    }


    virtual MC_STATUS get_next( int& val ) override final
    {
        return cast_and_call_getter<T>( val, [this]( T& src_val )
                                             {
                                                 return m_values.get_next( src_val );
                                             } );
    }
    virtual MC_STATUS get_next( double& val ) override final
    {
        return cast_and_call_getter<T>( val, [this]( T& src_val )
                                             {
                                                 return m_values.get_next( src_val );
                                             } );
    }
    virtual MC_STATUS get_next( float& val ) override final
    {
        return cast_and_call_getter<T>( val, [this]( T& src_val )
                                             {
                                                 return m_values.get_next( src_val );
                                             } );
    }
    virtual MC_STATUS get_next( short& val ) override final
    {
        return cast_and_call_getter<T>( val, [this]( T& src_val )
                                             {
                                                 return m_values.get_next( src_val );
                                             } );
    }
    virtual MC_STATUS get_next( long& val ) override final
    {
        return cast_and_call_getter<T>( val, [this]( T& src_val )
                                             {
                                                 return m_values.get_next( src_val );
                                             } );
    }
    virtual MC_STATUS get_next( unsigned int& val ) override final
    {
        return cast_and_call_getter<T>( val, [this]( T& src_val )
                                             {
                                                 return m_values.get_next( src_val );
                                             } );
    }
    virtual MC_STATUS get_next( unsigned short& val ) override final
    {
        return cast_and_call_getter<T>( val, [this]( T& src_val )
                                             {
                                                 return m_values.get_next( src_val );
                                             } );
    }
    virtual MC_STATUS get_next( unsigned long& val ) override final
    {
        return cast_and_call_getter<T>( val, [this]( T& src_val )
                                             {
                                                 return m_values.get_next( src_val );
                                             } );
    }
    virtual MC_STATUS get_next( get_string_parms& val ) override final
    {
        return cast_and_call_getter<T>( val, [this]( T& src_val )
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
        return VR;
    }

    virtual std::unique_ptr<value_representation> clone() const override
    {
        return std::unique_ptr<value_representation>( new binary_vr( *this ) );
    }

protected:
    binary_vr( const binary_vr& rhs )
        : value_representation( rhs ),
          m_values( rhs.m_values )
    {
    }

private:
    // Maximum even-length 16-bit unsigned integer
    static constexpr size_t MAX_SIZE = std::numeric_limits<uint16_t>::max() - 1u;
    typedef vr_value_list<T, MAX_SIZE> value_list_t;

private:
    value_list_t m_values;
};

template<class T, MC_VR VR>
MC_STATUS binary_vr<T, VR>::from_stream( rx_stream&      stream,
                                         TRANSFER_SYNTAX syntax )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;
    uint32_t value_length = 0;
    value_list_t tmp_values;

    if( syntax == IMPLICIT_LITTLE_ENDIAN )
    {
        ret = stream.read_val( value_length, syntax );
    }
    else
    {
        uint16_t value_length_16u;
        ret = stream.read_val( value_length_16u, syntax );
        value_length = value_length_16u;
    }

    if( ret == MC_NORMAL_COMPLETION )
    {
        if( value_length % sizeof(T) == 0 )
        {
            const uint32_t num_items = value_length / sizeof(T);
            for( uint32_t i = 0; i < num_items && ret == MC_NORMAL_COMPLETION; ++i )
            {
                T val;
                ret = stream.read_val( val, syntax );
                if( ret == MC_NORMAL_COMPLETION )
                {
                    ret = tmp_values.set_next( val );
                }
            }

            if( ret == MC_NORMAL_COMPLETION )
            {
                // Only update values if everything succeeded
                m_values.swap( tmp_values );
            }
        }
        else
        {
            ret = MC_INVALID_LENGTH_FOR_VR;
        }
    }
    else
    {
        // Do nothing. Will return error
    }

    return ret;
}

template<class T, MC_VR VR>
MC_STATUS binary_vr<T, VR>::to_stream( tx_stream&      stream,
                                       TRANSFER_SYNTAX syntax )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    const uint32_t value_size = m_values.size();
    if( syntax == IMPLICIT_LITTLE_ENDIAN )
    {
        // sizes are always 32-bit for implicit little endian
        ret = stream.write_val( value_size, syntax );
    }
    else
    {
        // set_next ensures size is under 16-bit limit
        assert( value_size < std::numeric_limits<uint16_t>::max() );
        ret = stream.write_val( static_cast<uint16_t>( value_size ), syntax );
    }

    if( ret == MC_NORMAL_COMPLETION && m_values.is_null() == false )
    {
        for( typename value_list_t::const_iterator itr = m_values.cbegin();
             ret == MC_NORMAL_COMPLETION && itr != m_values.cend();
             ++itr )
        {
            ret = stream.write_val( *itr, syntax );
        }
    }
    else
    {
        // Do nothing. Will return error if write_val failed or
        // MC_NORMAL_COMPLETION if zero length value
    }

    return ret;
}


} // namespace vrs
} // namespace fume

#endif
