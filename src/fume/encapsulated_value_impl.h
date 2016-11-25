#ifndef ENCAPSULATED_VALUE_IMPL_H
#define ENCAPSULATED_VALUE_IMPL_H
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
#include <algorithm>
#include <deque>

// local public
#include "mcstatus.h"

// local private
#include "fume/encapsulated_value.h"

namespace fume
{

/** A template class implementation of encapsulated_value_stream
 *  which wraps a seekable_stream instance.
 *
 * This class provides access to an encapsulated value stored in a
 * seekable_stream object. It allows this encapsulated value to be
 * stored to either memory or on-disk.
 *
 * Note that if an error occurs at any point during the population
 * of this object that the contained data may be in an inconsistent
 * state. It is recommended that a local copy of this object be
 * created/used while reading data, and upon successful reading/storage
 * the local copy being swapped with the persistent copy. This will
 * ensure the data is kept consistent
 */

template<class SeekableStream>
class encapsulated_value_impl : public encapsulated_value
{
public:
    encapsulated_value_impl()
        : m_is_encapsulated( false ),
          m_received_empty_offset_table( true ),
          m_end_of_table_offset( 0 )
    {
    }

    virtual ~encapsulated_value_impl()
    {
    }

    virtual MC_STATUS write_vr_data_to_stream
    (
        tx_stream&      stream,
        TRANSFER_SYNTAX syntax
    ) override final;

    virtual MC_STATUS write_data_to_stream
    (
        tx_stream&      stream,
        TRANSFER_SYNTAX syntax
    ) override final;

    virtual MC_STATUS write_first_frame_to_stream
    (
        encapsulated_value_sink& stream,
        TRANSFER_SYNTAX          syntax
    ) override final;

    virtual MC_STATUS write_next_frame_to_stream
    (
        encapsulated_value_sink& stream,
        TRANSFER_SYNTAX          syntax
    ) override final;

    virtual MC_STATUS write_frame_to_stream
    (
        unsigned int             idx,
        encapsulated_value_sink& stream,
        TRANSFER_SYNTAX          syntax
    ) override final;

    virtual MC_STATUS clear() override final
    {
        return m_stream.clear();
    }
    virtual uint64_t size() const override final
    {
        return m_stream.size();
    }

    virtual MC_STATUS provide_value_length( uint32_t        length,
                                            TRANSFER_SYNTAX syntax ) override final
    {
        m_is_encapsulated = length == std::numeric_limits<uint32_t>::max();
        return m_stream.clear();
    }

    virtual MC_STATUS provide_offset_table( const uint32_t* table,
                                            uint32_t        table_elements,
                                            TRANSFER_SYNTAX syntax ) override final;

    virtual MC_STATUS start_of_frame( uint32_t        size,
                                      TRANSFER_SYNTAX syntax ) override final;

    virtual MC_STATUS end_of_sequence( TRANSFER_SYNTAX syntax ) override final;

    virtual MC_STATUS finalize() override final
    {
        return MC_NORMAL_COMPLETION;
    }

    virtual MC_STATUS write( const void* buffer,
                             uint32_t    buffer_bytes ) override final
    {
        return m_stream.write( buffer, buffer_bytes );
    }

    virtual uint64_t tell_write() const override final
    {
        return m_stream.tell_write();
    }

    virtual std::unique_ptr<encapsulated_value> clone() const override final
    {
        return std::unique_ptr<encapsulated_value>
               (
                   new encapsulated_value_impl( *this )
               );
    }

private:
    encapsulated_value_impl( const encapsulated_value_impl& rhs )
        : m_stream( rhs.m_stream ),
          m_offset_table( rhs.m_offset_table ),
          m_is_encapsulated( rhs.m_is_encapsulated ),
          m_received_empty_offset_table( rhs.m_received_empty_offset_table ),
          m_end_of_table_offset( rhs.m_end_of_table_offset )
    {
    }

