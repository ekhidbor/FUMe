#ifndef RECORD_OBJECT_H
#define RECORD_OBJECT_H
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
#include "mcstatus.h"
#include "mc3media.h"

// local private
#include "fume/item_object.h"

namespace fume
{

class record_object : public item_object
{
public:
    record_object( int         dicomdir_file_id,
                   int         parent_id,
                   int         id,
                   const char* record_type,
                   bool        created_empty );
    virtual ~record_object()
    {
    }

    int get_dicomdir_file_id() const
    {
        return m_dicomdir_file_id;
    }

    int get_parent_record() const
    {
        return m_parent_id;
    }

    MC_STATUS set_next_record( int record_id )
    {
        // Caller performs record validation
        m_next_record = record_id;
        return MC_NORMAL_COMPLETION;
    }
    int get_next_record() const
    {
        return m_next_record;
    }

    MC_STATUS set_child_record( int child_record )
    {
        // Caller performs record validation
        m_child_record = child_record;
        return MC_NORMAL_COMPLETION;
    }
    int get_child_record() const
    {
        return m_child_record;
    }

    MC_STATUS get_record_type( MC_DIR_RECORD_TYPE& type ) const;

    uint32_t get_offset() const
    {
        return m_offset;
    }

    virtual MC_STATUS to_stream( tx_stream&      stream,
                                 TRANSFER_SYNTAX syntax ) const override;

private:
    int m_dicomdir_file_id;
    int m_parent_id;
    int m_next_record;
    int m_child_record;
    // Used to fill in DICOMDIR offset parameters
    mutable uint32_t m_offset;
};

}

#endif
