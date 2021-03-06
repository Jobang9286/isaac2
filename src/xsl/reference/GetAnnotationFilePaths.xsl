<?xml version="1.0"?>
<!--
/**
 ** Isaac Genome Alignment Software
 ** Copyright (c) 2010-2014 Illumina, Inc.
 ** All rights reserved.
 **
 ** This software is provided under the terms and conditions of the
 ** GNU GENERAL PUBLIC LICENSE Version 3
 **
 ** You should have received a copy of the GNU GENERAL PUBLIC LICENSE Version 3
 ** along with this program. If not, see
 ** <https://github.com/illumina/licenses/>.
 **
 ** \file GetSupportedSeedLengths.xsl
 **
 ** \author Roman Petrovski
 **/
-->
<xsl:stylesheet version="1.0" 
xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
>

<xsl:output method="text"/>

<xsl:template match="/"> 
    <xsl:for-each select="SortedReference/Annotations/Annotation/File">
        <xsl:value-of select="."/><xsl:text>
</xsl:text>
    </xsl:for-each>
</xsl:template>

</xsl:stylesheet>
