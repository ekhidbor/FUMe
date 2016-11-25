#ifndef MC3MSG_H
#define MC3MSG_H
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

// std
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

// local
#include "mcstatus.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef uint16_t  MC_COMMAND;
typedef uint16_t  MC_UChar;

#ifdef _WIN64
typedef size_t MC_size_t;
#else
typedef int MC_size_t;
#endif

// MC_COMMAND
#define C_CANCEL_RQ        ((MC_COMMAND)0x0FFF)
#define C_STORE_RQ         ((MC_COMMAND)0x0001)
#define C_STORE_RSP        ((MC_COMMAND)0x8001)
#define C_GET_RQ           ((MC_COMMAND)0x0010)
#define C_GET_RSP          ((MC_COMMAND)0x8010)
#define C_CANCEL_GET_RQ    C_CANCEL_RQ
#define C_FIND_RQ          ((MC_COMMAND)0x0020)
#define C_FIND_RSP         ((MC_COMMAND)0x8020)
#define C_CANCEL_FIND_RQ   C_CANCEL_RQ
#define C_MOVE_RQ          ((MC_COMMAND)0x0021)
#define C_MOVE_RSP         ((MC_COMMAND)0x8021)
#define C_CANCEL_MOVE_RQ   C_CANCEL_RQ
#define C_ECHO_RQ          ((MC_COMMAND)0x0030)
#define C_ECHO_RSP         ((MC_COMMAND)0x8030)
#define N_EVENT_REPORT_RQ  ((MC_COMMAND)0x0100)
#define N_EVENT_REPORT_RSP ((MC_COMMAND)0x8100)
#define N_GET_RQ           ((MC_COMMAND)0x0110)
#define N_GET_RSP          ((MC_COMMAND)0x8110)
#define N_SET_RQ           ((MC_COMMAND)0x0120)
#define N_SET_RSP          ((MC_COMMAND)0x8120)
#define N_ACTION_RQ        ((MC_COMMAND)0x0130)
#define N_ACTION_RSP       ((MC_COMMAND)0x8130)
#define N_CREATE_RQ        ((MC_COMMAND)0x0140)
#define N_CREATE_RSP       ((MC_COMMAND)0x8140)
#define N_DELETE_RQ        ((MC_COMMAND)0x0150)
#define N_DELETE_RSP       ((MC_COMMAND)0x8150)
#define INVALID_COMMAND    ((MC_COMMAND)0xFFFF)

typedef enum
{ 
    AE = 00,
    AS,
    CS,
    DA,
    DS,
    DT,
    IS,
    LO,
    LT,
    PN,
    SH,
    ST,
    TM,
    UC,
    UR,
    UT,
    UI,
    SS,
    US,
    AT,
    SL,
    UL,
    FL,
    FD,
    UNKNOWN_VR,
    OB,
    OW,
    OD,
    OF,
    SQ,
    OL
} MC_VR;

typedef enum
{
    MC_ERROR_MESSAGE = 1,
    MC_WARNING_MESSAGE,
    MC_INFO_MESSAGE,
    MC_TRACE_MESSAGE,
    MC_T1_MESSAGE,
    MC_T2_MESSAGE,
    MC_T3_MESSAGE,
    MC_T4_MESSAGE,
    MC_T5_MESSAGE,
    MC_T6_MESSAGE,
    MC_T7_MESSAGE,
    MC_T8_MESSAGE,
    MC_T9_MESSAGE,
    MC_OTHER_MESSAGE
} MsgLogType;

typedef enum
{ 
    String_Type = 0,
    Int_Type,
    UInt_Type,
    ShortInt_Type,
    UShortInt_Type,
    LongInt_Type,
    ULongInt_Type,
    Float_Type,
    Double_Type,
    Buffer_Type
} MC_DT;

