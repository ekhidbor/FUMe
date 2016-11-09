%%{
    machine datetime;

    day = ( [0-2] digit ) | ( '3' [01] );
    month = ( '0' digit ) | ( '1' [12] );
    year = digit{4};

    hour = ([01] digit) | ('2' [0123]);
    minute = [0-5] digit;
    second = ([0-5] digit) | ('60');

    offset = ('-' ('1200') | ([01] [01] minute)) |
             ('+' ('1400') | ([01] [0-3] minute));

    date = year month day;
    time = hour minute second ('.' digit{1,6})?;
}%%