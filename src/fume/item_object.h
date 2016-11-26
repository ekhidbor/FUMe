#ifndef ITEM_OBJECT_H
#define ITEM_OBJECT_H
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

// local private
#include "fume/data_dictionary.h"
#include "fume/serializable.h"

namespace fume
{

class item_object : public data_dictionary, public serializable
{
public:
    item_object( int id, bool created_empty )
        : data_dictionary( id, created_empty )
    {
    }
    virtual ~item_object()
    {
    }

    void empty_item()
    {
        return clear();
    }

    virtual MC_STATUS set_transfer_syntax( TRANSFER_SYNTAX syntax ) override final
    {
        return MC_INVALID_MESSAGE_ID;
    }
    virtual MC_STATUS get_transfer_syntax( TRANSFER_SYNTAX& syntax ) override final
    {
        return MC_INVALID_MESSAGE_ID;
    }

// serializable
public:
    virtual MC_STATUS to_stream( tx_stream&      stream,
                                 TRANSFER_SYNTAX syntax ) override;
    virtual MC_STATUS from_stream( rx_stream&      stream,
                                   TRANSFER_SYNTAX syntax ) override;
};

}


#endif
