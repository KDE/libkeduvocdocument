/***************************************************************************

                   create a KEduVocDocument from a Vokabeln file

    -----------------------------------------------------------------------

    begin        : Wed Jun 15 19:32:00 PDT 2005

    copyright    : (C) 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
                   (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvocvokabelnreader.h"

#include <QIODevice>

#include <klocale.h>
#include <kdebug.h>

#include "keduvocdocument.h"
#include "keduvocexpression.h"
#include "keduvocgrade.h"

KEduVocVokabelnReader::KEduVocVokabelnReader( QIODevice *file )
{
    // the file must be already open
    m_inputFile = file;
    m_errorMessage = "";
}


bool KEduVocVokabelnReader::readDoc( KEduVocDocument *doc )
{
    m_doc = doc;

    m_doc->setAuthor( "http://www.vokabeln.de" );

    QTextStream inputStream( m_inputFile );
    inputStream.setCodec( "ISO-8859-1" );
    inputStream.setAutoDetectUnicode( false );
    inputStream.seek( 0 );

    QString title;
    QString lang1;
    QString lang2;
    QString expression;
    QString original;
    QString translation;
    QString lessonDescr;
    QString temp;
    QString comment;

    int i;
    int wordCount;
    int lesson;

    int lines = 10000;

    QStringList titles;
    QStringList languages;
    QStringList words;

    temp = inputStream.readLine();

    if ( temp.startsWith ( QChar::fromLatin1 ( '"' ) ) ) {
        while ( temp != QChar::fromLatin1('0') ) {
            if ( title.isEmpty() ) {
                title = temp;
            } else {
                comment.append( temp.append('\n') );
            }
            temp = inputStream.readLine();
        }
    }

    // 1 line header
    if (comment.isEmpty()) {
        titles = title.split( "\"," );
        m_doc->setTitle(titles[0].mid(1));
    }
    // longer header
    else {
        titles = comment.split( "\"," );
        m_doc->setDocumentComment(titles[0]);
        m_doc->setTitle(title.mid(1));
    }

    wordCount = titles[1].section( ',', 0, 0 ).toInt();

    inputStream.readLine();

    lang1 = inputStream.readLine();
    languages = lang1.split( "\"," );

    m_doc->appendIdentifier();
    m_doc->identifier(0).setLocale( languages[0].mid( 1 ) );
    m_doc->identifier(0).setName( languages[0].mid( 1 ) );
    m_doc->appendIdentifier();
    m_doc->identifier(1).setLocale( languages[1].mid( 1 ) );
    m_doc->identifier(1).setName( languages[1].mid( 1 ) );

    while ( !temp.contains("8. Lernhilfe") ) {
        temp = inputStream.readLine(); //DO NOT translate
    }

    for ( i = 0; i <= 14; i++ ) {
        inputStream.readLine();
    }

    for ( i = 0; i < wordCount - 1; i++ ) {
        int c = 0;
        expression.clear();

        while ( c < 2 ) {
            temp = inputStream.readLine();
            c+= temp.count( "\",", Qt::CaseSensitive );
            expression.append( temp );
            if ( c < 2 ) {
                expression.append( " " );
            }
        }

        words = expression.split( "\"," );
        original = words[0].mid( 1 );
        translation = words[1].mid( 1 );
        lesson = words[2].toInt() - 1;

        KEduVocExpression kve;
        kve.setTranslation( 0, original );
        kve.setTranslation( 1, translation );
        kve.translation( 1 ).gradeFrom( 0 ).setGrade( 0 );
        kve.translation( 0 ).gradeFrom( 1 ).setGrade( 0 );
        kve.setLesson( lesson );

        m_doc->appendEntry( &kve );

        inputStream.readLine();
        inputStream.readLine();
    }

    inputStream.readLine();
    inputStream.readLine();
    inputStream.readLine();

    for ( int i = 0; !inputStream.atEnd() && i < lines; i++ ) {
        lessonDescr = inputStream.readLine();
        lessonDescr = lessonDescr.mid( 1, lessonDescr.length() - 2 );
        if ( !lessonDescr.isEmpty() )
            m_doc->appendLesson( lessonDescr );
        else
            break;
        inputStream.readLine();
    }

    return true;
}
