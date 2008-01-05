/***************************************************************************
                     create a KEduVocDocument from a XDXF file
    -----------------------------------------------------------------------
    copyright     : (C) 2007 Peter Hedlund <peter.hedlund@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvocxdxfreader.h"

#include <QIODevice>

#include <KLocale>

#include "keduvocexpression.h"
#include "keduvocdocument.h"

KEduVocXdxfReader::KEduVocXdxfReader( KEduVocDocument *doc )
{
    m_doc = doc;
}


bool KEduVocXdxfReader::read( QIODevice *device )
{
    setDevice( device );

    while ( !atEnd() ) {
        readNext();

        if ( isStartElement() ) {
            if ( name() == "xdxf" )
                readXdxf();
            else
                raiseError( i18n( "This is not a XDXF document" ) );
        }
    }

    return !error();
}


void KEduVocXdxfReader::readUnknownElement()
{
    while ( !atEnd() ) {
        readNext();

        if ( isEndElement() )
            break;

        if ( isStartElement() )
            readUnknownElement();
    }
}


void KEduVocXdxfReader::readXdxf()
{
    ///The language attributes are required and should be ISO 639-2 codes, but you never know...
    QStringRef id1 = attributes().value( "lang_from" );
    m_doc->appendIdentifier();
    if ( !id1.isNull() ) {
        m_doc->identifier(0).setLocale( id1.toString().toLower() );
        m_doc->identifier(0).setName( id1.toString().toLower() );
    }
    QStringRef id2 = attributes().value( "lang_to" );
    m_doc->appendIdentifier();
    if ( !id2.isNull() ) {
        m_doc->identifier(1).setLocale( id2.toString().toLower() );
        m_doc->identifier(1).setName( id2.toString().toLower() );
    }

    while ( !atEnd() ) {
        readNext();

        if ( isEndElement() )
            break;

        if ( isStartElement() ) {
            if ( name() == "description" )
                m_doc->setDocumentComment( readElementText() );
            else if ( name() == "full_name" )
                m_doc->setTitle( readElementText() );
            else if ( name() == "ar" )
                readEntry();
            else
                readUnknownElement();
        }
    }

    m_doc->setAuthor( "http://xdxf.sf.net" );
}


void KEduVocXdxfReader::readEntry()
{
    QString front;
    QString back;

    while ( !( isEndElement() && name() == "ar" ) ) {
        readNext();
        if ( isStartElement() && name() == "k" )
            front = readElementText();
        else if ( isCharacters() || isEntityReference() )
            back.append( text().toString() );
    }

    KEduVocExpression expr = KEduVocExpression( front );
    expr.setTranslation( 1, back );
    m_doc->lesson()->appendEntry( &expr );
}
