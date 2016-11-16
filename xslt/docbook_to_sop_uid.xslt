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

<!-- This XSLT file outputs a comma-delimited table of SOP class UIDs
     from the DocBook version of the DICOM standard, PS3.4. -->
<xsl:strip-space elements="*"/>
<xsl:template match="/">
    <!-- Tables -->
    <xsl:apply-templates select="//db:table"/>
</xsl:template>

<xsl:template match="db:table">
    <!-- Look for tables where the second column has a title of "SOP CLass UID" -->
    <xsl:if test="contains(normalize-space(db:thead/db:tr/db:th[2]/db:para/db:emphasis), 'SOP Class UID')">
        <xsl:apply-templates select="db:tbody/db:tr"/>
    </xsl:if>
</xsl:template>

<xsl:template match="//db:tr">
    <xsl:apply-templates/>
    <xsl:text>&#xA;</xsl:text>
</xsl:template>
<!-- Outputs the SOP Class Name -->
<xsl:template match="db:td[1]/db:para">
    <!-- Output a quotation mark -->
    <xsl:text>&quot;</xsl:text>
    <!-- Output the element with whitespace normalized -->
    <xsl:value-of select="normalize-space()"/>
    <!-- Output a quotation mark and delimiter -->
    <xsl:text>&quot;,</xsl:text>
</xsl:template>
<!-- Outputs the SOP Class UID -->
<xsl:template match="db:td[2]/db:para">
    <!-- Output a quotation mark -->
    <xsl:text>&quot;</xsl:text>
    <!-- Output the element with whitespace normalized and "zero width space"
         characters stripped out -->
    <xsl:value-of select="translate(normalize-space(),'&#x200b;','')"/>
    <!-- Output a quotation mark -->
    <xsl:text>&quot;</xsl:text>
</xsl:template>
<!-- Consume -->
<xsl:template match="db:td[3]/db:para">
</xsl:template>

</xsl:stylesheet>
