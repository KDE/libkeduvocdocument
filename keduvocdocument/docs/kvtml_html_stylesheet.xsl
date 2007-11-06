<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<!-- XSL Stylesheet to transform kvtml-2 files to html
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

Copyright 2007: Frederik Gladhorn <frederik.gladhorn@kdemail.net>

The easiest way to use the stylesheet is to include it in the .kvtml file:
 <?xml-stylesheet type="text/xsl" href="kvtml_style.xsl"?>

 -->

<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 3.2 Final//EN"/>

<xsl:template match="/">

 <html>
 <style type="text/css">
  tr[languageheader] {
	color: black;
	font-size: 130%;
	background-color: lightblue
	}
 </style>

 <head>
  <title>
    <xsl:value-of select="kvtml/information/title"/>
  </title>
 </head>

 <body>
   <h2><xsl:value-of select="kvtml/information/title"/></h2>
   <table border="1">
     <tr languageheader="true">
        <xsl:for-each select="kvtml/identifiers/identifier">
          <td><xsl:value-of select="name"/></td>
        </xsl:for-each>
     </tr>
      <xsl:for-each select="kvtml/entries/entry">
      <tr>
        <xsl:for-each select="translation">
          <td><xsl:value-of select="text"/></td>
        </xsl:for-each>
      </tr>
      </xsl:for-each>
   </table>

 </body>
 </html>
</xsl:template>


</xsl:stylesheet>






