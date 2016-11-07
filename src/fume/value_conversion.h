#ifndef VALUE_CONVERSION_H
#define VALUE_CONVERSION_H

// std
#include <cstring>
#include <string>
#include <functional>

// boost
#include "boost/numeric/conversion/cast.hpp"
#include "boost/lexical_cast.hpp"

// local public
#include "mcstatus.h"

// local private
#include "fume/value_representation.h"

namespace fume
{

template<class Dst, class Src>
MC_STATUS cast_and_call_setter( Src                              val,
                                std::function<MC_STATUS ( Dst )> setter )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        ret = setter( boost::numeric_cast<Dst>( val ) );
    }
    catch( const boost::bad_numeric_cast& )
    {
        ret = MC_VALUE_OUT_OF_RANGE;
    }

    return ret;
}

template<class Dst>
MC_STATUS cast_and_call_setter( const char*                      val,
                                std::function<MC_STATUS ( Dst )> setter )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        ret = setter( boost::lexical_cast<Dst>( val ) );
    }
    catch( const boost::bad_lexical_cast& )
    {
        ret = MC_INCOMPATIBLE_VR;
    }

    return ret;
}

template <class Src>
MC_STATUS cast_and_call_string_setter
(
    Src                                        val,
    std::function<MC_STATUS ( std::string&& )> setter
)
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        ret = setter( boost::lexical_cast<std::string>( val ) );
    }
    catch( const boost::bad_numeric_cast& )
    {
        ret = MC_VALUE_OUT_OF_RANGE;
    }

    return ret;
}

template<class Src, class Dst>
MC_STATUS cast_and_call_getter( Dst&                              val,
                                std::function<MC_STATUS ( Src& )> getter )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        Src src;
        ret = getter( src );
        if( ret == MC_NORMAL_COMPLETION )
        {
            val = boost::numeric_cast<Dst>( src );
        }
    }
    catch( const boost::bad_numeric_cast& )
    {
        ret = MC_VALUE_OUT_OF_RANGE;
    }

    return ret;
}

template<class Src>
MC_STATUS cast_and_call_getter( get_string_parms&                 val,
                                std::function<MC_STATUS ( Src& )> getter )
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        Src src;
        ret = getter( src );
        if( ret == MC_NORMAL_COMPLETION )
        {
            // TODO: optimize
            const std::string& str( boost::lexical_cast<std::string>( src ) );
            if( str.size() < val.second )
            {
                strncpy( val.first, str.c_str(), val.second );
                ret = MC_NORMAL_COMPLETION;
            }
            else
            {
                ret = MC_BUFFER_TOO_SMALL;
            }
        }
    }
    catch( const boost::bad_lexical_cast& )
    {
        ret = MC_VALUE_OUT_OF_RANGE;
    }

    return ret;
}

template <class Dst>
MC_STATUS cast_and_call_string_getter
(
    Dst&                                            val,
    std::function<MC_STATUS (const std::string*& )> getter
)
{
    MC_STATUS ret = MC_CANNOT_COMPLY;

    try
    {
        const std::string* cur_val = nullptr;
        ret = getter( cur_val );
        if( ret == MC_NORMAL_COMPLETION )
        {
            assert( cur_val != nullptr );
            val = boost::lexical_cast<Dst>( *cur_val );
        }
        else
        {
            // Do nothing. Will return error from get
        }
    }
    catch( const boost::bad_numeric_cast& )
    {
        ret = MC_VALUE_OUT_OF_RANGE;
    }

    return ret;
}

}

#endif
