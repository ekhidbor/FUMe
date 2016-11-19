%%{
    machine dt_validator;

    include datetime "datetime.rl";

    # A concatenated date-time character string in the format:
    # YYYYMMDDHHMMSS.FFFFFF&ZZXX
    dt = date time offset? ' '*;
    main := dt? %{ ok = true; };
}%%

/**
  This is an automatically generated file generated with the
  Ragel State Machine Compiler. Changes to this file should
  be made to the dt_validator.rl file used to produce it.

  This file is packaged as a part of the FUMe project.

  To the extent possible under law, the person who associated CC0 with
  FUMe has waived all copyright and related or neighboring rights
  to FUMe.

  You should have received a copy of the CC0 legalcode along with this
  work.  If not, see http://creativecommons.org/publicdomain/zero/1.0/.
**/

#pragma GCC diagnostic ignored "-Wunused-const-variable"

// std
#include <string>

using std::string;

namespace fume
{

namespace vrs
{

%% write data;

bool validate_dt( const string& val )
{
    bool ok = false;

    if( val.size() <= 26u )
    {
        int cs = 0;
        const char* p = val.c_str();
        const char* pe = p + val.size();
        const char* eof = pe;

        %% write init;
        %% write exec;
    }
    else
    {
        ok = false;
    }

    return ok;
}

} // namespace vrs

} // namespace fume

