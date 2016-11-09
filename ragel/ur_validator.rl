%%{
    machine ur_validator;

    # See section 2 of RFC3986 for these definitions
    gen_delims = ':' | '/' | '?' | '#'  | '[' | ']' | '@';

    sub_delims = '!' | '$' | '&' | '\'' | '(' | ')' |
                 '*' | '+' | ',' | ';'  | '=';

    unreserved = alpha | digit | '-' | '.' | '_' | '~';

    pct_encoded = '%' xdigit xdigit;

    pchar = unreserved | pct_encoded | sub_delims | ':' | '@';
    segment_nz_nc = (unreserved | pct_encoded | sub_delims | '@')+;
    segment_nz = pchar+;
    segment    = pchar*;

    path_empty = '';
    path_rootless = segment_nz ('/' segment);
    path_noscheme = segment_nz_nc ('/' segment)*;
    path_absolute = '/' (segment_nz ('/' segment)*)?;
    path_abempty = ('/' segment)*;

    path = (path_abempty | path_absolute | path_noscheme | path_rootless)?;

    port = digit*;

    reg_name = (unreserved | pct_encoded | sub_delims)*;

    dec_octet =           digit   |
                   ([1-9] digit)  |
                   ('1' digit{2}) |
                ('2' [0-4] digit) |
                     ('25' [0-5]) ;

    ipv4address = dec_octet ('.' dec_octet){3};

    h16 = xdigit{1,4};

    ls32 = (h16 ':' h16) | ipv4address;

    ipv6address = (                          (h16 ':'){6} ls32) |
                  (                     '::' (h16 ':'){5} ls32) |
                  ((              h16)? '::' (h16 ':'){4} ls32) |
                  (((h16 ':'){,1} h16)? '::' (h16 ':'){3} ls32) |
                  (((h16 ':'){,2} h16)? '::' (h16 ':'){2} ls32) |
                  (((h16 ':'){,3} h16)? '::' (h16 ':')    ls32) |
                  (((h16 ':'){,4} h16)? '::'              ls32) |
                  (((h16 ':'){,5} h16)? '::'              h16 ) |
                  (((h16 ':'){,6} h16)? '::'                  )  ;

    ipvfuture = 'v' xdigit+ '.' (unreserved | sub_delims | ':')+;

    ip_literal = '[' (ipv6address | ipvfuture) ']';

    host = ip_literal | ipv4address | reg_name;

    userinfo = (unreserved | pct_encoded | sub_delims | ':')*;

    authority = (userinfo '@')? host (':' port)?;

    fragment = (pchar | '/' | '?');
    query = (pchar | '/' | '?')*;
    scheme = alpha (alpha | digit | '+' | '-' | '.')*;
    hier_part = '//' authority (path_abempty | path_absolute | path_rootless)?;

    uri = scheme ':' hier_part ('?' query)? ('#' fragment)? ' '*;

    main := uri? %{ ok = true; };
}%%

/**
  This is an automatically generated file generated with the
  Ragel State Machine Compiler. Changes to this file should
  be made to the ur_validator.rl file used to produce it.

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

static constexpr size_t MAX_UR_LEN = 0xFFFFFFFEu;

%% write data;

bool validate_ur( const string& val )
{
    bool ok = false;

    if( val.size() <= MAX_UR_LEN )
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
