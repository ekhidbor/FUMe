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
#include <utility>
#include <memory>

// local public
#include "mcstatus.h"
#include "mc3msg.h"

// local private
#include "fume/serializable.h"

namespace fume
{

typedef std::pair<const void*, unsigned long> set_buf_parms;
typedef std::pair<      void*, unsigned long> get_buf_parms;

typedef std::pair<MC_UChar*, MC_size_t> get_ustring_parms;
typedef std::pair<char*,     MC_size_t> get_string_parms;

struct set_func_parms
{
    SetValueCallback callback;
    void*            callback_parm;
    int              message_id;
    unsigned long    tag;
};

struct get_func_parms
{
    GetValueCallback callback;
    void*            callback_parm;
    int              message_id;
    unsigned long    tag;
};

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
// Note: in a lot of these functions the derived classes
// implement them by just returning MC_INCOMPATIBLE_VR to
// indicate they aren't supported by the VR. We could have made
// all these functions none-pure with a default implmementation
// to do this. We aren't doing this in order to ensure we take
// the time to go through each VR and implement the functionality
// for that VR instead of depending on a default behavior which
// we may forget to override
public:
    virtual MC_STATUS set( double val ) = 0;
    virtual MC_STATUS set( float val ) = 0;
    virtual MC_STATUS set( int val ) = 0;
    virtual MC_STATUS set( short val ) = 0;
    virtual MC_STATUS set( long val ) = 0;
    virtual MC_STATUS set( const char* val ) = 0;
    virtual MC_STATUS set( const MC_UChar* val ) = 0;
    virtual MC_STATUS set( unsigned int val ) = 0;
    virtual MC_STATUS set( unsigned short val ) = 0;
    virtual MC_STATUS set( unsigned long val ) = 0;
    virtual MC_STATUS set( const set_buf_parms& val ) = 0;
    virtual MC_STATUS set( const set_func_parms& val ) = 0;

    // Sets the value of the data element to NULL (ie. zero length)
    virtual MC_STATUS set_null() = 0;

    virtual MC_STATUS set_next( double val ) = 0;
    virtual MC_STATUS set_next( float val ) = 0;
    virtual MC_STATUS set_next( int val ) = 0;
    virtual MC_STATUS set_next( short val ) = 0;
    virtual MC_STATUS set_next( long val ) = 0;
    virtual MC_STATUS set_next( const char* val ) = 0;
    virtual MC_STATUS set_next( const MC_UChar* val ) = 0;
    virtual MC_STATUS set_next( unsigned int val ) = 0;
    virtual MC_STATUS set_next( unsigned short val ) = 0;
    virtual MC_STATUS set_next( unsigned long val ) = 0;
    virtual MC_STATUS set_next( const set_buf_parms& val ) = 0;

    // For string value representations, sets the next value of
    // the data element to NULL (ie zero length string)
    virtual MC_STATUS set_next_null() = 0;

    // Removes the "current" value
    virtual MC_STATUS delete_current() = 0;

// value_representation -- accessors
public:
    virtual MC_STATUS get( double& val ) const = 0;
    virtual MC_STATUS get( float& val ) const = 0;
    virtual MC_STATUS get( int& val ) const = 0;
    virtual MC_STATUS get( short& val ) const = 0;
    virtual MC_STATUS get( long& val ) const = 0;
    virtual MC_STATUS get( get_string_parms& val ) const = 0;
    virtual MC_STATUS get( get_ustring_parms& val ) const = 0;
    virtual MC_STATUS get( unsigned int& val ) const = 0;
    virtual MC_STATUS get( unsigned short& val ) const = 0;
    virtual MC_STATUS get( unsigned long& val ) const = 0;
    virtual MC_STATUS get( get_buf_parms& val ) const = 0;
    virtual MC_STATUS get( const get_func_parms& val ) const = 0;

    virtual MC_STATUS get_next( double& val ) const = 0;
    virtual MC_STATUS get_next( float& val ) const = 0;
    virtual MC_STATUS get_next( int& val ) const = 0;
    virtual MC_STATUS get_next( short& val ) const = 0;
    virtual MC_STATUS get_next( long& val ) const = 0;
    virtual MC_STATUS get_next( get_string_parms& val ) const = 0;
    virtual MC_STATUS get_next( get_ustring_parms& val ) const = 0;
    virtual MC_STATUS get_next( unsigned int& val ) const = 0;
    virtual MC_STATUS get_next( unsigned short& val ) const = 0;
    virtual MC_STATUS get_next( unsigned long& val ) const = 0;
    virtual MC_STATUS get_next( get_buf_parms& val ) const = 0;

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
