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
                xmlns:set="http://exslt.org/sets"
                xmlns:dcm="http://example.com"
                extension-element-prefixes="func set"
                exclude-result-prefixes="db fn dcm" >

    <xsl:output method="xml" encoding="UTF-8" indent="yes"
/>

<!-- This XSLT file contains shared functions used to generate an XML version
     of the Macro and Module Attributes tables from the DocBook version of the
     DICOM standard, PS3.3. The syntax of the tables in the standard is surprisingly complicated, as macros can "include" other macros, and there
     is a syntax for indicating when data elements are "children" of sequence
     elements in the macros. This script attempts to parse macro "includes" as
     well as build a hierarchy of items based on this syntax.

     NOTE: This script may be fragile the way it determines "include" directives
     and determines what is a macro table. But this is the most machine-readable
     form the standard is in, so this is what we're using. It was developed
     against the 2016d version of the standard
-->

<xsl:include href="utility_functions.xslt" />

<xsl:strip-space elements="*"/>

<func:function name="dcm:indentation">
   <xsl:param name="row" />
   <xsl:choose>
        <!-- For "Include" rows, the first column always has an xref element
             inside an emphasis element. It also has a colspan of 4, but
             we don't need to check that -->
        <xsl:when test="$row/db:td[1]/db:para/db:emphasis/db:xref">
            <func:result select="string-length(db:td[1]/db:para/db:emphasis) - string-length(translate(db:td[1]/db:para/db:emphasis, '&gt;', ''))" />
        </xsl:when>
        <xsl:when test="$row/db:td[@colspan = '1'][1]/db:para">
            <func:result select="string-length(db:td[1]/db:para) - string-length(translate(db:td[1]/db:para, '&gt;', ''))" />
        </xsl:when>
    </xsl:choose>
</func:function>

<xsl:template match="db:tr">
    <xsl:choose>
        <!-- For "Include" rows, the first column always has an xref element
             inside an emphasis element. It also has a colspan of 4, but
             we don't need to check that -->
        <xsl:when test="db:td[1]/db:para/db:emphasis/db:xref">
            <xsl:variable as="xs:string" name="link_id" select="db:td[1]/db:para/db:emphasis/db:xref/@linkend"/>
            <Include ID="{$link_id}" />
        </xsl:when>
        <!-- For "Item" rows, the first column has a colspan of 1. We
             also set the indentation for Item elements because some
             macro definitions have indented ones. Also need to recursively
             apply templates in the case of an enumerated item -->
        <xsl:when test="db:td[@colspan = '1'][1]/db:para">
            <xsl:variable name="name" select="normalize-space(translate(db:td[1]/db:para, '&gt;', ''))" />
            <xsl:variable name="indentation" select="dcm:indentation(.)"/>
            <xsl:variable name="tag" select="normalize-space(db:td[2]/db:para)"/>
            <xsl:variable name="type" select="normalize-space(db:td[3]/db:para)"/>

            <xsl:if test="dcm:ends-with($name, 'Sequence')">
                <xsl:variable name="next" select="following-sibling::db:tr[dcm:indentation(.) = $indentation]"/>
                <xsl:variable name="lower" select="following-sibling::db:tr[(dcm:indentation(.) = $indentation) or (dcm:indentation(.) = ($indentation + 1))]"/>
                <Sequence Name="{$name}" Tag="{$tag}" Type="{$type}">
                    <!-- Get the elements with an indentation level one greater
                         than that of the current indentation level which preceed
                         the next element with the same indentation level as that
                         of the current element.
                    -->
                    <xsl:apply-templates select="set:leading($lower, $next)"/>
                </Sequence>
            </xsl:if>
            <xsl:if test="not(dcm:ends-with($name, 'Sequence'))">

                <Item Name="{$name}" Tag="{$tag}" Type="{$type}">
                      <!-- Enumerated values are defined by variablelist elements
                           in the fourth column -->
                      <xsl:apply-templates select="db:td[4]/db:variablelist"/>
                </Item>
            </xsl:if>
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
