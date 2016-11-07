#ifndef APPLICATION_H
#define APPLICATION_H
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
#include <cstdint>
#include <string>
#include <utility>
#include <unordered_map>

// local public
#include "mcstatus.h"
#include "mergecom.h"

namespace fume
{

typedef std::pair<CallbackFunction, void*> callback_parms_t;

class application
{
public:
    application( const char* ae_title )
        : m_ae_title( ae_title )
    {
    }

    const std::string& ae_title() const
    {
        return m_ae_title;
    }

    MC_STATUS register_callback_function( unsigned long    tag,
                                          CallbackFunction function,
                                          void*            function_context );
    MC_STATUS release_callback_function( unsigned long tag );

    callback_parms_t get_callback_function( unsigned long tag ) const;

private:
    typedef std::unordered_map<uint32_t, callback_parms_t> callback_map_t;

private:
    callback_map_t m_callbacks;
    std::string    m_ae_title;
};


}

#endif
