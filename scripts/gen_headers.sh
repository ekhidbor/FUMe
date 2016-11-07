#!/bin/bash

cheetah-2.7 compile ../cheetah/*.tmpl
mv ../cheetah/*.py ./

if [ ! -e "../definitions/diction_standard.csv" ]
then
    ./gen_dictionary.sh > ../definitions/diction_standard.csv
fi

exec ./gen_headers.py ../definitions/diction_standard.csv ../definitions/mcstatus.csv
