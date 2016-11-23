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
     * returned by both bytes_written() and bytes_read() will
     * be zero. Note that this function may not change the
     * size of the underlying storage medium.
     *
     * \return MC_NORMAL_COMPLETION if the operation completed successfully.
     *         An implementation-defined error code if it did not
     */
    virtual MC_STATUS clear() = 0;
    /** Resets the position from which data is read from the stream to zero.
     *
     * Upon successful completion of this function, the value returned by
     * bytes_read() will be zero, and the data will be read from the start
     * of the stream
     */
    virtual MC_STATUS rewind_read() = 0;

    virtual std::unique_ptr<seekable_stream> clone() = 0;
};


}

#endif
