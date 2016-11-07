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
                xmlns:fn="http://www.w3.org/2005/xpath-functions">

    <xsl:output method="text" encoding="UTF-8" indent="no"
/>

<!-- This XSLT file outputs a comma-delimited table of DICOM network command
     elements from the DocBook version of the DICOM standard, PS3.7. 

     This file is a heavily modified version of Fleur Kelpin's original,
     available here: https://gist.github.com/fdlk/b52ef3af3b150a55314e
     However the output is in no way compatible with the output of the
     original script -->
<xsl:strip-space elements="*"/>
<xsl:template match="/">
    <!-- Registry of DICOM Command Elements -->
    <xsl:apply-templates select="//db:table[@label='E.1-1']/db:tbody"/>
    <!-- Retired Command Fields -->
    <xsl:apply-templates select="//db:table[@label='E.2-1']/db:tbody"/>
    <!-- Registry of DICOM File Meta Elements -->
    <xsl:apply-templates select="//db:table[@label='7-1']/db:tbody"/>
    <!-- Registry of DICOM Directory Structuring Elements -->
    <xsl:apply-templates select="//db:table[@label='8-1']/db:tbody"/>
    <!-- Registry of DICOM Data Elements -->
    <xsl:apply-templates select="//db:table[@label='6-1']/db:tbody"/>
</xsl:template>
<xsl:template match="//db:tr">
    <xsl:apply-templates/>
    <xsl:text>&#xA;</xsl:text>
</xsl:template>
<!-- Outputs the Tag group and ID in the from (GROUP,ITEM) -->
<xsl:template match="db:td[1]/db:para">
    <!-- Output a quotation mark -->
    <xsl:text>&quot;</xsl:text>
    <!-- Output the element with whitespace normalized -->
    <xsl:value-of select="normalize-space()"/>
    <!-- Output a quotation mark and delimiter -->
    <xsl:text>&quot;,</xsl:text>
</xsl:template>
<!-- Outputs the name of the data element -->
<xsl:template match="db:td[2]/db:para">
    <!-- Output a quotation mark -->
    <xsl:text>&quot;</xsl:text>
    <!-- Output the element with whitespace normalized -->
    <xsl:value-of select="normalize-space()"/>
    <!-- Output a quotation mark and delimiter -->
    <xsl:text>&quot;,</xsl:text>
</xsl:template>
<!-- Outputs a "keyword" for the data element -->
<xsl:template match="db:td[3]/db:para">
    <!-- Output a quotation mark -->
    <xsl:text>&quot;</xsl:text>
    <!-- Output the element with whitespace normalized and "zero width space"
         characters stripped out -->
    <xsl:value-of select="translate(normalize-space(),'&#x200b;','')"/>
    <!-- Output a quotation mark and delimiter -->
    <xsl:text>&quot;,</xsl:text>
</xsl:template>
<!-- Outputs a two character value representation for the data element -->
<xsl:template match="db:td[4]/db:para">
    <!-- Output a quotation mark -->
    <xsl:text>&quot;</xsl:text>
    <!-- Output the element with whitespace normalized -->
    <xsl:value-of select="normalize-space()"/>
    <!-- Output a quotation mark and delimiter -->
    <xsl:text>&quot;,</xsl:text>
</xsl:template>
<!-- Outputs the value multiplicity of the data element -->
<xsl:template match="db:td[5]/db:para">
    <!-- Output a quotation mark -->
    <xsl:text>&quot;</xsl:text>
    <!-- Output the element with whitespace normalized -->
    <xsl:value-of select="normalize-space()"/>
    <!-- Output a quotation mark and delimiter -->
    <xsl:text>&quot;,</xsl:text>
    <!-- If we're outputting the Retired fields table in PS3.7 -->
    <xsl:if test="ancestor::db:table[@label='E.2-1']">
        <!-- Output a "RET" field -->
        <xsl:text>&quot;RET&quot;</xsl:text>
    </xsl:if>
</xsl:template>
<!-- Consume the description column -->
<xsl:template match="db:td[6]">
    <xsl:choose>
        <!-- Tables in PS3.6 have column 6 as an indicator for retired fields.
             So output them -->
        <xsl:when test="ancestor::db:table[@label='7-1']/db:tbody">
            <!-- Output a quotation mark -->
            <xsl:text>&quot;</xsl:text>
            <!-- Output the element with whitespace normalized -->
            <xsl:value-of select="normalize-space()"/>
            <!-- Output a quotation mark -->
            <xsl:text>&quot;</xsl:text>
        </xsl:when>
        <xsl:when test="ancestor::db:table[@label='8-1']/db:tbody">
            <!-- Output a quotation mark -->
            <xsl:text>&quot;</xsl:text>
            <!-- Output the element with whitespace normalized -->
            <xsl:value-of select="normalize-space()"/>
            <!-- Output a quotation mark -->
            <xsl:text>&quot;</xsl:text>
        </xsl:when>
        <xsl:when test="ancestor::db:table[@label='6-1']/db:tbody">
            <!-- Output a quotation mark -->
            <xsl:text>&quot;</xsl:text>
            <!-- Output the element with whitespace normalized -->
            <xsl:value-of select="normalize-space()"/>
            <!-- Output a quotation mark -->
            <xsl:text>&quot;</xsl:text>
        </xsl:when>
        <!-- The Retired fields table in PS3.7 doesn't have a 6th column. The
             Command fields table uses the 6th column as a description colunn,
             which we don't use -->
        <xsl:otherwise>
        </xsl:otherwise>
    </xsl:choose>
</xsl:template>


</xsl:stylesheet>
