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

// boost
#include "boost/bimap.hpp"

// local privatw
#include "fume/transfer_syntax_to_uid.h"

using std::string;
using std::pair;
using std::begin;
using std::end;
using boost::bimap;

namespace fume
{

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

transfer_syntax_map_t create_transfer_syntax_to_uid_map()
{
    return boost::bimap<TRANSFER_SYNTAX, string>( begin( TRANSFER_SYNTAXES ),
                                                  end( TRANSFER_SYNTAXES ) );
}

} // namespace fume

