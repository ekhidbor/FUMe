#!/usr/bin/python

from Cheetah.Template import Template

chars_to_delete = "'\"&(),"
for i in range(127, 256):
    chars_to_delete += chr(i)

chars_to_translate = ""
for i in range(0, 256):
    chars_to_translate += chr(i)

chars_to_translate = chars_to_translate.replace('/', ' ')
chars_to_translate = chars_to_translate.replace('-', ' ')

def format_tag_name(tag_name, status):
    # Convert unicode mu to u
    tag_name = tag_name.replace('\xc2\xb5', 'u')
    # This removes unwanted characters and replaces some delimiters with spaces
    translated = tag_name.upper().translate(chars_to_translate, chars_to_delete)
    # Append "RETIRED" if needed
    if status.find("RET") >= 0 and translated.find("RETIRED") < 0:
        translated += " RETIRED"
    # Remove duplicate spaces
    items = translated.split()
    # Re-join with underscore and prefix with "MC_ATT_"
    return 'MC_ATT_' + '_'.join(items)

def format_tag(tag):
    import re
    # Some tags have "x" values in them. Replace with zero
    tag = tag.replace('x', '0')
    try:
        m = re.match("\\(([0-9A-F]{4}),([0-9A-F]{4})\\)", tag)

        return "0x%s%s" % (m.group(1), m.group(2))
    except BaseException:
        print tag

class gen_headers(Template):
    def __init__(self, file):
        from csv import DictReader
        self.__csv_values = DictReader(open(file, "r"))

    def csv_values(self):
        return self.__csv_values


def main(args):
    from diction_h import diction_h
    from mcstatus_h import mcstatus_h
    from MC_Error_Message_cpp import MC_Error_Message_cpp
    diction_tmpl = diction_h(args[1])
    mcstatus_tmpl = mcstatus_h(args[2])
    MC_Error_Message_tmpl = MC_Error_Message_cpp(args[2])

    with open("../include/diction.h", "w") as diction_h_file:
        diction_h_file.write(str(diction_tmpl))

    with open("../include/mcstatus.h", "w") as mcstatus_h_file:
        mcstatus_h_file.write(str(mcstatus_tmpl))

    with open("../src/MC_Error_Message.cpp", "w") as MC_Error_Message_cpp_file:
        MC_Error_Message_cpp_file.write(str(MC_Error_Message_tmpl))

if __name__ == "__main__":
    import sys
    main(sys.argv)
