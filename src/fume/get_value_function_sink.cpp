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
#include "fume/get_value_function_sink.h"

namespace fume
{

get_value_function_sink::get_value_function_sink( const get_func_parms& parms,
                                                  bool  write_offset_table )
    : m_parms( parms ),
      m_write_offset_table( write_offset_table ),
      m_first( true ),
      m_bytes_written( 0 )
{
    assert( parms.callback != nullptr );
}

MC_STATUS get_value_function_sink::provide_value_length( uint32_t        length,
                                                         TRANSFER_SYNTAX syntax )
{
    // callback functions passed into MC_Get_Encapsulated_Value_To_Function
    // don't get the length
    return MC_NORMAL_COMPLETION;
}

MC_STATUS get_value_function_sink::provide_offset_table
(
    const uint32_t* table,
    uint32_t        table_elements,
    TRANSFER_SYNTAX syntax
)
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

MC_STATUS get_value_function_sink::write( const void* buffer,
                                          uint32_t    buffer_bytes )
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

MC_STATUS get_value_function_sink::finalize()
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

}
