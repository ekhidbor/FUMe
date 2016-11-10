/**
 * This file is a part of the FUMe project.
 *
 * To the extent possible under law, the person who associated CC0 with
 * FUMe has waived all copyright and related or neighboring rights
 * to FUMe.

 * You should have received a copy of the CC0 legalcode along with this
 * work.  If not, see http://creativecommons.org/publicdomain/zero/1.0/.
 */

// std
#include <cassert>
#include <ctime>
#include <limits>
#include <memory>
#include <algorithm>
#include <string>

// boost
#include "boost/numeric/conversion/cast.hpp"

// local public
#include "mc3msg.h"

// local private
#include "fume/library_context.h"
#include "fume/application.h"
#include "fume/file_object.h"
#include "fume/item_object.h"
#include "fume/vr_factory.h"
#include "fume/transfer_syntax_to_uid.h"

using std::numeric_limits;
using std::unordered_map;
using std::lock_guard;
using std::mutex;
using std::unique_ptr;
using std::all_of;
using std::pair;
using std::string;
using std::default_random_engine;

using boost::bimap;
using boost::numeric_cast;
using boost::bad_numeric_cast;

namespace fume
{

unique_ptr<library_context> g_context;

library_context::library_context()
      // Generate IDs greater than 0
    : m_tag_vr_dict( create_default_tag_vr_dict() ),
      m_transfer_syntax_map( create_transfer_syntax_to_uid_map() ),
      m_rng( static_cast<default_random_engine::result_type>( clock() ) ),
      m_id_gen( 1, numeric_limits<int>::max() )
{
}

library_context::~library_context()
{
}

int library_context::create_file_object( const char* filename,
                                         const char* service_name,
                                         MC_COMMAND  command )
{
    // NOTE: error codes from this function are NEGATIVE because the
    // positive values indicate file IDs returned
    int ret = -MC_CANNOT_COMPLY;
    if( service_name != nullptr )
    {
        // TODO: don't create empty
        ret = create_empty_file_object( filename );
    }
    else
    {
        ret = -MC_NULL_POINTER_PARM;
    }

    return ret;
}

int library_context::create_empty_file_object( const char* filename )
{
    // NOTE: error codes from this function are NEGATIVE because the
    // positive values indicate file IDs returned
    int ret = -MC_CANNOT_COMPLY;

    if( filename != nullptr )
    {
        lock_guard<mutex> lock(m_mutex);
        const int id = generate_id();
        // generate_id shall maintain uniqueness, but assert here
        assert( m_data_dictionaries.count( id ) == 0 );

        // Create empty file object
        data_dictionary_ptr file_obj( new file_object( id, filename, true ) );

        // TODO: initialize file object dictionary based on service name/command

        // Insert the file object into the dictionary. Two-step process
        // (create then swap) to prevent memory leak in case operator[]
        // throws an exception
        m_data_dictionaries[id].swap( file_obj );

        ret = id;
    }
    else
    {
        ret = -MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS library_context::free_file_object( int id )
{
    return free_dictionary_object<file_object>( id, MC_INVALID_FILE_ID );
}

int library_context::create_item_object( const char* item_name )
{
    // NOTE: error codes from this function are NEGATIVE because the
    // positive values indicate file IDs returned
    int ret = -MC_CANNOT_COMPLY;

    if( item_name != nullptr )
    {
        lock_guard<mutex> lock(m_mutex);
        const int id = generate_id();
        // generate_id shall maintain uniqueness, but assert here
        assert( m_data_dictionaries.count( id ) == 0 );

        // Create file object out of lock scope
        // TODO: don't create empty
        data_dictionary_ptr item_obj( new item_object( id, true ) );

        // TODO: initialize item object dictionary based on item name

        // Insert the file object into the dictionary. Two-step process
        // (create then swap) to prevent memory leak in case operator[]
        // throws an exception
        m_data_dictionaries[id].swap( item_obj );

        ret = id;
    }
    else
    {
        ret = -MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS library_context::free_item_object( int id )
{
    return free_dictionary_object<item_object>( id, MC_INVALID_ITEM_ID );
}

data_dictionary* library_context::get_object( int id )
{
    lock_guard<mutex> lock(m_mutex);

    data_dictionary_map::const_iterator itr( m_data_dictionaries.find( id ) );
    return itr == m_data_dictionaries.cend() ? nullptr : itr->second.get();
}

unique_ptr<value_representation>
library_context::create_vr( uint32_t               tag,
                            const data_dictionary* dict ) const
{
    unique_vr_ptr_t ret = nullptr;

    MC_VR type = UNKNOWN_VR;
    unsigned short min_vals = 0;
    unsigned short max_vals = 0;
    unsigned short multiple = 0;

    if( get_vr_info( tag,
                     dict,
                     type,
                     min_vals,
                     max_vals,
                     multiple ) == MC_NORMAL_COMPLETION )
    {
        ret = fume::create_vr( type, min_vals, max_vals, multiple );
    }
    else
    {
        ret = nullptr;
    }

    return ret;
}

MC_STATUS library_context::get_vr_type( uint32_t               tag,
                                        const data_dictionary* dict,
                                        MC_VR&                 type ) const
{
    unsigned short min_vals = 0;
    unsigned short max_vals = 0;
    unsigned short multiple = 0;

    return get_vr_info( tag, dict, type, min_vals, max_vals, multiple );
}

MC_STATUS library_context::get_vr_info( uint32_t               tag,
                                        const data_dictionary* dict,
                                        MC_VR&                 type,
                                        unsigned short&        min_vals,
                                        unsigned short&        max_vals,
                                        unsigned short&        multiple ) const
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    tag_to_vr_map::const_iterator itr = m_tag_vr_dict.find( tag );
    if( itr != m_tag_vr_dict.cend() )
    {
        if( dict == nullptr ||
            get_conditional_tag_vr( tag, *dict, type ) == false )
        {
            type = static_cast<MC_VR>( itr->second.vr );
        }
        else
        {
            // Do nothing. get_conditional_tag_vr provided a value
            // for actual_vr
        }

        min_vals = itr->second.min_vals;
        max_vals = itr->second.max_vals;
        multiple = itr->second.multiple;

        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_INVALID_TAG;
    }

    return ret;
}

int library_context::register_application( const char* ae_title )
{
    // NOTE: error codes from this function are NEGATIVE because the
    // positive values indicate file IDs returned
    int ret = -MC_CANNOT_COMPLY;

    if( ae_title != nullptr )
    {
        lock_guard<mutex> lock(m_mutex);

        // Make sure application with the given AE Title doesn't already
        // exist
        if( all_of( m_applications.cbegin(),
                    m_applications.cend(),
                    [ae_title]( const application_map::value_type& entry )
                    {
                        return entry.second->ae_title() != ae_title;
                    } ) == true )
        {
            // Create file object out of lock scope
            application_ptr app_obj( new application( ae_title ) );

            const int id = generate_id();
            // generate_id shall maintain uniqueness, but assert here
            assert( m_applications.count( id ) == 0 );

            // Insert the file object into the dictionary. Two-step process
            // (create then swap) to prevent memory leak in case operator[]
            // throws an exception
            m_applications[id].swap( app_obj );

            ret = id;
        }
        else
        {
            ret = -MC_ALREADY_REGISTERED;
        }
    }
    else
    {
        ret = -MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS library_context::release_application( int id )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    // Delete application pointer outside of lock scope. No
    // need to do this except for performance and consistency
    // with file/message/item freeing logic
    application_ptr to_free = nullptr;

    lock_guard<mutex> lock(m_mutex);

    application_map::iterator itr( m_applications.find( id ) );
    if( itr != m_applications.cend() )
    {
        to_free.swap( itr->second );

        m_applications.erase( itr );
        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = MC_INVALID_APPLICATION_ID;
    }

    return ret;
}

application* library_context::get_application( int id )
{
    lock_guard<mutex> lock(m_mutex);

    application_map::const_iterator itr( m_applications.find( id ) );
    return itr == m_applications.cend() ? nullptr : itr->second.get();
}

MC_STATUS library_context::get_transfer_syntax_from_enum
(
    TRANSFER_SYNTAX syntax,
    char*           uid,
    int             uid_length
) const
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( uid != nullptr )
    {
        transfer_syntax_map_t::left_map::const_iterator itr =
            m_transfer_syntax_map.left.find( syntax );
        if( itr != m_transfer_syntax_map.left.end() )
        {
            if( static_cast<int>( itr->second.size() ) < uid_length )
            {
                strncpy( uid, itr->second.c_str(), uid_length );
                ret = MC_NORMAL_COMPLETION;
            }
            else
            {
                ret = MC_BUFFER_TOO_SMALL;
            }
        }
        else
        {
            ret = MC_INVALID_TRANSFER_SYNTAX;
        }
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS library_context::get_enum_from_transfer_syntax
( 
    const char*      uid,
    TRANSFER_SYNTAX& syntax
) const
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( uid != nullptr )
    {
        transfer_syntax_map_t::right_map::const_iterator itr =
            m_transfer_syntax_map.right.find( uid );
        if( itr != m_transfer_syntax_map.right.end() )
        {
            syntax = itr->second;
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            ret = MC_INVALID_TRANSFER_SYNTAX;
        }
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

int library_context::generate_id()
{
    // This function must only be called by a function which performs
    // its own locking.

    int ret = -1;
    do
    {
        ret = m_id_gen( m_rng );
    }
    while( m_data_dictionaries.count( ret ) != 0 &&
           m_applications.count( ret )      != 0 );

    return ret;
}


}
