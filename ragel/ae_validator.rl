%%{
    machine ae_validator;

    include charsets "charsets.rl";

    # Allow any character except control characters and backslash. Space
    # is excluded as well because there must be at least one non-space
    # character present
    ae_char = multival_print_char - ' ';

    ae = ' '* (ae_char ' '*)+;
    main := ae? %{ ok = true; };
}%%

/**
  This is an automatically generated file generated with the
  Ragel State Machine Compiler. Changes to this file should
  be made to the ae_validator.rl file used to produce it.

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

static constexpr size_t MAX_AE_LEN = 16u;

%% write data;

bool validate_ae( const string& val )
{
    bool ok = false;

    if( val.size() <= MAX_AE_LEN )
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
