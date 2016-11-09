%%{
    machine tm_validator;

    include datetime "datetime.rl";

    # A string of characters of the format YYYYMMDD; where YYYY shall
    # contain year, MM shall contain the month, and DD shall contain
    # the day, interpreted as a date of the Gregorian calendar system.
    tm = time ' '*;
    main := tm? %{ ok = true; };
}%%

/**
  This is an automatically generated file generated with the
  Ragel State Machine Compiler. Changes to this file should
  be made to the tm_validator.rl file used to produce it.

  This file is packaged as a part of the FUMe project.

  To the extent possible under law, the person who associated CC0 with
  FUMe has waived all copyright and related or neighboring rights
  to FUMe.

  You should have received a copy of the CC0 legalcode along with this
  work.  If not, see http://creativecommons.org/publicdomain/zero/1.0/.
**/

// std
#include <string>

using std::string;

namespace fume
{

namespace vrs
{

static constexpr size_t MAX_TM_LEN = 14u;

%% write data;

bool validate_tm( const string& val )
{
    bool ok = false;

    if( val.size() <= MAX_TM_LEN )
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

