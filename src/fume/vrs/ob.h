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
#include <memory>

// local public
#include "mc3media.h"

// local private
#include "fume/value_representation.h"

namespace fume
{

class encapsulated_value;

namespace vrs
{

// Other Byte value representations
// TODO: derive ob from other_vr<uint8_t, OB> and also make it implemente
// tx_stream and rx_stream to enable implementation of
// MC_Set_(Next)?_Encapsulated_Value_From_Function and
// MC_Get_(Next)?_Encapsulated_Value_From_Function
class ob final : public value_representation
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

// value_representation -- modifiers
public:
    virtual MC_STATUS set( const set_func_parms& val ) override final;

    MC_STATUS set_encapsulated( const set_func_parms& val );

    MC_STATUS set_next_encapsulated( const set_func_parms& val );

    MC_STATUS close_encapsulated();

    // Sets the value of the data element to NULL (ie. zero length)
    virtual MC_STATUS set_null() override final;

// value_representation -- accessors
public:
    virtual MC_STATUS get( const get_func_parms& val ) override final;

    MC_STATUS get_encapsulated( const get_func_parms& val );

    MC_STATUS get_next_encapsulated( const get_func_parms& val );

    MC_STATUS get_frame( unsigned int idx, const get_func_parms& val );

    // Returns the number of elements
    virtual int count() const override final
    {
        return static_cast<int>( is_null() == false );
    }

    // Indicates whether or not the element is null
    virtual bool is_null() const override final;

    virtual MC_VR vr() const override final
    {
        return OB;
    }

    virtual std::unique_ptr<value_representation> clone() const override final;

private:
    ob( const ob& rhs );

private:
    std::unique_ptr<encapsulated_value> m_stream;
};

} // namespace vrs
} // namespace fume

#endif
