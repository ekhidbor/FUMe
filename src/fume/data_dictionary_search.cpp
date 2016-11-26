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

// local public
#include "mcstatus.h"

// local private
#include "fume/data_dictionary_search.h"
#include "fume/data_dictionary.h"

namespace fume
{

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

}