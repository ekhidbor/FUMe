#ifndef VALUE_REPRESENTATION_H
#define VALUE_REPRESENTATION_H
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
#include <memory>

// local public
#include "mcstatus.h"

// local private
#include "fume/serializable.h"
#include "fume/value_representation_types.h"

namespace fume
{

class value_representation : public serializable
{
// value_representation -- constructor and destructor
public:
    value_representation( unsigned int min_vals,
                          unsigned int max_vals,
                          unsigned int multiple )
    {
        // TODO: implement
    }
    virtual ~value_representation()
    {
    }

// value_representation -- modifiers
// Note: we make these functions non-pure with a default
// implementation of to return MC_INCOMPATIBLE_VR to
// improve legibility of the derived implementations.
// There are so many functions in the interface legibility
// is impacted by derived instances re-implementing them
// all. With a default-deny implementation, it's easier
// to see which classes implement which subset of the
// functionality
public:
    virtual MC_STATUS set( double val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( float val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( int val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( short val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( long val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( const char* val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( const MC_UChar* val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( unsigned int val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( unsigned short val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( unsigned long val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( const set_buf_parms& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set( const set_func_parms& val )
    {
        return MC_INCOMPATIBLE_VR;
    }

    // Sets the value of the data element to NULL (ie. zero length)
    virtual MC_STATUS set_null()
    {
        return MC_INCOMPATIBLE_VR;
    }

    virtual MC_STATUS set_next( double val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( float val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( int val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( short val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( long val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( const char* val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( const MC_UChar* val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( unsigned int val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( unsigned short val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( unsigned long val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS set_next( const set_buf_parms& val )
    {
        return MC_INCOMPATIBLE_VR;
    }

    // For string value representations, sets the next value of
    // the data element to NULL (ie zero length string)
    virtual MC_STATUS set_next_null()
    {
        return MC_INCOMPATIBLE_VR;
    }

    // Removes the "current" value
    virtual MC_STATUS delete_current()
    {
        return MC_INCOMPATIBLE_VR;
    }

// value_representation -- accessors
public:
    virtual MC_STATUS get( double& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( float& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( int& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( short& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( long& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( get_string_parms& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( get_ustring_parms& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( unsigned int& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( unsigned short& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( unsigned long& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( get_buf_parms& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get( const get_func_parms& val )
    {
        return MC_INCOMPATIBLE_VR;
    }

    virtual MC_STATUS get_next( double& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( float& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( int& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( short& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( long& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( get_string_parms& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( get_ustring_parms& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( unsigned int& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( unsigned short& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( unsigned long& val )
    {
        return MC_INCOMPATIBLE_VR;
    }
    virtual MC_STATUS get_next( get_buf_parms& val )
    {
        return MC_INCOMPATIBLE_VR;
    }

    // Returns the number of elements
    virtual int count() const = 0;

    // Indicates whether or not the element is null
    virtual bool is_null() const = 0;

    virtual MC_VR vr() const = 0;

    virtual std::unique_ptr<value_representation> clone() const = 0;

protected:
    value_representation( const value_representation& rhs )
    {
    }
};

} // namespace fume

#endif
