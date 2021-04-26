<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<!-- XSL Stylesheet to transform kvtml-2 files to html

SPDX-License-Identifier: GPL-2.0-or-later
SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

The easiest way to use the stylesheet is to include it in the .kvtml file:
<?xml-stylesheet type="text/xsl" href="kvtml_html_stylesheet.xsl.xsl"?>

 -->

<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 3.2 Final//EN"/>

	<xsl:template match="/">
	<html>
	<style type="text/css">
		td, table {
			border: solid 1px black;
			border-collapse: collapse;
			width: <xsl:value-of select="100 div count(/kvtml/identifiers/identifier)"/>%;
		}
		tr[languageheader] {
			color: black;
			font-size: 1.5em;
			background-color: lightblue;
		}
		.comment {
			color: blue;
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
		<xsl:for-each select="lesson">
			<a href="#{name}"><xsl:value-of select="name"/></a>
			<br/>
		</xsl:for-each>

		<xsl:apply-templates select="lesson"/>
	</xsl:template>
 
	<xsl:template match="lesson">
		<h3>Lesson: <a name="{name}"><xsl:value-of select="name"/></a></h3>
		<table border="1">
			<tr languageheader="true">
				<xsl:apply-templates select="/kvtml/identifiers"/>
			</tr>
			<xsl:apply-templates select="entries/entry"/>

			<xsl:for-each select="entryid">
				<xsl:variable name="id" select="."/>
				<xsl:apply-templates select="/kvtml/entries/entry[@id=$id]"/>
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
		<td><xsl:value-of select="text"/>
			<xsl:apply-templates select="comment"/>
		</td>
	</xsl:template>

	<xsl:template match="comment">
		<span class="comment">
			<br/><xsl:value-of select="."/>
		</span>
	</xsl:template>
</xsl:stylesheet>






