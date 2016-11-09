#ifndef MERGECOM_H
#define MERGECOM_H
/**
 * This file is a part of the FUMe project.
 *
 * To the extent possible under law, the person who associated CC0 with
 * FUMe has waived all copyright and related or neighboring rights
 * to FUMe.
 *
 * You should have received a copy of the CC0 legalcode along with this
 * work.  If not, see http://creativecommons.org/publicdomain/zero/1.0/.
 *
 * Function declarations in this file were produced from publically available
 * documentation. Enumerated values were determined by passing an original
 * mergecom.h file through doxygen and reviewing the documented names and
 * values.
 */

// std

// local
#include "mcstatus.h"
#include "mc3msg.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef MC_STATUS (*CallbackFunction)
(
    int            CBmessageID,
    unsigned long  CBtag,
    void*          CBuserInfo,
    CALLBACK_TYPE  CBtype,
    unsigned long* CBdataSizePtr,
    void**         CBdataBufferPtr,
    int            CBisFirst,
    int*           CBisLastPtr
);

typedef MC_STATUS (*pCallbackFunction)
(
    int            CBmessageID,
    char*          CBprivateCode,
    unsigned short CBgroup,
    unsigned char  CBelementByte,
    void*          CBuserInfo,
    CALLBACK_TYPE  CBtype,
    unsigned long* CBdataSizePtr,
    void**         CBdataBufferPtr,
    int            CBisFirst,
    int*           CBisLastPtr
);

MCEXPORT MC_STATUS MC_Register_Application( int* ApplicationID, char* AEtitle );

MCEXPORT MC_STATUS MC_Release_Application( int* ApplicationID );

MCEXPORT MC_STATUS MC_Register_Callback_Function ( int              ApplicationID,
                                                   unsigned long    Tag,
                                                   void*            UserInfo,
                                                   CallbackFunction Callback );

MCEXPORT MC_STATUS MC_Release_Callback_Function ( int           ApplicationID,
                                                  unsigned long Tag );

MCEXPORT MC_STATUS MC_Set_Message_Transfer_Syntax( int             MessageID,
                                                   TRANSFER_SYNTAX TransferSyntax );

MCEXPORT MC_STATUS MC_Get_Message_Transfer_Syntax( int              MessageID,
                                                   TRANSFER_SYNTAX* TransferSyntax );

MCEXPORT MC_STATUS MC_Open_Association( int         ApplicationID,
                                        int*        AssociationID,
                                        const char* RemoteApplicationTitle,
                                        int*        RemoteHostPortNumber,
                                        char*       RemoteHostTCPIPName,
                                        char*       ServiceList );

MCEXPORT MC_STATUS MC_Send_Request_Message( int AssociationID, int MessageID );

MCEXPORT MC_STATUS MC_Read_Message( int         AssociationID,
                                    int         Timeout,
                                    int*        MessageID,
                                    char**      ServiceName,
                                    MC_COMMAND* Command );

MCEXPORT MC_STATUS MC_Close_Association( int* AssociationID );

MC_STATUS MC_Cleanup_Memory( int Timeout );

#ifdef __cplusplus
}
#endif

#endif
