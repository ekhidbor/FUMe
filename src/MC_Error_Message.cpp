/**
  This is an automatically generated file. Changes to this file
  should instead either be made in a separate file or to the
  data element CSV file used to generate this file

  This file was mechanically generated by running the mcstatus.csv file,
  containing a comma-delimited file of all status codes through the
  "gen_header.py" python script, which uses a cheetah template to mechanically
  format a valid C header. The status codes were obtained by running an original
  mcstatus.h through doxygen to obtain enum names and values. The descriptive
  text is an original work produced from reading public documentation on the API.

  This file is packaged as a part of the FUMe project.

  To the extent possible under law, the person who associated CC0 with
  FUMe has waived all copyright and related or neighboring rights
  to FUMe.

  You should have received a copy of the CC0 legalcode along with this
  work.  If not, see http://creativecommons.org/publicdomain/zero/1.0/.
**/

// local public
#include "mcstatus.h"
#include "mc3msg.h"

char* MC_Error_Message( MC_STATUS StatusCode )
{
    const char* ret = nullptr;

    switch( StatusCode )
    {
        case MC_NORMAL_COMPLETION:
            ret = "The operation completed successfully";
            break;
        case MC_ALREADY_REGISTERED:
            ret = "The application entity title passed in as a parameter has already been registered";
            break;
        case MC_ASSOCIATION_ABORTED:
            ret = "The association was aborted";
            break;
        case MC_ASSOCIATION_CLOSED:
            ret = "The association was closed";
            break;
        case MC_ASSOCIATION_REJECTED:
            ret = "The association was rejected";
            break;
        case MC_ATTRIBUTE_HAS_VALUES:
            ret = "Cannot change value representation to SQ when attribute has value";
            break;
        case MC_BUFFER_TOO_SMALL:
            ret = "The buffer passed in as a parameter is too small for the output value";
            break;
        case MC_CALLBACK_CANNOT_COMPLY:
            ret = "The callback function returned an error";
            break;
        case MC_CALLBACK_DATA_SIZE_NEGATIVE:
            ret = "The size returned from the callback function was negative";
            break;
        case MC_CALLBACK_DATA_SIZE_UNEVEN:
            ret = "The size returned from the callback function was not even";
            break;
        case MC_CALLBACK_PARM_ERROR:
            ret = "An output parameter returned from the callback function was invalid";
            break;
        case MC_CALLBACK_REGISTERED:
            ret = "The function cannot be called when a Callback Function is registered for the attribute";
            break;
        case MC_CANNOT_COMPLY:
            ret = "The function cannot perform the requested action";
            break;
        case MC_CANT_ACCESS_PROFILE:
            ret = "The profile file cannot be accessed";
            break;
        case MC_CONFIG_INFO_ERROR:
            ret = "An error occurred while accessing configuration information";
            break;
        case MC_CONFIG_INFO_MISSING:
            ret = "Required configuration information could not be found";
            break;
        case MC_DDFILE_ERROR:
            ret = "An error occurred while accessing the data dictionary";
            break;
        case MC_DOES_NOT_VALIDATE:
            ret = "The message failed validation";
            break;
        case MC_EMPTY_VALUE:
            ret = "The attribute does not have an assigned value";
            break;
        case MC_END_OF_DATA:
            ret = "The function completed but there is still more data?";
            break;
        case MC_EXT_INFO_UNAVAILABLE:
            ret = "No extended negotiation information was received";
            break;
        case MC_FOUND:
            ret = "?";
            break;
        case MC_FUNCTION_UNAVAILABLE:
            ret = "?";
            break;
        case MC_INCOMPATIBLE_VR:
            ret = "This function cannot be called on a VR of this type";
            break;
        case MC_INCOMPATIBLE_VALUE:
            ret = "This value cannot be assigned to this VR";
            break;
        case MC_INVALID_APPLICATION_ID:
            ret = "The application ID passed in as a parameter is invalid";
            break;
        case MC_INVALID_APPLICATION_TITLE:
            ret = "The application title passed in as a parameter is invalid";
            break;
        case MC_INVALID_ASSOC_ID:
            ret = "The association ID passed in as a parameter is invalid";
            break;
        case MC_INVALID_CHARS_IN_VALUE:
            ret = "The string passed in as a parameter contains invalid characters for this value representation";
            break;
        case MC_INVALID_COMMAND:
            ret = "The command passed in as a parameter is invalid";
            break;
        case MC_INVALID_DATA_TYPE:
            ret = "The data type passed in as a paramter is invalid";
            break;
        case MC_END_OF_LIST:
            ret = "No more elements are present in the list";
            break;
        case MC_INVALID_GROUP:
            ret = "The group number passed in as a parameter is invalid";
            break;
        case MC_INVALID_HOST_NAME:
            ret = "The host name passed in as a parameter is invalid";
            break;
        case MC_INVALID_ITEM_ID:
            ret = "The item ID passed in as a parameter is invalid";
            break;
        case MC_INVALID_LENGTH_FOR_TITLE:
            ret = "The application entity title passed in as a parameter has an invalid length";
            break;
        case MC_INVALID_LENGTH_FOR_VR:
            ret = "The value passed in as a parameter has an invalid length for this value representation";
            break;
        case MC_INVALID_LICENSE:
            ret = "Unused";
            break;
        case MC_INVALID_MESSAGE_ID:
            ret = "The Message ID passed in as a parameter is invalid";
            break;
        case MC_INVALID_MESSAGE_RECEIVED:
            ret = "An invalid message was received";
            break;
        case MC_INVALID_PARAMETER_NAME:
            ret = "The configuration value name passed in as a parameter is invalid";
            break;
        case MC_INVALID_PORT_NUMBER:
            ret = "The port number passed in as a parameter is invalid";
            break;
        case MC_INVALID_PRIVATE_CODE:
            ret = "The private code passed in as a parameter is invalid";
            break;
        case MC_INVALID_SERVICE_LIST_NAME:
            ret = "The service list passed in as a parameter is invalid";
            break;
        case MC_INVALID_TAG:
            ret = "The tag passed in as a parameter is invalid";
            break;
        case MC_INVALID_TRANSFER_SYNTAX:
            ret = "The transfer syntax passed in as a parameter is invalid";
            break;
        case MC_INVALID_VALUE_FOR_VR:
            ret = "The value passed in as a parameter is not valid for the value representation";
            break;
        case MC_INVALID_VALUE_NUMBER:
            ret = "The value number passed in as a parameter is not valid for the tag";
            break;
        case MC_INVALID_VR_CODE:
            ret = "The VR code passed in as a parameter is invalid";
            break;
        case MC_LOG_EMPTY:
            ret = "The log is empty";
            break;
        case MC_MESSAGE_EMPTY:
            ret = "The message contains no attributes";
            break;
        case MC_MESSAGE_VALIDATES:
            ret = "The object validated successfully";
            break;
        case MC_MISSING_CONFIG_PARM:
            ret = "A required configuration parameter is missing";
            break;
        case MC_MSGFILE_ERROR:
            ret = "An error occurred while accessing the message configuration file";
            break;
        case MC_MUST_BE_POSITIVE:
            ret = "The value passed in as a parameter must be positive";
            break;
        case MC_NETWORK_SHUT_DOWN:
            ret = "The network connection was shut down";
            break;
        case MC_NO_APPLICATIONS_REGISTERED:
            ret = "An application entity title has not been registered";
            break;
        case MC_NO_CALLBACK:
            ret = "?";
            break;
        case MC_NO_CONDITION_FUNCTION:
            ret = "There is no function available to check a conditional attribute";
            break;
        case MC_NO_FILE_SYSTEM:
            ret = "There is no filesystem present on this system and no configuration functions specified";
            break;
        case MC_NO_INFO_REGISTERED:
            ret = "Extended negotiation information has not been registered";
            break;
        case MC_NO_LICENSE:
            ret = "Unused";
            break;
        case MC_NO_MERGE_INI:
            ret = "The merge.ini file cannot be found";
            break;
        case MC_NO_MORE_ATTRIBUTES:
            ret = "No more attributes are present in the message";
            break;
        case MC_NO_MORE_VALUES:
            ret = "No more values are present in the attribute";
            break;
        case MC_NO_PROFILE:
            ret = "?";
            break;
        case MC_NO_REQUEST_PENDING:
            ret = "There are no association requests pending";
            break;
        case MC_NON_SERVICE_ATTRIBUTE:
            ret = "The attribute is invalid for the specified message/service";
            break;
        case MC_NOT_FOUND:
            ret = "The requested data could not be found";
            break;
        case MC_NOT_ONE_OF_ENUMERATED_VALUES:
            ret = "The value is not in the list of valid enumerated values";
            break;
        case MC_NOT_ONE_OF_DEFINED_TERMS:
            ret = "The value is not in the list of valid defined terms";
            break;
        case MC_NULL_POINTER_PARM:
            ret = "The value passed in as a parameter is NULL";
            break;
        case MC_NULL_VALUE:
            ret = "The attribute value is NULL";
            break;
        case MC_PROTOCOL_ERROR:
            ret = "?";
            break;
        case MC_REQUIRED_ATTRIBUTE_MISSING:
            ret = "An attribute required for this message is missing";
            break;
        case MC_REQUIRED_DATASET_MISSING:
            ret = "?";
            break;
        case MC_REQUIRED_VALUE_MISSING:
            ret = "A value required for this attribute is missing";
            break;
        case MC_STATE_VIOLATION:
            ret = "The requested operation is not valid for the current state of the association";
            break;
        case MC_SYSTEM_CALL_INTERRUPTED:
            ret = "A signal interupted the system call";
            break;
        case MC_SYSTEM_ERROR:
            ret = "An unexpected error has occurred";
            break;
        case MC_TAG_ALREADY_EXISTS:
            ret = "The attribute passed in as a parameter already exists in the message";
            break;
        case MC_TEMP_FILE_ERROR:
            ret = "An error occurred while accessing a temporary file";
            break;
        case MC_TIMEOUT:
            ret = "The specified operation timed out";
            break;
        case MC_TOO_FEW_VALUES:
            ret = "The attribute has too few values";
            break;
        case MC_TOO_MANY_BLOCKS:
            ret = "The group has too many private blocks";
            break;
        case MC_TOO_MANY_VALUES:
            ret = "The attribute has too many values";
            break;
        case MC_UNABLE_TO_CHECK_CONDITION:
            ret = "It is not possible to check the condition for a conditional attribute";
            break;
        case MC_UNACCEPTABLE_SERVICE:
            ret = "The message being sent uses a service which was not negoatiated";
            break;
        case MC_UNEXPECTED_EOD:
            ret = "Not all data has been received";
            break;
        case MC_UNKNOWN_ITEM:
            ret = "?";
            break;
        case MC_UNKNOWN_SERVICE:
            ret = "The service list passed in as a parameter has not been registered";
            break;
        case MC_VALUE_MAY_NOT_BE_NULL:
            ret = "The attribute may not have a NULL value";
            break;
        case MC_VALUE_NOT_ALLOWED:
            ret = "The attribute may not hava a value";
            break;
        case MC_VALUE_OUT_OF_RANGE:
            ret = "The value passed in as a parameter is out of range";
            break;
        case MC_VALUE_TOO_LARGE:
            ret = "The value passed in as a parameter is too large for its value representation";
            break;
        case MC_VR_ALREADY_VALID:
            ret = "The value representation for the specified attribute is already valid";
            break;
        case MC_LIBRARY_ALREADY_INITIALIZED:
            ret = "The library is already initialized";
            break;
        case MC_LIBRARY_NOT_INITIALIZED:
            ret = "The library has not been initialized";
            break;
        case MC_INVALID_DIRECTORY_RECORD_OFFSET:
            ret = "A record offset value is invalid";
            break;
        case MC_INVALID_FILE_ID:
            ret = "The file ID passed in as a parameter is invalid";
            break;
        case MC_INVALID_DICOMDIR_ID:
            ret = "The DICOMDIR ID passed in as a parameter is invalid";
            break;
        case MC_INVALID_ENTITY_ID:
            ret = "The entity ID passed in as a parameter is invalid";
            break;
        case MC_INVALID_MRDR_ID:
            ret = "The MRDR ID passed in as a parameter is invalid";
            break;
        case MC_UNABLE_TO_GET_ITEM_ID:
            ret = "The requested Item ID cannot be retrieved";
            break;
        case MC_INVALID_PAD:
            ret = "The file padding passed in as a parameter is invalid for the file size";
            break;
        case MC_ENTITY_ALREADY_EXISTS:
            ret = "?";
            break;
        case MC_INVALID_LOWER_DIR_RECORD:
            ret = "The child directory record is invalid for its parent type";
            break;
        case MC_BAD_DIR_RECORD_TYPE:
            ret = "The directory record type is invalid";
            break;
        case MC_UNKNOWN_HOST_CONNECTED:
            ret = "?";
            break;
        case MC_INACTIVITY_TIMEOUT:
            ret = "A timeout occurred while trying to read a message";
            break;
        case MC_INVALID_SOP_CLASS_UID:
            ret = "The SOP class UID passed in as a parameter is invalid";
            break;
        case MC_INVALID_VERSION:
            ret = "?";
            break;
        case MC_OUT_OF_ORDER_TAG:
            ret = "A tag was not written in ascending order";
            break;
        case MC_CONNECTION_FAILED:
            ret = "The TCP network connection failed";
            break;
        case MC_UKNOWN_HOST_NAME:
            ret = "A network device with the specified hostname could not be found";
            break;
        case MC_INVALID_FILE:
            ret = "?";
            break;
        case MC_NEGOTIATION_ABORTED:
            ret = "The association was aborted";
            break;
        case MC_INVALID_SR_ID:
            ret = "The SR ID passed in as a parameter is invalid";
            break;
        case MC_UNABLE_TO_GET_SR_ID:
            ret = "The specified SR ID cannot be retrieved";
            break;
        case MC_DUPLICATE_NAME:
            ret = "A service with the specified name already exists";
            break;
        case MC_DUPLICATE_SYNTAX:
            ret = "A transfer syntax with the specified name already exists";
            break;
        case MC_EMPTY_LIST:
            ret = "?";
            break;
        case MC_MISSING_NAME:
            ret = "?";
            break;
        case MC_INVALID_SERVICE_NAME:
            ret = "The service name passed in as a parameter is invalid";
            break;
        case MC_SERVICE_IN_USE:
            ret = "The service being freed is currently in use";
            break;
        case MC_NO_CONTEXT:
            ret = "?";
            break;
        case MC_OFFSET_TABLE_TOO_SHORT:
            ret = "?";
            break;
        case MC_MISSING_DELIMITER:
            ret = "?";
            break;
        case MC_COMPRESSION_FAILURE:
            ret = "An error occurred while compressing/decompressing the data";
            break;
        case MC_END_OF_FRAME:
            ret = "The function completed but there are still more frames";
            break;
        case MC_MUST_CONTINUE_BEFORE_READING:
            ret = "?";
            break;
        case MC_COMPRESSOR_REQUIRED:
            ret = "A compressor is required for the message";
            break;
        case MC_DECOMPRESSOR_REQUIRED:
            ret = "A decompressor is required for the message";
            break;
        case MC_DATA_AVAILABLE:
            ret = "?";
            break;
        case MC_ZLIB_ERROR:
            ret = "?";
            break;
        case MC_NOT_META_SOP:
            ret = "?";
            break;
        case MC_INVALID_ITEM_TRANSFER_SYNTAX:
            ret = "?";
            break;
        case MC_LICENSE_ERROR:
            ret = "Unused";
            break;
        case MC_MAX_OPERATIONS_EXCEEDED:
            ret = "The maximum number of operation requests has been reached";
            break;
        case MC_INVALID_RECORD_ID:
            ret = "The Record ID passed in as a parameter is invalid";
            break;
        case MC_INVALID_DICOMDIR_FILE:
            ret = "The DICOMDIR file is invalid";
            break;
        case MC_INVALID_VR:
            ret = "?";
            break;
        default:
            ret = "Invalid Status Code";
            break;
    }

    return const_cast<char*>( ret );
}