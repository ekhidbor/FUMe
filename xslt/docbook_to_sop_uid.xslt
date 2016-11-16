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
     from the DocBook version of the DICOM standard, PS3.4. 

      This script is almost certainly extremely fragile in how it outputs
      SOP UIDs and may require modification if used for anything other
      than the 2016d standard.
-->
<xsl:strip-space elements="*"/>
<xsl:template match="/">
    <!-- Tables -->
    <xsl:apply-templates select="//db:table"/>
    <xsl:apply-templates select="//db:section"/>
</xsl:template>

<xsl:template match="db:section">
    <xsl:if test="contains(normalize-space(db:title), 'SOP Class UID')">
    <xsl:if test="substring-after(normalize-space(db:title), 'SOP Class UID') = ''">
        <xsl:apply-templates select="db:para[1]"/>
    </xsl:if>
    </xsl:if>

    <xsl:if test="contains(normalize-space(db:title), 'SOP Class Definition and UID')">
        <xsl:apply-templates select="db:para[1]"/>
    </xsl:if>

    <xsl:if test="contains(normalize-space(db:title), 'SOP Class Definitions and UIDs')">
        <xsl:apply-templates select="db:para[1]"/>
    </xsl:if>
</xsl:template>

<xsl:template match="db:section/db:para[1]">
    <xsl:if test="contains(normalize-space(), '1.2.840.10008')">
        <!-- Output a quotation mark -->
        <xsl:text>&quot;</xsl:text>
        <xsl:value-of select="normalize-space(../@label)"/>
        <!-- Output a quotation mark and delimiter -->
        <xsl:text>&quot;,</xsl:text>

        <!-- Output a quotation mark -->
        <xsl:text>&quot;</xsl:text>
        <!-- Match SOP classes which don't contain 'shall' (most do contain shall) -->
        <xsl:if test="not(contains(normalize-space(), 'shall'))">
            <!-- If they don't contain 'shall' they contain 'is' -->
            <xsl:variable name="description" select="substring-after(substring-before(normalize-space(), ' is'), 'The ')"/>
            <!-- In one case we get the description from the parent title -->
            <xsl:if test="$description='SOP Class UID'">
                <xsl:value-of select="normalize-space(../db:title)"/>
            </xsl:if>
            <!-- Normally the description is in the paragraph -->
            <xsl:if test="$description!='SOP Class UID'">
                <xsl:value-of select="$description"/>
            </xsl:if>
        </xsl:if>
        <xsl:if test="contains(normalize-space(), 'shall')">
            <xsl:variable name="description" select="substring-after(substring-before(normalize-space(), ' shall'), 'The ')"/>
            <xsl:if test="$description='SOP Class UID'">
                <xsl:value-of select="normalize-space(../db:title)"/>
            </xsl:if>
            <xsl:if test="$description!='SOP Class UID'">
                <xsl:value-of select="$description"/>
            </xsl:if>
        </xsl:if>
        <!-- Output a quotation mark and delimiter -->
        <xsl:text>&quot;,</xsl:text>

        <!-- Output a quotation mark -->
        <xsl:text>&quot;</xsl:text>
        <!-- Most UIDs are contained in quotation marks -->
        <xsl:if test="contains(normalize-space(), '&quot;')">
            <xsl:value-of select="substring-before(substring-after(normalize-space(), '&quot;'), '&quot;.')"/>
        </xsl:if>
        <!-- Except one which isn't -->
        <xsl:if test="not(contains(normalize-space(), '&quot;'))">
            <xsl:value-of select="substring-before(substring-after(normalize-space(), 'to '), ' by')"/>
        </xsl:if>
        <!-- Output a quotation mark and newline -->
        <xsl:text>&quot;&#xA;</xsl:text>
    </xsl:if>
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
    <xsl:value-of select="normalize-space(../../../../../@label)"/>
    <!-- Output a quotation mark and delimiter -->
    <xsl:text>&quot;,</xsl:text>

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
