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
#include <vector>
#include <limits>
#include <algorithm>

// local public
#include "mcstatus.h"
#include "mc3msg.h"
#include "diction.h"

// local private
#include "fume/sink_callback_io.h"
#include "fume/rx_stream.h"
#include "fume/tx_stream.h"
#include "fume/vrs/ob.h"
#include "fume/library_context.h"

using std::vector;
using std::numeric_limits;
using std::min;

namespace fume
{

class encapsulated_callback_rx : public tx_stream
{
public:
    virtual MC_STATUS provide_value_length( uint32_t        length,
                                            TRANSFER_SYNTAX syntax ) = 0;
    virtual MC_STATUS provide_offset_table( const uint32_t* table,
                                            uint32_t        table_elements,
                                            TRANSFER_SYNTAX syntax ) = 0;

    virtual MC_STATUS start_of_frame( uint32_t size, TRANSFER_SYNTAX syntax ) = 0;
    virtual MC_STATUS end_of_sequence( TRANSFER_SYNTAX syntax ) = 0;

    virtual MC_STATUS finalize() = 0;
};

class application_callback_rx final : public encapsulated_callback_rx
{
public:
    application_callback_rx( const callback_parms_t& callback,
                             int                     message_id,
                             uint32_t                tag )
        : m_callback( callback ),
          m_message_id( message_id ),
          m_tag( tag ),
          m_first( true ),
          m_bytes_written( 0 )
    {
    }

    virtual MC_STATUS provide_value_length( uint32_t        length,
                                            TRANSFER_SYNTAX syntax ) override final;
    virtual MC_STATUS provide_offset_table( const uint32_t* table,
                                            uint32_t        table_elements,
                                            TRANSFER_SYNTAX syntax ) override final;

    virtual MC_STATUS write( const void* buffer, uint32_t buffer_bytes ) override final;
    virtual uint64_t bytes_written() const override final
    {
        return m_bytes_written;
    }

    virtual MC_STATUS start_of_frame( uint32_t        size,
                                      TRANSFER_SYNTAX syntax ) override final;
    virtual MC_STATUS end_of_sequence( TRANSFER_SYNTAX syntax ) override final;

    virtual MC_STATUS finalize() override final;

private:
    application_callback_rx( const application_callback_rx& );
    application_callback_rx& operator=( const application_callback_rx& );

private:
    const callback_parms_t& m_callback;
    int                     m_message_id;
    uint32_t                m_tag;
    bool                    m_first;
    uint64_t                m_bytes_written;
};

class vr_callback_rx final : public encapsulated_callback_rx
{
public:
    vr_callback_rx( const get_func_parms& parms, bool write_offset_table )
        : m_parms( parms ),
          m_write_offset_table( write_offset_table ),
          m_first( true ),
          m_bytes_written( 0 )
    {
    }

    virtual MC_STATUS provide_value_length( uint32_t        length,
                                            TRANSFER_SYNTAX syntax ) override final
    {
        // callback functions passed into MC_Get_Encapsulated_Value_To_Function
        // don't get the length
        return MC_NORMAL_COMPLETION;
    }

    virtual MC_STATUS provide_offset_table
    (
        const uint32_t* table,
        uint32_t        table_elements,
        TRANSFER_SYNTAX syntax
    ) override final
    {
        // callback functions passed into MC_Get_Encapsulated_Value_To_Function
        // don't get the offset table. However MC_Get_Offset_Table_To_Function
        // does get it.
        const uint32_t table_size = table_elements * sizeof(uint32_t);
        // Note: this data is in native format, and the callback
        // is expecting it in native format, so there is no need to
        // convert it here. It can just be treated as raw data
        return m_write_offset_table == true ? write( table, table_size ) :
                                              MC_NORMAL_COMPLETION;
    }

    virtual MC_STATUS write( const void* buffer, uint32_t buffer_bytes ) override final;
    virtual uint64_t bytes_written() const override final
    {
        return m_bytes_written;
    }

    virtual MC_STATUS start_of_frame( uint32_t        size,
                                      TRANSFER_SYNTAX syntax ) override final
    {
        // MC_Get_Encapsulated_Value_To_Function doesn't get the start of frame
        // tag
        return MC_NORMAL_COMPLETION;
    }
    virtual MC_STATUS end_of_sequence( TRANSFER_SYNTAX syntax ) override final
    {
        // MC_Get_Encapsulated_Value_To_Function doesn't get the end of sequence
        // tag
        return MC_NORMAL_COMPLETION;
    }

