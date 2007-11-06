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
 <?xml-stylesheet type="text/xsl" href="kvtml_html_style.xsl"?>

 -->

<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 3.2 Final//EN"/>

	<xsl:template match="/">
	<html>
	<style type="text/css">
		td, table {
			border: solid 1px black;
			border-collapse: collapse;
		}
		tr[languageheader] {
			color: black;
			font-size: 130%;
			background-color: lightblue;
		}
	
	</style>
	
	<head>
		<title><xsl:value-of select="kvtml/information/title"/></title>
	</head>
	
	<body><xsl:apply-templates select="kvtml" /></body>
	</html>
	</xsl:template>

	<xsl:template match="kvtml">
		<h2><xsl:value-of select="information/title"/></h2>
		<xsl:apply-templates select="lessons"/>
	</xsl:template>


	<xsl:template match="lessons">
		<xsl:apply-templates select="lesson"/>
	</xsl:template>
 
	<xsl:template match="lesson">
		<xsl:value-of select="name"/>
		<table border="1">
			<tr languageheader="true">
				<xsl:apply-templates select="/kvtml/identifiers"/>
			</tr>
			<xsl:apply-templates select="entries/entry"/>

			<xsl:for-each select="entryid">
				<xsl:variable name="id" select="."/>
				<xsl:for-each select="/kvtml/entries/entry[@id=$id]">
					<xsl:apply-templates select="."/>
				</xsl:for-each>
			</xsl:for-each>	
		</table>
	</xsl:template>
	
	<xsl:template match="identifiers/identifier">
		<td><xsl:value-of select="name"/></td>
	</xsl:template>
	
	<xsl:template match="entry">
		<tr><xsl:apply-templates select="translation"/></tr>
	</xsl:template>
	
	<xsl:template match="translation">
		<td><xsl:value-of select="text"/></td>
	</xsl:template>

</xsl:stylesheet>