typedef enum
{ 
    INVALID_TRANSFER_SYNTAX = 0,
    IMPLICIT_LITTLE_ENDIAN = 100,
    EXPLICIT_LITTLE_ENDIAN,
    EXPLICIT_BIG_ENDIAN,
    IMPLICIT_BIG_ENDIAN,
    DEFLATED_EXPLICIT_LITTLE_ENDIAN,
    RLE,
    JPEG_BASELINE,
    JPEG_EXTENDED_2_4,
    JPEG_EXTENDED_3_5,
    JPEG_SPEC_NON_HIER_6_8,
    JPEG_SPEC_NON_HIER_7_9,
    JPEG_FULL_PROG_NON_HIER_10_12,
    JPEG_FULL_PROG_NON_HIER_11_13,
    JPEG_LOSSLESS_NON_HIER_14,
    JPEG_LOSSLESS_NON_HIER_15,
    JPEG_EXTENDED_HIER_16_18,
    JPEG_EXTENDED_HIER_17_19,
    JPEG_SPEC_HIER_20_22,
    JPEG_SPEC_HIER_21_23,
    JPEG_FULL_PROG_HIER_24_26,
    JPEG_FULL_PROG_HIER_25_27,
    JPEG_LOSSLESS_HIER_28,
    JPEG_LOSSLESS_HIER_29,
    JPEG_LOSSLESS_HIER_14,
    JPEG_2000_LOSSLESS_ONLY,
    JPEG_2000,
    JPEG_LS_LOSSLESS,
    JPEG_LS_LOSSY,
    MPEG2_MPML,
    PRIVATE_SYNTAX_1,
    PRIVATE_SYNTAX_2,
    JPEG_2000_MC_LOSSLESS_ONLY,
    JPEG_2000_MC,
    MPEG2_MPHL,
    MPEG4_AVC_H264_HP_LEVEL_4_1,
    MPEG4_AVC_H264_BDC_HP_LEVEL_4_1,
    JPIP_REFERENCED,
    JPIP_REFERENCED_DEFLATE
} TRANSFER_SYNTAX;

typedef enum 
{
    Validation_Level1 = 1,
    Validation_Level2 = 2,
    Validation_Level3 = 3
} VAL_LEVEL;

typedef enum
{ 
    XmlOptDefault                  = 0x0,
    XmlOptIncludeBulks             = 0x1,
    XmlOptIncludePixelData         = 0x2,
    XmlOptExcludeSequences         = 0x4,
    XmlOptExcludePrivateAttributes = 0x8,
    XmlOptBase64Binary             = 0x10
} XML_OPTIONS;

typedef enum
{ 
    REQUEST_FOR_DATA = 300,
    REQUEST_FOR_DATA_LENGTH,
    PROVIDING_DATA,
    PROVIDING_DATA_LENGTH,
    PROVIDING_MEDIA_DATA_LENGTH,
    PROVIDING_OFFSET_TABLE,
    FREE_DATA,
    REQUEST_FOR_DATA_WITH_OFFSET
} CALLBACK_TYPE;

