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

// local public
#include "mc3media.h"

// local private
#include "fume/vrs/other_vr.h"
#include "fume/tx_stream.h"

namespace fume
{
namespace vrs
{

// Other Byte value representations
// TODO: derive ob from other_vr<uint8_t, OB> and also make it implemente
// tx_stream and rx_stream to enable implementation of
// MC_Set_(Next)?_Encapsulated_Value_From_Function and
// MC_Get_(Next)?_Encapsulated_Value_From_Function
class ob final : public other_vr<uint8_t, OB>, public tx_stream
{
public:
    ob()
        : other_vr<uint8_t, OB>()
    {
    }
    virtual ~ob()
    {
    }

    virtual MC_STATUS write( const void* buffer,
                            size_t       buffer_bytes ) override final;
    virtual uint32_t bytes_written() const override final
    {
        return static_cast<uint32_t>( count() );
    }

    virtual std::unique_ptr<value_representation> clone() const override final
    {
        return std::unique_ptr<value_representation>( new ob( *this ) );
    }

private:
    ob( const ob& rhs )
        : other_vr<uint8_t, OB>( rhs )
    {
    }
};

} // namespace vrs
} // namespace fume

#endif
