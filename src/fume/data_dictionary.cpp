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
#include <memory>
#include <limits>
#include <algorithm>

// boost
#include "boost/numeric/conversion/cast.hpp"

// local public

// local private
#include "fume/data_dictionary.h"
#include "fume/library_context.h"
#include "fume/value_representation.h"
#include "fume/tx_stream.h"
#include "fume/callback_vr_io.h"

using std::numeric_limits;
using std::for_each;
using std::find_if;

using boost::numeric_cast;
using boost::bad_numeric_cast;

namespace fume
{

data_dictionary::data_dictionary( int id, bool created_empty )
    : m_id( id ),
      m_created_empty( created_empty ),
      m_application_id( -1 )
{
    //m_value_dict_itr = m_value_dict.cbegin();
}

data_dictionary::~data_dictionary()
{
}

MC_STATUS data_dictionary::check_tag_const( unsigned long id ) const
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        const bool has_value =
            m_value_dict.count( numeric_cast<uint32_t>( id ) ) > 0;
        ret = has_value ? MC_NORMAL_COMPLETION : MC_INVALID_TAG;
    }
    catch( const bad_numeric_cast& )
    {
        ret = MC_INVALID_TAG;
    }

    return ret;
}

MC_STATUS data_dictionary::check_tag( unsigned long id )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        const bool has_value =
            m_value_dict.count( numeric_cast<uint32_t>( id ) ) > 0;
        ret = (has_value || created_empty()) ? MC_NORMAL_COMPLETION :
                                               MC_INVALID_TAG;
    }
    catch( const bad_numeric_cast& )
    {
        ret = MC_INVALID_TAG;
    }

    return ret;
}

value_representation& data_dictionary::operator[]( unsigned long id )
{
    // Caller should have already done this. Verifies
    // tag either is in this object or file was created empty
    // TODO: don't use assert here. Move id cast logic in here instead
    // of in the MC_Get_Value and MC_Set_Value functions
    assert( check_tag( id ) == MC_NORMAL_COMPLETION );

    // check_tag verifies tag is a valid 32-bit unsigned integer
    unique_vr_ptr& ret( m_value_dict[static_cast<uint32_t>(id)] );
    // If value was just created or was empty, create a new one
    if( ret == nullptr )
    {
        // Caller should have already enforced this
        assert( g_context != nullptr );
        ret = g_context->create_vr( id, this );
    }
    else
    {
        // Do nothing. value already non-null
    }

    // If we are here ret should not be NULL
    assert( ret != nullptr );
    return *ret;
}

value_representation* data_dictionary::at( unsigned long id ) const
{
    // Caller should have already done this
    assert( check_tag_const( id ) == MC_NORMAL_COMPLETION );

    // check_tag verifies tag is a valid 32-bit unsigned integer
    value_dict::const_iterator itr =
        m_value_dict.find( static_cast<uint32_t>(id) );

    // check_tag_const should have already ensured this
    assert( itr != m_value_dict.cend() );

    return itr->second.get();
}

MC_STATUS data_dictionary::set_empty( unsigned long id )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        const value_dict::iterator itr =
            m_value_dict.find( numeric_cast<uint32_t>( id ) );

        if( itr != m_value_dict.end() )
        {
            itr->second = nullptr;
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            ret = MC_INVALID_TAG;
        }
    }
    catch( const bad_numeric_cast& )
    {
        ret = MC_INVALID_TAG;
    }

    return ret;
}

void data_dictionary::set_all_empty()
{
    for_each( m_value_dict.begin(),
              m_value_dict.end(),
              []( value_dict::value_type& val )
              {
                  val.second = nullptr;
              } );
}

MC_STATUS data_dictionary::delete_attribute( unsigned long id )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        const uint32_t idu32 = numeric_cast<uint32_t>( id );
        const size_t elements_removed = m_value_dict.erase( idu32 );
        // In case nonstandard VR, remove it from that list
        m_nonstandard_vr_dict.erase( idu32 );

        // Invalid tag if no elements removed
        ret = elements_removed > 0 ? MC_NORMAL_COMPLETION : MC_INVALID_TAG;
    }
    catch( const bad_numeric_cast& )
    {
        ret = MC_INVALID_TAG;
    }

    return ret;
}