typedef enum
{ 
    ACCEPT_LIST_OF_APPLICATION_TITLES,
    CAPTURE_FILE,
    COMPRESSION_RGB_TRANSFORM_FORMAT,
    DECODER_TAG_FILTER,
    DEFLATED_EXPLICIT_LITTLE_ENDIAN_SYNTAX,
    DICTIONARY_ACCESS,
    DICTIONARY_FILE,
    EXPLICIT_BIG_ENDIAN_SYNTAX,
    EXPLICIT_LITTLE_ENDIAN_SYNTAX,
    IMPLEMENTATION_CLASS_UID,
    IMPLEMENTATION_VERSION,
    IMPLICIT_BIG_ENDIAN_SYNTAX,
    IMPLICIT_LITTLE_ENDIAN_SYNTAX,
    INITIATOR_NAME,
    IP_TYPE,
    JPEG_2000_LOSSLESS_ONLY_SYNTAX,
    JPEG_2000_MC_LOSSLESS_ONLY_SYNTAX,
    JPEG_2000_MC_SYNTAX,
    JPEG_2000_SYNTAX,
    JPEG_BASELINE_SYNTAX,
    JPEG_EXTENDED_2_4_SYNTAX,
    JPEG_EXTENDED_3_5_SYNTAX,
    JPEG_EXTENDED_HIER_16_18_SYNTAX,
    JPEG_EXTENDED_HIER_17_19_SYNTAX,
    JPEG_FULL_PROG_HIER_24_26_SYNTAX,
    JPEG_FULL_PROG_HIER_25_27_SYNTAX,
    JPEG_FULL_PROG_NON_HIER_10_12_SYNTAX,
    JPEG_FULL_PROG_NON_HIER_11_13_SYNTAX,
    JPEG_LOSSLESS_HIER_14_SYNTAX,
    JPEG_LOSSLESS_HIER_28_SYNTAX,
    JPEG_LOSSLESS_HIER_29_SYNTAX,
    JPEG_LOSSLESS_NON_HIER_14_SYNTAX,
    JPEG_LOSSLESS_NON_HIER_15_SYNTAX,
    JPEG_LS_LOSSLESS_SYNTAX,
    JPEG_LS_LOSSY_SYNTAX,
    JPEG_SPEC_HIER_20_22_SYNTAX,
    JPEG_SPEC_HIER_21_23_SYNTAX,
    JPEG_SPEC_NON_HIER_6_8_SYNTAX,
    JPEG_SPEC_NON_HIER_7_9_SYNTAX,
    JPIP_REFERENCED_DEFLATE_SYNTAX,
    JPIP_REFERENCED_SYNTAX,
    LARGE_DATA_STORE,
    LICENSE,
    LOCAL_APPL_CONTEXT_NAME,
    LOG_FILE,
    MPEG2_MPHL_SYNTAX,
    MPEG2_MPML_SYNTAX,
    MPEG4_AVC_H264_BDC_HP_LEVEL_4_1_SYNTAX,
    MPEG4_AVC_H264_HP_LEVEL_4_1_SYNTAX,
    MSG_INFO_FILE,
    NULL_TYPE3_VALIDATION,
    PEGASUS_DISP_REG_NAME,
    PEGASUS_DISP_REGISTRATION,
    PEGASUS_OP_D2SEPLUS_NAME,
    PEGASUS_OP_D2SEPLUS_REGISTRATION,
    PEGASUS_OP_J2KE_NAME,
    PEGASUS_OP_J2KE_REGISTRATION,
    PEGASUS_OP_J2KP_NAME,
    PEGASUS_OP_J2KP_REGISTRATION,
    PEGASUS_OP_JLSE_NAME,
    PEGASUS_OP_JLSE_REGISTRATION,
    PEGASUS_OP_JLSP_NAME,
    PEGASUS_OP_JLSP_REGISTRATION,
    PEGASUS_OP_LIE3PLUS_NAME,
    PEGASUS_OP_LIE3PLUS_REGISTRATION,
    PEGASUS_OP_LIP3PLUS_NAME,
    PEGASUS_OP_LIP3PLUS_REGISTRATION,
    PEGASUS_OP_SE2DPLUS_NAME,
    PEGASUS_OP_SE2DPLUS_REGISTRATION,
    PEGASUS_OPCODE_PATH,
    PRIVATE_SYNTAX_1_SYNTAX,
    PRIVATE_SYNTAX_2_SYNTAX,
    RECEIVER_NAME,
    RLE_SYNTAX,
    TEMP_FILE_DIRECTORY,
    UNKNOWN_VR_CODE,
    NUM_STRING_PARAMS
} StringParm;

typedef enum
{ 
    ERROR_DESTINATIONS,
    INFO_DESTINATIONS,
    WARNING_DESTINATIONS,
    T1_DESTINATIONS,
    T2_DESTINATIONS,
    T3_DESTINATIONS,
    T4_DESTINATIONS,
    T5_DESTINATIONS,
    T6_DESTINATIONS,
    T7_DESTINATIONS,
    T8_DESTINATIONS,
    T9_DESTINATIONS,
    NUM_LOG_PARAMS
} LogParm;

