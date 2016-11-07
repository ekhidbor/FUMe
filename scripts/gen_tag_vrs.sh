#!/bin/bash

cheetah-2.7 compile ../cheetah/*.tmpl
mv ../cheetah/*.py ./

exec ./gen_tag_vrs.py ../definitions/diction_standard.csv ../definitions/diction_system.csv