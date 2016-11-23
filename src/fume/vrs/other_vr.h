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
#include <cassert>
#include <vector>
#include <algorithm>
#include <memory>

// local public
#include "mc3msg.h"

// local private
#include "fume/value_representation.h"
#include "fume/tx_stream.h"
#include "fume/rx_stream.h"
#include "fume/seekable_stream.h"
#include "fume/memory_stream.h"
#include "fume/sink_callback_io.h"

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
        : value_representation( 1u, 1u, 1u ),
          m_stream( new memory_stream() )
    {
    }
    virtual ~other_vr()
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
    virtual MC_STATUS set( const set_func_parms& val ) override final;

    // Sets the value of the data element to NULL (ie. zero length)
    virtual MC_STATUS set_null() override final
    {
        return m_stream->clear();
    }

// value_representation -- accessors
public:
    virtual MC_STATUS get( const get_func_parms& val ) override final;

    // Returns the number of elements
    virtual int count() const override final
    {
        return static_cast<int>( m_stream->bytes_written() != 0 );
    }

    // Indicates whether or not the element is null
    virtual bool is_null() const override final
    {
        return m_stream->bytes_written() == 0;
    }

    virtual MC_VR vr() const override final
    {
        return VR;
    }

    virtual std::unique_ptr<value_representation> clone() const override
    {
        std::unique_ptr<other_vr> ret( new other_vr() );
        ret->m_stream = m_stream->clone();

        return std::move( ret );
    }

private:
    std::unique_ptr<seekable_stream> m_stream;
};

template<class T, MC_VR VR>
MC_STATUS other_vr<T, VR>::from_stream( rx_stream&      stream,
                                        TRANSFER_SYNTAX syntax )
{
    uint32_t value_length = 0;
    std::unique_ptr<seekable_stream> tmp_stream( new memory_stream() );

    MC_STATUS ret = stream.read_val( value_length, syntax );

    if( ret == MC_NORMAL_COMPLETION )
    {
        if( ((value_length % sizeof(T)) == 0) && (value_length % 2) == 0 )
        {
            const uint32_t num_items = value_length / sizeof(T);
            for( uint32_t i = 0; i < num_items && ret == MC_NORMAL_COMPLETION; ++i )
            {
                T val;
                ret = stream.read_val( val, syntax );
                if( ret == MC_NORMAL_COMPLETION )
                {
                    // Always write values to the local stream in explicit
                    // little endian. Big Endian is retired, so it's the most
                    // likely endianness we're going to be writing
                    ret = tmp_stream->write_val( val, EXPLICIT_LITTLE_ENDIAN );
                }
                else
                {
                    // Do nothing. Will return error
                }
            }

            if( ret == MC_NORMAL_COMPLETION )
            {
                // Only update values if everything succeeded
                m_stream.swap( tmp_stream );
            }
            else
            {
                // Leave current data unchanged
            }
        }
        else
        {
            ret = MC_INVALID_LENGTH_FOR_VR;
        }
    }

    return ret;
}

template<class T, MC_VR VR>
MC_STATUS other_vr<T, VR>::to_stream( tx_stream&      stream,
                                      TRANSFER_SYNTAX syntax )
{
    // TODO: ensure this in the callback setter
    const uint32_t data_length = static_cast<uint32_t>( m_stream->bytes_written() );
    MC_STATUS ret = stream.write_val( data_length, syntax );
    if( ret == MC_NORMAL_COMPLETION && data_length > 0u )
    {
        // from_stream and set both verify data size is a multiple of the
        // datatype size
        assert( (data_length % sizeof(T)) == 0 );
        const uint32_t num_elems = data_length / sizeof(T);
        ret = m_stream->rewind_read();
        for( uint32_t i = 0;
             ret == MC_NORMAL_COMPLETION && i < num_elems;
             ++i )
        {
            T val;
            ret = m_stream->read_val( val, EXPLICIT_LITTLE_ENDIAN );
            if( ret == MC_NORMAL_COMPLETION )
            {
                ret = stream.write_val( val, syntax );
            }
            else
            {
                // Do nothing. Will return error
            }
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
    std::unique_ptr<seekable_stream> tmp_stream( new memory_stream() );

    if( val.callback != nullptr )
    {
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

                for( size_t i = 0;
                     ret == MC_NORMAL_COMPLETION && i < num_elems;
                     ++i )
                {
                    ret = tmp_stream->write_val( begin[i],
                                                 EXPLICIT_LITTLE_ENDIAN );
                }

                if( ret == MC_NORMAL_COMPLETION )
                {
                    m_stream.swap( tmp_stream );
                }
                else
                {
                    // Leave the current value unmodified
                }
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
MC_STATUS other_vr<T, VR>::get( const get_func_parms& val )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( val.callback != nullptr )
    {
        // Only encapsulated values can exceed 32-bits in length
        uint32_t remaining_bytes =
            static_cast<uint32_t>( m_stream->bytes_written() -
                                   m_stream->bytes_read() );

        bool first = true;

        MC_STATUS call_ret = MC_NORMAL_COMPLETION;
        ret = m_stream->rewind_read();
        while( call_ret == MC_NORMAL_COMPLETION && 
               ret == MC_NORMAL_COMPLETION &&
               remaining_bytes > 0 )
        {
            assert( (remaining_bytes % sizeof(T)) == 0 );

            const uint32_t remaining_elems = remaining_bytes / sizeof(T);

            T val;
            ret = m_stream->read_val( val, EXPLICIT_LITTLE_ENDIAN );

            if( ret == MC_NORMAL_COMPLETION )
            {
                const bool last = remaining_elems == 1;
                call_ret = val.callback( val.message_id,
                                         val.tag,
                                         val.callback_parm,
                                         sizeof(val),
                                         static_cast<void*>( &val ),
                                         static_cast<int>( first ),
                                         static_cast<int>( last ) );
                first = false;
            }
            else
            {
                // Do nothing. Will return error
            }
        }

        if( call_ret != MC_NORMAL_COMPLETION )
        {
            ret = MC_CALLBACK_CANNOT_COMPLY;
        }
        else
        {
            // Do nothing. Will return error code or success
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
