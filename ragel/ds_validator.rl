%%{
    machine ds_validator;

    # A string of characters representing either a fixed point number or a
    # floating point number. A fixed point number shall contain only the
    # characters 0-9 with an optional leading "+" or "-" and an optional
    # "." to mark the decimal point. A floating point number shall be conveyed
    # as defined in ANSI X3.9, with an "E" or "e" to indicate the start of the
    # exponent. Decimal Strings may be padded with leading or trailing spaces.
    # Embedded spaces are not allowed.

    # A string of characters identifying a controlled concept. Leading
    # or trailing spaces (20H) are not significant.
    ds = ' '* [+\-]? (digit* '.')? digit+ ([eE] digit+)? ' '*;
    main := ds? %{ ok = true; };
}%%

/**
  This is an automatically generated file generated with the
  Ragel State Machine Compiler. Changes to this file should
  be made to the ds_validator.rl file used to produce it.

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

%% write data;

bool validate_ds( const string& val )
{
    bool ok = false;

    if( val.size() == 16u )
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

