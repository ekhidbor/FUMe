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

// boost
#include "boost/range/iterator_range.hpp"

// local public
#include "mcstatus.h"
#include "mc3msg.h"

namespace fume
{

class value_representation;
class tx_stream;
class rx_stream;

class data_dictionary
{
public:
    data_dictionary( int id, bool created_empty );
    virtual ~data_dictionary();

    // Will attempt to create a value_representation if is empty
    value_representation* at( uint32_t tag );

    // Will return NULL if the value representation is empty
    const value_representation* at( uint32_t tag ) const;

    MC_STATUS set_empty( uint32_t tag );

    MC_STATUS delete_attribute( uint32_t tag );
    MC_STATUS delete_range( uint32_t first_tag, uint32_t last_tag );

    MC_STATUS add_standard_attribute( uint32_t tag );
    MC_STATUS add_nonstandard_attribute( uint32_t tag, MC_VR vr );

    MC_STATUS copy_values( const data_dictionary& source,
                           const unsigned long*   tags );

    void copy_values( const data_dictionary& source,
                      uint32_t               first_tag,
                      uint32_t               last_tag );

    bool created_empty() const
    {
        return m_created_empty;
    }

    bool has_tag( uint32_t tag ) const;

    MC_STATUS set_callbacks( int application_id );

    virtual MC_STATUS set_transfer_syntax( TRANSFER_SYNTAX syntax ) = 0;
    virtual MC_STATUS get_transfer_syntax( TRANSFER_SYNTAX& syntax ) const = 0;

    int id() const
    {
        return m_id;
    }

protected:
    typedef std::unique_ptr<value_representation>       unique_vr_ptr;
    typedef std::map<uint32_t, unique_vr_ptr>           value_dict;
    typedef value_dict::const_iterator                  value_dict_const_itr;
    typedef value_dict::iterator                        value_dict_itr;
    typedef boost::iterator_range<value_dict_const_itr> const_value_range;
    typedef boost::iterator_range<value_dict_itr>       value_range;

protected:
    // Used to implement empty_file, empty_item, and empty_message
    void set_all_empty();

    value_dict::iterator begin()
    {
        return m_value_dict.begin();
    }

    value_dict::const_iterator begin() const
    {
        return m_value_dict.begin();
    }

    value_dict::iterator end()
    {
        return m_value_dict.end();
    }

    value_dict::const_iterator end() const
    {
        return m_value_dict.end();
    }

    // Will always attempt to create a value_representation
    value_representation& operator[]( uint32_t tag );

    MC_STATUS read_values( rx_stream&      stream,
                           TRANSFER_SYNTAX syntax,
                           uint32_t        size );

    MC_STATUS write_values( tx_stream&                 stream,
                            TRANSFER_SYNTAX            syntax,
                            value_dict::const_iterator begin,
                            value_dict::const_iterator end ) const
    {
        return write_values( stream, syntax, m_application_id, begin, end );
    }

    MC_STATUS write_values( tx_stream&                 stream,
                            TRANSFER_SYNTAX            syntax,
                            int                        app_id,
                            value_dict::const_iterator begin,
                            value_dict::const_iterator end ) const;

    const_value_range get_value_range( uint32_t begin_tag, uint32_t end_tag ) const;
    value_range get_value_range( uint32_t begin_tag, uint32_t end_tag );

    int application_id() const
    {
        return m_application_id;
    }

private:
    typedef std::unordered_map<uint32_t, MC_VR> nonstandard_vr_dict;

private:
    data_dictionary( const data_dictionary& );
    data_dictionary& operator=( const data_dictionary& );

    MC_STATUS get_vr_type( uint32_t tag, MC_VR& type ) const;
    unique_vr_ptr create_vr( uint32_t tag ) const;

private:
    value_dict          m_value_dict;
    int                 m_id;
    bool                m_created_empty;
    nonstandard_vr_dict m_nonstandard_vr_dict;
    int                 m_application_id;
};

} // namespace fume


#endif
