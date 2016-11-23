#ifndef SQ_H
#define SQ_H
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
#include <limits>

// local private
#include "fume/value_representation.h"
#include "fume/value_conversion.h"
#include "fume/vrs/vr_value_list.h"

namespace fume
{
namespace vrs
{

// vr_sq is used to contain objects which have a sequence value
// representation. This class doesn't actually implement the
// item object but rather references one by its integer item
// ID (returned from MC_Create_Item)
class sq final : public value_representation
{
public:
    sq( unsigned int min_vals, unsigned int max_vals, unsigned int multiple );
    ~sq();

// serializable (value_representation)
public:
    virtual MC_STATUS to_stream( tx_stream&      stream,
                                 TRANSFER_SYNTAX syntax ) override final;
    virtual MC_STATUS from_stream( rx_stream&      stream,
                                   TRANSFER_SYNTAX syntax ) override final;

// value_representation -- modifiers
public:
    virtual MC_STATUS set( int val ) override final;

    virtual MC_STATUS set( double val ) override final
    {
        return cast_and_call_setter<int>( val, [this]( int dst_val )
                                               {
                                                   return set( dst_val );
                                               } );
    }
    virtual MC_STATUS set( float val ) override final
    {
        return cast_and_call_setter<int>( val, [this]( int dst_val )
                                               {
                                                   return set( dst_val );
                                               } );
    }
    virtual MC_STATUS set( short val ) override final
    {
        return cast_and_call_setter<int>( val, [this]( int dst_val )
                                               {
                                                   return set( dst_val );
                                               } );
    }
    virtual MC_STATUS set( long val ) override final
    {
        return cast_and_call_setter<int>( val, [this]( int dst_val )
                                               {
                                                   return set( dst_val );
                                               } );
    }
    virtual MC_STATUS set( const char* val ) override final
    {
        return cast_and_call_setter<int>( val, [this]( int dst_val )
                                               {
                                                   return set( dst_val );
                                               } );
    }
    virtual MC_STATUS set( const MC_UChar* val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( unsigned int val ) override final
    {
        return cast_and_call_setter<int>( val, [this]( int dst_val )
                                               {
                                                   return set( dst_val );
                                               } );
    }
    virtual MC_STATUS set( unsigned short val ) override final
    {
        return cast_and_call_setter<int>( val, [this]( int dst_val )
                                               {
                                                   return set( dst_val );
                                               } );
    }
    virtual MC_STATUS set( unsigned long val ) override final
    {
        return cast_and_call_setter<int>( val, [this]( int dst_val )
                                               {
                                                   return set( dst_val );
                                               } );
    }

    // Sets the value of the data element to NULL (ie. zero length).
    // NOTE: the specification for this function is that items are /not/
    // freed
    virtual MC_STATUS set_null() override final
    {
        m_items.set_null();
        return MC_NORMAL_COMPLETION;
    }

    virtual MC_STATUS set_next( int val ) override final;

    virtual MC_STATUS set_next( double val ) override final
    {
        return cast_and_call_setter<int>( val, [this]( int dst_val )
                                               {
                                                   return set_next( dst_val );
                                               } );
    }
    virtual MC_STATUS set_next( float val ) override final
    {
        return cast_and_call_setter<int>( val, [this]( int dst_val )
                                               {
                                                   return set_next( dst_val );
                                               } );
    }
    virtual MC_STATUS set_next( short val ) override final
    {
        return cast_and_call_setter<int>( val, [this]( int dst_val )
                                               {
                                                   return set_next( dst_val );
                                               } );
    }
    virtual MC_STATUS set_next( long val ) override final
    {
        return cast_and_call_setter<int>( val, [this]( int dst_val )
                                               {
                                                   return set_next( dst_val );
                                               } );
    }
    virtual MC_STATUS set_next( const char* val ) override final
    {
        return cast_and_call_setter<int>( val, [this]( int dst_val )
                                               {
                                                   return set_next( dst_val );
                                               } );
    }
    virtual MC_STATUS set_next( unsigned int val ) override final
    {
        return cast_and_call_setter<int>( val, [this]( int dst_val )
                                               {
                                                   return set_next( dst_val );
                                               } );
    }
    virtual MC_STATUS set_next( unsigned short val ) override final
    {
        return cast_and_call_setter<int>( val, [this]( int dst_val )
                                               {
                                                   return set_next( dst_val );
                                               } );
    }
    virtual MC_STATUS set_next( unsigned long val ) override final
    {
        return cast_and_call_setter<int>( val, [this]( int dst_val )
                                               {
                                                   return set_next( dst_val );
                                               } );
    }

    // Removes the "current" value
    virtual MC_STATUS delete_current() override final
    {
        return m_items.delete_current();
    }

// value_representation -- accessors
public:
    virtual MC_STATUS get( int& val ) override final;

    virtual MC_STATUS get( double& val ) override final
    {
        return cast_and_call_getter<int>( val, [this]( int& src_val )
                                               {
                                                   return get( src_val );
                                               } );
    }
    virtual MC_STATUS get( float& val ) override final
    {
        return cast_and_call_getter<int>( val, [this]( int& src_val )
                                               {
                                                   return get( src_val );
                                               } );
    }
    virtual MC_STATUS get( short& val ) override final
    {
        return cast_and_call_getter<int>( val, [this]( int& src_val )
                                               {
                                                   return get( src_val );
                                               } );
    }
    virtual MC_STATUS get( long& val ) override final
    {
        return cast_and_call_getter<int>( val, [this]( int& src_val )
                                               {
                                                   return get( src_val );
                                               } );
    }
    virtual MC_STATUS get( get_string_parms& val ) override final
    {
        return cast_and_call_getter<int>( val, [this]( int& src_val )
                                               {
                                                   return get( src_val );
                                               } );
    }
    virtual MC_STATUS get( get_ustring_parms& val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( unsigned int& val ) override final
    {
        return cast_and_call_getter<int>( val, [this]( int& src_val )
                                               {
                                                   return get( src_val );
                                               } );
    }
    virtual MC_STATUS get( unsigned short& val ) override final
    {
        return cast_and_call_getter<int>( val, [this]( int& src_val )
                                               {
                                                   return get( src_val );
                                               } );
    }
    virtual MC_STATUS get( unsigned long& val ) override final
    {
        return cast_and_call_getter<int>( val, [this]( int& src_val )
                                               {
                                                   return get( src_val );
                                               } );
    }
    virtual MC_STATUS get( get_buf_parms& val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( const get_func_parms& val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }

    virtual MC_STATUS get_next( int& val ) override final;

    virtual MC_STATUS get_next( double& val ) override final
    {
        return cast_and_call_getter<int>( val, [this]( int& src_val )
                                               {
                                                   return get_next( src_val );
                                               } );
    }
    virtual MC_STATUS get_next( float& val ) override final
    {
        return cast_and_call_getter<int>( val, [this]( int& src_val )
                                               {
                                                   return get_next( src_val );
                                               } );
    }
    virtual MC_STATUS get_next( short& val ) override final
    {
        return cast_and_call_getter<int>( val, [this]( int& src_val )
                                               {
                                                   return get_next( src_val );
                                               } );
    }
    virtual MC_STATUS get_next( long& val ) override final
    {
        return cast_and_call_getter<int>( val, [this]( int& src_val )
                                               {
                                                   return get_next( src_val );
                                               } );
    }
    virtual MC_STATUS get_next( get_string_parms& val ) override final
    {
        return cast_and_call_getter<int>( val, [this]( int& src_val )
                                               {
                                                   return get_next( src_val );
                                               } );
    }
    virtual MC_STATUS get_next( unsigned int& val ) override final
    {
        return cast_and_call_getter<int>( val, [this]( int& src_val )
                                               {
                                                   return get_next( src_val );
                                               } );
    }
    virtual MC_STATUS get_next( unsigned short& val ) override final
    {
        return cast_and_call_getter<int>( val, [this]( int& src_val )
                                               {
                                                   return get_next( src_val );
                                               } );
    }
    virtual MC_STATUS get_next( unsigned long& val ) override final
    {
        return cast_and_call_getter<int>( val, [this]( int& src_val )
                                               {
                                                   return get_next( src_val );
                                               } );
    }

    // Returns the number of elements
    virtual int count() const override final
    {
        return static_cast<int>( m_items.count() );
    }

    // Indicates whether or not the element is null
    virtual bool is_null() const override final
    {
        return m_items.is_null();
    }

    virtual MC_VR vr() const override final
    {
        return SQ;
    }

    virtual std::unique_ptr<value_representation> clone() const override
    {
        return std::unique_ptr<value_representation>( new sq( *this ) );
    }

private:
    sq( const sq& rhs )
        : value_representation( rhs ),
          // TODO: Do we ned to do a deep copy?
          m_items( rhs.m_items )
    {
    }

private:
  typedef vr_value_list<int, std::numeric_limits<uint32_t>::max() - 1> value_list_t;

private:
    value_list_t m_items;
};

} // namespace vrs
} // namespace fume

#endif
