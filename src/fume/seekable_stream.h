#ifndef SEEKABLE_STREAM_H
#define SEEKABLE_STREAM_H
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
#include <memory>

// local public
#include "mcstatus.h"

// local private
#include "fume/tx_stream.h"
#include "fume/rx_stream.h"

namespace fume
{

class seekable_stream : public tx_stream, public rx_stream
{
public:
    seekable_stream()
    {
    }
    virtual ~seekable_stream()
    {
    }

    /** Resets the effective length of the stream to zero.
     *
     * Upon successful completion of this function, the values
     * returned by tell_write(), tell_read(), and size() will
     * be zero. Note that this function may not change the
     * size of the underlying storage medium.
     *
     * \return MC_NORMAL_COMPLETION if the operation completed successfully.
     *         An implementation-defined error code if it did not
     */
    virtual MC_STATUS clear() = 0;
    /** Resets the position from which data is read from or written to the
     *  stream to zero.
     *
     * Upon successful completion of this function, the value returned by
     * tell_read() and tell_write() will be zero, and the data will be read
     * from or written to the start of the stream
     */
    MC_STATUS rewind()
    {
        return seek( 0u );
    }

    virtual MC_STATUS seek( uint64_t position ) = 0;
    virtual uint64_t size() const = 0;

    virtual std::unique_ptr<seekable_stream> clone() = 0;
};


}

#endif
