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
#include <cassert>

// local private
#include "fume/vr_factory.h"
// VR classes
#include "fume/vrs/ae.h"
#include "fume/vrs/as.h"
#include "fume/vrs/at.h"
#include "fume/vrs/cs.h"
#include "fume/vrs/da.h"
#include "fume/vrs/ds.h"
#include "fume/vrs/dt.h"
#include "fume/vrs/fd.h"
#include "fume/vrs/fl.h"
#include "fume/vrs/is.h"
#include "fume/vrs/lo.h"
#include "fume/vrs/lt.h"
#include "fume/vrs/ob.h"
#include "fume/vrs/od.h"
#include "fume/vrs/of.h"
#include "fume/vrs/ol.h"
#include "fume/vrs/ow.h"
#include "fume/vrs/pn.h"
#include "fume/vrs/sh.h"
#include "fume/vrs/sl.h"
#include "fume/vrs/sq.h"
#include "fume/vrs/ss.h"
#include "fume/vrs/st.h"
#include "fume/vrs/ul.h"
#include "fume/vrs/us.h"
#include "fume/vrs/ut.h"
#include "fume/vrs/tm.h"
#include "fume/vrs/uc.h"
#include "fume/vrs/ui.h"
#include "fume/vrs/ur.h"

namespace fume
{

std::unique_ptr<value_representation> create_vr( MC_VR        vr,
                                                 unsigned int min_vals,
                                                 unsigned int max_vals,
                                                 unsigned int multiple )
{
    std::unique_ptr<value_representation> ret = nullptr;

    switch( vr )
    {
        case AE:
            ret.reset( new fume::vrs::ae( min_vals, max_vals, multiple ) );
            break;
        case AS:
            ret.reset( new fume::vrs::as( min_vals, max_vals, multiple ) );
            break;
        case CS:
            ret.reset( new fume::vrs::cs( min_vals, max_vals, multiple ) );
            break;
        case DA:
            ret.reset( new fume::vrs::da( min_vals, max_vals, multiple ) );
            break;
        case DS:
            ret.reset( new fume::vrs::ds( min_vals, max_vals, multiple ) );
            break;
        case DT:
            ret.reset( new fume::vrs::dt( min_vals, max_vals, multiple ) );
            break;
        case IS:
            ret.reset( new fume::vrs::is( min_vals, max_vals, multiple ) );
            break;
        case LO:
            ret.reset( new fume::vrs::lo( min_vals, max_vals, multiple ) );
            break;
        case LT:
            // Note: only 1 value allowed in LT
            ret.reset( new fume::vrs::lt() );
            break;
        case PN:
            ret.reset( new fume::vrs::pn( min_vals, max_vals, multiple ) );
            break;
        case SH:
            ret.reset( new fume::vrs::sh( min_vals, max_vals, multiple ) );
            break;
        case ST:
            ret.reset( new fume::vrs::st( min_vals, max_vals, multiple ) );
            break;
        case TM:
            ret.reset( new fume::vrs::tm( min_vals, max_vals, multiple ) );
            break;
        case UC:
            ret.reset( new fume::vrs::uc( min_vals, max_vals, multiple ) );
            break;
        case UR:
            // Note: only 1 value allowed in UR
            ret.reset( new fume::vrs::ur() );
            break;
        case UT:
            // Note: only 1 value allowed in UT
            ret.reset( new fume::vrs::ut() );
            break;
        case UI:
            ret.reset( new fume::vrs::ui( min_vals, max_vals, multiple ) );
            break;
        case SS:
            ret.reset( new fume::vrs::ss( min_vals, max_vals, multiple ) );
            break;
        case US:
            ret.reset( new fume::vrs::us( min_vals, max_vals, multiple ) );
            break;
        case AT:
            ret.reset( new fume::vrs::at( min_vals, max_vals, multiple ) );
            break;
        case SL:
            ret.reset( new fume::vrs::sl( min_vals, max_vals, multiple ) );
            break;
        case UL:
            ret.reset( new fume::vrs::ul( min_vals, max_vals, multiple ) );
            break;
        case FL:
            ret.reset( new fume::vrs::fl( min_vals, max_vals, multiple ) );
            break;
        case FD:
            ret.reset( new fume::vrs::fd( min_vals, max_vals, multiple ) );
            break;
        case UNKNOWN_VR:
            // TODO: implement
            // ret = fume::vrs::un() );
            ret = nullptr;
            break;
        case OB:
            // Note: only 1 value allowed in OB
            ret.reset( new fume::vrs::ob() );
            break;
        case OW:
            // Note: only 1 value allowed in OW
            ret.reset( new fume::vrs::ow() );
            break;
        case OD:
            // Note: only 1 value allowed in OD
            ret.reset( new fume::vrs::od() );
            break;
        case OF:
            // Note: only 1 value allowed in OB
            ret.reset( new fume::vrs::ob() );
            break;
        case SQ:
            ret.reset( new fume::vrs::sq( min_vals, max_vals, multiple ) );
            break;
        default:
            // This path indicates an internal error
            assert( false );
            ret = nullptr;
            break;
    }

    return ret;
}

}
