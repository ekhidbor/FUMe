#ifndef DATA_DICTIONARY_SEARCH_H
#define DATA_DICTIONARY_SEARCH_H
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

// boost
#include "boost/range/iterator_range.hpp"

// local public
#include "mcstatus.h"

// local private
#include "fume/data_dictionary_types.h"

namespace fume
{

class tx_stream;
class data_dictionary;

typedef boost::iterator_range<dictionary_iter> dictionary_value_range;

dictionary_value_range get_value_range( data_dictionary& dict,
                                        uint32_t         begin_tag,
                                        uint32_t         end_tag );

bool erase_tag( data_dictionary& dict, uint32_t tag );
void erase_range( data_dictionary& dict, uint32_t begin_tag, uint32_t end_tag );

bool has_tag( data_dictionary& dict, uint32_t tag );

void copy_values( data_dictionary&     dest,
                  data_dictionary&     source,
                  const unsigned long* tags );

void copy_values( data_dictionary& dest,
                  data_dictionary& source,
                  uint32_t         first_tag,
                  uint32_t         last_tag );

}

#endif
