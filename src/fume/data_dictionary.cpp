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
#include "fume/callback_io.h"
#include "fume/vr_factory.h"

using std::numeric_limits;
using std::for_each;
using std::find_if;
using std::unique_ptr;

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

bool data_dictionary::has_tag( uint32_t tag ) const
{
    return m_value_dict.count( tag ) > 0;
}

// NOTE: this internal function asserts that the Tag is valid. It
// is intended as an internal function where the caller is absolutely
// sure that the tag id is valid and an invalid tag id represents an
// internal bug
value_representation& data_dictionary::operator[]( uint32_t tag )
{
    unique_vr_ptr& ret( m_value_dict[tag] );

    // If value was just created or was empty, create a new one
    if( ret == nullptr )
    {
        ret = create_vr( tag );
    }
    else
    {
        // Do nothing. Value already non-NULL
    }

    assert( ret != nullptr );
    return *ret;
}

value_representation* data_dictionary::at( uint32_t tag )
{
    value_representation* ret = nullptr;

    const value_dict::const_iterator itr = m_value_dict.find( tag );
    if( itr != m_value_dict.cend() )
    {
        // Already exists; return
        ret = itr->second.get();
    }
    else
    {
        // Doesn't exist
        if( m_created_empty )
        {
            // This is an object created with _Empty If tag is valid,
            // insert into dictionary
            unique_vr_ptr new_vr( create_vr( tag ) );
            // Can return NULL if tag invalid
            if( new_vr != nullptr )
            {
                ret = new_vr.get();
                m_value_dict[tag].swap( new_vr );
            }
            else
            {
                ret = nullptr;
            }
        }
        else
        {
            // Not created empty. Return NULL without modifying the
            // map
            ret = nullptr;
        }
    }

    return ret;
}

const value_representation* data_dictionary::at( uint32_t tag ) const
{
    value_dict::const_iterator itr = m_value_dict.find( tag );

    return itr != m_value_dict.cend() ? itr->second.get() : nullptr;
}

MC_STATUS data_dictionary::set_empty( uint32_t tag )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    const value_dict::iterator itr = m_value_dict.find( tag );

    if( itr != m_value_dict.end() )
    {
        itr->second = nullptr;
        ret = MC_NORMAL_COMPLETION;
    }
    else
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

MC_STATUS data_dictionary::delete_attribute( uint32_t tag )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    const size_t elements_removed = m_value_dict.erase( tag );
    // In case nonstandard VR, remove it from that list
    m_nonstandard_vr_dict.erase( tag );

    // Invalid tag if no elements removed
    ret = elements_removed > 0 ? MC_NORMAL_COMPLETION : MC_INVALID_TAG;

    return ret;
}

MC_STATUS data_dictionary::delete_range( uint32_t first_tag, uint32_t last_tag )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( first_tag <= last_tag )
    {
        const value_range& range( get_value_range( first_tag, last_tag ) );

        m_value_dict.erase( range.first, range.second );
        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_INVALID_TAG;
    }

    return ret;
}

data_dictionary::const_value_range
data_dictionary::get_value_range( uint32_t begin_tag, uint32_t end_tag ) const
{
    const_value_range ret;
    ret.first = find_if( m_value_dict.begin(),
                         m_value_dict.end(),
                         [begin_tag]( value_dict::const_reference val )
                         {
                             return val.first >= begin_tag;
                         } );
    ret.second = find_if( ret.first,
                          m_value_dict.end(),
                          [end_tag]( value_dict::const_reference val )
                          {
                              return val.first > end_tag;
                          } );

    return ret;
}

data_dictionary::value_range
data_dictionary::get_value_range( uint32_t begin_tag, uint32_t end_tag )
{
    value_range ret;
    ret.first = find_if( m_value_dict.begin(),
                         m_value_dict.end(),
                         [begin_tag]( value_dict::const_reference val )
                         {
                             return val.first >= begin_tag;
                         } );
    ret.second = find_if( ret.first,
                          m_value_dict.end(),
                          [end_tag]( value_dict::const_reference val )
                          {
                              return val.first > end_tag;
                          } );

    return ret;
}

