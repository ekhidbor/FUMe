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

// boost
#include "boost/bimap.hpp"

// local public
#include "mc3msg.h"
#include "mcstatus.h"

// local private
#include "fume/tag_to_vr.h"
#include "fume/transfer_syntax_to_uid.h"

namespace fume
{

class data_dictionary;
class value_representation;
class application;

class library_context
{
public:
    library_context();
    ~library_context();

    int create_file_object( const char* filename,
                            const char* service_name,
                            MC_COMMAND  command );
    MC_STATUS free_file_object( int id );

    int create_item_object( const char* item_name );
    MC_STATUS free_item_object( int id );

    data_dictionary* get_object( int id );
    // create_vr can optionally take in a data_dictionary object for
    // dictionary-specific VR specializations (eg. pixel data)
    std::unique_ptr<value_representation>
    create_vr( unsigned long          tag,
               const data_dictionary* dict ) const;

    MC_STATUS get_vr_type( unsigned long          tag,
                           const data_dictionary* dict,
                           MC_VR&                 type ) const;

    MC_STATUS get_vr_info( unsigned long          tag,
                           const data_dictionary* dict,
                           MC_VR&                 type,
                           unsigned short&        min_vals,
                           unsigned short&        max_vals,
                           unsigned short&        multiple ) const;

    int register_application( const char* ae_title );
    MC_STATUS release_application( int id );

    application* get_application( int id );

    MC_STATUS get_transfer_syntax_from_enum( TRANSFER_SYNTAX syntax,
                                             char*           uid,
                                             int             uid_length ) const;

    MC_STATUS get_enum_from_transfer_syntax( const char*      uid,
                                             TRANSFER_SYNTAX& syntax ) const;

private:
    typedef std::unique_ptr<data_dictionary> data_dictionary_ptr;
    typedef std::unique_ptr<application> application_ptr;
    typedef std::unordered_map<int, data_dictionary_ptr> data_dictionary_map;
    typedef std::unordered_map<int, application_ptr> application_map;

private:
    int generate_id();

private:
    data_dictionary_map                m_data_dictionaries;
    application_map                    m_applications;
    tag_to_vr_map                      m_tag_vr_dict;
    transfer_syntax_map_t              m_transfer_syntax_map;
    std::default_random_engine         m_rng;
    std::uniform_int_distribution<int> m_id_gen;

    mutable std::mutex                 m_mutex;
};

extern std::unique_ptr<library_context> g_context;

}

#endif
