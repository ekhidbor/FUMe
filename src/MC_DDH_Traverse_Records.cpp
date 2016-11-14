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
#include <deque>

// local public
#include "mcstatus.h"
#include "mc3media.h"

// local private
#include "fume/library_context.h"
#include "fume/dicomdir_object.h"
#include "fume/record_object.h"

using std::deque;

using fume::g_context;
using fume::dicomdir_object;
using fume::record_object;
using fume::data_dictionary;

template<class RecordType>
MC_STATUS traverse_records( const RecordType&   parent,
                            void*               user_data,
                            DDHTraverseCallback callback )
{
    MC_STATUS ret = MC_NORMAL_COMPLETION;

    deque<int> children;
    children.push_back( parent.get_child_record() );
    while( ret == MC_NORMAL_COMPLETION && children.empty() == false )
    {
        int child = children.back();
        children.pop_back();

        if( child > 0 )
        {
            const record_object* child_obj =
                dynamic_cast<record_object*>( g_context->get_object( child ) );
            if( child_obj != nullptr )
            {
                MC_TRAVERSAL_STATUS stat = callback( child, user_data );
                switch( stat )
                {
                    case MC_TS_CONTINUE:
                    {
                        // Traverse the next record after traversing this
                        // record's children
                        children.push_back( child_obj->get_next_record() );
                        children.push_back( child_obj->get_child_record() );
                        break;
                    }
                    case MC_TS_STOP_LEVEL:
                    {
                        // Do nothing. will traverse parent's next item
                        break;
                    }
                    case MC_TS_STOP_LOWER:
                    {
                        // Traverse the next record
                        children.push_back( child_obj->get_next_record() );
                        break;
                    }
                    case MC_TS_STOP:
                    {
                        // Remove all records to prematurely stop traversal
                        children.clear();
                        break;
                    }
                    case MC_TS_ERROR:
                    {
                        ret = MC_CALLBACK_CANNOT_COMPLY;
                        break;
                    }
                    default:
                    {
                        ret = MC_CALLBACK_PARM_ERROR;
                        break;
                    }
                }
            }
            else
            {
                ret = MC_BAD_DIR_RECORD_TYPE;
            }
        }
        else
        {
            // Do nothing. Will continue traversal
        }
    }

    return ret;
}

MC_STATUS MC_DDH_Traverse_Records( int                 RootID,
                                   void*               UserData,
                                   DDHTraverseCallback YourTraverseCallback )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context != nullptr && YourTraverseCallback != nullptr )
        {
            data_dictionary* dict = g_context->get_object( RootID );
            if( dict != nullptr )
            {
                dicomdir_object* dicomdir = dynamic_cast<dicomdir_object*>( dict );
                record_object* record = dynamic_cast<record_object*>( dict );

                if( dicomdir != nullptr )
                {
                    ret = traverse_records( *dicomdir,
                                            UserData,
                                            YourTraverseCallback );
                }
                else if( record != nullptr )
                {
                    ret = traverse_records( *record,
                                            UserData,
                                            YourTraverseCallback );
                }
                else
                {
                    ret = MC_INVALID_DICOMDIR_ID;
                }
            }
            else
            {
                ret = ret = MC_INVALID_DICOMDIR_ID;
            }
        }
        else if( g_context == nullptr )
        {
            ret = MC_LIBRARY_NOT_INITIALIZED;
        }
        else
        {
            ret = MC_NULL_POINTER_PARM;
        }
    }
    catch( ... )
    {
        ret = MC_SYSTEM_ERROR;
    }

    return ret;
}