    MC_STATUS correct_frame_size();

private:
    SeekableStream       m_stream;
    std::deque<uint64_t> m_offset_table;
    bool                 m_is_encapsulated;
    bool                 m_received_empty_offset_table;
    uint64_t             m_end_of_table_offset;
};

template<class SeekableStream>
MC_STATUS encapsulated_value_impl<SeekableStream>::write_vr_data_to_stream
(
    tx_stream&      stream,
    TRANSFER_SYNTAX syntax
)
{
    const uint32_t value_length =
        m_is_encapsulated ? std::numeric_limits<uint32_t>::max() :
                            static_cast<uint32_t>( m_stream.size() );
    MC_STATUS ret = stream.write_val( value_length, syntax );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = write_data_to_stream( stream, syntax );
    }
    else
    {
        // Do nothing. Return error
    }

    return ret;
}

template<class SeekableStream>
MC_STATUS encapsulated_value_impl<SeekableStream>::write_data_to_stream
(
    tx_stream&      stream,
    TRANSFER_SYNTAX syntax
)
{
    uint64_t bytes_remaining = m_stream.size();
    MC_STATUS ret = m_stream.rewind();
    while( ret == MC_NORMAL_COMPLETION && bytes_remaining > 0 )
    {
        uint8_t buf[1024];
        uint32_t to_read = std::min( bytes_remaining,
                                     static_cast<uint64_t>( sizeof(buf) ) );
        ret = m_stream.read( buf, to_read );
        bytes_remaining -= to_read;
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = stream.write( buf, to_read );
        }
        else
        {
            // Don't write data to stream. Return error
        }
    }

    return ret;
}

template<class SeekableStream>
MC_STATUS encapsulated_value_impl<SeekableStream>::write_first_frame_to_stream
(
    encapsulated_value_sink& stream,
    TRANSFER_SYNTAX          syntax
)
{
    return write_frame_to_stream( 0, stream, syntax );
}

template<class SeekableStream>
MC_STATUS encapsulated_value_impl<SeekableStream>::write_next_frame_to_stream
(
    encapsulated_value_sink& stream,
    TRANSFER_SYNTAX          syntax
)
{
    MC_STATUS ret = MC_NORMAL_COMPLETION;

    if( m_is_encapsulated == true )
    {
        ret = read_encapsulated_frame( m_stream, syntax, stream );
    }
    else
    {
        ret = MC_INVALID_TRANSFER_SYNTAX;
    }

    return ret;
}

template<class SeekableStream>
MC_STATUS encapsulated_value_impl<SeekableStream>::write_frame_to_stream
(
    unsigned int             idx,
    encapsulated_value_sink& stream,
    TRANSFER_SYNTAX          syntax
)
{
    MC_STATUS ret = MC_NORMAL_COMPLETION;

    if( m_is_encapsulated == true )
    {
        if( m_offset_table.size() > idx )
        {
            const uint64_t frame_offset = m_end_of_table_offset +
                                          m_offset_table[idx];
            ret = m_stream.seek( frame_offset );
            if( ret == MC_NORMAL_COMPLETION )
            {
                ret = write_next_frame_to_stream( stream, syntax );
            }
            else
            {
                // Return error
            }
        }
        else
        {
            ret = MC_NO_MORE_VALUES;
        }
    }
    else
    {
        ret = MC_INVALID_TRANSFER_SYNTAX;
    }

    return ret;
}

