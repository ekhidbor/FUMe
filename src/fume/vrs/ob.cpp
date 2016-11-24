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
#include <limits>

// local public
#include "diction.h"

// local private
#include "fume/vr_data_parser.h"
#include "fume/source_callback_io.h"
#include "fume/get_value_function_sink.h"
#include "fume/encapsulated_value.h"
#include "fume/vrs/ob.h"

using std::numeric_limits;
using std::unique_ptr;

using fume::write_data_from_function;

namespace fume
{
namespace vrs
{

ob::ob()
    : value_representation( 1u, 1u, 1u )
{
}

ob::~ob()
{
}

MC_STATUS ob::to_stream( tx_stream& stream, TRANSFER_SYNTAX syntax )
{
    return m_stream->write_vr_data_to_stream( stream, syntax );
}

MC_STATUS ob::from_stream( rx_stream& stream, TRANSFER_SYNTAX syntax )
{
    //unique_ptr<encapsulated_value_stream> tmp( new encapsulated_value() );
    unique_ptr<encapsulated_value> tmp;
    MC_STATUS ret = read_vr_data( stream, syntax, *tmp );
    if( ret == MC_NORMAL_COMPLETION )
    {
        m_stream.swap( tmp );
    }
    else
    {
        // Leave value unchanged and return error
    }

    return ret;
}

MC_STATUS ob::set( const set_func_parms& val )
{
    //unique_ptr<encapsulated_value> tmp( new encapsulated_value() );
    unique_ptr<encapsulated_value> tmp;
    MC_STATUS ret = write_data_from_function( val, *tmp );
    if( ret == MC_NORMAL_COMPLETION )
    {
        m_stream.swap( tmp );
    }
    else
    {
        // Leave value unchanged and return error
    }

    return ret;
}

MC_STATUS ob::set_null()
{
    return m_stream->clear();
}

MC_STATUS ob::get( const get_func_parms& val )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( val.callback != nullptr )
    {
        get_value_function_sink dest( val, false );
        // encapsulated values are always written in explicit little endian
        ret = m_stream->write_data_to_stream( dest, EXPLICIT_LITTLE_ENDIAN );
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS ob::get_encapsulated( const get_func_parms& val )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( val.callback != nullptr )
    {
        get_value_function_sink dest( val, false );
        ret = m_stream->write_first_frame_to_stream( dest,
                                                     EXPLICIT_LITTLE_ENDIAN );
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS ob::get_next_encapsulated( const get_func_parms& val )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( val.callback != nullptr )
    {
        get_value_function_sink dest( val, false );
        ret = m_stream->write_next_frame_to_stream( dest,
                                                    EXPLICIT_LITTLE_ENDIAN );
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS ob::get_frame( unsigned int idx, const get_func_parms& val )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( val.callback != nullptr )
    {
        get_value_function_sink dest( val, false );
        ret = m_stream->write_frame_to_stream( idx,
                                               dest,
                                               EXPLICIT_LITTLE_ENDIAN );
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

bool ob::is_null() const
{
    return m_stream->size() == 0;
}

} // namespace vrs
} // namespace fume
