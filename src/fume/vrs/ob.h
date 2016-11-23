#ifndef OB_H
#define OB_H
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
#include <limits>
#include <memory>
#include <deque>

// local public
#include "mc3media.h"

// local private
#include "fume/value_representation.h"
#include "fume/encapsulated_tx_stream.h"

namespace fume
{

class seekable_stream;

namespace vrs
{

// Other Byte value representations
// TODO: derive ob from other_vr<uint8_t, OB> and also make it implemente
// tx_stream and rx_stream to enable implementation of
// MC_Set_(Next)?_Encapsulated_Value_From_Function and
// MC_Get_(Next)?_Encapsulated_Value_From_Function
class ob final : public value_representation, public encapsulated_tx_stream
{
public:
    ob();
    virtual ~ob();

// serializable
public:
    virtual MC_STATUS to_stream( tx_stream&      stream,
                                 TRANSFER_SYNTAX syntax ) override final;
    virtual MC_STATUS from_stream( rx_stream&      stream,
                                   TRANSFER_SYNTAX syntax ) override final;

// encapsulated_tx_stream
public:
    virtual MC_STATUS write( const void* buffer,
                             uint32_t    buffer_bytes ) override final;
    virtual uint64_t bytes_written() const override final;

    virtual MC_STATUS provide_value_length( uint32_t        length,
                                            TRANSFER_SYNTAX syntax ) override final;

    virtual MC_STATUS provide_offset_table( const uint32_t* table,
                                            uint32_t        table_elements,
                                            TRANSFER_SYNTAX syntax ) override final;

    virtual MC_STATUS start_of_frame( uint32_t        size,
                                      TRANSFER_SYNTAX syntax ) override final;

    virtual MC_STATUS end_of_sequence( TRANSFER_SYNTAX syntax ) override final;

    virtual MC_STATUS finalize() override final;

// value_representation -- modifiers
public:
    virtual MC_STATUS set( const set_func_parms& val ) override final;

    // Sets the value of the data element to NULL (ie. zero length)
    virtual MC_STATUS set_null() override final;

// value_representation -- accessors
public:
    virtual MC_STATUS get( const get_func_parms& val ) override final;

    // Returns the number of elements
    virtual int count() const override final;

    // Indicates whether or not the element is null
    virtual bool is_null() const override final;

    virtual MC_VR vr() const override final
    {
        return OB;
    }

    virtual std::unique_ptr<value_representation> clone() const override final;

private:
    std::unique_ptr<seekable_stream> m_stream;
    std::deque<uint64_t>             m_offset_table;
};

} // namespace vrs
} // namespace fume

#endif
