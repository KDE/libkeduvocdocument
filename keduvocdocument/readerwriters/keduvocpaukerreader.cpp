/*
 * create a KEduVocDocument from a Pauker file
 * SPDX-FileCopyrightText: 2004, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "keduvocpaukerreader.h"

#include <KLocalizedString>
#include <QIODevice>
#include <QTextStream>

#include "keduvocexpression.h"
#include "keduvoclesson.h"
#include <QDebug>

KEduVocPaukerReader::KEduVocPaukerReader( QIODevice & dev )
    :m_dev( dev )
{
}

QString KEduVocPaukerReader::errorMessage() const
{
    return i18n( "Parse error at line %1, column %2:\n%3", lineNumber(), columnNumber(), errorString() );
}

bool KEduVocPaukerReader::isParsable()
{
    //@todo fix the xml isParsable to not expect lines as xml doesn't require lines
    QTextStream ts( &m_dev );
    QString line1( ts.readLine() );
    QString line2( ts.readLine() );

    m_dev.seek( 0 );
    return ( ( line1.startsWith(QLatin1String("<?xml")) )
           && ( line2.indexOf( QLatin1String("pauker"), 0 ) >  0 ) );
}

KEduVocDocument::FileType KEduVocPaukerReader::fileTypeHandled()
{
    return KEduVocDocument::Pauker;
}

KEduVocDocument::ErrorCode KEduVocPaukerReader::read( KEduVocDocument &doc)
{
    m_doc = &doc;

    setDevice( &m_dev );

    while ( !atEnd() ) {
        readNext();

        if ( isStartElement() ) {
            if ( name() == "Lesson" )
                readPauker();
            else
            {
                qWarning() << i18n( "This is not a Pauker document" ) ;
                return KEduVocDocument::FileTypeUnknown;
            }
        }
    }

    return error() ? KEduVocDocument::FileReaderFailed : KEduVocDocument::NoError;
}


void KEduVocPaukerReader::readUnknownElement()
{
    while ( !atEnd() ) {
        readNext();

        if ( isEndElement() )
            break;

        if ( isStartElement() )
            readUnknownElement();
    }
}


void KEduVocPaukerReader::readPauker()
{
    m_doc->setAuthor( QStringLiteral("http://pauker.sf.net") );
    ///Pauker does not provide any column titles
    m_doc->appendIdentifier();
    m_doc->appendIdentifier();

    while ( !atEnd() ) {
        readNext();

        if ( isEndElement() )
            break;

        if ( isStartElement() ) {
            if ( name() == "Description" )
                m_doc->setDocumentComment( readElementText() );
            else if ( name() == "Batch" )
                readBatch();
            else
                readUnknownElement();
        }
    }
}


void KEduVocPaukerReader::readBatch()
{
    while ( !atEnd() ) {
        readNext();

        if ( isEndElement() )
            break;

        if ( isStartElement() ) {
            if ( name() == "Card" )
                readCard();
            else
                readUnknownElement();
        }
    }
}


void KEduVocPaukerReader::readCard()
{
    QString front;
    QString back;

    while ( !atEnd() ) {
        readNext();

        if ( isEndElement() )
            break;

        if ( isStartElement() ) {
            if ( name() == "FrontSide" )
                front = readText();
            else if ( name() == "ReverseSide" )
                back = readText();
            else
                readUnknownElement();
        }
    }

    KEduVocLesson* lesson = new KEduVocLesson(i18n("Vocabulary"), m_doc->lesson());
    m_doc->lesson()->appendChildContainer(lesson);

    KEduVocExpression* expr = new KEduVocExpression( QStringList() << front << back);
    lesson->appendEntry( expr );
}


QString KEduVocPaukerReader::readText()
{
    QString result;

    while ( !atEnd() ) {
        readNext();

        if ( isEndElement() )
            break;

        if ( isStartElement() ) {
            if ( name() == "Text" )
                result = readElementText();
            else
                readUnknownElement();
        }
    }
    return result;
}
