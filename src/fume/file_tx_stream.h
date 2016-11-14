#ifndef FILE_TX_STREAM_H
#define FILE_TX_STREAM_H
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

// local public
#include "mcstatus.h"
#include "mc3media.h"

// local private
#include "fume/tx_stream.h"

namespace fume
{

class file_tx_stream final : public tx_stream
{
public:
    file_tx_stream( const std::string& filename,
                    WriteFileCallback  callback,
                    void*              user_info );
    ~file_tx_stream()
    {
    }

    MC_STATUS finalize();

// tx_stream
public:
    virtual MC_STATUS write( const void* buffer,
                             size_t      buffer_bytes ) override final;

    virtual uint32_t bytes_written() const override final
    {
        return m_bytes_written;
    }

private:
    file_tx_stream( const file_tx_stream& );
    file_tx_stream& operator=( const file_tx_stream& );

private:
    // Note: the passed in filename must have a lifetime exceeding
    // that of this object
    const std::string& m_filename;
    uint32_t           m_bytes_written;
    // Callback pointer and user info are owned by caller and
    // not this class
    WriteFileCallback  m_callback;
    void*              m_user_info;
    bool               m_first;
};

}

#endif
