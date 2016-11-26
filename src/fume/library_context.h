#ifndef LIBRARY_CONTEXT_H
#define LIBRARY_CONTEXT_H
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
#include <unordered_map>
#include <mutex>
#include <random>
#include <memory>
#include <string>

// boost
#include "boost/bimap.hpp"

// local public
#include "mc3msg.h"
#include "mc3media.h"
#include "mcstatus.h"

// local private
#include "fume/tag_to_vr.h"
#include "fume/configuration_maps.h"

namespace fume
{

class data_dictionary;
class file_object;
class application;
class value_representation;

class library_context
{
public:
    library_context();
    ~library_context();

    int create_file_object( const char* filename,
                            const char* service_name,
                            MC_COMMAND  command );
    int create_dicomdir_object( const char*        filename,
                                const char*        fileset,
                                file_object* template_file );
    int create_empty_file_object( const char* filename );

    MC_STATUS free_file_object( int id );

    int create_item_object( const char* item_name );
    int create_empty_item_object();
    MC_STATUS free_item_object( int id );

    int create_record_object( int         file_id,
                              int         parent_id,
                              const char* record_type );
    MC_STATUS free_record_object( int id );

    data_dictionary* get_object( int id );
    // create_vr can optionally take in a data_dictionary object for
    // dictionary-specific VR specializations (eg. pixel data)

    std::unique_ptr<value_representation>
    create_vr( uint32_t         tag,
               data_dictionary* dict ) const;

    MC_STATUS get_vr_type( uint32_t         tag,
                           data_dictionary* dict,
                           MC_VR&           type ) const;

    MC_STATUS get_vr_info( uint32_t         tag,
                           data_dictionary* dict,
                           MC_VR&           type,
                           unsigned short&  min_vals,
                           unsigned short&  max_vals,
                           unsigned short&  multiple ) const;

    MC_STATUS get_string_config_value( StringParm          parm,
                                       const std::string*& value ) const;

public:

    int register_application( const char* ae_title );
    MC_STATUS release_application( int id );

    application* get_application( int id );

private:
    typedef std::unique_ptr<data_dictionary> data_dictionary_ptr;
    typedef std::unique_ptr<application> application_ptr;
    typedef std::unordered_map<int, data_dictionary_ptr> data_dictionary_map;
    typedef std::unordered_map<int, application_ptr> application_map;

private:
    int generate_id();

    template<class Derived>
    MC_STATUS free_dictionary_object( int id, MC_STATUS invalid_id_stat );

private:
    data_dictionary_map                m_data_dictionaries;
    application_map                    m_applications;
    const tag_to_vr_map                m_tag_vr_dict;
    config_maps                        m_config_maps;
    std::default_random_engine         m_rng;
    std::uniform_int_distribution<int> m_id_gen;

    mutable std::mutex                 m_mutex;
};

template<class Derived>
MC_STATUS library_context::free_dictionary_object( int       id,
                                                   MC_STATUS invalid_id_stat )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    // Declare above lock_guard so destructor
    // runs with library_context unlocked. Need
    // to do this so data_dictionary elements
    // containing sequence items don't deadlock
    data_dictionary_ptr to_free = nullptr;

    std::lock_guard<std::mutex> lock(m_mutex);

    data_dictionary_map::iterator itr( m_data_dictionaries.find( id ) );
    if( itr != m_data_dictionaries.end() &&
        dynamic_cast<Derived*>( itr->second.get() ) != nullptr )
    {
        // Delete data_dictionary object after mutating
        // map. data_dictionary elements can contain
        // data_dictionary elements (SQ VR), so calling
        // map::erase can be indirectly called from within
        // map::erase, which is discomforting
        to_free.swap( itr->second );

        // Now erasing the element won't call the data_dictionary
        // destructor
        m_data_dictionaries.erase( itr );
        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        ret = invalid_id_stat;
    }

    return ret;
}

extern std::unique_ptr<library_context> g_context;

}

#endif
