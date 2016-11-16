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
#include <unordered_set>
#include <algorithm>

// local public
#include "mcstatus.h"
#include "mc3media.h"
#include "diction.h"

// local private
#include "fume/library_context.h"
#include "fume/dicomdir_object.h"
#include "fume/record_object.h"
#include "fume/value_representation.h"

using std::unordered_set;
using std::for_each;

using fume::g_context;
using fume::record_object;
using fume::data_dictionary;
using fume::dicomdir_object;
using fume::value_representation;

static MC_TRAVERSAL_STATUS add_record_to_delete( int id, void* ptr )
{
    MC_TRAVERSAL_STATUS ret = MC_TS_ERROR;

    unordered_set<int>* to_delete = static_cast<unordered_set<int>*>( ptr );
    if( ptr != nullptr )
    {
        to_delete->insert( id );
        ret = MC_TS_CONTINUE;
    }
    else
    {
        ret = MC_TS_ERROR;
    }

    return ret;
}

static MC_STATUS get_records_to_delete( int                 parent_id,
                                        unordered_set<int>& records )
{
    unordered_set<int> to_delete;
    to_delete.insert( parent_id );

    MC_STATUS ret = MC_DDH_Traverse_Records( parent_id,
                                             static_cast<void*>( &to_delete ),
                                             add_record_to_delete );
    if( ret == MC_NORMAL_COMPLETION )
    {
        records.swap( to_delete );
    }
    else
    {
        // Do nothing. Will return error without updating records set
    }

    return ret;
}

static MC_STATUS remove_and_delete_records( data_dictionary&          dict,
                                            const unordered_set<int>& records )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    value_representation* sequence = dict.at( MC_ATT_DIRECTORY_RECORD_SEQUENCE );
    if( sequence != nullptr && sequence->is_null() == false )
    {
        int cur_id = 0;
        ret = sequence->get( cur_id );
        while( ret == MC_NORMAL_COMPLETION )
        {
            if( records.count( cur_id ) > 0 )
            {
                // delete_current will update the iterator so no call
                // to get_next is necessary
                ret = sequence->delete_current();
            }
            else
            {
                ret = sequence->get_next( cur_id );
            }
        }

        // If the traversal has finished
        if( ret == MC_NO_MORE_VALUES )
        {
            // Delete all items (the above call to delete_current doesn't
            // actually delete the items)
            for_each( records.cbegin(),
                      records.cend(),
                      []( int record_id )
                      {
                          (void)g_context->free_item_object( record_id );
                      } );
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            // Do nothing. Will return error
        }
    }
    else if( sequence->is_null() == true )
    {
        if( records.empty() == false )
        {
            // Delete all items then return an error to indicate
            // the inconsistency
            for_each( records.cbegin(),
                      records.cend(),
                      []( int record_id )
                      {
                          (void)g_context->free_item_object( record_id );
                      } );
            ret = MC_SYSTEM_ERROR;
        }
        else
        {
            ret = MC_NORMAL_COMPLETION;
        }
    }
    else
    {
        ret = MC_SYSTEM_ERROR;
    }

    return ret;
}

template<class RecordType>
static MC_STATUS remove_record_from_parent( RecordType&    parent,
                                            record_object& child )
{
    MC_STATUS ret = MC_NORMAL_COMPLETION;

    int itr_child_id = parent.get_child_record();
    if( itr_child_id == child.id() )
    {
        parent.set_child_record( child.get_next_record() );
        ret = MC_NORMAL_COMPLETION;
    }
    else if( itr_child_id > 0 )
    {
        do
        {
            record_object* itr_child =
                dynamic_cast<record_object*>
                (
                    g_context->get_object( itr_child_id )
                );
            if( itr_child != nullptr )
            {
                if( itr_child->get_next_record() == child.id() )
                {
                    itr_child->set_next_record( child.get_next_record() );
                    // Terminate loop
                    itr_child_id = -1;
                    ret = MC_NORMAL_COMPLETION;
                }
                else
                {
                    itr_child_id = itr_child->get_next_record();
                    ret = MC_NORMAL_COMPLETION;
                }
            }
            else
            {
                ret = MC_INVALID_RECORD_ID;
            }
        }
        while( ret == MC_NORMAL_COMPLETION && itr_child_id > 0 );
    }
    else
    {
        // Supposed parent id has no children. Indicate serious error
        ret = MC_SYSTEM_ERROR;
    }

    return ret;
}

static MC_STATUS adjust_record_references( record_object& record )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    const int parent_id = record.get_parent_record();
    assert( parent_id > 0 );

    data_dictionary* parent = g_context->get_object( parent_id );
    if( parent != nullptr )
    {
        record_object* parent_record = dynamic_cast<record_object*>( parent );
        dicomdir_object* parent_dicomdir = dynamic_cast<dicomdir_object*>( parent );

        if( parent_record != nullptr )
        {
            ret = remove_record_from_parent( *parent_record, record );
        }
        else if( parent_dicomdir != nullptr )
        {
            ret = remove_record_from_parent( *parent_dicomdir, record );
        }
        else
        {
            ret = MC_SYSTEM_ERROR;
        }
    }
    else
    {
        ret = MC_SYSTEM_ERROR;
    }

    return ret;
}

static MC_STATUS delete_record( int record_id )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    record_object* record =
        dynamic_cast<record_object*>( g_context->get_object( record_id ) );
    if( record != nullptr )
    {
        unordered_set<int> to_delete;
        ret = get_records_to_delete( record_id, to_delete );
        if( ret == MC_NORMAL_COMPLETION )
        {
            ret = adjust_record_references( *record );
            if( ret == MC_NORMAL_COMPLETION )
            {
                const int dicomdir_id = record->get_dicomdir_file_id();
                dicomdir_object* dicomdir =
                    dynamic_cast<dicomdir_object*>
                    (
                        g_context->get_object( dicomdir_id )
                    );
                if( dicomdir != nullptr )
                {
                    ret = remove_and_delete_records( *dicomdir, to_delete );
                }
                else
                {
                    // The parent file ID is invalid, which shouldn't
                    // happen
                    ret = MC_SYSTEM_ERROR;
                }
            }
            else
            {
                // Do nothing. Will return error
            }
        }
        else
        {
            // Do nothing. Will return error
        }
    }
    else
    {
        ret = MC_INVALID_RECORD_ID;
    }

    return ret;
}

MC_STATUS MC_DDH_Delete_Record( int RecordID )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context != nullptr )
        {
            record_object* record =
                dynamic_cast<record_object*>( g_context->get_object( RecordID ) );
            dicomdir_object* dicomdir =
                dynamic_cast<dicomdir_object*>( g_context->get_object( RecordID ) );
            if( record != nullptr )
            {
                ret = delete_record( RecordID );
            }
            else if( dicomdir != nullptr )
            {
                ret = MC_NORMAL_COMPLETION;
                while( ret == MC_NORMAL_COMPLETION &&
                       dicomdir->get_child_record() > 0 )
                {
                    // delete_record will remove the record from the
                    // dicomdir object and adjust the child record
                    // pointer, so there is no need to loop through
                    // the records. Just keep getting the first child
                    // in the list
                    ret = delete_record( dicomdir->get_child_record() );
                }
            }
            else
            {
                ret = MC_INVALID_RECORD_ID;
            }
        }
    }
    catch( ... )
    {
        ret = MC_SYSTEM_ERROR;
    }

    return ret;
}
