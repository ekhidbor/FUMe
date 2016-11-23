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
#include "mc3msg.h"
#include "diction.h"

// local private
#include "fume/tag_to_vr.h"
#include "fume/data_dictionary.h"
#include "fume/value_representation.h"

namespace fume
{

static bool get_conditional_pixel_data_vr( data_dictionary& dict,
                                           MC_VR&           tag_vr )
{
    bool ret = false;

    TRANSFER_SYNTAX syntax = INVALID_TRANSFER_SYNTAX;
    if( dict.get_transfer_syntax( syntax ) == MC_NORMAL_COMPLETION )
    {
        if( syntax == IMPLICIT_LITTLE_ENDIAN )
        {
            // Implicit little endian is always OW
            tag_vr = OW;
            ret = true;
        }
        else if( syntax == EXPLICIT_LITTLE_ENDIAN ||
                 syntax == DEFLATED_EXPLICIT_LITTLE_ENDIAN )
        {
            int bits_allocated = 0;
            value_representation* vr = dict.at( MC_ATT_BITS_ALLOCATED );
            if( vr != nullptr &&
                vr->get( bits_allocated ) == MC_NORMAL_COMPLETION )
            {
                // OB if data is a byte or less; OW otherwise
                tag_vr = bits_allocated <= 8 ? OB : OW;
                ret = true;
            }
            else
            {
                // Could not read value from dictionary. Use default VR
                ret = false;
            }
        }
        else
        {
            // All transfer syntaxes except implicit little endian,
            // explicit little endian and deflated explicit little
            // endian are "encapsulated" syntaxes which use VR OB
            tag_vr = OB;
            ret = true;
        }
    }
    else
    {
        ret = false;
    }

    return ret;
}

static bool get_conditional_waveform_data_vr( data_dictionary& dict,
                                              MC_VR&           tag_vr )
{
    bool ret = false;

    TRANSFER_SYNTAX syntax = INVALID_TRANSFER_SYNTAX;
    if( dict.get_transfer_syntax( syntax ) == MC_NORMAL_COMPLETION )
    {
        if( syntax == IMPLICIT_LITTLE_ENDIAN )
        {
            // Implicit little endian is always OW
            tag_vr = OW;
            ret = true;
        }
        else
        {
            int bits_allocated = 0;
            value_representation* vr =
                dict.at( MC_ATT_WAVEFORM_BITS_ALLOCATED );
            if( vr != nullptr &&
                vr->get( bits_allocated ) == MC_NORMAL_COMPLETION )
            {
                // OB if data is a byte or less; OW otherwise
                tag_vr = bits_allocated <= 8 ? OB : OW;
                ret = true;
            }
            else
            {
                // Could not read value from dictionary. Use default VR
                ret = false;
            }
        }
    }
    else
    {
        ret = false;
    }

    return ret;
}

static bool get_conditional_dark_current_vr( data_dictionary& dict,
                                             MC_VR&           tag_vr )
{
    bool ret = false;

    TRANSFER_SYNTAX syntax = INVALID_TRANSFER_SYNTAX;
    if( dict.get_transfer_syntax( syntax ) == MC_NORMAL_COMPLETION )
    {
        if( syntax == IMPLICIT_LITTLE_ENDIAN )
        {
            // Implicit little endian is always OW
            tag_vr = OW;
            ret = true;
        }
        else
        {
            int bits_allocated = 0;
            value_representation* vr = dict.at( MC_ATT_BITS_ALLOCATED );
            if( vr != nullptr &&
                vr->get( bits_allocated ) == MC_NORMAL_COMPLETION )
            {
                // OB if data is a byte or less; OW otherwise
                tag_vr = bits_allocated <= 8 ? OB : OW;
                ret = true;
            }
            else
            {
                // Could not read value from dictionary. Use default VR
                ret = false;
            }
        }
    }
    else
    {
        ret = false;
    }

    return ret;
}

static bool get_conditional_overlay_vr( data_dictionary& dict,
                                        MC_VR&           tag_vr )
{
    bool ret = false;

    TRANSFER_SYNTAX syntax = INVALID_TRANSFER_SYNTAX;
    if( dict.get_transfer_syntax( syntax ) == MC_NORMAL_COMPLETION )
    {
        tag_vr = syntax == IMPLICIT_LITTLE_ENDIAN ? OW : OB;
        ret = true;
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool get_conditional_tag_vr( uint32_t         tag,
                             data_dictionary& dict,
                             MC_VR&           tag_vr )
{
    bool ret = false;

    if( tag == MC_ATT_PIXEL_DATA )
    {
        ret = get_conditional_pixel_data_vr( dict, tag_vr );
    }
    else if( tag                == MC_ATT_WAVEFORM_PADDING_VALUE ||
             tag                == MC_ATT_WAVEFORM_DATA          ||
             tag                == MC_ATT_CHANNEL_MINIMUM_VALUE  ||
             tag                == MC_ATT_CHANNEL_MAXIMUM_VALUE  ||
             (tag & 0xFF00FFFF) == MC_ATT_AUDIO_SAMPLE_DATA_RETIRED )
    {
        ret = get_conditional_waveform_data_vr( dict, tag_vr );
    }
    else if( tag == MC_ATT_DARK_CURRENT_COUNTS )
    {
        ret = get_conditional_dark_current_vr( dict, tag_vr );
    }
    else if( (tag & 0xFF00FFFF) == MC_ATT_OVERLAY_DATA )
    {
        ret = get_conditional_overlay_vr( dict, tag_vr );
    }
    else
    {
        ret = false;
    }

    return ret;
}

} // namespace fume
