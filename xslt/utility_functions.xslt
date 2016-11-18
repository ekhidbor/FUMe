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
                xmlns:fn="http://www.w3.org/2005/xpath-functions"
                xmlns:func="http://exslt.org/functions"
                xmlns:dcm="http://example.com"
                extension-element-prefixes="func"
                exclude-result-prefixes="fn" >

    <xsl:output method="xml" encoding="UTF-8" indent="yes"
/>

<func:function name="dcm:ends-with">
    <xsl:param name="str" />
    <xsl:param name="end" />
    <xsl:if test="contains($str, $end)">
    <xsl:if test="substring-after($str, $end) = ''">
        <func:result select="true()"/>
    </xsl:if>
    <xsl:if test="not(substring-after($str, $end) = '')">
        <func:result select="false()"/>
    </xsl:if>
    </xsl:if>
    <xsl:if test="not(contains($str, $end))">
        <func:result select="false()"/>
    </xsl:if>
</func:function>

</xsl:stylesheet>
