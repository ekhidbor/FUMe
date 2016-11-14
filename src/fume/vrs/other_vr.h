#ifndef OTHER_VR_H
#define OTHER_VR_H
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
#include <vector>
#include <algorithm>

// local public
#include "mc3msg.h"

// local private
#include "fume/value_representation.h"
#include "fume/tx_stream.h"

namespace fume
{
namespace vrs
{

// Base class for the "other" value representations
template<class T, MC_VR VR>
class other_vr : public value_representation
{
public:
    other_vr()
        : value_representation( 1u, 1u, 1u )
    {
    }
    virtual ~other_vr()
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
        return MC_INCOMPATIBLE_VR;
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
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( const char* val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( const MC_UChar* val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( const set_buf_parms& val ) override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( const set_func_parms& val ) override final;

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
        return MC_INCOMPATIBLE_VR;
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
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( const char* val ) override final
    {
        return MC_INCOMPATIBLE_VR;
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
    virtual MC_STATUS delete_current() override final
    {
        return MC_INCOMPATIBLE_VR;
    }

// value_representation -- accessors
public:
    virtual MC_STATUS get( int& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( float& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( double& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( short& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( long& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
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
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( get_string_parms& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( get_ustring_parms& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( get_buf_parms& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( const get_func_parms& val ) const override final;

    virtual MC_STATUS get_next( int& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( float& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( double& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( short& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( long& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
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
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( get_string_parms& val ) const override final
    {
        return MC_INCOMPATIBLE_VR;
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
        return static_cast<int>( m_values.empty() == false );
    }

    // Indicates whether or not the element is null
    virtual bool is_null() const override final
    {
        return m_values.empty();
    }

    virtual MC_VR vr() const override final
    {
        return VR;
    }

    virtual std::unique_ptr<value_representation> clone() const override
    {
        return std::unique_ptr<value_representation>( new other_vr( *this ) );
    }

protected:
    other_vr( const other_vr& rhs )
        : value_representation( rhs ),
          m_values( rhs.m_values )
    {
    }

protected:
    template<class Iterator>
    void append( Iterator begin, Iterator end )
    {
        m_values.insert( m_values.cend(), begin, end );
    }

    size_t data_size() const
    {
        return m_values.size();
    }

private:
    std::deque<T> m_values;
};

template<class T, MC_VR VR>
MC_STATUS other_vr<T, VR>::to_stream( tx_stream&      stream,
                                      TRANSFER_SYNTAX syntax ) const
{
    MC_STATUS ret =
        stream.write_val( static_cast<uint32_t>( m_values.size() * sizeof(T) ),
                          syntax );
    if( ret == MC_NORMAL_COMPLETION && m_values.empty() == false )
    {
        for( typename std::deque<T>::const_iterator itr = m_values.cbegin();
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

template<class T, MC_VR VR>
MC_STATUS other_vr<T, VR>::set( const set_func_parms& val )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( val.callback != nullptr )
    {
        m_values.clear();

        bool first = true;
        MC_STATUS call_ret = MC_NORMAL_COMPLETION;
        int user_last = 0;
        do
        {
            void* user_buf = nullptr;
            int user_size = 0;
            user_last = 0;

            call_ret = val.callback( val.message_id,
                                     val.tag,
                                     static_cast<int>( first ),
                                     val.callback_parm,
                                     &user_size,
                                     &user_buf,
                                     &user_last );

            if( (call_ret == MC_NORMAL_COMPLETION) &&
                (user_buf != nullptr)              &&
                (user_size > 0)                    &&
                ((user_size % sizeof(T)) == 0)     &&
                ((user_size % 2) == 0)              )
            {
                const size_t num_elems =
                    static_cast<size_t>( user_size ) / sizeof(T);
                const T* begin = static_cast<T*>( user_buf );
                const T* end = begin + num_elems;

                m_values.insert( m_values.cend(), begin, end );

                ret = MC_NORMAL_COMPLETION;
            }
            else if( call_ret != MC_NORMAL_COMPLETION )
            {
                ret = MC_CALLBACK_CANNOT_COMPLY;
            }
            else if( user_size > 0 && (user_size % 2) != 0 )
            {
                ret = MC_CALLBACK_DATA_SIZE_UNEVEN;
            }
            else
            {
                ret = MC_CALLBACK_PARM_ERROR;
            }

            first = false;
        }
        while( call_ret == MC_NORMAL_COMPLETION && user_last == 0 );
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return MC_NORMAL_COMPLETION;
}

template<class T, MC_VR VR>
MC_STATUS other_vr<T, VR>::get( const get_func_parms& val ) const
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( val.callback != nullptr )
    {
        constexpr size_t MAX_ELEMS = 256u;
        std::vector<T> buf;

        typename std::deque<T>::const_iterator begin = m_values.cbegin();
        typename std::deque<T>::const_iterator end = std::min( begin + MAX_ELEMS,
                                                               m_values.cend() );

        MC_STATUS call_ret = MC_NORMAL_COMPLETION;
        while( call_ret == MC_NORMAL_COMPLETION && begin != m_values.cend() )
        {
            bool first = begin == m_values.cbegin();
            bool last  = end   == m_values.cend();

            buf.clear();
            buf.insert( buf.cend(), begin, end );

            const int buf_size = static_cast<int>( buf.size() * sizeof(T) );

            call_ret = val.callback( val.message_id,
                                     val.tag,
                                     val.callback_parm,
                                     buf_size,
                                     static_cast<void*>( buf.data() ),
                                     static_cast<int>( first ),
                                     static_cast<int>( last ) );

            begin = end;
            end = std::min( begin + MAX_ELEMS, m_values.cend() );
        }

        if( call_ret != MC_NORMAL_COMPLETION )
        {
            ret = MC_CALLBACK_CANNOT_COMPLY;
        }
        else
        {
            ret = MC_NORMAL_COMPLETION;
        }
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

} // namespace vrs
} // namespace fume

#endif
