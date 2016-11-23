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
#include <cstdint>
#include <algorithm>
#include <limits>

// boost
#include "boost/scope_exit.hpp"

// local public
#include "mc3msg.h"
#include "diction.h"

// local private
#include "fume/vrs/sq.h"
#include "fume/library_context.h"
#include "fume/item_object.h"
#include "fume/tx_stream.h"
#include "fume/rx_stream.h"

using std::for_each;
using std::min;
using std::deque;
using std::numeric_limits;

namespace fume
{
namespace vrs
{

static MC_STATUS write_element_length( tx_stream&      stream,
                                       TRANSFER_SYNTAX syntax,
                                       bool            empty )
{
    const uint32_t size = empty ? 0u : 0xFFFFFFFFu;
    return stream.write_val( size, syntax );
}

static MC_STATUS write_sequence_delimitation( tx_stream&      stream,
                                              TRANSFER_SYNTAX syntax )
{
    MC_STATUS ret = stream.write_tag( MC_ATT_SEQUENCE_DELIMITATION_ITEM, syntax );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = stream.write_val( static_cast<uint32_t>( 0x00000000u ), syntax );
    }
    else
    {
        // Do nothing. Will return error from write_vr
    }

    return ret;
}

static int read_item( rx_stream& stream, TRANSFER_SYNTAX syntax )
{
    assert( g_context != nullptr );

    int id = g_context->create_empty_item_object();
    if( id > 0 )
    {
        item_object* item =
            dynamic_cast<item_object*>( g_context->get_object( id ) );
        assert( item != nullptr );

        const MC_STATUS stat = item->from_stream( stream, syntax );
        if( stat != MC_NORMAL_COMPLETION )
        {
            id = -static_cast<int>( stat );
        }
        else
        {
            // Do nothing on success. Will return item id
        }
    }
    else
    {
        // Do nothing. Will return errro
    }

    return id;
}

static MC_STATUS write_item( tx_stream& stream, TRANSFER_SYNTAX syntax, int id )
{
    // Caller should have done this
    assert( g_context != nullptr );

    MC_STATUS ret = MC_CANNOT_COMPLY;

    item_object* item =
        dynamic_cast<item_object*>( g_context->get_object( id ) );
    if( item != nullptr )
    {
        // Write the item tag here. The Item object could do this
        // but we do it here to keep symmetry between reading and
        // writing an item
        ret = stream.write_tag( MC_ATT_ITEM, syntax );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = item->to_stream( stream, syntax );
        }
        else
        {
            // Do nothing. Will return error
        }
    }
    else
    {
        ret = MC_INVALID_ITEM_ID;
    }

    return ret;
}

template<class Iterator>
static void free_items( Iterator begin, Iterator end )
{
    if( g_context != nullptr )
    {
        // Delete all items
        for_each( begin,
                  end,
                  []( int item )
                  {
                      g_context->free_item_object( item );
                  } );
    }
}

sq::sq( unsigned int min_vals, unsigned int max_vals, unsigned int multiple )
    : value_representation( min_vals, max_vals, multiple )
{
}

sq::~sq()
{
    free_items( m_items.cbegin(), m_items.cend() );
}

MC_STATUS sq::from_stream( rx_stream& stream, TRANSFER_SYNTAX syntax )
{
    uint32_t value_length = 0;
    value_list_t tmp_items;

    // If the function succeeds then the old items need to
    // be freed. If it failed the leftover items need to
    // be freed. In either way we need to free all the
    // items in the list
    BOOST_SCOPE_EXIT( &tmp_items )
    {
        free_items( tmp_items.cbegin(), tmp_items.cend() );
    } BOOST_SCOPE_EXIT_END
    
    MC_STATUS ret = stream.read_val( value_length, syntax );
    if( ret == MC_NORMAL_COMPLETION )
    {
        const bool delimited = value_length == numeric_limits<uint32_t>::max();
        const uint32_t start_offset = stream.bytes_read();
        uint32_t cur_offset = stream.bytes_read();

        while( ret == MC_NORMAL_COMPLETION &&
               ((cur_offset - start_offset) + 1) < value_length )
        {
            uint32_t tag = 0;
            ret = stream.read_tag( tag, syntax );
            if( ret == MC_NORMAL_COMPLETION )
            {
                if( tag == MC_ATT_ITEM )
                {
                    const int item_id = read_item( stream, syntax );
                    if( item_id > 0 )
                    {
                        ret = tmp_items.set_next( item_id );
                    }
                    else
                    {
                        ret = static_cast<MC_STATUS>(-item_id);
                    }
                }
                else if( tag == MC_ATT_SEQUENCE_DELIMITATION_ITEM )
                {
                    uint32_t delimiter_len = 0;
                    ret = stream.read_val( delimiter_len, syntax );
                    if( ret == MC_NORMAL_COMPLETION )
                    {
                        // If we're expecting a sequence delimiter, then terminate
                        // the loop
                        if( delimited == true && delimiter_len == 0 )
                        {
                            break;
                        }
                        else
                        {
                            // If we weren't expected a sequence delimiter, than 
                            // something went wrong
                            ret = MC_END_OF_DATA;
                        }
                    }
                    else
                    {
                        // Do nothing. Return error
                    }
                }
            }
        }
    }
    else
    {
        // Do nothing. Will return error
    }

    if( ret == MC_NORMAL_COMPLETION )
    {
        tmp_items.swap( m_items );
    }

    return ret;
}

MC_STATUS sq::to_stream( tx_stream& stream, TRANSFER_SYNTAX syntax )
{
    MC_STATUS ret = write_element_length( stream, syntax, m_items.is_null() );
    if( ret == MC_NORMAL_COMPLETION && m_items.is_null() == false )
    {
        for( value_list_t::const_iterator itr = m_items.cbegin();
             ret == MC_NORMAL_COMPLETION && itr != m_items.cend();
             ++itr )
        {
            ret = write_item( stream, syntax, *itr );
        }

        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = write_sequence_delimitation( stream, syntax );
        }
        else
        {
            // Do nothing. Will return error
        }
    }
    else
    {
        // Do nothing. Will return error if write_element_length failed or
        // MC_NORMAL_COMPLETION if zero length value
    }

    return ret;
}

MC_STATUS sq::set( int val )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    assert( g_context != nullptr );

    if( dynamic_cast<item_object*>( g_context->get_object( val ) ) != nullptr )
    {
        ret = m_items.set( val );
    }
    else
    {
        ret = MC_INCOMPATIBLE_VR;
    }

    return ret;
}

MC_STATUS sq::set_next( int val )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    assert( g_context != nullptr );

    if( dynamic_cast<item_object*>( g_context->get_object( val ) ) != nullptr )
    {
        ret = m_items.set_next( val );
    }
    else
    {
        ret = MC_INCOMPATIBLE_VR;
    }

    return ret;
}

MC_STATUS sq::get( int& val )
{
    return m_items.get( val );
}

MC_STATUS sq::get_next( int& val )
{
    return m_items.get_next( val );
}

} // namespace vrs

} // namespace fume
