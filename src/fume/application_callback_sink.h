#ifndef APPLICATION_CALLBACK_SINK_H
#define APPLICATION_CALLBACK_SINK_H
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

// local public
#include "mcstatus.h"

// local private
#include "fume/application.h"
#include "fume/encapsulated_value_sink.h"

namespace fume
{

class application_callback_sink final : public encapsulated_value_sink
{
public:
    application_callback_sink( const callback_parms_t& callback,
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
    application_callback_sink( const application_callback_sink& );
    application_callback_sink& operator=( const application_callback_sink& );

private:
    const callback_parms_t& m_callback;
    int                     m_message_id;
    uint32_t                m_tag;
    bool                    m_first;
    uint64_t                m_bytes_written;
};

}

#endif
