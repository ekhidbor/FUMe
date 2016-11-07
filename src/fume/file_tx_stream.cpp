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
#include "fume/file_tx_stream.h"

using std::string;

namespace fume
{

file_tx_stream::file_tx_stream( const string&     filename,
                                WriteFileCallback callback,
                                void*             user_info )
    : m_filename( filename ),
      m_callback( callback ),
      m_user_info( user_info ),
      m_first( true )
{
    // Checked by caller
    assert( m_callback != nullptr );
}

MC_STATUS file_tx_stream::write( const void* buffer, size_t buffer_bytes )
{
    const MC_STATUS stat = m_callback( const_cast<char*>( m_filename.c_str() ),
                                       m_user_info,
                                       // TODO: determine if this is safe
                                       static_cast<int>( buffer_bytes ),
                                       const_cast<void*>( buffer ),
                                       static_cast<int>( m_first ),
                                       static_cast<int>( false ) );
    m_first = false;
    return stat != MC_NORMAL_COMPLETION ? MC_CALLBACK_CANNOT_COMPLY :
                                          MC_NORMAL_COMPLETION;
}

MC_STATUS file_tx_stream::finalize()
{
    const MC_STATUS stat = m_callback( const_cast<char*>( m_filename.c_str() ),
                                       m_user_info,
                                       0,
                                       nullptr,
                                       static_cast<int>( m_first ),
                                       static_cast<int>( true ) );

    return stat != MC_NORMAL_COMPLETION ? MC_CALLBACK_CANNOT_COMPLY :
                                          MC_NORMAL_COMPLETION;
}

TRANSFER_SYNTAX file_tx_stream::transfer_syntax() const
{
    // TODO: obtain from data_dictionary
    return EXPLICIT_LITTLE_ENDIAN;
}

}
