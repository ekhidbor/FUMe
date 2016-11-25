#ifndef FILE_RX_STREAM_H
#define FILE_RX_STREAM_H
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
#include <string>
#include <deque>

// local public
#include "mcstatus.h"
#include "mc3media.h"

// local private
#include "fume/rx_stream.h"

namespace fume
{

class file_rx_stream final : public rx_stream
{
public:
    file_rx_stream( const std::string& filename,
                    ReadFileCallback   callback,
                    void*              user_info );
    ~file_rx_stream()
    {
    }

// tx_stream
public:
    virtual MC_STATUS read( void* buffer, uint32_t buffer_bytes ) override final;
    virtual MC_STATUS peek( void* buffer, uint32_t buffer_bytes ) override final;

    virtual uint64_t tell_read() const override final
    {
        return m_bytes_read;
    }

private:
    file_rx_stream( const file_rx_stream& );
    file_rx_stream& operator=( const file_rx_stream& );

    MC_STATUS ensure_bytes_available( uint32_t ensure_bytes_available );

private:
    // Note: the passed in filename must have a lifetime exceeding
    // that of this object
    const std::string&  m_filename;
    uint64_t            m_bytes_read;
    std::deque<uint8_t> m_data_buffer;
    // Callback pointer and user info are owned by caller and
    // not this class
    ReadFileCallback   m_callback;
    void*              m_user_info;
    bool               m_first;
    bool               m_last;
};

}

#endif
