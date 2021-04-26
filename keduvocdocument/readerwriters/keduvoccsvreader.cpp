/*
 * create a KEduVocDocument from a text file
 * SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
 * SPDX-FileCopyrightText: 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "keduvoccsvreader.h"

#include <KLocalizedString>
#include <QIODevice>
#include <QTextStream>

#include "keduvoclesson.h"
#include "keduvocexpression.h"

KEduVocCsvReader::KEduVocCsvReader( QIODevice & file )
{
    // the file must be already open
    m_inputFile = &file;

    m_errorMessage = QLatin1String("");
}

KEduVocDocument::FileType KEduVocCsvReader::fileTypeHandled()
{
    return KEduVocDocument::Csv;
}

KEduVocDocument::ErrorCode KEduVocCsvReader::read(KEduVocDocument & doc )
{
    m_doc = &doc;

    QString separator = m_doc->csvDelimiter();

    QTextStream inputStream( m_inputFile );
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    inputStream.setCodec( "UTF-8" );
#endif
    inputStream.setAutoDetectUnicode( true );
    inputStream.seek( 0 );

    int languageCount = 0;

    KEduVocLesson* lesson = new KEduVocLesson( i18n("Vocabulary"), m_doc->lesson());
    m_doc->lesson()->appendChildContainer(lesson);

    while ( !inputStream.atEnd() ) {
        QString s = inputStream.readLine();

        if ( !s.simplified().isEmpty() ) {
            KEduVocExpression* expression = new KEduVocExpression( s.split(separator) );
            languageCount = qMax( languageCount,
                expression->translationIndices().count() );
            lesson->appendEntry( expression );
        }
    }

    for ( int j = 0; j < languageCount; j++ )
    {
        m_doc->appendIdentifier();
    }

    return KEduVocDocument::NoError;
}
