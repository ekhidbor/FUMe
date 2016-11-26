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
#include <memory>

// boost
#include "boost/numeric/conversion/cast.hpp"

// local public
#include "mcstatus.h"

// local private
#include "fume/data_dictionary_search.h"
#include "fume/data_dictionary.h"
#include "fume/value_representation.h"

using boost::numeric_cast;
using boost::bad_numeric_cast;

using std::move;

namespace fume
{

static void copy_values( value_dict&      dest,
                         data_dictionary& source,
                         uint32_t         first_tag,
                         uint32_t         last_tag );

dictionary_value_range get_value_range( data_dictionary& dict,
                                        uint32_t         begin_tag,
                                        uint32_t         end_tag )
{
    dictionary_iter itr_begin =
        find_if( dict.begin(),
                 dict.end(),
                 [begin_tag]( value_dict::const_reference val )
                 {
                     return val.first >= begin_tag;
                 } );
    dictionary_iter itr_end =
        find_if( itr_begin,
                 dict.end(),
                 [end_tag]( value_dict::const_reference val )
                 {
                     return val.first > end_tag;
                 } );

    return dictionary_value_range( itr_begin, itr_end );
}

void erase_range( data_dictionary& dict, uint32_t begin_tag, uint32_t end_tag )
{
    const dictionary_value_range& range( get_value_range( dict,
                                                          begin_tag,
                                                          end_tag ) );
    dict.erase( range.begin(), range.end() );
}

bool erase_tag( data_dictionary& dict, uint32_t tag )
{
    bool found = false;
    dictionary_iter itr = dict.find( tag );

    found = itr != dict.end();
    if( found )
    {
        dict.erase( itr );
    }

    return found;
}

void copy_values( data_dictionary&     dest,
                  data_dictionary&     source,
                  const unsigned long* tags )
{
    value_dict to_add;

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

                    copy_values( to_add, source, tag_start, tag_end );
                }
                else
                {
                    const value_representation* vr = source.at( tag_u32 );
                    if( vr != nullptr )
                    {
                        to_add[tag_u32] = vr->clone();
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
        // Copy all
        copy_values( to_add, source, 0x00000000, 0xFFFFFFFF );
    }

    dest.insert( move( to_add ) );
}

void copy_values( data_dictionary& dest,
                  data_dictionary& source,
                  uint32_t         first_tag,
                  uint32_t         last_tag )
{
    value_dict tmp_vals;
    copy_values( tmp_vals, source, first_tag, last_tag );

    dest.insert( move( tmp_vals ) );
}

void copy_values( value_dict&      dest,
                  data_dictionary& source,
                  uint32_t         first_tag,
                  uint32_t         last_tag )
{
    const dictionary_value_range& range( get_value_range( source,
                                                          first_tag,
                                                          last_tag ) );

    for( value_dict::const_reference source_elem : range )
    {
        if( source_elem.second != nullptr )
        {
            dest[source_elem.first] = source_elem.second->clone();
        }
    }
}

}