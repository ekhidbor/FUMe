#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H
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
#include <cstdlib>

// local public
#include "mcstatus.h"
#include "mc3msg.h"

namespace fume
{

class tx_stream;
class rx_stream;

class serializable
{
public:
    virtual ~serializable()
    {
    }

    virtual MC_STATUS to_stream( tx_stream&      stream,
                                 TRANSFER_SYNTAX syntax ) const = 0;
    virtual MC_STATUS from_stream( rx_stream& stream ) = 0;
};

}

#endif
