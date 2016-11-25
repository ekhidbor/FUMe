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

// local public
#include "mcstatus.h"
#include "mc3msg.h"
#include "diction.h"

// local private
#include "fume/application_callback_sink.h"

namespace fume
{


MC_STATUS application_callback_sink::provide_value_length( uint32_t        length,
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

MC_STATUS application_callback_sink::provide_offset_table
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

MC_STATUS application_callback_sink::write( const void* data,
                                            uint32_t    buffer_bytes )
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

MC_STATUS application_callback_sink::start_of_frame( uint32_t        size,
                                                     TRANSFER_SYNTAX syntax )
{
    MC_STATUS ret = write_tag( MC_ATT_ITEM, syntax );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = write_val( static_cast<uint32_t>( 0u ), syntax );
    }
    else
    {
        // Do nothing. Will return error
    }

    return ret;
}

MC_STATUS application_callback_sink::end_of_sequence( TRANSFER_SYNTAX syntax )
{
    MC_STATUS ret = write_tag( MC_ATT_SEQUENCE_DELIMITATION_ITEM, syntax );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = write_val( static_cast<uint32_t>( 0u ), syntax );
    }
    else
    {
        // Do nothing. Will return error
    }

    return ret;
}

MC_STATUS application_callback_sink::finalize()
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

}
