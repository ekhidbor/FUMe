#ifndef VR_VALUE_LIST
#define VR_VALUE_LIST
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
#include <deque>
#include <string>
#include <type_traits>
#include <limits>
#include <memory>
#include <algorithm>
#include <numeric>
#include <utility>

// local public
#include "mcstatus.h"

// local private

namespace fume
{

namespace vrs
{

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

template<class T>
static size_t get_value_size( const std::deque<T>& val )
{
    return val.size() * sizeof(T);
}

template<class T>
static size_t get_new_size( const std::deque<T>& val, const T& )
{
    return get_value_size( val ) + sizeof(T);
}

static size_t get_value_size( const std::deque<std::string>& values )
{
    // Calculate the total length of all the strings
    const size_t str_size = std::accumulate( values.cbegin(),
                                             values.cend(),
                                             static_cast<size_t>(0u),
                                             []( size_t             sum,
                                                 const std::string& val )
                                             {
                                                 return sum + val.size();
                                             } );
    // There are delimiters between each value. Therefore n - 1 delimiters
    // for n values
    const size_t delim_size = values.size() <= 1u ? 0 : values.size() - 1u;
    const size_t total_size = str_size + delim_size;

    return total_size;
}

static size_t get_new_size( const std::deque<std::string>& values,
                            const std::string&             val )
{
    // If list is empty, new size is just the size of the new value
    // If list is not empty, new size is the size of the list + a delimiter +
    // the size of the new value
    return values.empty() ? val.size() : get_value_size( values ) + 1 + val.size();
}

#pragma GCC diagnostic pop

template<class T, size_t MaxSize>
class vr_value_list
{
public:
    typedef typename std::deque<T>::iterator iterator;
    typedef typename std::deque<T>::const_iterator const_iterator;

public:
    vr_value_list()
        : m_current_idx( 0 )
    {
        static_assert( MaxSize < std::numeric_limits<uint32_t>::max(),
                       "MaxSize must be < 0xFFFFFFFF" );
    }

    vr_value_list( const vr_value_list& rhs )
        : m_values( rhs.m_values ),
          m_current_idx( rhs.m_current_idx )
    {
    }

    vr_value_list( vr_value_list&& rhs ) noexcept
        : m_values( std::move( rhs.m_values ) ),
          m_current_idx( std::move( rhs.m_current_idx ) )
    {
        rhs.m_current_idx = 0;
    }

    MC_STATUS set( T&& val )
    {
        container_t tmp;
        tmp.push_back( std::move( val ) );

        MC_STATUS ret = MC_CANNOT_COMPLY;
        if( get_value_size( tmp ) <= MaxSize )
        {
            // Atomically clear and add. If the push_back above fails then
            // the value is not modified
            m_values.swap( tmp );
            m_current_idx = 0;
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            ret = MC_TOO_MANY_VALUES;
        }

        return ret;
    }

    MC_STATUS set( const T& val )
    {
        container_t tmp;
        tmp.push_back( val );

        MC_STATUS ret = MC_CANNOT_COMPLY;
        if( get_value_size( tmp ) <= MaxSize )
        {
            // Atomically clear and add. If the push_back above fails then
            // the value is not modified
            m_values.swap( tmp );
            m_current_idx = 0;
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            ret = MC_TOO_MANY_VALUES;
        }

        return ret;
    }

    MC_STATUS set_next( T&& val )
    {
        MC_STATUS ret = MC_CANNOT_COMPLY;

        if( get_new_size( m_values, val ) <= MaxSize )
        {
            m_values.push_back( std::move( val ) );
            m_current_idx = 0;
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            ret = MC_TOO_MANY_VALUES;
        }

        return ret;
    }

    MC_STATUS set_next( const T& val )
    {
        MC_STATUS ret = MC_CANNOT_COMPLY;

        if( get_new_size( m_values, val ) <= MaxSize )
        {
            m_values.push_back( val );
            m_current_idx = 0;
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            ret = MC_TOO_MANY_VALUES;
        }

        return ret;
    }

    MC_STATUS get( const T*& val )
    {
        MC_STATUS ret = MC_CANNOT_COMPLY;

        if( m_values.empty() == false )
        {
            m_current_idx = 0;
            val = &m_values[m_current_idx];
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            ret = MC_NULL_VALUE;
        }

        return ret;
    }

    MC_STATUS get( T& val )
    {
        MC_STATUS ret = MC_CANNOT_COMPLY;

        if( m_values.empty() == false )
        {
            m_current_idx = 0;
            val = m_values[m_current_idx];
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            ret = MC_NULL_VALUE;
        }

        return ret;
    }

    MC_STATUS get_next( const T*& val )
    {
        MC_STATUS ret = MC_CANNOT_COMPLY;

        if( m_values.empty() == false )
        {
            m_current_idx = std::min( m_values.size(), m_current_idx + 1 );
            if( m_current_idx < m_values.size() )
            {
                val = &m_values[m_current_idx];
                ret = MC_NORMAL_COMPLETION;
            }
            else
            {
                ret = MC_NO_MORE_VALUES;
            }
        }
        else
        {
            ret = MC_NULL_VALUE;
        }

        return ret;
    }

    MC_STATUS get_next( T& val )
    {
        MC_STATUS ret = MC_CANNOT_COMPLY;

        if( m_values.empty() == false )
        {
            m_current_idx = std::min( m_values.size(), m_current_idx + 1 );
            if( m_current_idx < m_values.size() )
            {
                val = m_values[m_current_idx];
                ret = MC_NORMAL_COMPLETION;
            }
            else
            {
                ret = MC_NO_MORE_VALUES;
            }
        }
        else
        {
            ret = MC_NULL_VALUE;
        }

        return ret;
    }

    MC_STATUS delete_current()
    {
        MC_STATUS ret = MC_CANNOT_COMPLY;

        if( m_values.empty() == false )
        {
            if( m_current_idx < m_values.size() )
            {
                m_values.erase( m_values.cbegin() + m_current_idx );
                // Adjust the index to continue to be valid if we removed
                // the last element of a multi-element list
                if( m_current_idx > 0 && m_current_idx >= m_values.size() )
                {
                    --m_current_idx;
                }
                else
                {
                    // Do nothing. Index is still valid
                }
            }
            else
            {
                ret = MC_NO_MORE_VALUES;
            }
        }
        else
        {
            ret = MC_NULL_VALUE;
        }

        return ret;
    }

    void set_null()
    {
        m_values.clear();
    }

    bool is_null() const
    {
        return m_values.empty();
    }

    uint32_t count() const
    {
        return m_values.size();
    }

    uint32_t size() const
    {
        // we ensure size is less than 32-bits
        return get_value_size( m_values );
    }

    iterator begin()
    {
        return m_values.begin();
    }

    const_iterator cbegin() const
    {
        return m_values.cbegin();
    }

    iterator end()
    {
        return m_values.end();
    }

    const_iterator cend() const
    {
        return m_values.cend();
    }

    void swap( vr_value_list& rhs )
    {
        m_values.swap( rhs.m_values );
        std::swap( m_current_idx, rhs.m_current_idx );
    }

private:
    typedef std::deque<T> container_t;

private:
    container_t            m_values;
    typename container_t::size_type m_current_idx;
};

}

}

#endif