    MC_STATUS finalize() override final;

private:
    vr_callback_rx( const vr_callback_rx& );
    vr_callback_rx& operator=( const vr_callback_rx& );

private:
    const get_func_parms& m_parms;
    bool                  m_write_offset_table;
    bool                  m_first;
    uint64_t              m_bytes_written;
};

class ob_callback_rx final : public encapsulated_callback_rx
{
public:
    ob_callback_rx( fume::vrs::ob& element )
        : m_element( element )
    {
    }

    virtual MC_STATUS provide_value_length( uint32_t        length,
                                            TRANSFER_SYNTAX syntax ) override final
    {
        // OB class will write value length based on whether or not the data
        // is encapsulated
        return MC_NORMAL_COMPLETION;
    }

    virtual MC_STATUS provide_offset_table( const uint32_t* table,
                                            uint32_t        table_elements,
                                            TRANSFER_SYNTAX syntax ) override final;

    virtual MC_STATUS write( const void* buffer, uint32_t buffer_bytes ) override final
    {
        return m_element.write( buffer, buffer_bytes );
    }

    virtual uint64_t bytes_written() const override final
    {
        return m_element.bytes_written();
    }

    virtual MC_STATUS start_of_frame( uint32_t        size,
                                      TRANSFER_SYNTAX syntax ) override final;
    virtual MC_STATUS end_of_sequence( TRANSFER_SYNTAX syntax ) override final;

