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
#include "fume/record_type_to_string.h"

using std::string;
using std::pair;
using std::begin;
using std::end;
using boost::bimap;

namespace fume
{

static const record_type_map_t::value_type RECORD_IDS[] =
{
    { MC_REC_TYPE_PATIENT,          "PATIENT"          },
    { MC_REC_TYPE_STUDY,            "STUDY"            },
    { MC_REC_TYPE_SERIES,           "SERIES"           },
    { MC_REC_TYPE_IMAGE,            "IMAGE"            },
    { MC_REC_TYPE_RT_DOSE,          "RT DOSE"          },
    { MC_REC_TYPE_RT_STRUCTURE_SET, "RT STRUCTURE SET" },
    { MC_REC_TYPE_RT_PLAN,          "RT PLAN"          },
    { MC_REC_TYPE_RT_TREAT_RECORD,  "RT TREAT RECORD"  },
    { MC_REC_TYPE_PRESENTATION,     "PRESENTATION"     },
    { MC_REC_TYPE_WAVEFORM,         "WAVEFORM"         },
    { MC_REC_TYPE_SR_DOCUMENT,      "SR DOCUMENT"      },
    { MC_REC_TYPE_KEY_OBJECT_DOC,   "KEY OBJECT DOC"   },
    { MC_REC_TYPE_SPECTROSCOPY,     "SPECTROSCOPY"     },
    { MC_REC_TYPE_RAW_DATA,         "RAW DATA"         },
    { MC_REC_TYPE_REGISTRATION,     "REGISTRATION"     },
    { MC_REC_TYPE_FIDUCIAL,         "FIDUCIAL"         },
    { MC_REC_TYPE_HANGING_PROTOCOL, "HANGING PROTOCOL" },
    { MC_REC_TYPE_ENCAP_DOC,        "ENCAP DOC"        },
    { MC_REC_TYPE_HL7_STRUC_DOC,    "HL7 STRUC DOC"    },
    { MC_REC_TYPE_VALUE_MAP,        "VALUE MAP"        },
    { MC_REC_TYPE_STEREOMETRIC,     "STEREOMETRIC"     },
    { MC_REC_TYPE_MEASUREMENT,      "MEASUREMENT"      },
    { MC_REC_TYPE_SURFACE,          "SURFACE"          },
    { MC_REC_TYPE_SURFACE_SCAN,     "SURFACE SCAN"     },
    { MC_REC_TYPE_PALETTE,          "PALETTE"          },
    { MC_REC_TYPE_IMPLANT,          "IMPLANT"          },
    { MC_REC_TYPE_IMPLANT_ASSY,     "IMPLANT ASSY"     },
    { MC_REC_TYPE_IMPLANT_GROUP,    "IMPLANT GROUP"    },
    { MC_REC_TYPE_TRACT,            "TRACT"            },
    { MC_REC_TYPE_ASSESSMENT,       "ASSESSMENT"       },
    { MC_REC_TYPE_PRIVATE,          "PRIVATE"          },
    { MC_REC_TYPE_UNKNOWN,          "UNKNOWN"          }
};

record_type_map_t create_record_type_to_string_map()
{
    return record_type_map_t( begin( RECORD_IDS ), end( RECORD_IDS ) );
}

} // namespace fume

