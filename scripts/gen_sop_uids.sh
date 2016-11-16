#!/bin/bash

# Download the DocBook versions of PS3.4 used by this script
if [ ! -e ../extern/DICOM/part04.xml ]
then
    curl -o ../extern/DICOM/part04.xml 'http://dicom.nema.org/medical/dicom/current/source/docbook/part04/part04.xml'
fi

echo '"Section","SOP Class Description","SOP Class UID"'
xsltproc ../xslt/docbook_to_sop_uid.xslt ../extern/DICOM/part04.xml | sort -t , --key=2,3
