#!/bin/bash

cheetah-2.7 compile ../cheetah/*.tmpl
mv ../cheetah/*.py ./

exec ./gen_config_maps.py ../definitions/config.csv