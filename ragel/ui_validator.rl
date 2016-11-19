%%{
    machine ui_validator;

    # A character string containing a UID that is used to uniquely identify a
    # wide variety of items. The UID is a series of numeric components separated
    # by the period "." character.
    ui = digit+ ('.' digit+)*;
    main := ui? %{ ok = true; };
}%%

/**
  This is an automatically generated file generated with the
  Ragel State Machine Compiler. Changes to this file should
  be made to the ui_validator.rl file used to produce it.

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

static constexpr size_t MAX_UI_LEN = 64u;

%% write data;

bool validate_ui( const string& val )
{
    bool ok = false;

    if( val.size() <= MAX_UI_LEN )
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
