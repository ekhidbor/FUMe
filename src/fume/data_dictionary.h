#ifndef DATA_DICTIONARY_H
#define DATA_DICTIONARY_H
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
#include <cstdint>
#include <map>
#include <unordered_map>
#include <memory>
#include <utility>
#include <functional>

// boost
#include "boost/range/iterator_range.hpp"

// local public
#include "mcstatus.h"
#include "mc3msg.h"

// local private
#include "fume/data_dictionary_types.h"

namespace fume
{

class value_representation;
class tx_stream;
class rx_stream;

typedef std::unique_ptr<value_representation>       unique_vr_ptr;
typedef std::map<uint32_t, unique_vr_ptr>           value_dict;
typedef value_dict::const_iterator                  value_dict_const_itr;
typedef value_dict::iterator                        value_dict_itr;
typedef boost::iterator_range<value_dict_const_itr> const_value_range;
typedef boost::iterator_range<value_dict_itr>       value_range;

class data_dictionary
{
public:
    data_dictionary( int id, bool created_empty );
    virtual ~data_dictionary();

    // Will attempt to create a value_representation if is empty
    value_representation* at( uint32_t tag );

    // Will return NULL if the value representation is empty
    const value_representation* at( uint32_t tag ) const;

    // Will always attempt to create a value_representation
    value_representation& operator[]( uint32_t tag );

    bool has_tag( uint32_t tag ) const;
    dictionary_iter find( uint32_t tag );

    void erase( dictionary_iter itr );
    void erase( dictionary_iter begin, dictionary_iter end );

    // Used to implement empty_file, empty_item, and empty_message
    void clear();

    dictionary_iter begin()
    {
        return m_value_dict.cbegin();
    }

    dictionary_iter end()
    {
        return m_value_dict.cend();
    }

    MC_STATUS add_standard_attribute( uint32_t tag );
    MC_STATUS add_nonstandard_attribute( uint32_t tag, MC_VR vr );

    MC_STATUS get_vr_type( uint32_t tag, MC_VR& type );

    MC_STATUS copy_values( data_dictionary&     source,
                           const unsigned long* tags );

    void copy_values( data_dictionary& source,
                      uint32_t         first_tag,
                      uint32_t         last_tag );

    bool created_empty() const
    {
        return m_created_empty;
    }

    MC_STATUS set_callbacks( int application_id );

    virtual MC_STATUS set_transfer_syntax( TRANSFER_SYNTAX syntax ) = 0;
    virtual MC_STATUS get_transfer_syntax( TRANSFER_SYNTAX& syntax ) = 0;

    int id() const
    {
        return m_id;
    }

    int application_id() const
    {
        return m_application_id;
    }

protected:

    MC_STATUS read_values_from_item( rx_stream&      stream,
                                     TRANSFER_SYNTAX syntax,
                                     int             app_id,
                                     uint32_t        size );

    MC_STATUS read_values( rx_stream&      stream,
                           TRANSFER_SYNTAX syntax,
                           int             app_id );

    MC_STATUS read_values_upto( rx_stream&      stream,
                                TRANSFER_SYNTAX syntax,
                                int             app_id,
                                uint32_t        end_tag );

private:
    data_dictionary( const data_dictionary& );
    data_dictionary& operator=( const data_dictionary& );

    unique_vr_ptr create_vr( uint32_t tag );

private:
    value_dict          m_value_dict;
    int                 m_id;
    bool                m_created_empty;
    nonstandard_vr_dict m_nonstandard_vr_dict;
    int                 m_application_id;
};

} // namespace fume


#endif
