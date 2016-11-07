#ifndef FILE_OBJECT_H
#define FILE_OBJECT_H
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
#include <array>

// local public
#include "mcstatus.h"
#include "mc3media.h"

// local private
#include "fume/data_dictionary.h"

namespace fume
{

class file_object final : public data_dictionary
{
public:
    file_object( int id, const char* filename, bool created_empty );
    virtual ~file_object()
    {
    }

    MC_STATUS write( int               alignment,
                     void*             user_info,
                     WriteFileCallback callback ) const;

    // TODO: Add all variants of this function
    MC_STATUS open( void*            user_info,
                    ReadFileCallback callback );

    MC_STATUS set_preamble( const void* preamble );
    MC_STATUS get_preamble( void* preamble ) const;

    MC_STATUS reset_filename( const char* filename );
    MC_STATUS get_filename( char* filename, int filesize ) const;

    virtual MC_STATUS set_transfer_syntax( TRANSFER_SYNTAX syntax ) override final;
    virtual MC_STATUS get_transfer_syntax( TRANSFER_SYNTAX& syntax ) const override final;

// serializable
public:
    virtual MC_STATUS to_stream( tx_stream& stream ) const override final;
    virtual MC_STATUS from_stream( rx_stream& stream ) override final;

private:
    std::string              m_filename;
    std::array<uint8_t, 128> m_preamble;
};

}


#endif