MC_STATUS data_dictionary::copy_values( const data_dictionary& source,
                                        const unsigned long*   tags )
{
    if( tags != nullptr )
    {
        for( size_t i = 0; tags[i] != 0; ++i )
        {
            try
            {
                const uint32_t tag_u32 = numeric_cast<uint32_t>( tags[i] );
                // If this is a group length tag (has a elemt ID of 9)
                if( (tag_u32 & 0x0000FFFFu) == 0 )
                {
                    // Copy all tags with that group ID
                    const uint32_t tag_start = tag_u32 & 0xFFFF0000u;
                    const uint32_t tag_end   = tag_u32 | 0x0000FFFFu;
                    const_value_range range = source.get_value_range( tag_start,
                                                                      tag_end );
                    for( value_dict_const_itr itr = range.first;
                         itr != range.second;
                         ++itr )
                    {
                        if( itr->second != nullptr )
                        {
                            m_value_dict[itr->first] = itr->second->clone();
                        }
                    }
                }
                else
                {
                    const value_representation* vr = source.at( tag_u32 );
                    if( vr != nullptr )
                    {
                        m_value_dict[tag_u32] = vr->clone();
                    }
                }
            }
            catch( const bad_numeric_cast& )
            {
                // Just ignore like any other tag not present in the message
            }
        }
    }
    else
    {
        for( value_dict_const_itr itr = source.begin();
             itr != source.end();
             ++itr )
        {
            if( itr->second != nullptr )
            {
                m_value_dict[itr->first] = itr->second->clone();
            }
        }
    }

    return MC_NORMAL_COMPLETION;
}

MC_STATUS data_dictionary::add_standard_attribute( uint32_t tag )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( m_value_dict.count( tag ) == 0 )
    {
        // should not be possible for this to be NULL
        assert( g_context != nullptr );

        MC_VR id_vr = UNKNOWN_VR;
        ret = g_context->get_vr_type( tag, this, id_vr );
        if( ret == MC_NORMAL_COMPLETION )
        {
            // add_standard_attribute adds a "placeholder" attribute.
            // It does not add a zero-length attribute.
            m_value_dict[tag] = nullptr;
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

    return ret;
}

MC_STATUS data_dictionary::add_nonstandard_attribute( uint32_t tag, MC_VR vr )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( vr_is_valid( vr ) == true )
    {
        // Make sure there isn't already a "placeholder" with this tag
        // in the value map and the nonstandard VR map
        if( m_value_dict.count( tag ) == 0 &&
            m_nonstandard_vr_dict.count( tag ) == 0 )
        {
            // should not be possible for this to be NULL
            assert( g_context != nullptr );

            MC_VR preexisting_vr = UNKNOWN_VR;
            ret = g_context->get_vr_type( tag, this, preexisting_vr );
            // If the tag was not found in the main data dictionary
            if( ret != MC_NORMAL_COMPLETION )
            {
                // Add the tag to the nonstandard map
                m_nonstandard_vr_dict[tag] = vr;
                // And add a "placeholder" attribute
                m_value_dict[tag] = nullptr;
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
                                         TRANSFER_SYNTAX            syntax,
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
            ret = stream.write_tag( itr->first, syntax );
            if( ret == MC_NORMAL_COMPLETION )
            {
                if( callback.first != nullptr )
                {
                    MC_VR tag_vr = UNKNOWN_VR;

                    ret = get_vr_type( itr->first, tag_vr );
                    if( ret == MC_NORMAL_COMPLETION )
                    {
                        ret = stream.write_vr( tag_vr, syntax );
                        if( ret == MC_NORMAL_COMPLETION )
                        {
                            ret = write_vr_data_from_callback( stream,
                                                               syntax,
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
                    ret = stream.write_vr( itr->second->vr(), syntax );
                    if( ret == MC_NORMAL_COMPLETION )
                    {
                        ret = itr->second->to_stream( stream, syntax );
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
    const nonstandard_vr_dict::const_iterator itr = m_nonstandard_vr_dict.find( tag );
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

unique_ptr<value_representation> data_dictionary::create_vr( uint32_t tag ) const
{
    unique_vr_ptr ret = nullptr;

    const nonstandard_vr_dict::const_iterator ns_itr =
        m_nonstandard_vr_dict.find( tag );
    // If this is a "nonstandard" tag
    if( ns_itr != m_nonstandard_vr_dict.cend() )
    {
        // Create a VR based on the contents of the nonstandard
        // map, allowing any number of values
        ret = fume::create_vr( ns_itr->second,
                               1,
                               numeric_limits<unsigned short>::max(),
                               1 );
    }
    else
    {
        // Should never happen
        assert( g_context != nullptr );
        // Otherwise create a VR using the global tag -> VR table
        ret = g_context->create_vr( tag, this );
    }

    return ret;
}

}
