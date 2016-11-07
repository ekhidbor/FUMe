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
#include <cstring>

// local public
#include "mcstatus.h"
#include "mc3msg.h"

// local private
#include "fume/library_context.h"
#include "fume/data_dictionary.h"
#include "fume/value_representation.h"

using fume::g_context;
using fume::data_dictionary;
using fume::value_representation;
using fume::get_buf_parms;
using fume::get_string_parms;
using fume::get_ustring_parms;

template<class T>
static MC_STATUS get_next_value( int            msg,
                                 unsigned long  tag,
                                 T&             value )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context != nullptr )
        {
            const data_dictionary* dict = g_context->get_object( msg );
            if( dict != nullptr )
            {
                ret = dict->check_tag_const( tag );
                if( ret == MC_NORMAL_COMPLETION )
                {
                    const value_representation* element = dict->at( tag );
                    if( element != nullptr )
                    {
                        ret = element->get_next( value );
                    }
                    else
                    {
                        ret = MC_EMPTY_VALUE;
                    }
                }
                else
                {
                    // Do nothing. Returns value from check_tag
                }
            }
            else
            {
                ret = MC_INVALID_MESSAGE_ID;
            }
        }
        else
        {
            ret = MC_LIBRARY_NOT_INITIALIZED;
        }
    }
    catch( ... )
    {
        ret = MC_SYSTEM_ERROR;
    }

    return ret;
}

MC_STATUS MC_Get_Next_Value_To_Double( int           MsgFileItemID,
                                       unsigned long Tag,
                                       double*       Value )
{
    return Value == nullptr ? MC_NULL_POINTER_PARM :
                              get_next_value( MsgFileItemID, Tag, *Value );
}

MC_STATUS MC_Get_Next_Value_To_Float( int           MsgFileItemID,
                                      unsigned long Tag,
                                      float*        Value )
{
    return Value == nullptr ? MC_NULL_POINTER_PARM :
                              get_next_value( MsgFileItemID, Tag, *Value );
}

MC_STATUS MC_Get_Next_Value_To_Int( int           MsgFileItemID,
                                    unsigned long Tag,
                                    int*          Value )
{
    return Value == nullptr ? MC_NULL_POINTER_PARM :
                              get_next_value( MsgFileItemID, Tag, *Value );
}

MC_STATUS MC_Get_Next_Value_To_ShortInt( int           MsgFileItemID,
                                         unsigned long Tag,
                                         short int*    Value )
{
    return Value == nullptr ? MC_NULL_POINTER_PARM :
                              get_next_value( MsgFileItemID, Tag, *Value );
}

MC_STATUS MC_Get_Next_Value_To_LongInt( int           MsgFileItemID,
                                        unsigned long Tag,
                                        long int*     Value )
{
    return Value == nullptr ? MC_NULL_POINTER_PARM :
                              get_next_value( MsgFileItemID, Tag, *Value );
}

MC_STATUS MC_Get_Next_Value_To_String( int           MsgFileItemID,
                                       unsigned long Tag,
                                       MC_size_t     BufferSize,
                                       char*         Value )
{
    get_string_parms parms( Value, BufferSize );
    return get_next_value( MsgFileItemID, Tag, parms );
}

MC_STATUS MC_Get_Next_Value_To_UnicodeString( int           MsgFileItemID,
                                              unsigned long Tag,
                                              MC_size_t     BufferSize,
                                              int*          ValueSize,
                                              MC_UChar*     Value )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;
    if( ValueSize != nullptr )
    {
        get_ustring_parms parms( Value, BufferSize );
        ret = get_next_value( MsgFileItemID, Tag, parms );
        if( ret == MC_NORMAL_COMPLETION )
        {
            *ValueSize = static_cast<int>( parms.second );
        }
        else
        {
            // Do nothing. Returns error from get_next_value
        }
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}

MC_STATUS MC_Get_Next_Value_To_UInt( int           MsgFileItemID,
                                     unsigned long Tag,
                                     unsigned int* Value )
{
    return Value == nullptr ? MC_NULL_POINTER_PARM :
                              get_next_value( MsgFileItemID, Tag, *Value );
}

MC_STATUS MC_Get_Next_Value_To_UShortInt( int             MsgFileItemID,
                                          unsigned long   Tag,
                                          unsigned short* Value )
{
    return Value == nullptr ? MC_NULL_POINTER_PARM :
                              get_next_value( MsgFileItemID, Tag, *Value );
}

MC_STATUS MC_Get_Next_Value_To_ULongInt( int            MsgFileItemID,
                                    unsigned long  Tag,
                                    unsigned long* Value )
{
    return Value == nullptr ? MC_NULL_POINTER_PARM :
                              get_next_value( MsgFileItemID, Tag, *Value );
}

