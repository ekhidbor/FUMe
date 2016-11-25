#ifndef GET_VALUE_FUNCTION_SINK_H
#define GET_VALUE_FUNCTION_SINK_H
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
#include "fume/value_representation.h"
#include "fume/encapsulated_value_sink.h"

namespace fume
{

class get_value_function_sink final : public encapsulated_value_sink
{
public:
    get_value_function_sink( const get_func_parms& parms,
                             bool  write_offset_table );

    virtual MC_STATUS provide_value_length( uint32_t        length,
                                            TRANSFER_SYNTAX syntax ) override final;

    virtual MC_STATUS provide_offset_table
    (
        const uint32_t* table,
        uint32_t        table_elements,
        TRANSFER_SYNTAX syntax
    ) override final;

    virtual MC_STATUS write( const void* buffer,
                             uint32_t    buffer_bytes ) override final;
    virtual uint64_t tell_write() const override final
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
    get_value_function_sink( const get_value_function_sink& );
    get_value_function_sink& operator=( const get_value_function_sink& );

private:
    const get_func_parms& m_parms;
    bool                  m_write_offset_table;
    bool                  m_first;
    uint64_t              m_bytes_written;
};

}

#endif
