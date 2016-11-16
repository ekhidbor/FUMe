#!/usr/bin/python

from Cheetah.Template import Template

def parse_configs(config):
    val_map = { "string" : {}, "int" : {}, "long" : {}, "bool" : {} }

    for val in config:
        val_map[val['Type']][val['Key']] = val['Value']

    return (val_map['string'].items(), \
            val_map['int'].items(),    \
            val_map['long'].items(),   \
            val_map['bool'].items())


class gen_config_maps(Template):
    def __init__(self, config):
        self.__config_lists = parse_configs(config)

    def config_lists(self):
        return self.__config_lists

def main(args):
    from csv import DictReader
    from configuration_maps_cpp import configuration_maps_cpp

    # VR map auto-generated from the standard document
    config = DictReader(open(args[0], "r"))

    config_tmpl = configuration_maps_cpp(config)

    with open("../src/fume/configuration_maps.cpp", "w") as config_maps_file:
        config_maps_file.write(str(config_tmpl))

if __name__ == "__main__":
    import sys
    main(sys.argv[1:])
