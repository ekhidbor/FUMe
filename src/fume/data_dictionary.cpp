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

// local public
#include "diction.h"

// local private
#include "fume/data_dictionary.h"
#include "fume/data_dictionary_search.h"
#include "fume/library_context.h"
#include "fume/value_representation.h"
#include "fume/source_callback_io.h"
#include "fume/vr_factory.h"
#include "fume/vr_field.h"
#include "fume/rx_stream.h"

using std::numeric_limits;
using std::unique_ptr;

namespace fume
{

static MC_STATUS create_vr_from_stream
(
    rx_stream&                        stream,
    TRANSFER_SYNTAX                   syntax,
    uint32_t                          tag,
    unique_ptr<value_representation>& element
)
{
    MC_STATUS ret = MC_NORMAL_COMPLETION;

    if( syntax != IMPLICIT_LITTLE_ENDIAN )
    {
        MC_VR vr = UNKNOWN_VR;
        ret = stream.read_vr( vr, syntax );
        if( ret == MC_NORMAL_COMPLETION )
        {
            element = create_vr( vr, 1, 1, 1 );
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            // Do nothing. Will return error
        }
    }
    else
    {
        assert( g_context != nullptr );
        element = g_context->create_vr( tag, nullptr );
        ret = MC_NORMAL_COMPLETION;
    }

    return ret;
}

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

dictionary_iter data_dictionary::find( uint32_t tag )
{
    return m_value_dict.find( tag );
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

void data_dictionary::clear()
{
    m_value_dict.clear();
}

void data_dictionary::erase( dictionary_iter itr )
{
    m_value_dict.erase( itr );
}

void data_dictionary::erase( dictionary_iter begin, dictionary_iter end )
{
    m_value_dict.erase( begin, end );
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

void data_dictionary::insert( value_dict&& values )
{
    value_dict tmp_values( move( values ) );

    for( value_dict::reference source_elem : tmp_values )
    {
        if( source_elem.second != nullptr )
        {
            m_value_dict[source_elem.first].swap( source_elem.second );
        }
        else
        {
            // If value is NULL do not modify original value
        }
    }
}

MC_STATUS data_dictionary::read_values_from_item( rx_stream&      stream,
                                                  TRANSFER_SYNTAX syntax,
                                                  int             app_id,
                                                  uint32_t        size )
{
    const uint32_t start_offset = stream.tell_read();
    MC_STATUS ret = MC_NORMAL_COMPLETION;
    bool graceful_end_of_data = false;

    value_dict tmp_value_dict;

    while( ret == MC_NORMAL_COMPLETION &&
           ((stream.tell_read() - start_offset) + 1) < size )
    {
        uint32_t tag = 0;
        ret = stream.read_tag( tag, syntax );
        if( ret == MC_NORMAL_COMPLETION )
        {
            if( tag != MC_ATT_ITEM_DELIMITATION_ITEM     &&
                tag != MC_ATT_ITEM                       &&
                tag != MC_ATT_SEQUENCE_DELIMITATION_ITEM )
            {
                unique_vr_ptr element;
                ret = create_vr_from_stream( stream, syntax, tag, element );
                if( ret == MC_NORMAL_COMPLETION && element != nullptr )
                {
                    ret = element->from_stream( stream, syntax );
                    if( ret == MC_NORMAL_COMPLETION )
                    {
                        tmp_value_dict[tag].swap( element );
                    }
                    else
                    {
                        // Do nothing. Will return error
                    }
                }
                else if( element == nullptr )
                {
                    ret = MC_INVALID_TAG;
                }
                else
                {
                    // Do nothing. Will return error
                }
            }
            // If we've reached an item delimiter
            else if( tag == MC_ATT_ITEM_DELIMITATION_ITEM )
            {
                // Read the length (which should be zero, but we aren't going
                // to check)
                uint32_t delim_length = 0;
                ret = stream.read_val( delim_length, syntax );
                break;
            }
            // We received something we shouln't have
            else
            {
                ret = MC_UNEXPECTED_EOD;
            }
        }
        else if( ret == MC_END_OF_DATA )
        {
            // No more data when trying to read the start of a tag.
            // terminate gracefully
            graceful_end_of_data = true;
        }
    }

    // Upon success, copy the values into the dictionary
    if( ret == MC_NORMAL_COMPLETION ||
        (ret == MC_END_OF_DATA && graceful_end_of_data == true) )
    {
        for( value_dict::reference val : tmp_value_dict )
        {
            m_value_dict[val.first].swap( val.second );
        }

        ret = MC_NORMAL_COMPLETION;
    }

    return ret;
}

MC_STATUS data_dictionary::read_values( rx_stream&      stream,
                                        TRANSFER_SYNTAX syntax,
                                        int             app_id )
{
    MC_STATUS ret = MC_NORMAL_COMPLETION;
    bool graceful_end_of_data = false;

    value_dict tmp_value_dict;

    while( ret == MC_NORMAL_COMPLETION )
    {
        uint32_t tag = 0;
        ret = stream.read_tag( tag, syntax );
        if( ret == MC_NORMAL_COMPLETION )
        {
            if( tag != MC_ATT_ITEM_DELIMITATION_ITEM     &&
                tag != MC_ATT_ITEM                       &&
                tag != MC_ATT_SEQUENCE_DELIMITATION_ITEM )
            {
                unique_vr_ptr element;
                ret = create_vr_from_stream( stream, syntax, tag, element );
                if( ret == MC_NORMAL_COMPLETION && element != nullptr )
                {
                    ret = element->from_stream( stream, syntax );
                    if( ret == MC_NORMAL_COMPLETION )
                    {
                        tmp_value_dict[tag].swap( element );
                    }
                    else
                    {
                        // Do nothing. Will return error
                    }
                }
                else if( element == nullptr )
                {
                    ret = MC_INVALID_TAG;
                }
                else
                {
                    // Do nothing. Will return error
                }
            }
            else
            {
                // We're not expecting to get delimiter tags
                ret = MC_INVALID_TAG;
            }
        }
        else if( ret == MC_END_OF_DATA )
        {
            // No more data when trying to read the start of a tag.
            // terminate gracefully
            graceful_end_of_data = true;
        }
    }

    // Upon success, copy the values into the dictionary
    if( ret == MC_NORMAL_COMPLETION ||
        (ret == MC_END_OF_DATA && graceful_end_of_data == true) )
    {
        for( value_dict::reference val : tmp_value_dict )
        {
            m_value_dict[val.first].swap( val.second );
        }

        ret = MC_NORMAL_COMPLETION;
    }

    return ret;
}

MC_STATUS data_dictionary::read_values_upto( rx_stream&      stream,
                                             TRANSFER_SYNTAX syntax,
                                             int             app_id,
                                             uint32_t        end_tag )
{
    MC_STATUS ret = MC_NORMAL_COMPLETION;
    bool graceful_end_of_data = false;

    value_dict tmp_value_dict;

    while( MC_NORMAL_COMPLETION == ret )
    {
        uint32_t tag = 0;
        ret = stream.peek_tag( tag, syntax );
        if( ret == MC_NORMAL_COMPLETION )
        {
            if( tag != MC_ATT_ITEM_DELIMITATION_ITEM     &&
                tag != MC_ATT_ITEM                       &&
                tag != MC_ATT_SEQUENCE_DELIMITATION_ITEM )
            {
                if( tag <= end_tag )
                {
                    // This shouldn't fail if the peek succeeded
                    (void)stream.read_tag( tag, syntax );

                    unique_vr_ptr element;
                    ret = create_vr_from_stream( stream, syntax, tag, element );
                    if( ret == MC_NORMAL_COMPLETION && element != nullptr )
                    {
                        ret = element->from_stream( stream, syntax );
                        if( ret == MC_NORMAL_COMPLETION )
                        {
                            tmp_value_dict[tag].swap( element );
                        }
                        else
                        {
                            // Do nothing. Will return error
                        }
                    }
                    else if( element == nullptr )
                    {
                        ret = MC_INVALID_TAG;
                    }
                    else
                    {
                        // Do nothing. Will return error
                    }
                }
                else
                {
                    graceful_end_of_data = true;
                    ret = MC_END_OF_DATA;
                }
            }
            else
            {
                // We're not expecting to get delimiter tags
                ret = MC_INVALID_TAG;
            }
        }
        else if( ret == MC_END_OF_DATA )
        {
            // No more data when trying to read the start of a tag.
            // terminate gracefully
            graceful_end_of_data = true;
        }
        else
        {
            // Do nothing. Will return error
        }
    }

    // Upon success, copy the values into the dictionary
    if( ret == MC_NORMAL_COMPLETION ||
        (ret == MC_END_OF_DATA && graceful_end_of_data == true) )
    {
        for( value_dict::reference val : tmp_value_dict )
        {
            m_value_dict[val.first].swap( val.second );
        }

        ret = MC_NORMAL_COMPLETION;
    }

    return ret;
}

MC_STATUS data_dictionary::get_vr_type( uint32_t tag, MC_VR& type )
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

unique_vr_ptr data_dictionary::create_vr( uint32_t tag )
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