typedef enum
{ 
    ARTIM_TIMEOUT,
    ASSOC_REPLY_TIMEOUT,
    COMPRESSION_CHROM_FACTOR,
    COMPRESSION_J2K_LOSSY_QUALITY,
    COMPRESSION_J2K_LOSSY_RATIO,
    COMPRESSION_LUM_FACTOR,
    CONNECT_TIMEOUT,
    DEFLATE_COMPRESSION_LEVEL,
    DESIRED_LAST_PDU_SIZE,
    FLATE_GROW_OUTPUT_BUF_SIZE,
    IGNORE_JPEG_BAD_SUFFIX,
    INACTIVITY_TIMEOUT,
    LARGE_DATA_SIZE,
    LIST_SQ_DEPTH_LIMIT,
    LIST_VALUE_LIMIT,
    LOG_FILE_LINE_LENGTH,
    LOG_FILE_SIZE,
    LOG_MEMORY_SIZE,
    MAX_PENDING_CONNECTIONS,
    NUM_HISTORICAL_LOG_FILES,
    NUMBER_OF_CAP_FILES,
    OBOW_BUFFER_SIZE,
    RELEASE_TIMEOUT,
    TCPIP_LISTEN_PORT,
    TCPIP_RECEIVE_BUFFER_SIZE,
    TCPIP_SEND_BUFFER_SIZE,
    WORK_BUFFER_SIZE,
    WRITE_TIMEOUT,
    NUM_INT_PARAMS
} IntParm;

typedef enum
{ 
    ACCEPT_ANY_APPLICATION_TITLE,
    ACCEPT_ANY_CONTEXT_NAME,
    ACCEPT_ANY_HOSTNAME,
    ACCEPT_ANY_PRESENTATION_CONTEXT,
    ACCEPT_ANY_PROTOCOL_VERSION,
    ACCEPT_DIFFERENT_IC_UID,
    ACCEPT_DIFFERENT_VERSION,
    ACCEPT_MULTIPLE_PRES_CONTEXTS,
    ACCEPT_RELATED_GENERAL_SERVICES,
    ACCEPT_STORAGE_SERVICE_CONTEXTS,
    ALLOW_COMMA_IN_DS_FL_FD_STRINGS,
    ALLOW_INVALID_LENGTH_FOR_VR,
    ALLOW_INVALID_PRIVATE_ATTRIBUTES,
    ALLOW_INVALID_PRIVATE_CREATOR_CODES,
    ALLOW_OUT_OF_ORDER_TAGS,
    ATT_00081190_USE_UT_VR,
    ATT_00287FE0_USE_UT_VR,
    ATT_0040E010_USE_UT_VR,
    ATT_0074100A_USE_ST_VR,
    AUTO_ECHO_SUPPORT,
    BLANK_FILL_LOG_FILE,
    COMBINE_DATA_WITH_HEADER,
    COMPRESSION_ALLOW_FRAGS,
    COMPRESSION_J2K_LOSSY_USE_QUALITY,
    COMPRESSION_J2K_SIGNED_TOLERANCE,
    COMPRESSION_USE_HEADER_QUERY,
    COMPRESSION_WHEN_J2K_USE_LOSSY,
    CREATE_OFFSET_TABLE,
    DEFLATE_ALLOW_FLUSH,
    DICOMDIR_STREAM_STORAGE,
    DUPLICATE_ENCAPSULATED_ICON,
    EFILM_OPEN_FILE,
    ELIMINATE_ITEM_REFERENCES,
    EMPTY_PRIVATE_CREATOR_CODES,
    EXPORT_EMPTY_PRIVATE_CREATOR_CODES,
    EXPORT_GROUP_LENGTHS_TO_MEDIA,
    EXPORT_GROUP_LENGTHS_TO_NETWORK,
    EXPORT_PRIVATE_ATTRIBUTES_TO_MEDIA,
    EXPORT_PRIVATE_ATTRIBUTES_TO_NETWORK,
    EXPORT_UN_VR_TO_MEDIA,
    EXPORT_UN_VR_TO_NETWORK,
    EXPORT_UNDEFINED_LENGTH_SQ,
    EXPORT_UNDEFINED_LENGTH_SQ_IN_DICOMDIR,
    FORCE_DICM_IN_PREFIX,
    FORCE_JAVA_BIG_ENDIAN,
    FORCE_OPEN_EMPTY_ITEM,
    HARD_CLOSE_TCP_IP_CONNECTION,
    INSURE_EVEN_UID_LENGTH,
    LIST_UN_ATTRIBUTES,
    LOG_FILE_BACKUP,
    MSG_FILE_ITEM_OBJ_TRACE,
    NETWORK_CAPTURE,
    PRIVATE_SYNTAX_1_ENCAPSULATED,
    PRIVATE_SYNTAX_1_EXPLICIT_VR,
    PRIVATE_SYNTAX_1_LITTLE_ENDIAN,
    PRIVATE_SYNTAX_2_ENCAPSULATED,
    PRIVATE_SYNTAX_2_EXPLICIT_VR,
    PRIVATE_SYNTAX_2_LITTLE_ENDIAN,
    REJECT_INVALID_VR,
    RELEASE_SQ_ITEMS,
    REMOVE_PADDING_CHARS,
    REMOVE_SINGLE_TRAILING_SPACE,
    RETURN_COMMA_IN_DS_FL_FD_STRINGS,
    REWRITE_CAPTURE_FILES,
    SEND_ECHO_PRIORITY,
    SEND_LENGTH_TO_END,
    SEND_MSG_ID_RESPONSE,
    SEND_RECOGNITION_CODE,
    SEND_RESPONSE_PRIORITY,
    SEND_SOP_CLASS_UID,
    SEND_SOP_INSTANCE_UID,
    TCPIP_DISABLE_NAGLE,
    TOLERATE_INVALID_IN_DEFAULT_CHARSET,
    UPDATE_GROUP_0028_ON_DUPLICATE,
    USE_FREE_DATA_CALLBACK,
    NUM_BOOL_PARAMS
} BoolParm;