MC_STATUS MC_Get_Next_Value( int           MsgFileItemID,
                             unsigned long Tag,
                             MC_DT         DataType,
                             MC_size_t     BufferSize,
                             void*         Value,
                             int*          ValueSize )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( Value != nullptr && ValueSize != nullptr )
    {
        switch( DataType )
        {
            case String_Type:
            {
                char* const ptr = static_cast<char*>( Value );
                ret = MC_Get_Next_Value_To_String( MsgFileItemID,
                                              Tag,
                                              BufferSize,
                                              ptr );
                if( ret == MC_NORMAL_COMPLETION )
                {
                    *ValueSize = static_cast<int>( strlen( ptr ) );
                }
                break;
            }
            case Int_Type:
            {
                if( BufferSize >= static_cast<MC_size_t>( sizeof(int) ) )
                {
                    int* const ptr = static_cast<int*>( Value );
                    ret = MC_Get_Next_Value_To_Int( MsgFileItemID, Tag, ptr );
                    if( ret == MC_NORMAL_COMPLETION )
                    {
                        *ValueSize = static_cast<int>( sizeof(int) );
                    }
                }
                else
                {
                    ret = MC_BUFFER_TOO_SMALL;
                }
                break;
            }
            case UInt_Type:
            {
                if( BufferSize >= static_cast<MC_size_t>( sizeof(unsigned int) ) )
                {
                    unsigned int* const ptr = static_cast<unsigned int*>( Value );
                    ret = MC_Get_Next_Value_To_UInt( MsgFileItemID, Tag, ptr );
                    if( ret == MC_NORMAL_COMPLETION )
                    {
                        *ValueSize = static_cast<int>( sizeof(unsigned int) );
                    }
                }
                else
                {
                    ret = MC_BUFFER_TOO_SMALL;
                }
                break;
            }
            case ShortInt_Type:
            {
                if( BufferSize >= static_cast<MC_size_t>( sizeof(short) ) )
                {
                    short* const ptr = static_cast<short*>( Value );
                    ret = MC_Get_Next_Value_To_ShortInt( MsgFileItemID, Tag, ptr );
                    if( ret == MC_NORMAL_COMPLETION )
                    {
                        *ValueSize = static_cast<int>( sizeof(short) );
                    }
                }
                else
                {
                    ret = MC_BUFFER_TOO_SMALL;
                }
                break;
            }
            case UShortInt_Type:
            {
                if( BufferSize >= static_cast<MC_size_t>( sizeof(unsigned short) ) )
                {
                    unsigned short* const ptr =
                        static_cast<unsigned short*>( Value );
                    ret = MC_Get_Next_Value_To_UShortInt( MsgFileItemID, Tag, ptr );
                    if( ret == MC_NORMAL_COMPLETION )
                    {
                        *ValueSize = static_cast<int>( sizeof(unsigned short) );
                    }
                }
                else
                {
                    ret = MC_BUFFER_TOO_SMALL;
                }
                break;
            }
            case LongInt_Type:
            {
                if( BufferSize >= static_cast<MC_size_t>( sizeof(long) ) )
                {
                    long* const ptr = static_cast<long*>( Value );
                    ret = MC_Get_Next_Value_To_LongInt( MsgFileItemID, Tag, ptr );
                    if( ret == MC_NORMAL_COMPLETION )
                    {
                        *ValueSize = static_cast<int>( sizeof(long) );
                    }
                }
                else
                {
                    ret = MC_BUFFER_TOO_SMALL;
                }
                break;
            }
            case ULongInt_Type:
            {
                if( BufferSize >= static_cast<MC_size_t>( sizeof(unsigned long) ) )
                {
                    unsigned long* const ptr =
                        static_cast<unsigned long*>( Value );
                    ret = MC_Get_Next_Value_To_ULongInt( MsgFileItemID, Tag, ptr );
                    if( ret == MC_NORMAL_COMPLETION )
                    {
                        *ValueSize = static_cast<int>( sizeof(unsigned long) );
                    }
                }
                else
                {
                    ret = MC_BUFFER_TOO_SMALL;
                }
                break;
            }
            case Float_Type:
            {
                if( BufferSize >= static_cast<MC_size_t>( sizeof(float) ) )
                {
                    float* const ptr = static_cast<float*>( Value );
                    ret = MC_Get_Next_Value_To_Float( MsgFileItemID, Tag, ptr );
                    if( ret == MC_NORMAL_COMPLETION )
                    {
                        *ValueSize = static_cast<int>( sizeof(float) );
                    }
                }
                else
                {
                    ret = MC_BUFFER_TOO_SMALL;
                }
                break;
            }
            case Double_Type:
            {
                if( BufferSize >= static_cast<MC_size_t>( sizeof(double) ) )
                {
                    double* const ptr = static_cast<double*>( Value );
                    ret = MC_Get_Next_Value_To_Double( MsgFileItemID, Tag, ptr );
                    if( ret == MC_NORMAL_COMPLETION )
                    {
                        *ValueSize = static_cast<int>( sizeof(double) );
                    }
                }
                else
                {
                    ret = MC_BUFFER_TOO_SMALL;
                }
                break;
            }
            default:
            {
                ret = MC_INVALID_DATA_TYPE;
                break;
            }
        }
    }
    else
    {
        ret = MC_NULL_POINTER_PARM;
    }

    return ret;
}
