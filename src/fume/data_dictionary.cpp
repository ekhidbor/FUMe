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
#include "fume/vr_factory.h"

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
    const uint32_t id_u32 = static_cast<uint32_t>(id);
    unique_vr_ptr& ret( m_value_dict[id_u32] );
    // If value was just created or was empty, create a new one
    if( ret == nullptr )
    {
        nonstandard_vr_dict::const_iterator ns_itr =
            m_nonstandard_vr_dict.find( id_u32 );
        // If this is a "nonstandard" tag
        if( ns_itr != m_nonstandard_vr_dict.cend() )
        {
            // Create a VR based on the contents of the nonstandard
            // map, allowing any number of values
            ret = create_vr( ns_itr->second,
                             1,
                             numeric_limits<unsigned short>::max(),
                             1 );
        }
        else
        {
            // Caller should have already enforced this
            assert( g_context != nullptr );
            // Otherwise create a VR using the global tag -> VR table
            ret = g_context->create_vr( id, this );
        }
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
              []( value_dict::reference val )
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
            const value_range& range( get_value_range( first_idu32, last_idu32 ) );

            m_value_dict.erase( range.first, range.second );
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

data_dictionary::const_value_range
data_dictionary::get_value_range( uint32_t begin_id, uint32_t end_id ) const
{
    const_value_range ret;
    ret.first = find_if( m_value_dict.begin(),
                         m_value_dict.end(),
                         [begin_id]( value_dict::const_reference val )
                         {
                             return val.first >= begin_id;
                         } );
    ret.second = find_if( ret.first,
                          m_value_dict.end(),
                          [end_id]( value_dict::const_reference val )
                          {
                              return val.first > end_id;
                          } );

    return ret;
}

data_dictionary::value_range
data_dictionary::get_value_range( uint32_t begin_id, uint32_t end_id )
{
    value_range ret;
    ret.first = find_if( m_value_dict.begin(),
                         m_value_dict.end(),
                         [begin_id]( value_dict::const_reference val )
                         {
                             return val.first >= begin_id;
                         } );
    ret.second = find_if( ret.first,
                          m_value_dict.end(),
                          [end_id]( value_dict::const_reference val )
                          {
                              return val.first > end_id;
                          } );

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

            MC_VR id_vr = UNKNOWN_VR;
            ret = g_context->get_vr_type( id, this, id_vr );
            if( ret == MC_NORMAL_COMPLETION )
            {
                // add_standard_attribute adds a "placeholder" attribute.
                // It does not add a zero-length attribute.
                m_value_dict[idu32] = nullptr;
                ret = MC_NORMAL_COMPLETION;
            }
            else
            {
                // Do nothing will return error from get_vr_type
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

MC_STATUS data_dictionary::add_nonstandard_attribute( unsigned long id,
                                                      MC_VR         vr )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( vr_is_valid( vr ) == true )
    {
        try
        {
            const uint32_t idu32 = numeric_cast<uint32_t>( id );
            // Make sure there isn't already a "placeholder" with this tag
            // in the value map and the nonstandard VR map
            if( m_value_dict.count( idu32 ) == 0 &&
                m_nonstandard_vr_dict.count( idu32 ) == 0 )
            {
                // should not be possible for this to be NULL
                assert( g_context != nullptr );

                MC_VR preexisting_vr = UNKNOWN_VR;
                ret = g_context->get_vr_type( id, this, preexisting_vr );
                // If the tag was not found in the main data dictionary
                if( ret != MC_NORMAL_COMPLETION )
                {
                    // Add the tag to the nonstandard map
                    m_nonstandard_vr_dict[idu32] = vr;
                    // And add a "placeholder" attribute
                    m_value_dict[idu32] = nullptr;
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
    }
    else
    {
        ret = MC_INVALID_VR_CODE;
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

MC_STATUS data_dictionary::write_values( tx_stream&                 stream,
                                         int                        app_id,
                                         value_dict::const_iterator begin,
                                         value_dict::const_iterator end ) const
{
    MC_STATUS ret = MC_NORMAL_COMPLETION;

    // should never happen
    assert( g_context != nullptr );

    const application* const app = g_context->get_application( app_id );

    for( value_dict::const_iterator itr = begin;
         itr != end && ret == MC_NORMAL_COMPLETION;
         ++itr )
    {
        callback_parms_t callback =
            app != nullptr ? app->get_callback_function( itr->first ) :
                             callback_parms_t( nullptr, nullptr );

        // Write an element if there is a Callback Function registered
        // for this tag or if there is a value written to it
        if( callback.first != nullptr || itr->second != nullptr )
        {
            // Write the tag number
            ret = stream.write_tag( itr->first );
            if( ret == MC_NORMAL_COMPLETION )
            {
                if( callback.first != nullptr )
                {
                    MC_VR tag_vr = UNKNOWN_VR;

                    ret = get_vr_type( itr->first, tag_vr );
                    if( ret == MC_NORMAL_COMPLETION )
                    {
                        ret = stream.write_vr( tag_vr );
                        if( ret == MC_NORMAL_COMPLETION )
                        {
                            ret = write_vr_data_from_callback( stream,
                                                               m_id,
                                                               itr->first,
                                                               callback );
                        }
                        else
                        {
                            // Do nothing. Will return error from 
                        }
                    }
                    else
                    {
                        // Do nothing. Will return error from get_vr_type
                    }
                }
                else if( itr->second != nullptr )
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
                        ret = itr->second->to_stream( stream );
                    }
                    else
                    {
                        // Do nothing. Will return error from write_vr
                    }
                }
                else
                {
                    // Should be impossible
                    assert( false );
                }
            }
            else
            {
                // Do nothing. Will return error from write_tag
            }
        }
        else
        {
            // a NULL value_representation pointer is defined as "not present
            // in the message" provided there also is not a Callback Function
            // registered for that tag. So do nothing
        }
    }

    return ret;
}

MC_STATUS data_dictionary::get_vr_type( uint32_t tag, MC_VR& type ) const
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    // Check the nonstandard VR map first
    const nonstandard_vr_dict::const_iterator itr =
        m_nonstandard_vr_dict.find( tag );
    if( itr == m_nonstandard_vr_dict.cend() )
    {
        // Should never happen
        assert( g_context != nullptr );

        MC_VR global_vr = UNKNOWN_VR;
        ret = g_context->get_vr_type( tag, this, global_vr );
        if( ret == MC_NORMAL_COMPLETION )
        {
            type = global_vr;
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            // Do nothing. Will return error from get_vr_type
        }
    }
    else
    {
        type = itr->second;
        ret = MC_NORMAL_COMPLETION;
    }

    return ret;
}

}
