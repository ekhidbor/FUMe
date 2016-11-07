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

// local public
#include "mc3msg.h"

// local private
#include "fume/vrs/sq.h"
#include "fume/library_context.h"
#include "fume/item_object.h"
#include "fume/tx_stream.h"

using std::for_each;
using std::min;
using std::deque;

namespace fume
{
namespace vrs
{

static MC_STATUS write_element_length( tx_stream& stream, bool empty )
{
    const uint32_t size = empty ? 0u : 0xFFFFFFFFu;
    return stream.write_val( size );
}

static MC_STATUS write_item_tag( tx_stream& stream )
{
    MC_STATUS ret = stream.write_tag( 0xFFFEE000u );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = stream.write_val( static_cast<uint32_t>( 0xFFFFFFFFu ) );
    }
    else
    {
        // Do nothing. Will return error from write_vr
    }

    return ret;
}

static MC_STATUS write_item_delimitation( tx_stream& stream )
{
    MC_STATUS ret = stream.write_tag( 0xFFFEE00Du );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = stream.write_val( static_cast<uint32_t>( 0x00000000u ) );
    }
    else
    {
        // Do nothing. Will return error from write_vr
    }

    return ret;
}

static MC_STATUS write_sequence_delimitation( tx_stream& stream )
{
    MC_STATUS ret = stream.write_tag( 0xFFFEE0DDu );
    if( ret == MC_NORMAL_COMPLETION )
    {
        ret = stream.write_val( static_cast<uint32_t>( 0x00000000u ) );
    }
    else
    {
        // Do nothing. Will return error from write_vr
    }

    return ret;
}

static MC_STATUS write_item( tx_stream& stream, int id )
{
    // Caller should have done this
    assert( g_context != nullptr );

    MC_STATUS ret = MC_CANNOT_COMPLY;

    const item_object* item =
        dynamic_cast<item_object*>( g_context->get_object( id ) );
    if( item != nullptr )
    {
        ret = write_item_tag( stream );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = item->to_stream( stream );
            if( ret == MC_NORMAL_COMPLETION )
            {
                ret = write_item_delimitation( stream );
            }
            else
            {
                // Do nothing. Will return error from to_stream
            }
        }
        else
        {
            // Do nothing. Will return error from write_item_tag
        }
    }
    else
    {
        ret = MC_INVALID_ITEM_ID;
    }

    return ret;
}

sq::sq( unsigned int min_vals, unsigned int max_vals, unsigned int multiple )
    : value_representation( min_vals, max_vals, multiple )
{
    m_current_idx = 0;
}

sq::~sq()
{
    if( g_context != nullptr )
    {
        // Delete all items
        for_each( m_items.begin(),
                  m_items.end(),
                  []( int item )
                  {
                      g_context->free_item_object( item );
                  } );
    }
}

MC_STATUS sq::to_stream( tx_stream& stream ) const
{
    // Caller should have done this
    assert( g_context != nullptr );

    MC_STATUS ret = write_element_length( stream, m_items.empty() );
    if( ret == MC_NORMAL_COMPLETION && m_items.empty() == false )
    {
        for( deque<int>::const_iterator itr = m_items.cbegin();
             ret == MC_NORMAL_COMPLETION && itr != m_items.cend();
             ++itr )
        {
            ret = write_item( stream, *itr );
        }

        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = write_sequence_delimitation( stream );
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
        m_items.clear();
        m_items.push_back( val );
        m_current_idx = 0;

        ret = MC_NORMAL_COMPLETION;
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
        m_items.push_back( val );
        m_current_idx = 0;

        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_INCOMPATIBLE_VR;
    }

    return ret;
}

MC_STATUS sq::get( int& val ) const
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( m_items.empty() == false )
    {
        m_current_idx = 0;
        val = m_items[m_current_idx];
        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_NULL_VALUE;
    }

    return ret;
}

MC_STATUS sq::get_next( int& val ) const
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( m_items.empty() == false )
    {
        m_current_idx = min( m_items.size(), m_current_idx + 1 );
        if( m_current_idx < m_items.size() )
        {
            val = m_items[m_current_idx];
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            ret = MC_NO_MORE_VALUES;
        }
    }
    else
    {
        ret = MC_NULL_VALUE;
    }

    return ret;
}

MC_STATUS sq::delete_current()
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( m_items.empty() == false )
    {
        if( m_current_idx < m_items.size() )
        {
            m_items.erase( m_items.cbegin() + m_current_idx );
            // Adjust the index to continue to be valid if we removed
            // the last element of a multi-element list
            if( m_current_idx > 0 && m_current_idx >= m_items.size() )
            {
                --m_current_idx;
            }
            else
            {
                // Do nothing. Index is still valid
            }
        }
        else
        {
            ret = MC_NO_MORE_VALUES;
        }
    }
    else
    {
        ret = MC_NULL_VALUE;
    }

    return ret;
} // namespace vrs
} // namespace fume

}
