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

<!-- This XSLT file outputs an XML version of the Message Exchange
     message exchange tables from the DocBook version of the DICOM standard,
     PS3.7. These are tags which, for a giveen service class, must be present
     in each message. Depending on SOP class there will be other tags which
     must be present in a message

     NOTE: This script may be fragile the way it determines which tables to
     extract fields from. It was developed against the 2016d version of the
     standard
-->
<xsl:strip-space elements="*"/>
<xsl:template match="/">
    <Headers>
        <!-- Tables -->
        <xsl:for-each select="//db:table[@label='7.1-1']">
            <Message Name="FILE">
                <xsl:apply-templates select="db:tbody"/>
            </Message>
        </xsl:for-each>
    </Headers>
</xsl:template>

<xsl:template match="db:tbody">
    <xsl:apply-templates select="db:tr"/>
</xsl:template>

<xsl:template match="db:tr">
    <xsl:variable name="tag" select="normalize-space(db:td[2]/db:para)"/>
    <xsl:variable name="type" select="normalize-space(db:td[3]/db:para)"/>
    <xsl:if test="not(contains($tag, 'No Tag'))">
        <Item Tag="{$tag}" Type="{$type}" VM="1">
        </Item>
    </xsl:if>
</xsl:template>

</xsl:stylesheet>
