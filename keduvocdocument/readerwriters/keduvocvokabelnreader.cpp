/*
 * create a KEduVocDocument from a Vokabeln file
 *
 * SPDX-FileCopyrightText: 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "keduvocvokabelnreader.h"

#include <KLocalizedString>
#include <QIODevice>

#include <QDebug>

#include "keduvocexpression.h"

KEduVocVokabelnReader::KEduVocVokabelnReader(QIODevice & file )
    : m_inputFile( &file )
{
    m_errorMessage = QLatin1String("");
}

bool KEduVocVokabelnReader::isParsable()
{
    QTextStream ts( m_inputFile );
    QString line1( ts.readLine() );
    QString line2( ts.readLine() );
    /*
     * Vokabeln.de files:
    The header seems to be something like this:

    "Name
    Lang1 - Lang2",123,234,456
    0

    or something longer:

    "Name
    Lang1 - Lang2
    [..]
    Blah, blah, blah...",123,234,456
    0
    */

    QString tmp;
    bool isgood = false;

    if ( line1.startsWith(QChar::fromLatin1('"'))) {
        ts.seek(0);
        tmp = ts.readLine();
        // There shouldn't be headers longer than 10 lines.
        for ( int x=0; x < 10; x++) {
            if (tmp.contains( QLatin1String("\",") )) {
                tmp = ts.readLine();
                if (tmp.endsWith('0')) {
                    isgood = true;
                    break;
                }
            }
            tmp = ts.readLine();
        }
    }

    m_inputFile->seek( 0 );
    return isgood;
}

KEduVocDocument::FileType KEduVocVokabelnReader::fileTypeHandled()
{
    return KEduVocDocument::Vokabeln;
}

KEduVocDocument::ErrorCode KEduVocVokabelnReader::read(KEduVocDocument & doc )
{
    qDebug() << "Reading vokabeln.de document...";
    m_doc = &doc;

    m_doc->setAuthor( QStringLiteral("http://www.vokabeln.de") );

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
    QString temp;
    QString comment;

    int i;
    int wordCount;
    int lessonNumber;
    int maxLessonNumber = -1;

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

            if ( inputStream.atEnd() ) {
                m_errorMessage = i18n( "Error while reading file: Truncated header" );
                return KEduVocDocument::FileReaderFailed;
            }

            temp = inputStream.readLine();
        }
    } else {
        m_errorMessage = i18n( "Error while reading file: No title" );
        return KEduVocDocument::FileReaderFailed;
    }

    // 1 line header
    if (comment.isEmpty()) {
        titles = title.split( QStringLiteral("\",") );
        m_doc->setTitle(titles[0].mid(1));
        qDebug() << "TitleA "<< m_doc->title();
    }
    // longer header
    else {
        titles = comment.split( QStringLiteral("\",") );
        m_doc->setDocumentComment(titles[0]);
        m_doc->setTitle(title.mid(1));
        qDebug() << "TitleB "<< m_doc->title();
    }

    wordCount = titles[1].section( ',', 0, 0 ).toInt();
    qDebug() << "WordCount "<< wordCount;

    inputStream.readLine();

    lang1 = inputStream.readLine();
    languages = lang1.split( QStringLiteral("\",") );

    if ( languages.size() < 2 ) {
        m_errorMessage = i18n( "Error while reading file: Didn't find two languages in %1", lang1 );
        return KEduVocDocument::FileReaderFailed;
    }

    m_doc->appendIdentifier();
    QString language = languages[0].mid( 1 );
    m_doc->identifier(0).setLocale(language);
    m_doc->identifier(0).setName(language);
    qDebug() << "First language: " << language;

    m_doc->appendIdentifier();
    language = languages[1].mid( 1 );
    m_doc->identifier(1).setLocale(language);
    m_doc->identifier(1).setName(language);
    qDebug() << "Second language: " << language;

    QString section8Header( QStringLiteral("8. Lernhilfe") ); //DO NOT translate
    while ( !temp.contains(section8Header) ) {
        if ( inputStream.atEnd() ) {
            m_errorMessage = i18n( "Error while reading file: Missing \"%1\"", section8Header );
            return KEduVocDocument::FileReaderFailed;
        }
        temp = inputStream.readLine();
    }

    for ( i = 0; i <= 14; ++i ) {
        inputStream.readLine();
    }


    for ( i = 0; i < wordCount - 1; ++i ) {
        int c = 0;
        expression.clear();

        while ( c < 2 ) {
            if ( inputStream.atEnd() ) {
                m_errorMessage = i18n( "Error while reading file: Expecting something like \"dog\",\"Hund\",1" );
                return KEduVocDocument::FileReaderFailed;
            }
            temp = inputStream.readLine();
            c+= temp.count( QStringLiteral("\","), Qt::CaseSensitive );
            expression.append( temp );
            if ( c < 2 ) {
                expression.append( ' ' );
            }
        }

        words = expression.split( QStringLiteral("\",") );
        original = words[0].mid( 1 );
        translation = words[1].mid( 1 );
        lessonNumber = words[2].toInt() - 1;
        maxLessonNumber = qMax( lessonNumber,  maxLessonNumber );

        qDebug() << "Reading entry: " << original << " - " << translation << " Lesson: " << lessonNumber;

        // fallback if it's not read correctly
        if (lessonNumber < 0) {
            qDebug() << "Warning, invalid lesson found!";
            lessonNumber = 0;
        }

        while(m_doc->lesson()->childContainerCount() <= lessonNumber) {
            KEduVocLesson* lesson = new KEduVocLesson(i18n("Lesson %1", lessonNumber), m_doc->lesson());
            m_doc->lesson()->appendChildContainer(lesson);
            qDebug() << "Created lesson " << lessonNumber;
        }

        KEduVocExpression* kve = new KEduVocExpression;
        kve->setTranslation( 0, original );
        kve->setTranslation( 1, translation );

        static_cast<KEduVocLesson*>(m_doc->lesson()->childContainer(lessonNumber))->appendEntry(kve);

        inputStream.readLine();
        inputStream.readLine();
    }

    inputStream.readLine();
    inputStream.readLine();
    inputStream.readLine();

    int ii = 0;
    while ( !inputStream.atEnd() && ii < lines && ii <= maxLessonNumber ) {
        QString lessonDescr = inputStream.readLine();
        qDebug() << "Found lesson description " << lessonDescr;
        lessonDescr = lessonDescr.mid( 1, lessonDescr.length() - 2 );
        m_doc->lesson()->childContainer(ii)->setName(lessonDescr);
        if ( lessonDescr.isEmpty() ) {
            break;
        }
        inputStream.readLine();
        ++ii;
    }

    return KEduVocDocument::NoError;
}