    virtual MC_STATUS finalize() override final
    {
        // No need to do anything here
        return MC_NORMAL_COMPLETION;
    }

private:
    ob_callback_rx( const ob_callback_rx& );
    ob_callback_rx& operator=( const ob_callback_rx& );

private:
    fume::vrs::ob& m_element;
};

static MC_STATUS write_sequence_delimiter( tx_stream&      stream,
                                           TRANSFER_SYNTAX syntax );

static MC_STATUS write_tag_and_length( tx_stream&      stream, 
                                       TRANSFER_SYNTAX syntax,
                                       uint32_t        tag,
                                       uint32_t        length );

static MC_STATUS read_raw_value_to_callback( rx_stream&                source,
                                             encapsulated_callback_rx& dest,
                                             uint32_t                  vr_length );

static MC_STATUS read_offset_table_to_callback( rx_stream&                source,
                                                TRANSFER_SYNTAX           syntax,
                                                encapsulated_callback_rx& dest );

static MC_STATUS read_encapsulated_value_to_callback
(
    rx_stream&                source,
    TRANSFER_SYNTAX           syntax,
    encapsulated_callback_rx& dest
);

MC_STATUS application_callback_rx::provide_value_length( uint32_t        length,
                                                         TRANSFER_SYNTAX syntax )
{
    // Caller should have checked this
    assert( m_callback.first != nullptr );

    int last = 0;
    void* buffer = nullptr;
    unsigned long size = size;

    // "first" parameter is not used for this function
    const MC_STATUS cbstat = m_callback.first( m_message_id,
                                               m_tag,
                                               m_callback.second,
                                               PROVIDING_DATA_LENGTH,
                                               &size,
                                               &buffer,
                                               static_cast<int>( false ),
                                               &last );

    return cbstat != MC_NORMAL_COMPLETION ? MC_CALLBACK_CANNOT_COMPLY :
                                            MC_NORMAL_COMPLETION;
}

MC_STATUS application_callback_rx::provide_offset_table
(
    const uint32_t* table,
    uint32_t        table_elements,
    TRANSFER_SYNTAX syntax
)
{
    // Caller should have checked this
    assert( m_callback.first != nullptr );
    assert( table != nullptr );

    // For offset table, the entire table is provided in one call,
    // and both first and last are set to true per the spec. The
    // m_first variable only affects the sending of data, not
    // the offset table
    int last = 1;
    void* buffer = const_cast<void*>( static_cast<const void*>( table ) );

    unsigned long size = table_elements * sizeof(uint32_t);
    // Note: this data is in native format, and the callback
    // is expecting it in native format, so there is no need to
    // convert it here. It can just be treated as raw data
    const MC_STATUS cbstat = m_callback.first( m_message_id,
                                               m_tag,
                                               m_callback.second,
                                               PROVIDING_OFFSET_TABLE,
                                               &size,
                                               &buffer,
                                               static_cast<int>( true ),
                                               &last );

    return cbstat != MC_NORMAL_COMPLETION ? MC_CALLBACK_CANNOT_COMPLY :
                                            MC_NORMAL_COMPLETION;
}

MC_STATUS application_callback_rx::write( const void* data, uint32_t buffer_bytes )
{
    // Caller should have checked this
    assert( m_callback.first != nullptr );
    assert( data != nullptr );

    int last = 0;
    void* buffer = const_cast<void*>( data );
    unsigned long size = buffer_bytes;
    MC_STATUS ret = MC_CANNOT_COMPLY;

    const MC_STATUS cbstat = m_callback.first( m_message_id,
                                               m_tag,
                                               m_callback.second,
                                               PROVIDING_DATA,
                                               &size,
                                               &buffer,
                                               static_cast<int>( m_first ),
                                               &last );

    m_first = false;
    if( cbstat == MC_NORMAL_COMPLETION )
    {
        m_bytes_written += buffer_bytes;
        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_CALLBACK_CANNOT_COMPLY;
    }

    return ret;
}

MC_STATUS application_callback_rx::start_of_frame( uint32_t        size,
                                                   TRANSFER_SYNTAX syntax )
{
    return write_tag_and_length( *this, syntax, MC_ATT_ITEM, size );
}

MC_STATUS application_callback_rx::end_of_sequence( TRANSFER_SYNTAX syntax )
{
    return write_sequence_delimiter( *this, syntax );
}

MC_STATUS application_callback_rx::finalize()
{
    // Caller should have checked this
    assert( m_callback.first != nullptr );

    // The spec says data can be NULL when last is true, but just to be safe
    uint8_t dummy_data = 0;
    int last = 1;
    void* buffer = static_cast<void*>( &dummy_data );
    unsigned long size = 0;

    const MC_STATUS cbstat = m_callback.first( m_message_id,
                                           m_tag,
                                           m_callback.second,
                                           PROVIDING_DATA,
                                           &size,
                                           &buffer,
                                           static_cast<int>( m_first ),
                                           &last );

    return cbstat == MC_NORMAL_COMPLETION ? MC_NORMAL_COMPLETION :
                                            MC_CALLBACK_CANNOT_COMPLY;
}

MC_STATUS vr_callback_rx::write( const void* buffer, uint32_t buffer_bytes )
{
    // Caller should have checked this
    assert( m_parms.callback != nullptr );
    assert( buffer != nullptr );

    const MC_STATUS cbstat = m_parms.callback( m_parms.message_id,
                                               m_parms.tag,
                                               m_parms.callback_parm,
                                               static_cast<int>( buffer_bytes ),
                                               const_cast<void*>( buffer ),
                                               static_cast<int>( m_first ),
                                               static_cast<int>( false ) );

    m_first = false;
    MC_STATUS ret = MC_CANNOT_COMPLY;
    if( cbstat == MC_NORMAL_COMPLETION )
    {
        m_bytes_written += buffer_bytes;
        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_CALLBACK_CANNOT_COMPLY;
    }

    return ret;
}

MC_STATUS vr_callback_rx::finalize()
{
    // Caller should have checked this
    assert( m_parms.callback != nullptr );

    // Pass in an address even though the size will be zero just in case
    uint8_t dummy_buffer = 0;
    const MC_STATUS cbstat = m_parms.callback( m_parms.message_id,
                                               m_parms.tag,
                                               m_parms.callback_parm,
                                               0,
                                               static_cast<void*>( &dummy_buffer ),
                                               static_cast<int>( m_first ),
                                               static_cast<int>( true ) );

    m_first = false;
    return cbstat == MC_NORMAL_COMPLETION ? MC_NORMAL_COMPLETION :
                                            MC_CALLBACK_CANNOT_COMPLY;
}

MC_STATUS ob_callback_rx::provide_offset_table( const uint32_t* table,
                                                uint32_t        table_elements,
                                                TRANSFER_SYNTAX syntax )
{
    // When the offset table is read this will be fed back through these
    // functions when provided to the user. So save in wire format
    assert( table != nullptr );

    const uint32_t table_bytes = table_elements * sizeof(uint32_t);

    MC_STATUS ret = write_tag_and_length( m_element,
                                          syntax,
                                          MC_ATT_ITEM,
                                          table_bytes );
    for( uint32_t i = 0; ret == MC_NORMAL_COMPLETION && i < table_elements; ++i )
    {
        ret = m_element.write_val( table[i], syntax );
    }

    return ret;
}

MC_STATUS ob_callback_rx::start_of_frame( uint32_t        size,
                                          TRANSFER_SYNTAX syntax )
{
    return write_tag_and_length( m_element, syntax, MC_ATT_ITEM, size );
}

MC_STATUS ob_callback_rx::end_of_sequence( TRANSFER_SYNTAX syntax )
{
    return write_sequence_delimiter( m_element, syntax );
}

MC_STATUS read_vr_data_to_callback( rx_stream&              stream,
                                    TRANSFER_SYNTAX         syntax,
                                    int                     message_id,
                                    uint32_t                tag,
                                    const callback_parms_t& callback )
{
    application_callback_rx dest( callback, message_id, tag );

    uint32_t vr_length = 0;
    MC_STATUS ret = stream.read_val( vr_length, syntax );
    if( ret == MC_NORMAL_COMPLETION )
    {
        // Write the length
        // caller should validate the callback function is registered with
        // a tag with ID of OB, OW, OD, OF, or OL. Therefore the length
        // will always be 32-bit
        MC_VR tag_vr = UNKNOWN_VR;
        assert( g_context != nullptr );
        assert( g_context->get_vr_type( tag,
                                        nullptr,
                                        tag_vr ) == MC_NORMAL_COMPLETION &&
                (tag_vr == OB || tag_vr == OW ||
                 tag_vr == OL || tag_vr == OD || tag_vr == OF) );

        ret = dest.provide_value_length( vr_length, syntax );
        if( ret == MC_NORMAL_COMPLETION )
        {
            if( vr_length == numeric_limits<uint32_t>::max() )
            {
                ret = read_encapsulated_value_to_callback( stream, syntax, dest );
            }
            else
            {
                ret = read_raw_value_to_callback( stream, dest, vr_length );
                if( ret == MC_NORMAL_COMPLETION )
                {
                    ret = dest.finalize();
                }
            }
        }
    }
    else
    {
        // Do nothing. Will return error from get_value_length
    }

    return ret;
}

MC_STATUS read_raw_value_to_callback( rx_stream&                stream,
                                      encapsulated_callback_rx& dest,
                                      uint32_t                  vr_length )
{
    MC_STATUS ret = MC_NORMAL_COMPLETION;

    uint8_t buffer[1024];
    uint32_t bytes_written = 0;
    while( ret == MC_NORMAL_COMPLETION && bytes_written < vr_length )
    {
        const uint32_t bytes_remaining = vr_length - bytes_written;
        const uint32_t bytes_read = min( static_cast<uint32_t>( sizeof(buffer) ),
                                         bytes_remaining );
        ret = stream.read( buffer, bytes_read );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = dest.write( buffer, bytes_read );
        }
        else
        {
            // Do nothing. Will return error
        }
    }

