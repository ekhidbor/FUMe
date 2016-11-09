%%{
    machine charsets;

    ESC = 0x1B;
    TAB = 0x09;
    LF  = 0x0A;
    FF  = 0x0C;
    CR  = 0x0D;

    # Default Character Repertoire excluding character code 5CH
    # (the BACKSLASH "\" in ISO-IR 6), and all control characters.
    multival_print_char = print - '\\';
    
    # Default Character Repertoire and/or as defined by (0008,0005)
    # excluding character code 5CH (the BACKSLASH "\" in ISO-IR 6),
    # and all Control Characters except ESC when used for ISO 2022
    # escape sequences.
    string_char = multival_print_char | ESC;

    # Default Character Repertoire and/or as defined by (0008,0005)
    # excluding Control Characters except TAB, LF, FF, CR (and ESC
    # when used for ISO 2022 escape sequences).
    text_char = print | TAB | LF | FF | CR | ESC;

}%%