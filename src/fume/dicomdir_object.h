#ifndef DICOMDIR_OBJECT_H
#define DICOMDIR_OBJECT_H
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
#include <unordered_map>

// local public
#include "mcstatus.h"
#include "mc3media.h"

// local private
#include "fume/file_object.h"

namespace fume
{

class dicomdir_object : public file_object
{
public:
    dicomdir_object( int                    id,
                     const char*            filename,
                     data_dictionary* source,
                     bool                   created_empty );
    virtual ~dicomdir_object()
    {
    }

    MC_STATUS set_child_record( int child_record )
    {
        // Caller performs validation
        m_child_record = child_record;
        return MC_NORMAL_COMPLETION;
    }
    int get_child_record() const
    {
        return m_child_record;
    }

    MC_STATUS update();

private:
    int m_child_record;
};

}

#endif
