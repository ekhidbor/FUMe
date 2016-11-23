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
                xmlns:func="http://exslt.org/functions"
                xmlns:dcm="http://example.com"
                extension-element-prefixes="func"
                exclude-result-prefixes="db fn dcm" >

    <xsl:output method="xml" encoding="UTF-8" indent="yes"
/>

<xsl:include href="utility_functions.xslt" />

<!-- This XSLT file outputs an XML version of the IOD Attributes
     tables from the DocBook version of the DICOM standard, PS3.3. This
     script attempts to follow references to module tables and embed
     references to tables which exist in the XML file generated by the
     macro_to_xml.xslt. The idea then is that the output of this file could
     be added to the output of that transform to generate all the IOD
     definitions
-->
<xsl:strip-space elements="*"/>
<xsl:template match="/">
    <IODS>
        <!-- Tables -->
        <xsl:apply-templates select="//db:table[dcm:ends-with(db:caption, 'IOD Modules')]/db:tbody"/>
    </IODS>
</xsl:template>

<func:function name="dcm:get_table_id">
   <xsl:param name="section_id" />
   <!-- Some IOD section references don't cleanly map to a table. So Correct
        them
    -->
   <xsl:choose>
        <!-- ICC Profile -->
        <xsl:when test="$section_id = 'sect_C.11.15.1'">
            <func:result select="'table_C.11.15-1'" />
        </xsl:when>
        <!-- Implant Assembly -->
        <xsl:when test="$section_id = 'sect_C.29.2'">
            <func:result select="'table_C.29.2-1'" />
        </xsl:when>
        <!-- Implant Template Group -->
        <xsl:when test="$section_id = 'sect_C.29.3'">
            <func:result select="'table_C.29.3-1'" />
        </xsl:when>
        <!-- Specimen -->
        <xsl:when test="$section_id = 'sect_C.7.1.2'">
            <func:result select="'table_C.7.6.22-1'" />
        </xsl:when>
        <!-- Palette Color Lookup Table Data -->
        <xsl:when test="$section_id = 'sect_C.7.6.3.1.6'">
            <func:result select="'table_C.7.6.19-1'" />
        </xsl:when>
        <xsl:otherwise>
            <func:result select="//db:section[@xml:id = string($section_id)]/db:table/@xml:id" />
        </xsl:otherwise>
    </xsl:choose>
</func:function>

<xsl:template match="db:tbody">
    <!-- Use the xml:id to identify the Macro. Include directives use this
         as an identifier -->
    <IOD ID="{../@xml:id}" Name="{../db:caption}">
        <xsl:apply-templates select="db:tr/db:td[.//db:xref][1]"/>
    </IOD>
</xsl:template>

<xsl:template match="db:tr/db:td">
    <xsl:variable name="name" select="preceding-sibling::db:td" />
    <xsl:variable name="section_id" select=".//db:xref/@linkend" />
    <xsl:variable name="table_id" select="dcm:get_table_id($section_id)" />
    <Include Name="{$name}" ID="{$table_id}" />
</xsl:template>

</xsl:stylesheet>