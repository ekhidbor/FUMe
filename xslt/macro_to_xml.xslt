<?xml version="1.0" encoding="UTF-8"?>
<!--
  This file is packaged as a part of the FUMe project.

  To the extent possible under law, the person who associated CC0 with
  FUMe has waived all copyright and related or neighboring rights
  to FUMe.

  You should have received a copy of the CC0 legalcode along with this
  work.  If not, see http://creativecommons.org/publicdomain/zero/1.0/.
-->
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:db="http://docbook.org/ns/docbook"
                xmlns:fn="http://www.w3.org/2005/xpath-functions"
                xmlns:dcm="http://example.com"
                exclude-result-prefixes="db fn dcm" >

    <xsl:output method="xml" encoding="UTF-8" indent="yes"
/>

<!-- This XSLT file outputs an XML version of the Macro Attributes
     tables from the DocBook version of the DICOM standard, PS3.3. The
     syntax of the tables in the standard is surprisingly complicated, as
     macros can "include" other macros, and there is a syntax for indicating
     when data elements are "children" of sequence elements in the macros. This
     script attempts to parse macro "includes" as well as build a hierarchy
     of items based on this syntax.

     NOTE: This script may be fragile the way it determines "include" directives
     and determines what is a macro table. But this is the most machine-readable
     form the standard is in, so this is what we're using. It was developed
     against the 2016d version of the standard
-->

<xsl:include href="ps3_3_functions.xslt" />

<xsl:strip-space elements="*"/>
<xsl:template match="/">
    <Macros>
        <!-- Tables -->
        <xsl:apply-templates select="//db:table/db:tbody"/>
    </Macros>
</xsl:template>

<xsl:template match="db:tbody">
    <!-- Macro Tables all have a caption which ends with "Macro Attributes" -->
    <xsl:if test="dcm:ends-with(normalize-space(../db:caption), 'Macro Attributes')">
    <!-- Some tables don't have the fourth column, indicating that such information
         should be obtained from PS3.4. So filter out those tables (for now)-->
    <xsl:if test="db:tr/db:td[4]/db:para">
        <!-- Use the xml:id to identify the Macro. Include directives use this
             as an identifier -->
        <Macro ID="{../@xml:id}" Name="{../db:caption}">
            <xsl:apply-templates select="db:tr[dcm:indentation(.) = 0]"/>
        </Macro>
    </xsl:if>
    </xsl:if>
</xsl:template>

</xsl:stylesheet>