    return ret;
}

MC_STATUS read_encapsulated_frame_to_callback
(
    rx_stream&                source,
    TRANSFER_SYNTAX           syntax,
    encapsulated_callback_rx& dest
)
{
    uint32_t tag = 0;
    MC_STATUS ret = source.read_tag( tag, syntax );
    if( ret == MC_NORMAL_COMPLETION )
    {
        if( tag == MC_ATT_ITEM )
        {
            uint32_t length = 0;
            ret = source.read_val( length, syntax );
            if( ret == MC_NORMAL_COMPLETION )
            {
                ret = dest.start_of_frame( length, syntax );
                if( ret == MC_NORMAL_COMPLETION )
                {
                    ret = read_raw_value_to_callback( source,
                                                      dest,
                                                      length );
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
        }
        else if( tag == MC_ATT_SEQUENCE_DELIMITATION_ITEM )
        {
            ret = dest.end_of_sequence( syntax );
            if( ret == MC_NORMAL_COMPLETION )
            {
                ret = MC_NO_MORE_VALUES;
            }
        }
        else
        {
            ret = MC_MISSING_DELIMITER;
        }
    }

    return ret;
}

MC_STATUS read_encapsulated_frames_to_callback
(
    rx_stream&                source,
    TRANSFER_SYNTAX           syntax,
    encapsulated_callback_rx& dest
)
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    do
    {
        ret = read_encapsulated_frame_to_callback( source, syntax, dest );
    }
    while( ret == MC_NORMAL_COMPLETION );

    return ret;
}

MC_STATUS read_encapsulated_value_to_callback
(
    rx_stream&                source,
    TRANSFER_SYNTAX           syntax,
    encapsulated_callback_rx& dest
)
{
    MC_STATUS ret = read_offset_table_to_callback( source, syntax, dest );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = read_encapsulated_frames_to_callback( source, syntax, dest );
    }
    else
    {
        // Do nothing. Will return error
    }

    return ret;
}