template<class SeekableStream>
MC_STATUS encapsulated_value_impl<SeekableStream>::provide_offset_table
(
    const uint32_t* table,
    uint32_t        table_elements,
    TRANSFER_SYNTAX syntax
)
{
    // Caller should only call this if encapsulated
    assert( m_is_encapsulated == true );

    MC_STATUS ret = MC_CANNOT_COMPLY;

    // Write the offset table as an item tag with the specified length.
    // NOTE: the length provided in the file is the number of elements
    // in the table.
    ret = m_stream.write_tag( MC_ATT_ITEM, EXPLICIT_LITTLE_ENDIAN );
    if( ret == MC_NORMAL_COMPLETION )
    {
        const uint32_t table_bytes = table_elements * sizeof(uint32_t);
        ret = m_stream.write_val( table_bytes, EXPLICIT_LITTLE_ENDIAN );
        if( ret == MC_NORMAL_COMPLETION )
        {
            if( table_elements > 0 )
            {
                assert( table != nullptr );
                std::deque<uint64_t> tmp_offsets( table, table + table_elements );

                for( uint32_t i = 0;
                     ret == MC_NORMAL_COMPLETION && i < table_elements;
                     ++i )
                {
                    ret = m_stream.write_val( table[i], EXPLICIT_LITTLE_ENDIAN );
                }

                if( ret == MC_NORMAL_COMPLETION )
                {
                    m_end_of_table_offset = m_stream.tell_write();
                    m_offset_table.swap( tmp_offsets );
                    // Signal that we recieved a populated offset table
                    // and should not attenpt to create our own
                    m_received_empty_offset_table = false;
                }
                else
                {
                    // Leave the structures unmodified and return an error
                }
            }
            else
            {
                m_end_of_table_offset = m_stream.tell_write();
                // Clear the offset table
                m_offset_table.clear();
                // Signal that we did not receive an offset table
                // and will need to create one
                m_received_empty_offset_table = true;
            }
        }
        else
        {
            // Leave everything unmodified and return error
        }
    }
    else
    {
        // Do nothing. Will return error and leave everything unmodified
    }

    return ret;
}

template<class SeekableStream>
MC_STATUS encapsulated_value_impl<SeekableStream>::start_of_frame
(
    uint32_t        size,
    TRANSFER_SYNTAX syntax
)
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    // If we didn't receive an offset table
    if( m_received_empty_offset_table == true )
    {
        ret = correct_frame_size();
        // Add an entry to the "virtual" offset table
        if( ret == MC_NORMAL_COMPLETION )
        {
            m_offset_table.push_back( m_stream.tell_write() );
        }
        else
        {
            // Return error
        }
    }
    else
    {
        ret = MC_NORMAL_COMPLETION;
    }

    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = m_stream.write_tag( MC_ATT_ITEM, EXPLICIT_LITTLE_ENDIAN );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = m_stream.write_val( size, EXPLICIT_LITTLE_ENDIAN );
        }
        else
        {
            // Do nothing. Will report error
        }
    }
    else
    {
        // Do nothing. Return error
    }

    return ret;
}

template<class SeekableStream>
MC_STATUS encapsulated_value_impl<SeekableStream>::end_of_sequence
(
    TRANSFER_SYNTAX syntax
)
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( m_received_empty_offset_table == true )
    {
        ret = correct_frame_size();
    }
    else
    {
        ret = MC_NORMAL_COMPLETION;
    }

    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = m_stream.write_tag( MC_ATT_SEQUENCE_DELIMITATION_ITEM,
                                        EXPLICIT_LITTLE_ENDIAN );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = m_stream.write_val( static_cast<uint32_t>( 0u ),
                                      EXPLICIT_LITTLE_ENDIAN );
        }
        else
        {
            // Do nothing. Will return error
        }
    }
    else
    {
        // Do nothing. Will return error
    }

    return ret;
}

template<class SeekableStream>
MC_STATUS encapsulated_value_impl<SeekableStream>::correct_frame_size()
{
    const uint64_t cur_pos = m_stream.tell_write();
    // Get the offset of the previous frame
    const uint64_t prev_pos = m_offset_table.empty() ? m_end_of_table_offset :
                                                       m_offset_table.back();
    // Size is equal to the current position minus the previous position
    // and the item delimiter and size (8 bytes)
    const uint32_t size =
        static_cast<uint32_t>( (cur_pos - prev_pos) - (sizeof(uint32_t) * 2) );

    // Seek position in this stream is the position of the size parameter
    // in the message. The offset table offset is referenced from the end
    // of the offset table, so add the offset of the end of that to the
    // position. That puts us at the item delimiter, so skip that
    const uint64_t seek_pos = m_end_of_table_offset + prev_pos + sizeof(uint32_t);
    MC_STATUS ret = m_stream.seek( seek_pos );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = m_stream.write_val( size, EXPLICIT_LITTLE_ENDIAN );
        if( ret == MC_NORMAL_COMPLETION )
        {
            // Put the stream back to where it was
            ret = m_stream.seek( cur_pos );
        }
        else
        {
            // Return error
        }
    }
    else
    {
        // return error
    }

    return ret;
}

}

#endif
