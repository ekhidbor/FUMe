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

MCEXPORT MC_STATUS MC_Validate_File( int       FileID,
                                     VAL_ERR** ErrorInfo,
                                     VAL_LEVEL ErrorLevel )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        // TODO: implement
        ret = MC_MESSAGE_VALIDATES;
    }
    catch( ... )
    {
        ret = MC_SYSTEM_ERROR;
    }

    return ret;
}
