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
#include <cstdint>

// local public
#include "mcstatus.h"
#include "diction.h"

// local private
#include "fume/data_dictionary_io.h"
#include "fume/data_dictionary_search.h"
#include "fume/data_dictionary.h"
#include "fume/application.h"
#include "fume/tx_stream.h"
#include "fume/rx_stream.h"
#include "fume/library_context.h"
#include "fume/source_callback_io.h"
#include "fume/vr_factory.h"

namespace fume
{

static MC_STATUS create_vr_from_stream( rx_stream&      stream,
                                        TRANSFER_SYNTAX syntax,
                                        uint32_t        tag,
                                        unique_vr_ptr&  element );

static MC_STATUS write_values( tx_stream&       stream,
                               TRANSFER_SYNTAX  syntax,
                               data_dictionary& dict,
                               int              app_id,
                               dictionary_iter  begin,
                               dictionary_iter  end );

MC_STATUS write_values( tx_stream&           stream,
                        TRANSFER_SYNTAX      syntax,
                        data_dictionary&     dict,
                        uint32_t             start_tag,
                        uint32_t             end_tag )
{
    return write_values( stream,
                         syntax,
                         dict,
                         dict.application_id(),
                         start_tag,
                         end_tag );
}

MC_STATUS write_values( tx_stream&       stream,
                        TRANSFER_SYNTAX  syntax,
                        data_dictionary& dict,
                        int              app_id,
                        uint32_t         start_tag,
                        uint32_t         end_tag )
{
    const dictionary_value_range range = get_value_range( dict,
                                                          start_tag,
                                                          end_tag );

    return write_values( stream,
                         syntax,
                         dict,
                         app_id,
                         range.begin(),
                         range.end() );
}

MC_STATUS write_values( tx_stream&           stream,
                        TRANSFER_SYNTAX      syntax,
                        data_dictionary&     dict,
                        int                  app_id )
{
    return write_values( stream,
                         syntax,
                         dict,
                         app_id,
                         dict.begin(),
                         dict.end() );
}

MC_STATUS write_values( tx_stream&       stream,
                        TRANSFER_SYNTAX  syntax,
                        data_dictionary& dict,
                        int              app_id,
                        dictionary_iter  begin,
                        dictionary_iter  end )
{
    const application* const app = g_context->get_application( app_id );
    const dictionary_value_range range( begin, end );

    MC_STATUS ret = MC_NORMAL_COMPLETION;

    for( const value_dict_item& item : range )
    {
        callback_parms_t callback =
            app != nullptr ? app->get_callback_function( item.first ) :
                             callback_parms_t( nullptr, nullptr );

        // Write the tag number
        ret = stream.write_tag( item.first, syntax );
        if( ret == MC_NORMAL_COMPLETION )
        {
            // Write an element if there is a Callback Function registered
            // for this tag or if there is a value written to it
            if( callback.first != nullptr )
            {
                MC_VR tag_vr = UNKNOWN_VR;

                ret = dict.get_vr_type( item.first, tag_vr );
                if( ret == MC_NORMAL_COMPLETION )
                {
                    ret = stream.write_vr( tag_vr, syntax );
                    if( ret == MC_NORMAL_COMPLETION )
                    {
                        ret = write_vr_data_from_callback( stream,
                                                           syntax,
                                                           dict.id(),
                                                           item.first,
                                                           callback );
                    }
                    else
                    {
                        // Do nothing. Will return error from 
                    }
                }
                else
                {
                    // Do nothing. Will return error from get_vr_type
                }
            }
            else
            {
                // this function writes the VR. Techically the
                // value_representation class could do this, but writing it
                // here keeps symmetry between to_stream and from_stream
                // implementations (since data_dictionary has to read the
                // vr to be able to create the object). It also makes things
                // consistent between using the callback function and using
                // the value_representation.
                ret = stream.write_vr( item.second->vr(), syntax );
                if( ret == MC_NORMAL_COMPLETION )
                {
                    ret = item.second->to_stream( stream, syntax );
                }
                else
                {
                    // Do nothing. Will return error from write_vr
                }
            }
        }
    }

    return ret;
}

MC_STATUS read_values_from_item( rx_stream&       stream,
                                 TRANSFER_SYNTAX  syntax,
                                 data_dictionary& dict,
                                 int              app_id,
                                 uint32_t         size )
{
    const uint32_t start_offset = stream.tell_read();
    MC_STATUS ret = MC_NORMAL_COMPLETION;
    bool graceful_end_of_data = false;

    value_dict tmp_value_dict;

    while( ret == MC_NORMAL_COMPLETION &&
           ((stream.tell_read() - start_offset) + 1) < size )
    {
        uint32_t tag = 0;
        ret = stream.read_tag( tag, syntax );
        if( ret == MC_NORMAL_COMPLETION )
        {
            if( tag != MC_ATT_ITEM_DELIMITATION_ITEM     &&
                tag != MC_ATT_ITEM                       &&
                tag != MC_ATT_SEQUENCE_DELIMITATION_ITEM )
            {
                unique_vr_ptr element;
                ret = create_vr_from_stream( stream, syntax, tag, element );
                if( ret == MC_NORMAL_COMPLETION && element != nullptr )
                {
                    ret = element->from_stream( stream, syntax );
                    if( ret == MC_NORMAL_COMPLETION )
                    {
                        tmp_value_dict[tag].swap( element );
                    }
                    else
                    {
                        // Do nothing. Will return error
                    }
                }
                else if( element == nullptr )
                {
                    ret = MC_INVALID_TAG;
                }
                else
                {
                    // Do nothing. Will return error
                }
            }
            // If we've reached an item delimiter
            else if( tag == MC_ATT_ITEM_DELIMITATION_ITEM )
            {
                // Read the length (which should be zero, but we aren't going
                // to check)
                uint32_t delim_length = 0;
                ret = stream.read_val( delim_length, syntax );
                break;
            }
            // We received something we shouln't have
            else
            {
                ret = MC_UNEXPECTED_EOD;
            }
        }
        else if( ret == MC_END_OF_DATA )
        {
            // No more data when trying to read the start of a tag.
            // terminate gracefully
            graceful_end_of_data = true;
        }
    }

    // Upon success, copy the values into the dictionary
    if( ret == MC_NORMAL_COMPLETION ||
        (ret == MC_END_OF_DATA && graceful_end_of_data == true) )
    {
        dict.insert( move( tmp_value_dict ) );
        ret = MC_NORMAL_COMPLETION;
    }

    return ret;
}

MC_STATUS read_values( rx_stream&       stream,
                       TRANSFER_SYNTAX  syntax,
                       data_dictionary& dict,
                       int              app_id )
{
    MC_STATUS ret = MC_NORMAL_COMPLETION;
    bool graceful_end_of_data = false;

    value_dict tmp_value_dict;

    while( ret == MC_NORMAL_COMPLETION )
    {
        uint32_t tag = 0;
        ret = stream.read_tag( tag, syntax );
        if( ret == MC_NORMAL_COMPLETION )
        {
            if( tag != MC_ATT_ITEM_DELIMITATION_ITEM     &&
                tag != MC_ATT_ITEM                       &&
                tag != MC_ATT_SEQUENCE_DELIMITATION_ITEM )
            {
                unique_vr_ptr element;
                ret = create_vr_from_stream( stream, syntax, tag, element );
                if( ret == MC_NORMAL_COMPLETION && element != nullptr )
                {
                    ret = element->from_stream( stream, syntax );
                    if( ret == MC_NORMAL_COMPLETION )
                    {
                        tmp_value_dict[tag].swap( element );
                    }
                    else
                    {
                        // Do nothing. Will return error
                    }
                }
                else if( element == nullptr )
                {
                    ret = MC_INVALID_TAG;
                }
                else
                {
                    // Do nothing. Will return error
                }
            }
            else
            {
                // We're not expecting to get delimiter tags
                ret = MC_INVALID_TAG;
            }
        }
        else if( ret == MC_END_OF_DATA )
        {
            // No more data when trying to read the start of a tag.
            // terminate gracefully
            graceful_end_of_data = true;
        }
    }

    // Upon success, copy the values into the dictionary
    if( ret == MC_NORMAL_COMPLETION ||
        (ret == MC_END_OF_DATA && graceful_end_of_data == true) )
    {
        dict.insert( move( tmp_value_dict ) );
        ret = MC_NORMAL_COMPLETION;
    }

    return ret;
}

MC_STATUS read_values_upto( rx_stream&       stream,
                            TRANSFER_SYNTAX  syntax,
                            data_dictionary& dict,
                            int              app_id,
                            uint32_t         end_tag )
{
    MC_STATUS ret = MC_NORMAL_COMPLETION;
    bool graceful_end_of_data = false;

    value_dict tmp_value_dict;

    while( MC_NORMAL_COMPLETION == ret )
    {
        uint32_t tag = 0;
        ret = stream.peek_tag( tag, syntax );
        if( ret == MC_NORMAL_COMPLETION )
        {
            if( tag != MC_ATT_ITEM_DELIMITATION_ITEM     &&
                tag != MC_ATT_ITEM                       &&
                tag != MC_ATT_SEQUENCE_DELIMITATION_ITEM )
            {
                if( tag <= end_tag )
                {
                    // This shouldn't fail if the peek succeeded
                    (void)stream.read_tag( tag, syntax );

                    unique_vr_ptr element;
                    ret = create_vr_from_stream( stream, syntax, tag, element );
                    if( ret == MC_NORMAL_COMPLETION && element != nullptr )
                    {
                        ret = element->from_stream( stream, syntax );
                        if( ret == MC_NORMAL_COMPLETION )
                        {
                            tmp_value_dict[tag].swap( element );
                        }
                        else
                        {
                            // Do nothing. Will return error
                        }
                    }
                    else if( element == nullptr )
                    {
                        ret = MC_INVALID_TAG;
                    }
                    else
                    {
                        // Do nothing. Will return error
                    }
                }
                else
                {
                    graceful_end_of_data = true;
                    ret = MC_END_OF_DATA;
                }
            }
            else
            {
                // We're not expecting to get delimiter tags
                ret = MC_INVALID_TAG;
            }
        }
        else if( ret == MC_END_OF_DATA )
        {
            // No more data when trying to read the start of a tag.
            // terminate gracefully
            graceful_end_of_data = true;
        }
        else
        {
            // Do nothing. Will return error
        }
    }

    // Upon success, copy the values into the dictionary
    if( ret == MC_NORMAL_COMPLETION ||
        (ret == MC_END_OF_DATA && graceful_end_of_data == true) )
    {
        dict.insert( move( tmp_value_dict ) );
        ret = MC_NORMAL_COMPLETION;
    }

    return ret;
}

MC_STATUS create_vr_from_stream( rx_stream&      stream,
                                 TRANSFER_SYNTAX syntax,
                                 uint32_t        tag,
                                 unique_vr_ptr&  element )
{
    MC_STATUS ret = MC_NORMAL_COMPLETION;

    if( syntax != IMPLICIT_LITTLE_ENDIAN )
    {
        MC_VR vr = UNKNOWN_VR;
        ret = stream.read_vr( vr, syntax );
        if( ret == MC_NORMAL_COMPLETION )
        {
            element = create_vr( vr, 1, 1, 1 );
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            // Do nothing. Will return error
        }
    }
    else
    {
        assert( g_context != nullptr );
        element = g_context->create_vr( tag, nullptr );
        ret = MC_NORMAL_COMPLETION;
    }

    return ret;
}


}