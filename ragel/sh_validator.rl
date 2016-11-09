%%{
    machine sh_validator;

    include charsets "charsets.rl";


    # A character string that may be padded with leading and/or trailing spaces.
    # The character code 5CH (the BACKSLASH "\" in ISO-IR 6) shall not be present,
    # as it is used as the delimiter between values in multiple valued data
    # elements. The string shall not have Control Characters except for ESC.
    main := string_char* %{ ok = true; };
}%%

/**
  This is an automatically generated file generated with the
  Ragel State Machine Compiler. Changes to this file should
  be made to the sh_validator.rl file used to produce it.

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

static constexpr size_t MAX_SH_LEN = 16u;

%% write data;

bool validate_sh( const string& val )
{
    bool ok = false;

    if( val.size() <= MAX_SH_LEN )
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
