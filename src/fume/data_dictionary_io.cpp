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

// local private
#include "fume/data_dictionary_io.h"
#include "fume/data_dictionary_search.h"
#include "fume/data_dictionary.h"
#include "fume/application.h"
#include "fume/tx_stream.h"
#include "fume/library_context.h"
#include "fume/source_callback_io.h"

namespace fume
{

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
    MC_STATUS ret = MC_NORMAL_COMPLETION;

    // should never happen
    assert( g_context != nullptr );

    const application* const app = g_context->get_application( app_id );
    const dictionary_value_range range = get_value_range( dict,
                                                          start_tag,
                                                          end_tag );

    for( const value_dict_item& item : range )
    {
        callback_parms_t callback =
            app != nullptr ? app->get_callback_function( item.first ) :
                             callback_parms_t( nullptr, nullptr );

        // Write an element if there is a Callback Function registered
        // for this tag or if there is a value written to it
        if( callback.first != nullptr || item.second != nullptr )
        {
            // Write the tag number
            ret = stream.write_tag( item.first, syntax );
            if( ret == MC_NORMAL_COMPLETION )
            {
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
                                                               app_id,
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
                else if( item.second != nullptr )
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
                else
                {
                    // Should be impossible
                    assert( false );
                }
            }
            else
            {
                // Do nothing. Will return error from write_tag
            }
        }
        else
        {
            // a NULL value_representation pointer is defined as "not present
            // in the message" provided there also is not a Callback Function
            // registered for that tag. So do nothing
        }
    }

    return ret;
}



}