MC_STATUS data_dictionary::delete_range( unsigned long first_id,
                                         unsigned long last_id )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( first_id <= last_id )
    {
        try
        {
            const uint32_t first_idu32 = numeric_cast<uint32_t>( first_id );
            const uint32_t last_idu32 = numeric_cast<uint32_t>( last_id );
            const value_dict::iterator begin =
                find_if( m_value_dict.begin(),
                         m_value_dict.end(),
                         [first_idu32]( const value_dict::value_type& val )
                         {
                             return val.first >= first_idu32;
                         } );
            const value_dict::iterator end =
                find_if( begin,
                         m_value_dict.end(),
                         [last_idu32]( const value_dict::value_type& val )
                         {
                             return val.first > last_idu32;
                         } );

            m_value_dict.erase( begin, end );
            ret = MC_NORMAL_COMPLETION;
        }
        catch( const bad_numeric_cast& )
        {
            ret = MC_INVALID_TAG;
        }
    }
    else
    {
        ret = MC_INVALID_TAG;
    }

    return ret;
}

MC_STATUS data_dictionary::add_standard_attribute( unsigned long id )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        const uint32_t idu32 = numeric_cast<uint32_t>( id );
        if( m_value_dict.count( idu32 ) == 0 )
        {
            // should not be possible for this to be NULL
            assert( g_context != nullptr );

            unique_vr_ptr ptr( g_context->create_vr( id, this ) );
            if( ptr != nullptr )
            {
                // Assign NULL element
                m_value_dict[idu32].swap( ptr );
                ret = MC_NORMAL_COMPLETION;
            }
            else
            {
                ret = MC_INVALID_TAG;
            }
        }
        else
        {
            ret = MC_TAG_ALREADY_EXISTS;
        }
    }
    catch( const bad_numeric_cast& )
    {
        ret = MC_INVALID_TAG;
    }

    return ret;
}

MC_STATUS data_dictionary::set_callbacks( int application_id )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    // Should not be possible for this to be NULL
    assert( g_context != nullptr );
    if( g_context->get_application( application_id ) != nullptr )
    {
        m_application_id = application_id;
        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_INVALID_APPLICATION_ID;
    }

    return ret;
}

MC_STATUS data_dictionary::to_stream( tx_stream& stream ) const
{
    MC_STATUS ret = MC_NORMAL_COMPLETION;

    // should never happen
    assert( g_context != nullptr );

    const application* app = g_context->get_application( m_application_id );

    for( value_dict::const_iterator itr = m_value_dict.cbegin();
         itr != m_value_dict.cend() && ret == MC_NORMAL_COMPLETION;
         ++itr )
    {
        // a NULL value_representation pointer is defined as "not present
        // in the message"
        if( itr->second != nullptr )
        {
            // Write the tag number
            ret = stream.write_tag( itr->first );
            if( ret == MC_NORMAL_COMPLETION )
            {
                // data_dictionary writes the VR. Techically the
                // value_representation class could do this, but writing it
                // here keeps symmetry between to_stream and from_stream
                // implementations (since data_dictionary has to read the
                // vr to be able to create the object). It also makes things
                // consistent between using the callback function and using
                // the value_representation.
                ret = stream.write_vr( itr->second->vr() );
                if( ret == MC_NORMAL_COMPLETION )
                {
                    callback_parms_t callback =
                        app != nullptr ? app->get_callback_function( itr->first ) :
                                         callback_parms_t( nullptr, nullptr );

                    // TODO: check for NULL VR and differentiate from
                    // uninitialized VR (Set_Value_To_NULL disables callback)
                    // Use the callback function if one is defined for this tag
                    if( callback.first != nullptr )
                    {
                        ret = write_vr_data_from_callback( stream,
                                                           m_id,
                                                           itr->first,
                                                           callback );
                    }
                    // Otherwise use the value_representation object
                    else
                    {
                        ret = itr->second->to_stream( stream );
                    }
                }
                else
                {
                    // Do nothing. Will terminate loop and return error from
                    // write_vr
                }
            }
            else
            {
                // Do nothing. Will terminate loop and return error from
                // write_tag
            }
        }
        else
        {
            // Do nothing. Loop to next element
        }
    }

    return ret;
}

MC_STATUS data_dictionary::from_stream( rx_stream& stream )
{
    // TODO: implement
    return MC_CANNOT_COMPLY;
}

}