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

// boost
#include "boost/numeric/conversion/cast.hpp"

// local private
#include "fume/callback_vr_io.h"
#include "fume/tx_stream.h"

using boost::numeric_cast;
using boost::bad_numeric_cast;

namespace fume
{

class callback_io
{
public:
    callback_io( const callback_parms_t& callback,
                 int                     message_id,
                 uint32_t                tag )
        : m_callback( callback ),
          m_message_id( message_id ),
          m_tag( tag ),
          m_done( false ),
          m_first( true )
    {
    }

    MC_STATUS get_value_length( uint32_t& length );
    MC_STATUS get_value_data( const void*& data, size_t& data_length );

    bool is_done() const
    {
        return m_done;
    }
private:
    const callback_parms_t& m_callback;
    int                     m_message_id;
    uint32_t                m_tag;
    bool                    m_done;
    bool                    m_first;
};

MC_STATUS callback_io::get_value_length( uint32_t& length )
{
    // Caller should have checked this
    assert( m_callback.first != nullptr );

    int last = 0;
    void* buffer = nullptr;
    unsigned long size = 0;
    MC_STATUS ret = MC_CANNOT_COMPLY;

    const MC_STATUS cbstat = m_callback.first( m_message_id,
                                               m_tag,
                                               m_callback.second,
                                               REQUEST_FOR_DATA_LENGTH,
                                               &size,
                                               &buffer,
                                               static_cast<int>( true ),
                                               &last );
    if( cbstat != MC_NORMAL_COMPLETION )
    {
        ret = MC_CALLBACK_CANNOT_COMPLY;
    }
    else if( size != 0xFFFFFFFF && (size % 2) != 0 )
    {
        ret = MC_CALLBACK_DATA_SIZE_UNEVEN;
    }
    else
    {
        // Callback success
        try
        {
            length = numeric_cast<uint32_t>( size );
            ret = MC_NORMAL_COMPLETION;
        }
        catch( const bad_numeric_cast& )
        {
            ret = MC_CALLBACK_DATA_SIZE_NEGATIVE;
        }
    }

    return ret;
}

MC_STATUS callback_io::get_value_data( const void*& data, size_t& data_length )
{
    // Caller should have checked this
    assert( m_callback.first != nullptr );

    int last = 0;
    void* buffer = nullptr;
    unsigned long size = 0;
    MC_STATUS ret = MC_CANNOT_COMPLY;

    const MC_STATUS cbstat = m_callback.first( m_message_id,
                                               m_tag,
                                               m_callback.second,
                                               REQUEST_FOR_DATA,
                                               &size,
                                               &buffer,
                                               static_cast<int>( m_first ),
                                               &last );
    if( cbstat != MC_NORMAL_COMPLETION )
    {
        ret = MC_CALLBACK_CANNOT_COMPLY;
    }
    else if( (size % 2) != 0 )
    {
        ret = MC_CALLBACK_DATA_SIZE_UNEVEN;
    }
    else
    {
        // Callback success
        try
        {
            data_length = numeric_cast<size_t>( size );
            data = buffer;
            m_done = last != 0;
            m_first = false;
            ret = MC_NORMAL_COMPLETION;
        }
        catch( const bad_numeric_cast& )
        {
            ret = MC_CALLBACK_DATA_SIZE_NEGATIVE;
        }
    }

    return ret;
}


MC_STATUS write_vr_data_from_callback( tx_stream&              stream,
                                       int                     message_id,
                                       uint32_t                tag,
                                       const callback_parms_t& callback )
{
    callback_io io( callback, message_id, tag );

    uint32_t vr_length = 0;
    MC_STATUS ret = io.get_value_length( vr_length );
    if( ret == MC_NORMAL_COMPLETION )
    {
        // Write the length
        ret = stream.write_val( vr_length );
        while( ret == MC_NORMAL_COMPLETION && io.is_done() == false )
        {
            const void* data = nullptr;
            size_t data_length = 0;

            ret = io.get_value_data( data, data_length );
            if( ret == MC_NORMAL_COMPLETION )
            {
                // Write the block of data
                ret = stream.write( data, data_length );
            }
            else
            {
                // Do nothing. Will break loop and return error from
                // get_value_data
            }
        }
    }
    else
    {
        // Do nothing. Will return error from get_value_length
    }

    return ret;
}


}
