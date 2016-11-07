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
                exclude-result-prefixes="db fn" >

    <xsl:output method="xml" encoding="UTF-8" indent="yes"
/>

<!-- This XSLT file outputs an XML version of the Module Attributes
     tables from the DocBook version of the DICOM standard, PS3.3. The
     syntax of the tables in the standard is surprisingly complicated, as
     macros can "include" other macros, and there is a syntax for indicating
     when data elements are "children" of sequence elements in the macros. 
     The "child element" syntax, implemented with "greater-than" characters,
     would be difficult (for me) to implement in XSLT. So for now we just
     use the XSLT to generate an "indentation level" within the macro
     definition. A subsequent python script will read in this XML file
     and generate the hierarchy

     NOTE: This script may be fragile the way it determines "include" directives
     and determines what is a macro table. But this is the most machine-readable
     for the standard is in, so this is what we're using. It was developed
     against the 2016d version of the standard -->
<xsl:strip-space elements="*"/>
<xsl:template match="/">
    <Macros>
        <!-- Tables -->
        <xsl:apply-templates select="//db:table/db:tbody"/>
    </Macros>
</xsl:template>

<xsl:template match="db:tbody">
    <!-- Module Tables all have a caption which ends with "Module Attributes" -->
    <xsl:if test="contains(normalize-space(../db:caption), 'Module Attributes')">
    <xsl:if test="substring-after(normalize-space(../db:caption), 'Module Attributes') = ''">
    <!-- Some tables don't have the fourth column, indicating that such information
         should be obtained from PS3.4. So filter out those tables (for now)-->
    <xsl:if test="db:tr/db:td[4]/db:para">
        <!-- Use the xml:id to identify the Macro. Include directives use this
             as an identifier -->
        <Module ID="{../@xml:id}" Name="{../db:caption}">
            <xsl:apply-templates select="db:tr"/>
        </Module>
    </xsl:if>
    </xsl:if>
    </xsl:if>
</xsl:template>

<xsl:template match="db:tr">
    <xsl:choose>
        <!-- For "Include" rows, the first column always has an xref element
             inside an emphasis element. It also has a colspan of 4, but
             we don't need to check that -->
        <xsl:when test="db:td[1]/db:para/db:emphasis/db:xref">
            <!-- Strip out greater than characters and compare the against
                 the original string length to determine indentation level.
                 A bit hack-ish but it works.
                 Also reference the xml:id of the macro being included
                 so other software can "embed" the macro in the structure -->
            <Include Indentation="{string-length(db:td[1]/db:para/db:emphasis) - string-length(translate(db:td[1]/db:para/db:emphasis, '&gt;', ''))}"
                     Entry="{db:td[1]/db:para/db:emphasis/db:xref/@linkend}" />
        </xsl:when>
        <!-- For "Item" rows, the first column has a colspan of 1. We
             also set the indentation for Item elements because some
             macro definitions have indented ones. Also need to recursively
             apply templates in the case of an enumerated item -->
        <xsl:when test="db:td[@colspan = '1'][1]/db:para">
            <Item Indentation="{string-length(db:td[1]/db:para) - string-length(translate(db:td[1]/db:para, '&gt;', ''))}"
                  Name="{normalize-space(translate(db:td[1]/db:para, '&gt;', ''))}"
                  Tag="{normalize-space(db:td[2]/db:para)}"
                  Type="{normalize-space(db:td[3]/db:para)}">
                  <!-- Enumerated values are defined by variablelist elements
                       in the fourth column -->
                  <xsl:apply-templates select="db:td[4]/db:variablelist"/>
            </Item>
        </xsl:when>
    </xsl:choose>
</xsl:template>

<!-- Generate enumerated values -->
<xsl:template match="db:variablelist[@spacing='compact']">
    <xsl:apply-templates select="db:varlistentry/db:term"/>
</xsl:template>

<xsl:template match="db:varlistentry/db:term">
    <ValidVal>
        <xsl:value-of select="normalize-space()"/>
    </ValidVal>
</xsl:template>

</xsl:stylesheet>
