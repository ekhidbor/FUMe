#include "mc3media.h"
#include "mc3msg.h"
#include "mergecom.h"

#include <stdio.h>
#include <assert.h>

char buffer[1024];

static MC_STATUS read_using_stdio( char*  Cbfilename,
                                   void*  CbuserInfo,
                                   int*   CbdataSizePtr,
                                   void** CbdataBufferPtr,
                                   int    CbisFirst,
                                   int*   CbisLastPtr )
{
    assert( CbuserInfo != NULL );
    FILE** f_ptr = (FILE**)CbuserInfo;

    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( CbisFirst != 0 )
    {
        *f_ptr = fopen( Cbfilename, "rb" );
        if( *f_ptr != NULL )
        {
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            ret = MC_CANNOT_COMPLY;
        }
    }
    else
    {
        // Do nothing. File pointer already initialized
    }

    if( *f_ptr != NULL )
    {
        size_t bytes_read = fread( buffer, 1, sizeof(buffer), *f_ptr );
        *CbdataSizePtr = (int)bytes_read;
        *CbdataBufferPtr = buffer;
        *CbisLastPtr = bytes_read != sizeof(buffer);

        if( *CbisLastPtr != 0 )
        {
            fclose( *f_ptr );
            *f_ptr = NULL;
        }

        ret = MC_NORMAL_COMPLETION;
    }
    else
    {
        // No file pointer. Error
        ret = MC_CANNOT_COMPLY;
    }

    return ret;
}

static MC_STATUS write_using_stdio( char* Cbfilename,
                                    void* CbuserInfo,
                                    int   CbdataSize,
                                    void* CbdataBuffer,
                                    int   CbisFirst,
                                    int   CbisLast )
{
    assert( CbuserInfo != NULL );
    FILE** f_ptr = (FILE**)CbuserInfo;

    MC_STATUS ret = MC_CANNOT_COMPLY;

    if( CbisFirst != 0 )
    {
        *f_ptr = fopen( Cbfilename, "wb" );
        if( *f_ptr != NULL )
        {
            ret = MC_NORMAL_COMPLETION;
        }
        else
        {
            ret = MC_CANNOT_COMPLY;
        }
    }
    else
    {
        // Do nothing. File pointer already initialized
    }

    if( *f_ptr != NULL )
    {
        if( CbdataSize > 0 && CbdataBuffer != NULL )
        {
            if( fwrite( CbdataBuffer, CbdataSize, 1, *f_ptr ) == 1u )
            {
                ret = MC_NORMAL_COMPLETION;
            }
            else
            {
                ret = MC_CANNOT_COMPLY;
            }
        }
        else
        {
            // Nothing to write. Not an error
            ret = MC_NORMAL_COMPLETION;
        }
    }
    else
    {
        // No file pointer. Error
        ret = MC_CANNOT_COMPLY;
    }

    if( CbisLast != 0 && *f_ptr != NULL )
    {
        fflush( *f_ptr );
        fclose( *f_ptr );
        *f_ptr = NULL;
    }
    else
    {
        // Do nothing
    }

    return ret;
}

void dump_stat( MC_STATUS stat )
{
    printf( "stat: %s\n", MC_Error_Message( stat ) );
}

int main( int argc, char* argv[] )
{
    int appid = 0;
    int fileid = 0;
    FILE* f = NULL;

    dump_stat( MC_Library_Initialization( NULL, NULL, NULL ) );


    dump_stat( MC_Register_Application( &appid, "test" ) );
    dump_stat( MC_Create_File( &fileid, argv[1], "DICOMDIR", C_STORE_RQ ) );
    dump_stat( MC_Open_File( appid, fileid, &f, read_using_stdio ) );

    if( f != NULL )
    {
        fclose( f );
        f = NULL;
    }

    dump_stat( MC_Reset_Filename( fileid, argv[2] ) );

    dump_stat( MC_Write_File( fileid, 0, &f, write_using_stdio ) );

    dump_stat( MC_Free_File( &fileid ) );
    dump_stat( MC_Release_Application( &appid ) );
    dump_stat( MC_Library_Release() );
}
