#!/usr/bin/python

def main(args):
    from csv import DictReader

    or_dict = {}
    vms_dict = {};

    reader = DictReader(open(args[1], "r"))

    for row in reader:
        vr = row['VR']
        if vr not in vms_dict:
            vms_dict[vr] = set()

        s = vms_dict[vr]
        s.add(row['VM'])

        if vr.find('or') >= 0:
            if vr not in or_dict:
                or_dict[vr] = set()

            s = or_dict[vr]
            s.add((row['Tag'], row['Field Name'], row['Status']))

        if vr == "OF" and row['VM'] == "1-n":
            print row['Field Name']

    for (vr, vms) in sorted(or_dict.items(), key=lambda x: x[0]):
        print "VR %s" % (vr)
        for val in sorted(vms):
            print "\t%s: %s (%s)" % val


if __name__ == "__main__":
    import sys
    main(sys.argv)