typedef enum
{ 
    CALLBACK_MIN_DATA_SIZE,
    CAPTURE_FILE_SIZE,
    PDU_MAXIMUM_LENGTH,
    NUM_LONG_PARAMS
} LongParm;

typedef struct
{
    unsigned long Tag;
    int           MsgItemID;
    int           ValueNumber;
    MC_STATUS     Status;
} VAL_ERR;

typedef MC_STATUS (*SetValueCallback)
(
    int           CBMsgFileItemID,
    unsigned long Cbtag,
    int           CbisFirst,
    void*         CbuserInfo,
    int*          CbdataSizePtr,
    void**        CbdataBufferPtr,
    int*          CbisLastPtr
);

typedef MC_STATUS (*GetValueCallback)
(
    int           CBMsgFileItemID,
    unsigned long CBtag,
    void*         CBuserInfo,
    int           CBdataSize,
    void*         CBdataBuffer,
    int           CBisFirst,
    int           CBisLast
);

typedef void* (*CfgFunction)(void);
typedef void* (*DictionaryFunction)(void);
typedef void* (*FutureFunction)(void);

MCEXPORT MC_STATUS MC_Library_Initialization( CfgFunction        cfg,
                                              DictionaryFunction dict,
                                              FutureFunction     future );

MCEXPORT MC_STATUS MC_Library_Release();

/**
 * Clears all existing values in the message and sets the first one
 */
