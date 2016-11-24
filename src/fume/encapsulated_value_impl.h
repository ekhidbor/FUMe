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
        : m_current_frame_idx( 0 ),
          m_is_encapsulated( false ),
          m_received_empty_offset_table( true )
    {
    }
    virtual ~encapsulated_value_impl()
    {
    }

    virtual MC_STATUS write_value_to_stream
    (
        tx_stream&      stream,
        TRANSFER_SYNTAX syntax
    ) override final;

    virtual MC_STATUS write_first_frame_to_stream
    (
        tx_stream&      stream,
        TRANSFER_SYNTAX syntax
    ) override final;

    virtual MC_STATUS write_next_frame_to_stream
    (
        tx_stream&      stream,
        TRANSFER_SYNTAX syntax
    ) override final;

    virtual MC_STATUS write_frame_to_stream
    (
        unsigned int    idx,
        tx_stream&      stream,
        TRANSFER_SYNTAX syntax
    ) override final;

    virtual MC_STATUS clear();
    virtual uint64_t size() const;

    virtual MC_STATUS provide_value_length( uint32_t        length,
                                            TRANSFER_SYNTAX syntax )
    {
        m_is_encapsulated = length == numeric_limits<uint32_t>::max();
        return MC_NORMAL_COMPLETION;
    }

    virtual MC_STATUS provide_offset_table( const uint32_t* table,
                                            uint32_t        table_elements,
                                            TRANSFER_SYNTAX syntax );

    virtual MC_STATUS start_of_frame( uint32_t size, TRANSFER_SYNTAX syntax );

    virtual MC_STATUS end_of_sequence( TRANSFER_SYNTAX syntax );

    virtual MC_STATUS finalize()
    {
        return MC_NORMAL_COMPLETION;
    }

    virtual MC_STATUS write( const void* buffer, uint32_t buffer_bytes )
    {
        return m_stream.write( buffer, buffer_bytes );
    }

    virtual uint64_t bytes_written() const
    {
        return m_stream.bytes_written();
    }

private:
    SeekableStream       m_stream;
    std::deque<uint64_t> m_offset_table;
    bool                 m_is_encapsulated;
    bool                 m_received_empty_offset_table;
};

template<class SeekableStream>
MC_STATUS encapsulated_value_stream_impl<SeekableStream>::provide_offset_table
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
        const uint32_t table_bytes = table_length * sizeof(uint32_t);
        ret = m_stream.write_val( table_bytes, EXPLICIT_LITTLE_ENDIAN );
        if( ret == MC_NORMAL_COMPLETION )
        {
            if( table_length > 0 )
            {
                assert( offset_table != nullptr );
                deque<uint64_t> tmp_offsets( offset_table,
                                             offset_table + table_length );

                for( uint32_t i = 0;
                     ret == MC_NORMAL_COMPLETION && i < table_length;
                     ++i )
                {
                    ret = m_stream.write_val( offset[table],
                                              EXPLICIT_LITTLE_ENDIAN );
                }

                if( ret == MC_NORMAL_COMPLETION )
                {
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
MC_STATUS encapsulated_value_stream_impl<SeekableStream>::start_of_frame
(
    uint32_t        size,
    TRANSFER_SYNTAX syntax
)
{
    // If we didn't receive an offset table
    if( m_received_empty_offset_table == true )
    {
        // Add an entry to the "virtual" offset table
        m_offset_table.push_back( m_stream->bytes_written() );
    }
    else
    {
        // Leave received offset table unmodified
    }

    MC_STATUS ret = m_stream.write_tag( MC_ATT_ITEM, EXPLICIT_LITTLE_ENDIAN );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = m_stream.write_val( size, EXPLICIT_LITTLE_ENDIAN );
    }
    else
    {
        // Do nothing. Will report error
    }

    return ret;
}

template<class SeekableStream>
MC_STATUS encapsulated_value_stream_impl<SeekableStream>::end_of_sequence
(
    TRANSFER_SYNTAX syntax
)
{
    MC_STATUS ret = m_stream.write_tag( MC_ATT_SEQUENCE_DELIMITATION_ITEM,
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

    return ret;
}

}

#endif