MC_STATUS read_offset_table_to_callback( rx_stream&                source,
                                         TRANSFER_SYNTAX           syntax,
                                         encapsulated_callback_rx& dest )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    uint32_t tag = 0;
    ret = source.read_tag( tag, syntax );
    if( ret == MC_NORMAL_COMPLETION )
    {
        if( tag == MC_ATT_ITEM )
        {
            uint32_t length = 0;
            ret = source.read_val( length, syntax );
            if( ret == MC_NORMAL_COMPLETION )
            {
                if( length % sizeof(uint32_t) == 0 )
                {
                    const uint32_t num_elems = length / sizeof(uint32_t);
                    // Make sure the vector has at least one element so the buffer
                    // we pass into the function is valid even if there is
                    // no offset table
                    vector<uint32_t> offset_table( min( 1u, num_elems ) );
                    ret = MC_NORMAL_COMPLETION;
                    for( uint32_t i = 0;
                         (ret == MC_NORMAL_COMPLETION) && (i < num_elems);
                         ++i )
                    {
                        ret = source.read_val( offset_table[i], syntax );
                    }

                    if( ret == MC_NORMAL_COMPLETION )
                    {
                        ret = dest.provide_offset_table( offset_table.data(),
                                                         length,
                                                         syntax );
                    }
                    else
                    {
                        // Do nothing. Will return error
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
        }
        else
        {
            ret = MC_MISSING_DELIMITER;
        }
    }
    else
    {
        // Do nothing. Will return error
    }

    return ret;
}

MC_STATUS read_offset_table_to_function( rx_stream&            stream, 
                                         TRANSFER_SYNTAX       syntax,
                                         const get_func_parms& parms )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( parms.callback != nullptr )
    {
        vr_callback_rx callback( parms, true );
        ret = read_offset_table_to_callback( stream, syntax, callback );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = callback.finalize();
        }
        else
        {
            // TODO: determine if we still need to call finalize here
            (void)callback.finalize();
        }
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS read_encapsulated_value_to_function( rx_stream&            stream,
                                               TRANSFER_SYNTAX       syntax,
                                               const get_func_parms& parms )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( parms.callback != nullptr )
    {
        vr_callback_rx callback( parms, false );
        ret = read_offset_table_to_callback( stream, syntax, callback );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = read_encapsulated_frame_to_callback( stream, syntax, callback );
            if( ret == MC_NORMAL_COMPLETION )
            {
                ret = callback.finalize();
            }
            else
            {
                // TODO: determine if we still need to call finalize here
                (void)callback.finalize();
            }
        }
        else
        {
            // TODO: determine if we still need to call finalize here
            (void)callback.finalize();
        }
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS read_next_encapsulated_value_from_function( rx_stream&            stream,
                                                      TRANSFER_SYNTAX       syntax,
                                                      const get_func_parms& parms )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( parms.callback != nullptr )
    {
        vr_callback_rx callback( parms, false );
        ret = read_encapsulated_frame_to_callback( stream, syntax, callback );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = callback.finalize();
        }
        else
        {
            // TODO: determine if we still need to call finalize here
            (void)callback.finalize();
        }
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS write_sequence_delimiter( tx_stream&      stream,
                                    TRANSFER_SYNTAX syntax )
{
    return write_tag_and_length( stream, syntax, MC_ATT_ITEM, 0u );
}

MC_STATUS write_tag_and_length( tx_stream&      stream, 
                                TRANSFER_SYNTAX syntax,
                                uint32_t        tag,
                                uint32_t        length )
{
    MC_STATUS ret = stream.write_tag( tag, syntax );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = stream.write_val( length, syntax );
    }
    else
    {
        // Do nothing. Will return error
    }

    return ret;
}

}
