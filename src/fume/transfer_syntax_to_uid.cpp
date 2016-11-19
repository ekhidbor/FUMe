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
#include <utility>
#include <iterator>
#include <string>

// boost
#include "boost/bimap.hpp"

// local private
#include "fume/transfer_syntax_to_uid.h"

using std::string;
using std::pair;
using std::begin;
using std::end;
using boost::bimap;

namespace fume
{

typedef bimap<TRANSFER_SYNTAX, string> transfer_syntax_map_t;

static const transfer_syntax_map_t::value_type TRANSFER_SYNTAXES[] =
{
    { INVALID_TRANSFER_SYNTAX,         ""                        },
    { IMPLICIT_LITTLE_ENDIAN,          "1.2.840.10008.1.2"       },
    { EXPLICIT_LITTLE_ENDIAN,          "1.2.840.10008.1.2.1"     },
    { EXPLICIT_BIG_ENDIAN,             "1.2.840.10008.1.2.2"     },
    { IMPLICIT_BIG_ENDIAN,             ""                        },
    { DEFLATED_EXPLICIT_LITTLE_ENDIAN, "1.2.840.10008.1.2.1.99"  },
    { RLE,                             "1.2.840.10008.1.2.5"     },
    { JPEG_BASELINE,                   "1.2.840.10008.1.2.4.50"  },
    { JPEG_EXTENDED_2_4,               "1.2.840.10008.1.2.4.51"  },
    { JPEG_EXTENDED_3_5,               "1.2.840.10008.1.2.4.52"  },
    { JPEG_SPEC_NON_HIER_6_8,          "1.2.840.10008.1.2.4.53"  },
    { JPEG_SPEC_NON_HIER_7_9,          "1.2.840.10008.1.2.4.54"  },
    { JPEG_FULL_PROG_NON_HIER_10_12,   "1.2.840.10008.1.2.4.55"  },
    { JPEG_FULL_PROG_NON_HIER_11_13,   "1.2.840.10008.1.2.4.56"  },
    { JPEG_LOSSLESS_NON_HIER_14,       "1.2.840.10008.1.2.4.57"  },
    { JPEG_LOSSLESS_NON_HIER_15,       "1.2.840.10008.1.2.4.58"  },
    { JPEG_EXTENDED_HIER_16_18,        "1.2.840.10008.1.2.4.59"  },
    { JPEG_EXTENDED_HIER_17_19,        "1.2.840.10008.1.2.4.60"  },
    { JPEG_SPEC_HIER_20_22,            "1.2.840.10008.1.2.4.61"  },
    { JPEG_SPEC_HIER_21_23,            "1.2.840.10008.1.2.4.62"  },
    { JPEG_FULL_PROG_HIER_24_26,       "1.2.840.10008.1.2.4.63"  },
    { JPEG_FULL_PROG_HIER_25_27,       "1.2.840.10008.1.2.4.64"  },
    { JPEG_LOSSLESS_HIER_28,           "1.2.840.10008.1.2.4.65"  },
    { JPEG_LOSSLESS_HIER_29,           "1.2.840.10008.1.2.4.66"  },
    { JPEG_LOSSLESS_HIER_14,           "1.2.840.10008.1.2.4.70"  },
    { JPEG_2000_LOSSLESS_ONLY,         "1.2.840.10008.1.2.4.90"  },
    { JPEG_2000,                       "1.2.840.10008.1.2.4.91"  },
    { JPEG_LS_LOSSLESS,                "1.2.840.10008.1.2.4.80"  },
    { JPEG_LS_LOSSY,                   "1.2.840.10008.1.2.4.81"  },
    { MPEG2_MPML,                      "1.2.840.10008.1.2.4.100" },
    { PRIVATE_SYNTAX_1,                ""                        },
    { PRIVATE_SYNTAX_2,                ""                        },
    { JPEG_2000_MC_LOSSLESS_ONLY,      "1.2.840.10008.1.2.4.92"  },
    { JPEG_2000_MC,                    "1.2.840.10008.1.2.4.93"  },
    { MPEG2_MPHL,                      "1.2.840.10008.1.2.4.101" },
    { MPEG4_AVC_H264_HP_LEVEL_4_1,     "1.2.840.10008.1.2.4.102" },
    { MPEG4_AVC_H264_BDC_HP_LEVEL_4_1, "1.2.840.10008.1.2.4.103" },
    { JPIP_REFERENCED,                 "1.2.840.10008.1.2.4.94"  },
    { JPIP_REFERENCED_DEFLATE,         "1.2.840.10008.1.2.4.95"  }
};

static const transfer_syntax_map_t& transfer_syntax_map()
{
    static bimap<TRANSFER_SYNTAX, string> ret( begin( TRANSFER_SYNTAXES ),
                                               end( TRANSFER_SYNTAXES ) );
    return ret;
}

MC_STATUS get_transfer_syntax_from_enum( TRANSFER_SYNTAX syntax,
                                         char*           uid,
                                         int             uid_length )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( uid != nullptr )
    {
        transfer_syntax_map_t::left_map::const_iterator itr =
            transfer_syntax_map().left.find( syntax );
        if( itr != transfer_syntax_map().left.end() )
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

MC_STATUS get_enum_from_transfer_syntax( const char*      uid,
                                         TRANSFER_SYNTAX& syntax )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( uid != nullptr )
    {
        transfer_syntax_map_t::right_map::const_iterator itr =
            transfer_syntax_map().right.find( uid );
        if( itr != transfer_syntax_map().right.end() )
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

} // namespace fume

