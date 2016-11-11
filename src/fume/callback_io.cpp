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

// local public
#include "mcstatus.h"
#include "mc3msg.h"
#include "diction.h"

// boost
#include "boost/numeric/conversion/cast.hpp"

// local private
#include "fume/callback_io.h"
#include "fume/tx_stream.h"
#include "fume/library_context.h"

using std::vector;

using boost::numeric_cast;
using boost::bad_numeric_cast;

namespace fume
{

static MC_STATUS get_buffer_from_function( const set_func_parms& parms,
                                           vector<uint8_t>&      buffer );
static bool is_encapsulated( TRANSFER_SYNTAX syntax );

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

MC_STATUS write_encapsulated_value_from_function( tx_stream&            stream,
                                                  const set_func_parms& parms )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( is_encapsulated( stream.transfer_syntax() ) == true )
    {
        // Write empty Offset table
        ret = stream.write_tag( MC_ATT_ITEM );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = stream.write_val( static_cast<uint32_t>( 0x00000000 ) );
            if( ret == MC_NORMAL_COMPLETION )
            {
                ret = write_next_encapsulated_value_from_function( stream, parms );
            }
            else
            {
                // Do nothing. Will return error from write_val
            }
        }
        else
        {
            // Do nothing. Will return error from write_val
        }
    }
    else
    {
        ret = MC_INVALID_TRANSFER_SYNTAX;
    }

    return ret;
}

MC_STATUS write_next_encapsulated_value_from_function( tx_stream&            stream,
                                                       const set_func_parms& parms )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( is_encapsulated( stream.transfer_syntax() ) == true )
    {
        ret = stream.write_tag( MC_ATT_ITEM );
        if( ret == MC_NORMAL_COMPLETION )
        {
            vector<uint8_t> buffer;
            // Get the full buffer from the callback function
            ret = get_buffer_from_function( parms, buffer );
            if( ret == MC_NORMAL_COMPLETION )
            {
                // Write the length of the buffer
                ret = stream.write_val( static_cast<uint32_t>( buffer.size() ) );
                if( ret == MC_NORMAL_COMPLETION )
                {
                    ret = stream.write( buffer.data(), buffer.size() );
                }
                else
                {
                    // Do nothing. Will return error from write_val
                }
            }
            else
            {
                // Do nothing. Will return error from get_buffer_from_function
            }
        }
        else
        {
            // Do nohting. Will return error from write_tag
        }
    }
    else
    {
        ret = MC_INVALID_TRANSFER_SYNTAX;
    }

    return ret;
}

MC_STATUS close_encapsulated_value( tx_stream& stream )
{
    MC_STATUS ret = stream.write_tag( MC_ATT_ITEM_DELIMITATION_ITEM );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = stream.write_val( static_cast<uint32_t>( 0x00000000 ) );
    }
    else
    {
        // Do nothing. Will return error from write_tag
    }

    return ret;
}

MC_STATUS get_buffer_from_function( const set_func_parms& parms,
                                    vector<uint8_t>&      buffer )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    vector<uint8_t> tmp_buf;

    if( parms.callback != nullptr )
    {
        bool first = true;
        int  cblast = 0;

        // typedef MC_STATUS (*SetValueCallback)
        // (
        //     int           CBMsgFileItemID,
        //     unsigned long Cbtag,
        //     int           CbisFirst,
        //     void*         CbuserInfo,
        //     int*          CbdataSizePtr,
        //     void**        CbdataBufferPtr,
        //     int*          CbisLastPtr
        // );
        do
        {
            int   cbsize = 0;
            void* cbbuf = nullptr;

            cblast = 0;

            MC_STATUS cbstat = parms.callback( parms.message_id,
                                               parms.tag,
                                               static_cast<int>( first ),
                                               parms.callback_parm,
                                               &cbsize,
                                               &cbbuf,
                                               &cblast );
            if( cbstat == MC_NORMAL_COMPLETION &&
                cbsize > 0                     &&
                (cbsize % 2) == 0              &&
                cbbuf != nullptr                )
            {
                const uint8_t* data = static_cast<uint8_t*>( cbbuf );
                tmp_buf.insert( tmp_buf.cend(), data, data + cbsize );

                ret = MC_NORMAL_COMPLETION;
            }
            else if( cbstat != MC_NORMAL_COMPLETION )
            {
                ret = MC_CALLBACK_CANNOT_COMPLY;
            }
            else if( cbsize <= 0 )
            {
                ret = MC_CALLBACK_DATA_SIZE_NEGATIVE;
            }
            else if( (cbsize % 2) != 0 )
            {
                ret = MC_CALLBACK_DATA_SIZE_UNEVEN;
            }
            else
            {
                ret = MC_CALLBACK_PARM_ERROR;
            }

            first = false;
        }
        while( ret == MC_NORMAL_COMPLETION && cblast == 0 );
    }

    if( ret == MC_NORMAL_COMPLETION )
    {
        tmp_buf.swap( buffer );
    }

    return ret;
}

bool is_encapsulated( TRANSFER_SYNTAX syntax )
{
    return syntax != IMPLICIT_LITTLE_ENDIAN &&
           syntax != EXPLICIT_LITTLE_ENDIAN &&
           syntax != EXPLICIT_BIG_ENDIAN    &&
           syntax != DEFLATED_EXPLICIT_LITTLE_ENDIAN;
}

}
