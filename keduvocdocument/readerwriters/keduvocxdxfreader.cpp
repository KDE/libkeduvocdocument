/*
 * create a KEduVocDocument from a XDXF file
 * SPDX-FileCopyrightText: 2007 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "keduvocxdxfreader.h"

#include <KLocalizedString>
#include <QIODevice>

#include "keduvocexpression.h"

KEduVocXdxfReader::KEduVocXdxfReader(QIODevice & dev)
    :m_dev( dev )
{
}

QString KEduVocXdxfReader::errorMessage() const
{
    return i18n( "Parse error at line %1, column %2:\n%3", lineNumber(), columnNumber(), errorString() );
}

bool KEduVocXdxfReader::isParsable()
{
    //@todo fix the xml isParsable to not expect lines as xml doesn't require lines
    QTextStream ts( &m_dev );
    QString line1( ts.readLine() );
    QString line2( ts.readLine() );

    m_dev.seek( 0 );
    return  ( ( line1.startsWith(QLatin1String("<?xml")) )
    && ( line2.indexOf( QLatin1String("xdxf"), 0 ) >  0 ) );
}

KEduVocDocument::FileType KEduVocXdxfReader::fileTypeHandled()
{
    return KEduVocDocument::Xdxf;
}

KEduVocDocument::ErrorCode KEduVocXdxfReader::read(KEduVocDocument &doc)
{
    QIODevice *device( &m_dev );
    m_doc = &doc;

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

    return error() ? KEduVocDocument::FileReaderFailed : KEduVocDocument::NoError;
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
    QStringRef id1 = attributes().value( QStringLiteral("lang_from") );
    m_doc->appendIdentifier();
    if ( !id1.isNull() ) {
        m_doc->identifier(0).setLocale( id1.toString().toLower() );
        m_doc->identifier(0).setName( id1.toString().toLower() );
    }
    QStringRef id2 = attributes().value( QStringLiteral("lang_to") );
    m_doc->appendIdentifier();
    if ( !id2.isNull() ) {
        m_doc->identifier(1).setLocale( id2.toString().toLower() );
        m_doc->identifier(1).setName( id2.toString().toLower() );
    }

    //Jam it all into one lesson
    KEduVocLesson* lesson = new KEduVocLesson(i18n("Lesson %1", 1), m_doc->lesson());
    m_doc->lesson()->appendChildContainer(lesson);

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

    m_doc->setAuthor( QStringLiteral("http://xdxf.sf.net") );
}


void KEduVocXdxfReader::readEntry()
{
    QString front;
    QString back;

    while ( !( isEndElement() && name() == "ar" ) ) {
        readNext();
        if ( isStartElement() && name() == "k" ) {
            front = readElementText();
        }
        else if ( isCharacters() || isEntityReference() ) {
            back.append( text().toString() );
        }
    }

    KEduVocExpression * expr = new KEduVocExpression( front );
    expr->setTranslation( 1, back );

    KEduVocLesson * lesson ( dynamic_cast<KEduVocLesson*>(m_doc->lesson()->childContainer(0) ) );
    if ( lesson ) {
        lesson->appendEntry( expr );
    }
}
