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
#include "diction.h"

// local private
#include "fume/library_context.h"
#include "fume/dicomdir_object.h"
#include "fume/record_object.h"
#include "fume/value_representation.h"

using fume::g_context;
using fume::dicomdir_object;
using fume::record_object;
using fume::data_dictionary;
using fume::value_representation;

template<class RecordType>
MC_STATUS append_record( RecordType& parent, int child_id )
{
    MC_STATUS ret = MC_NORMAL_COMPLETION;

    int next = parent.get_child_record();
    record_object* child = nullptr;
    while( ret == MC_NORMAL_COMPLETION && next > 0 )
    {
        child = dynamic_cast<record_object*>( g_context->get_object( next ) );
        if( child != nullptr )
        {
            next = child->get_next_record();
        }
        else
        {
            ret = MC_INVALID_RECORD_ID;
        }
    }

    if( ret == MC_NORMAL_COMPLETION )
    {
        if( child == nullptr )
        {
            // No child elements in chain
            ret = parent.set_child_record( child_id );
        }
        else
        {
            ret = child->set_next_record( child_id );
        }
    }

    return ret;
}

template<class RecordType>
int add_record( dicomdir_object& file_obj,
                RecordType&      parent,
                const char*      record_type )
{
    int record_id = g_context->create_record_object( file_obj.id(),
                                                     parent.id(),
                                                     record_type );
    if( record_id > 0 )
    {
        MC_STATUS stat = append_record( parent, record_id );
        if( stat == MC_NORMAL_COMPLETION )
        {
            value_representation* sequence =
                file_obj.at( MC_ATT_DIRECTORY_RECORD_SEQUENCE );
            if( sequence != nullptr )
            {
                if( sequence->is_null() == false )
                {
                    stat = sequence->set_next( record_id );
                }
                else
                {
                    stat = sequence->set( record_id );
                }

                if( stat != MC_NORMAL_COMPLETION )
                {
                    record_id = -static_cast<int>( stat );
                }
                else
                {
                    // Do nothing. Success
                }
            }
            else
            {
                record_id = -static_cast<int>( MC_INVALID_TAG );
            }
        }
        else
        {
            record_id = -static_cast<int>( stat );
        }
    }
    else
    {
        // Do nothing. Will return error
    }

    return record_id;
}

MC_STATUS MC_DDH_Add_Record( int ParentID, const char* RecordType, int* RecordID )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context != nullptr && RecordID != nullptr )
        {
            data_dictionary* dict = g_context->get_object( ParentID );
            if( dict != nullptr )
            {
                dicomdir_object* dicomdir = dynamic_cast<dicomdir_object*>( dict );
                record_object* record = dynamic_cast<record_object*>( dict );

                if( dicomdir != nullptr )
                {
                    // If record type is NULL, fill in with PATIENT
                    // since the parent is the file itself
                    if( RecordType == nullptr )
                    {
                        RecordType = "PATIENT";
                    }
                    else
                    {
                        // Do nothing. RecordType already provided
                    }

                    const int record_id = add_record( *dicomdir,
                                                      *dicomdir,
                                                      RecordType );
                    if( record_id > 0 )
                    {
                        *RecordID = record_id;
                        ret = MC_NORMAL_COMPLETION;
                    }
                    else
                    {
                        ret = static_cast<MC_STATUS>( -record_id );
                    }
                }
                else if( record != nullptr )
                {
                    const int dicomdir_file_id = record->get_dicomdir_file_id();
                    dicomdir = dynamic_cast<dicomdir_object*>
                               (
                                   g_context->get_object( dicomdir_file_id )
                               );
                    if( dicomdir != nullptr )
                    {
                        const int record_id = add_record( *dicomdir,
                                                          *record,
                                                          RecordType );
                        if( record_id > 0 )
                        {
                            *RecordID = record_id;
                            ret = MC_NORMAL_COMPLETION;
                        }
                        else
                        {
                            ret = static_cast<MC_STATUS>( -record_id );
                        }
                    }
                    else
                    {
                        ret = MC_BAD_DIR_RECORD_TYPE;
                    }
                }
                else
                {
                    ret = MC_INVALID_RECORD_ID;
                }
            }
            else
            {
                ret = MC_INVALID_RECORD_ID;
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
