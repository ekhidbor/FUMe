%%{
    machine pn_validator;

    include charsets "charsets.rl";

    # Any of the five components may be an empty string. The component
    # delimiter shall be the caret "^" character (5EH).
    component_char = string_char - '^';

    # 64 chars maximum per component group
    component_string = component_char{,64};

    # Any of the five components may be an empty string. The component delimiter
    # shall be the caret "^" character (5EH). Delimiters are required for
    # interior null components. Trailing null components and their delimiters
    # may be omitted. Multiple entries are permitted in each component and are
    # encoded as natural text strings, in the format preferred by the named
    # person.
    pn = component_string ('^' component_string){,4};
    main := pn? %{ ok = true; };
}%%

/**
  This is an automatically generated file generated with the
  Ragel State Machine Compiler. Changes to this file should
  be made to the pn_validator.rl file used to produce it.

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

bool validate_pn( const string& val )
{
    bool ok = false;

    int cs = 0;
    const char* p = val.c_str();
    const char* pe = p + val.size();
    const char* eof = pe;

    %% write init;
    %% write exec;

    return ok;
}

} // namespace vrs

} // namespace fume
