#!/bin/bash

# Download the DocBook versions of PS3.6 and PS3.7 used by this script
if [ ! -e ../extern/DICOM/part07.xml ]
then
    curl -o ../extern/DICOM/part07.xml 'http://dicom.nema.org/medical/dicom/current/source/docbook/part07/part07.xml'
fi

if [ ! -e ../extern/DICOM/part06.xml ]
then
    curl -o ../extern/DICOM/part06.xml 'http://dicom.nema.org/medical/dicom/current/source/docbook/part06/part06.xml'
fi

echo '"Tag","Field Name","Keyword","VR","VM","Status"'
xsltproc ../xslt/docbook_to_table.xslt ../extern/DICOM/part07.xml ../extern/DICOM/part06.xml | sort -t , --key=1,2
