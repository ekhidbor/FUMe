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

// local public
#include "diction.h"

// local private
#include "fume/record_object.h"
#include "fume/tx_stream.h"
#include "fume/value_representation.h"

namespace fume
{

record_object::record_object( int         dicomdir_file_id,
                              int         parent_id,
                              int         id,
                              bool        created_empty )
    : item_object( id, created_empty ),
      m_dicomdir_file_id( dicomdir_file_id ),
      m_parent_id( parent_id ),
      m_next_record( -1 ),
      m_child_record( -1 )
{
    (*this)[MC_ATT_OFFSET_OF_THE_NEXT_DIRECTORY_RECORD].set
    (
        static_cast<unsigned long>( 0x00000000u )
    );
    (*this)[MC_ATT_OFFSET_OF_REFERENCED_LOWER_LEVEL_DIRECTORY_ENTITY].set
    (
        static_cast<unsigned long>( 0x00000000u )
    );
}

MC_STATUS record_object::to_stream( tx_stream&      stream,
                                    TRANSFER_SYNTAX syntax ) const
{
    // Save off the byte offset of this record
    m_offset = stream.bytes_written();
    return item_object::to_stream( stream, syntax );
}

}
