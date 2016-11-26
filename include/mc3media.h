#ifndef MC3MEDIA_H
#define MC3MEDIA_H
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
 * mc3media.h file through doxygen and reviewing the documented names and
 * values.
 */

#include "mcstatus.h"
#include "mc3msg.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{ 
    MC_REC_TYPE_PATIENT, 
    MC_REC_TYPE_STUDY, 
    MC_REC_TYPE_SERIES, 
    MC_REC_TYPE_IMAGE, 
    MC_REC_TYPE_RT_DOSE, 
    MC_REC_TYPE_RT_STRUCTURE_SET, 
    MC_REC_TYPE_RT_PLAN, 
    MC_REC_TYPE_RT_TREAT_RECORD, 
    MC_REC_TYPE_PRESENTATION, 
    MC_REC_TYPE_WAVEFORM, 
    MC_REC_TYPE_SR_DOCUMENT, 
    MC_REC_TYPE_KEY_OBJECT_DOC, 
    MC_REC_TYPE_SPECTROSCOPY, 
    MC_REC_TYPE_RAW_DATA, 
    MC_REC_TYPE_REGISTRATION, 
    MC_REC_TYPE_FIDUCIAL, 
    MC_REC_TYPE_HANGING_PROTOCOL, 
    MC_REC_TYPE_ENCAP_DOC, 
    MC_REC_TYPE_HL7_STRUC_DOC, 
    MC_REC_TYPE_VALUE_MAP, 
    MC_REC_TYPE_STEREOMETRIC, 
    MC_REC_TYPE_MEASUREMENT, 
    MC_REC_TYPE_SURFACE, 
    MC_REC_TYPE_SURFACE_SCAN, 
    MC_REC_TYPE_PALETTE, 
    MC_REC_TYPE_IMPLANT, 
    MC_REC_TYPE_IMPLANT_ASSY, 
    MC_REC_TYPE_IMPLANT_GROUP, 
    MC_REC_TYPE_PRIVATE,
    MC_REC_TYPE_TRACT,
    MC_REC_TYPE_ASSESSMENT, 
    MC_REC_TYPE_UNKNOWN
} MC_DIR_RECORD_TYPE;

typedef enum
{
    MC_TS_CONTINUE, 
    MC_TS_STOP_LEVEL, 
    MC_TS_STOP_LOWER, 
    MC_TS_STOP, 
    MC_TS_ERROR 
} MC_TRAVERSAL_STATUS;

typedef MC_STATUS (*WriteFileCallback)
(
    char* Cbfilename,
    void* CbuserInfo,
    int   CbdataSize,
    void* CbdataBuffer,
    int   CbisFirst,
    int   CbisLast
);

typedef MC_STATUS (*ReadFileCallback)
(
    char*  Cbfilename,
    void*  CbuserInfo,
    int*   CbdataSizePtr,
    void** CbdataBufferPtr,
    int    CbisFirst,
    int*   CbisLastPtr
);

typedef MC_TRAVERSAL_STATUS (*DDHTraverseCallback)
(
    int   CurrentRecID,
    void* UserData
);

MCEXPORT MC_STATUS MC_Create_File( int*        FileIDPtr,
                                   const char* Filename,
                                   const char* ServiceName,
                                   MC_COMMAND  command );

MCEXPORT MC_STATUS MC_Create_Empty_File( int* FileIDPtr, const char* Filename );

MCEXPORT MC_STATUS MC_Free_File( int* FileID );

MCEXPORT MC_STATUS MC_Set_File_Preamble( int FileID, const char* Preamble );

MCEXPORT MC_STATUS MC_Empty_File( int FileID );

MCEXPORT MC_STATUS MC_Open_File( int              ApplicationID,
                                 int              FileID,
                                 void*            UserInfo,
                                 ReadFileCallback YourFromMediaFunction );

MCEXPORT MC_STATUS MC_Open_File_Upto_Tag( int              ApplicationID,
                                          int              FileID,
                                          void*            UserInfo,
                                          unsigned long    Tag,
                                          long*            Offset,
                                          ReadFileCallback YourFromMediaFunction );

MCEXPORT MC_STATUS MC_Write_File( int               FileID,
                                  int               NumBytes,
                                  void*             UserInfo,
                                  WriteFileCallback YourToMediaFunction );

MCEXPORT MC_STATUS MC_Write_File_By_Callback( int               ApplicationID,
                                              int               FileID,
                                              int               NumBytes,
                                              void*             UserInfo,
                                              WriteFileCallback YourToMediaFunction );

MCEXPORT MC_STATUS MC_Validate_File( int       FileID,
                                     VAL_ERR** ErrorInfo,
                                     VAL_LEVEL ErrorLevel );

MCEXPORT MC_STATUS MC_Reset_Filename( int FileID, const char* NewFilename );

MCEXPORT MC_STATUS MC_Get_Filename( int FileID, char* FileName, int FileSize );

MCEXPORT MC_STATUS MC_DDH_Create( const char* FilePath,
                                  const char* FileSetID,
                                  int         TemplateFileID,
                                  int*        DirMsgIDPtr );

MCEXPORT MC_STATUS MC_DDH_Open( const char* FilePath, int* DirMsgIDPtr );

MCEXPORT MC_STATUS MC_DDH_Update( int DirMsgID );

MCEXPORT MC_STATUS MC_DDH_Traverse_Records( int                 RootID,
                                            void*               UserData,
                                            DDHTraverseCallback YourTraverseCallback );

MCEXPORT MC_STATUS MC_DDH_Get_Record_Type( int                 RecordID,
                                           MC_DIR_RECORD_TYPE* RecType );

MCEXPORT MC_STATUS MC_DDH_Get_Parent_Record( int RecordID, int* ParentID );

MCEXPORT MC_STATUS MC_DDH_Get_Next_Record( int RecordID, int* NextID );

MCEXPORT MC_STATUS MC_DDH_Get_First_Lower_Record( int ParentID, int* LowerID );

MCEXPORT MC_STATUS MC_DDH_Copy_Values( int            SourceID,
                                       int            DestID,
                                       unsigned long* TagList );

MCEXPORT MC_STATUS MC_DDH_Add_Record( int         ParentID,
                                      const char* RecordType,
                                      int*        RecordID );

MCEXPORT MC_STATUS MC_DDH_Delete_Record( int RecordID );

MCEXPORT MC_STATUS MC_DDH_Release_Record( int RecordID );

#ifdef __cplusplus
}
#endif

#endif
