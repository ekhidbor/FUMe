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

// boost
#include "boost/numeric/conversion/cast.hpp"

// local public
#include "mcstatus.h"
#include "mc3msg.h"

// local private
#include "fume/library_context.h"
#include "fume/data_dictionary.h"
#include "fume/value_representation.h"

using boost::numeric_cast;
using boost::bad_numeric_cast;

using fume::g_context;
using fume::data_dictionary;
using fume::value_representation;
using fume::set_buf_parms;

template<class T>
static MC_STATUS set_next_value( int            msg,
                                 unsigned long  tag,
                                 const T&       value )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        if( g_context != nullptr )
        {
            data_dictionary* dict = g_context->get_object( msg );
            if( dict != nullptr )
            {
                const uint32_t tag_u32 = numeric_cast<uint32_t>( tag );
                ret = dict->check_tag( tag_u32 );
                if( ret == MC_NORMAL_COMPLETION )
                {
                    value_representation& element( (*dict)[tag_u32] );
                    ret = element.set_next( value );
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
    catch( const bad_numeric_cast& )
    {
        ret = MC_INVALID_TAG;
    }
    catch( ... )
    {
        ret = MC_SYSTEM_ERROR;
    }

    return ret;
}

MC_STATUS MC_Set_Next_Value_From_Double( int           MsgFileItemID,
                                         unsigned long Tag,
                                         double        Value )
{
    return set_next_value( MsgFileItemID, Tag, Value );
}

MC_STATUS MC_Set_Next_Value_From_Float( int           MsgFileItemID,
                                        unsigned long Tag,
                                        float         Value )
{
    return set_next_value( MsgFileItemID, Tag, Value );
}

MC_STATUS MC_Set_Next_Value_From_Int( int           MsgFileItemID,
                                      unsigned long Tag,
                                      int           Value )
{
    return set_next_value( MsgFileItemID, Tag, Value );
}

MC_STATUS MC_Set_Next_Value_From_ShortInt( int           MsgFileItemID,
                                           unsigned long Tag,
                                           short int     Value )
{
    return set_next_value( MsgFileItemID, Tag, Value );
}

MC_STATUS MC_Set_Next_Value_From_LongInt( int           MsgFileItemID,
                                          unsigned long Tag,
                                          long int      Value )
{
    return set_next_value( MsgFileItemID, Tag, Value );
}

MC_STATUS MC_Set_Next_Value_From_String( int           MsgFileItemID,
                                         unsigned long Tag,
                                         const char*   Value )
{
    return set_next_value( MsgFileItemID, Tag, Value );
}

MC_STATUS MC_Set_Next_Value_From_UnicodeString( int             MsgFileItemID,
                                                unsigned long   Tag,
                                                const MC_UChar* Value )
{
    return set_next_value( MsgFileItemID, Tag, Value );
}

MC_STATUS MC_Set_Next_Value_From_UInt( int           MsgFileItemID,
                                       unsigned long Tag,
                                       unsigned int  Value )
{
    return set_next_value( MsgFileItemID, Tag, Value );
}

MC_STATUS MC_Set_Next_Value_From_UShortInt( int            MsgFileItemID,
                                            unsigned long  Tag,
                                            unsigned short Value )
{
    return set_next_value( MsgFileItemID, Tag, Value );
}

MC_STATUS MC_Set_Next_Value_From_ULongInt( int           MsgFileItemID,
                                           unsigned long Tag,
                                           unsigned long Value )
{
    return set_next_value( MsgFileItemID, Tag, Value );
}

MC_STATUS MC_Set_Next_Value_From_Buffer( int           MsgFileItemID,
                                         unsigned long Tag,
                                         void*         Value,
                                         unsigned long ValueLength )
{
    set_buf_parms buffer( Value, ValueLength );
    return set_next_value( MsgFileItemID, Tag, buffer );
}

MC_STATUS MC_Set_Next_Value( int           MsgFileItemID,
                             unsigned long Tag,
                             MC_DT         DataType,
                             void*         Value )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;
    if( Value != nullptr )
    {
        switch( DataType )
        {
            case String_Type:
            {
                char* const ptr = static_cast<char*>( Value );
                ret = MC_Set_Next_Value_From_String( MsgFileItemID, Tag, ptr );
                break;
            }
            case Int_Type:
            {
                int* const ptr = static_cast<int*>( Value );
                ret = MC_Set_Next_Value_From_Int( MsgFileItemID, Tag, *ptr );
                break;
            }
            case UInt_Type:
            {
                unsigned int* const ptr = static_cast<unsigned int*>( Value );
                ret = MC_Set_Next_Value_From_UInt( MsgFileItemID, Tag, *ptr );
                break;
            }
            case ShortInt_Type:
            {
                short* const ptr = static_cast<short*>( Value );
                ret = MC_Set_Next_Value_From_ShortInt( MsgFileItemID, Tag, *ptr );
                break;
            }
            case UShortInt_Type:
            {
                unsigned short* const ptr = static_cast<unsigned short*>( Value );
                ret = MC_Set_Next_Value_From_UShortInt( MsgFileItemID, Tag, *ptr );
                break;
            }
            case LongInt_Type:
            {
                long* const ptr = static_cast<long*>( Value );
                ret = MC_Set_Next_Value_From_LongInt( MsgFileItemID, Tag, *ptr );
                break;
            }
            case ULongInt_Type:
            {
                unsigned long* const ptr = static_cast<unsigned long*>( Value );
                ret = MC_Set_Next_Value_From_ULongInt( MsgFileItemID, Tag, *ptr );
                break;
            }
            case Float_Type:
            {
                float* const ptr = static_cast<float*>( Value );
                ret = MC_Set_Next_Value_From_Float( MsgFileItemID, Tag, *ptr );
                break;
            }
            case Double_Type:
            {
                double* const ptr = static_cast<double*>( Value );
                ret = MC_Set_Next_Value_From_Double( MsgFileItemID, Tag, *ptr );
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
