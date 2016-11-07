#!/usr/bin/python

from Cheetah.Template import Template
import re

tag_re = re.compile('\\(([0-9A-F]{4}),([0-9A-F]{4})\\)')
vm_re = re.compile('(\\d+)(?:-(\\d*)(n?))?')

class diction_value:
    def __init__(self, vr, min_val, max_val, multiple):
        self.vr = vr
        self.min_val = min_val
        self.max_val = max_val
        self.multiple = multiple

    def __str__(self):
        return "VR: %s, min vals: %s, max_vals: %s, multiple: %s" % (self.vr, self.min_val, self.max_val, self.multiple)

def is_valid_standard_tag(tag):
    return ((tag & 0x00010000) == 0)

def generate_tags(tag):
    ret = []

    import re
    # Some tags have "x" values in them. Replace with F
    tag_high = tag.replace('x', 'F')
    # and replace with 0
    tag_low = tag.replace('x', '0')
    try:
        m_high = tag_re.match(tag_high)
        m_low  = tag_re.match(tag_low)

        int_high = int( "0x%s%s" % (m_high.group(1), m_high.group(2)), 16 )
        int_low = int( "0x%s%s" % (m_low.group(1), m_low.group(2)), 16 )

        # Take the difference between the "high" and "low" value. This will
        # leave just the "don't care" bits high
        diff = int_high - int_low

        diff_inv = ~diff
        # The inverted difference plus 1 makes the LSB "don't care" bit high.
        # When bitwise ANDed with the difference we obtain a value with just
        # the LSB "don't care" bit high
        incr = max((diff_inv + 1) & diff, 1)

        ret = range(int_low, int_high + incr, incr)
    except BaseException:
        ret = []

    return filter( is_valid_standard_tag, ret )

def parse_vm(vm):
    import re

    min_val = 1
    max_val = 1
    multiple = 1

    m = vm_re.match(vm)
    min_val = int(m.group(1))
    if m.group(3) is not None and len(m.group(3)) > 0:
        max_val = 0xFFFF
        if m.group(2) is not None and len(m.group(2)) > 0:
            multiple = int(m.group(2))
        else:
            multiple = 1
    elif m.group(2) is not None and len(m.group(2)) > 0:
        max_val = int(m.group(2))
        multiple = 1
    else:
        max_val = min_val
        multiple = 1

    return (min_val, max_val, multiple)

def parse_diction(diction):
    ret = {}
    for row in diction:
        row_tag = row['Tag']
        vr = None
        min_vals = None
        max_vals = None
        multiple = None

        if len(row['VR']) > 0:
            vr = format_vr(row['VR'])

            if len(row['VM']) > 0:
                (min_vals, max_vals, multiple) = parse_vm(row['VM'])

            ret.update(dict.fromkeys(generate_tags(row_tag),
                                     diction_value(vr, min_vals, max_vals, multiple)))

    return ret

def get_vr_map(diction_standard, diction_system):
    standard_map = parse_diction(diction_standard)
    system_map = parse_diction(diction_system)

    ret = {}

    for (tag, system_val) in system_map.iteritems():
        if tag in standard_map:
            standard_val = standard_map[tag]
            if system_val.vr is not None:
                standard_val.vr = system_val.vr
            if system_val.min_val is not None:
                standard_val.min_val = system_val.min_val
            if system_val.max_val is not None:
                standard_val.max_val = system_val.max_val
            if system_val.multiple is not None:
                standard_val.multiple = system_val.multiple
        else:
            standard_map[tag] = system_val

    # Special tags that don't belong in the dictionary
    del standard_map[0xfffee000]
    del standard_map[0xfffee00d]
    del standard_map[0xfffee0dd]

    return standard_map

def format_vr(vr):
    return "UNKNOWN_VR" if vr == "UN" else vr

def format_tag(tag):
    return "0x%08x" % (tag)

class gen_tag_vrs(Template):
    def __init__(self, diction_standard, diction_system):
        self.__vr_map = get_vr_map(diction_standard, diction_system)

    def vr_map(self):
        return self.__vr_map

def main(args):
    from csv import DictReader
    from tag_to_default_vr_cpp import tag_to_default_vr_cpp

    # VR map auto-generated from the standard document
    diction_standard = list(DictReader(open(args[0], "r")))
    # VR map used by the system to provide default VRs for
    # tags with conditional value representation
    diction_system = list(DictReader(open(args[1], "r")))

    vr_tmpl = tag_to_default_vr_cpp(diction_standard, diction_system)

    with open("../src/fume/tag_to_default_vr.cpp", "w") as tag_dict_file:
        tag_dict_file.write(str(vr_tmpl))

if __name__ == "__main__":
    import sys
    main(sys.argv[1:])
