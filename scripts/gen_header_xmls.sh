#!/bin/bash

# Download the DocBook versions of PS3.7 used by this script
if [ ! -e ../extern/DICOM/part07.xml ]
then
    curl -o ../extern/DICOM/part07.xml 'http://dicom.nema.org/medical/dicom/current/source/docbook/part07/part07.xml'
fi

# Download the DocBook versions of PS3.4 used by this script
if [ ! -e ../extern/DICOM/part10.xml ]
then
    curl -o ../extern/DICOM/part10.xml 'http://dicom.nema.org/medical/dicom/current/source/docbook/part10/part10.xml'
fi

xsltproc -o ../definitions/file_header.xml ../xslt/file_header_to_xml.xslt ../extern/DICOM/part10.xml
xsltproc -o ../definitions/message_headers.xml ../xslt/message_headers_to_xml.xslt ../extern/DICOM/part07.xml