MCEXPORT MC_STATUS MC_Set_Value( int           MsgFileItemID,
                                 unsigned long Tag,
                                 MC_DT         DataType,
                                 void*         Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Set_Value_From_Double( int           MsgFileItemID,
                                             unsigned long Tag,
                                             double        Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Set_Value_From_Float( int           MsgFileItemID,
                                            unsigned long Tag,
                                            float         Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Set_Value_From_Int( int           MsgFileItemID,
                                          unsigned long Tag,
                                          int           Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Set_Value_From_ShortInt( int           MsgFileItemID,
                                               unsigned long Tag,
                                               short int     Value );

// Valid for the following VRs: AT, DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Set_Value_From_LongInt( int           MsgFileItemID,
                                              unsigned long Tag,
                                              long int      Value );

// Valid for the following VRs: AE, AS, AT, CS, DA, DS, DT, FD, FL, IS,
//                              LO, LT, PN, SH, SL, SS, ST, TM, UI, UL,
//                              UR, US, UT, SQ
MCEXPORT MC_STATUS MC_Set_Value_From_String( int           MsgFileItemID,
                                             unsigned long Tag,
                                             const char*   Value );

// Valid for the following VRs: LO, LT, PN, SH, ST, UT
MCEXPORT MC_STATUS MC_Set_Value_From_UnicodeString( int             MsgFileItemID,
                                                    unsigned long   Tag,
                                                    const MC_UChar* Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Set_Value_From_UInt( int           MsgFileItemID,
                                           unsigned long Tag,
                                           unsigned int  Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Set_Value_From_UShortInt( int            MsgFileItemID,
                                                unsigned long  Tag,
                                                unsigned short Value );

// Valid for the following VRs: AT, DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Set_Value_From_ULongInt( int           MsgFileItemID,
                                               unsigned long Tag,
                                               unsigned long Value );

// Only valid for data elements with an unknown VR
MCEXPORT MC_STATUS MC_Set_Value_From_Buffer( int           MsgFileItemID,
                                             unsigned long Tag,
                                             void*         Value,
                                             unsigned long ValueLength );

MCEXPORT MC_STATUS MC_Set_Next_Value( int           MsgFileItemID,
                                      unsigned long Tag,
                                      MC_DT         DataType,
                                      void*         Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Set_Next_Value_From_Double( int           MsgFileItemID,
                                                  unsigned long Tag,
                                                  double        Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Set_Next_Value_From_Float( int           MsgFileItemID,
                                                 unsigned long Tag,
                                                 float         Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Set_Next_Value_From_Int( int           MsgFileItemID,
                                               unsigned long Tag,
                                               int           Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Set_Next_Value_From_ShortInt( int           MsgFileItemID,
                                                    unsigned long Tag,
                                                    short int     Value );

// Valid for the following VRs: AT, DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Set_Next_Value_From_LongInt( int           MsgFileItemID,
                                                   unsigned long Tag,
                                                   long int      Value );

// Valid for the following VRs: AE, AS, AT, CS, DA, DS, DT, FD, FL, IS,
//                              LO, LT, PN, SH, SL, SS, ST, TM, UI, UL,
//                              UR, US, UT, SQ
MCEXPORT MC_STATUS MC_Set_Next_Value_From_String( int           MsgFileItemID,
                                                  unsigned long Tag,
                                                  const char*   Value );

// Valid for the following VRs: LO, LT, PN, SH, ST, UT
MCEXPORT MC_STATUS MC_Set_Next_Value_From_UnicodeString( int             MsgFileItemID,
                                                         unsigned long   Tag,
                                                         const MC_UChar* Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Set_Next_Value_From_UInt( int           MsgFileItemID,
                                                unsigned long Tag,
                                                unsigned int  Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Set_Next_Value_From_UShortInt( int            MsgFileItemID,
                                                     unsigned long  Tag,
                                                     unsigned short Value );

// Valid for the following VRs: AT, DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Set_Next_Value_From_ULongInt( int           MsgFileItemID,
                                                    unsigned long Tag,
                                                    unsigned long Value );

// Only works with unknown VRs
MCEXPORT MC_STATUS MC_Set_Next_Value_From_Buffer( int           MsgFileItemID,
                                                  unsigned long Tag,
                                                  void*         Value,
                                                  unsigned long ValueLength );

// Valid for the following VRs: OB, OW, OD, OF, UR, UT
MCEXPORT MC_STATUS MC_Set_Value_From_Function( int              MsgFileItemID,
                                               unsigned long    Tag,
                                               void*            UserInfo,
                                               SetValueCallback YourSetFunction );

// Sets the value representation object for the specified Tag to NULL
MCEXPORT MC_STATUS MC_Set_Value_To_Empty( int MsgFileItemID, unsigned long Tag );

// Sets the value to have zero length
MCEXPORT MC_STATUS MC_Set_Value_To_NULL( int MsgFileItemID, unsigned long Tag );

// Can only be used on text VRs which allow a "\" delimiter
MCEXPORT MC_STATUS MC_Set_Next_Value_To_NULL( int MsgFileItemID, unsigned long Tag );

MCEXPORT MC_STATUS MC_Set_Encapsulated_Value_From_Function
(
    int              MsgFileItemID,
    unsigned long    Tag,
    void*            UserInfo,
    SetValueCallback YourSetFunction
);

MCEXPORT MC_STATUS MC_Set_Next_Encapsulated_Value_From_Function
(
    int              MsgFileItemID,
    unsigned long    Tag,
    void*            UserInfo,
    SetValueCallback YourSetFunction
);

MCEXPORT MC_STATUS MC_Close_Encapsulated_Value( int           MsgFileItemID,
                                                unsigned long Tag );

// Deletes the last value returned by MC_Get_Value or MC_Get_Next_Value
MCEXPORT MC_STATUS MC_Delete_Current_Value( int MsgFileItemID, unsigned long Tag );

MCEXPORT MC_STATUS MC_Get_Value( int           MsgFileItemID,
                                 unsigned long Tag,
                                 MC_DT         DataType,
                                 MC_size_t     BufferSize,
                                 void*         Value,
                                 int*          ValueSize );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Get_Value_To_Double( int           MsgFileItemID,
                                           unsigned long Tag,
                                           double*       Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Get_Value_To_Float( int           MsgFileItemID,
                                          unsigned long Tag,
                                          float*        Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Get_Value_To_Int( int           MsgFileItemID,
                                        unsigned long Tag,
                                        int*          Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Get_Value_To_ShortInt( int           MsgFileItemID,
                                             unsigned long Tag,
                                             short int*    Value );

// Valid for the following VRs: AT, DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Get_Value_To_LongInt( int           MsgFileItemID,
                                            unsigned long Tag,
                                            long int*     Value );

// Valid for the following VRs: AS, AT, CS, DA, DS, DT, FD, FL, IS, LO, LT,
//                              PN, SH, SL, SS, ST, TM, UI, UL, US, SQ
MCEXPORT MC_STATUS MC_Get_Value_To_String( int           MsgFileItemID,
                                           unsigned long Tag,
                                           MC_size_t     BufferSize,
                                           char*         Value );

// Valid for the following VRs: LO, LT, PN, SH, ST, UT
MCEXPORT MC_STATUS MC_Get_Value_To_UnicodeString( int           MsgFileItemID,
                                                  unsigned long Tag,
                                                  MC_size_t     BufferSize,
                                                  int*          ValueSize,
                                                  MC_UChar*     Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Get_Value_To_UInt( int           MsgFileItemID,
                                         unsigned long Tag,
                                         unsigned int* Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Get_Value_To_UShortInt( int             MsgFileItemID,
                                              unsigned long   Tag,
                                              unsigned short* Value );

// Valid for the following VRs: AT, DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Get_Value_To_ULongInt( int            MsgFileItemID,
                                             unsigned long  Tag,
                                             unsigned long* Value );

// Valid for the following VRs: OB, OW, or unknown
MCEXPORT MC_STATUS MC_Get_Value_To_Buffer( int           MsgFileItemID,
                                           unsigned long Tag,
                                           MC_size_t     BufferSize,
                                           void*         Value,
                                           int*          ValueSize );

// Valid for the following VRs: OB, OW, OD, OF, UR, UT, SL, SS, UL, US, AT,
//                              FL, FD
MCEXPORT MC_STATUS MC_Get_Value_To_Function( int              MsgFileItemID,
                                             unsigned long    Tag,
                                             void*            UserInfo,
                                             GetValueCallback YourGetFunction );

MCEXPORT MC_STATUS MC_Get_Next_Value( int           MsgFileItemID,
                                      unsigned long Tag,
                                      MC_DT         DataType,
                                      MC_size_t     BufferSize,
                                      void*         Value,
                                      int*          ValueSize );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Get_Next_Value_To_Double( int           MsgFileItemID,
                                                unsigned long Tag,
                                                double*       Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Get_Next_Value_To_Float( int           MsgFileItemID,
                                               unsigned long Tag,
                                               float*        Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Get_Next_Value_To_Int( int           MsgFileItemID,
                                             unsigned long Tag,
                                             int*          Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Get_Next_Value_To_ShortInt( int           MsgFileItemID,
                                                  unsigned long Tag,
                                                  short int*    Value );

// Valid for the following VRs: AT, DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Get_Next_Value_To_LongInt( int           MsgFileItemID,
                                                 unsigned long Tag,
                                                 long int*     Value );

// Valid for the following VRs: AS, AT, CS, DA, DS, DT, FD, FL, IS, LO, LT,
//                              PN, SH, SL, SS, ST, TM, UI, UL, US, SQ
MCEXPORT MC_STATUS MC_Get_Next_Value_To_String( int           MsgFileItemID,
                                                unsigned long Tag,
                                                MC_size_t     BufferSize,
                                                char*         Value );

// Valid for the following VRs: LO, LT, PN, SH, ST, UT
MCEXPORT MC_STATUS MC_Get_Next_Value_To_UnicodeString( int           MsgFileItemID,
                                                       unsigned long Tag,
                                                       MC_size_t     BufferSize,
                                                       int*          ValueSize,
                                                       MC_UChar*     Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Get_Next_Value_To_UInt( int           MsgFileItemID,
                                              unsigned long Tag,
                                              unsigned int* Value );

// Valid for the following VRs: DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Get_Next_Value_To_UShortInt( int             MsgFileItemID,
                                                   unsigned long   Tag,
                                                   unsigned short* Value );

// Valid for the following VRs: AT, DS, FD, FL, IS, SL, SS, UL, US, SQ
MCEXPORT MC_STATUS MC_Get_Next_Value_To_ULongInt( int            MsgFileItemID,
                                                  unsigned long  Tag,
                                                  unsigned long* Value );

MCEXPORT MC_STATUS MC_Get_Value_Count( int           MsgFileItemID,
                                       unsigned long Tag,
                                       int*          CountPtr );

MCEXPORT MC_STATUS MC_Delete_Attribute( int MsgFileItemID, unsigned long Tag );

MCEXPORT MC_STATUS MC_Delete_Range( int           MsgFileItemID,
                                    unsigned long FirstTag,
                                    unsigned long LastTag );

MCEXPORT MC_STATUS MC_Add_Standard_Attribute( int           MsgFileItemID,
                                              unsigned long tag );

MCEXPORT MC_STATUS MC_Add_Nonstandard_Attribute( int           MsgFileItemID,
                                                 unsigned long Tag,
                                                 MC_VR         ValueRep );

#ifndef _WIN32
MCEXPORT void MC_List_Item ( int ItemID, FILE* StreamHandle );
#else
MCEXPORT void MC_List_Item( int ItemID, const char* Afilename );
#endif

// API extension
MCEXPORT void MC_List_Item_To_Filename( int ItemID, const char* Afilename );

MCEXPORT MC_STATUS MC_Free_Item( int* ItemID );
MCEXPORT MC_STATUS MC_Free_Message( int* MessageID );

MCEXPORT MC_STATUS MC_Open_Message( int*        MessageIDPtr,
                                    const char* ServiceName,
                                    MC_COMMAND  Command );

MCEXPORT MC_STATUS MC_Open_Item( int* ItemIDPtr, const char* ItemName );

MCEXPORT MC_STATUS MC_Empty_Item( int ItemID );

MCEXPORT MC_STATUS MC_Validate_Message( int       MessageID,
                                        VAL_ERR** ErrorInfo,
                                        VAL_LEVEL ErrorLevel );

MCEXPORT MC_STATUS MC_Get_Next_Validate_Error( int MessageID, VAL_ERR** ErrorInfo );

MCEXPORT MC_STATUS MC_Set_Message_Callbacks( int ApplicationID, int MessageFileID );

MCEXPORT MC_STATUS MC_Get_Transfer_Syntax_From_Enum( TRANSFER_SYNTAX SyntaxType,
                                                     char*           Uid,
                                                     int             UidLength );

MCEXPORT MC_STATUS MC_Get_Enum_From_Transfer_Syntax( char*            Uid,
                                                     TRANSFER_SYNTAX* SyntaxType );

MCEXPORT char* MC_Error_Message( MC_STATUS StatusCode );


#ifdef __cplusplus
}
#endif

#endif
