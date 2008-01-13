/***************************************************************************

                    create a KEduVocDocument from a text file

    -----------------------------------------------------------------------

    begin        : Sat Sep 06 11:00:53 MET 1999

    copyright    : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                   (C) 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include "keduvoccsvreader.h"

#include <QIODevice>
#include <QTextStream>

#include <klocale.h>

#include "keduvocdocument.h"
#include "keduvoclesson.h"
#include "keduvocexpression.h"

KEduVocCsvReader::KEduVocCsvReader( QIODevice *file )
{
    // the file must be already open
    m_inputFile = file;
    m_errorMessage = "";
}


bool KEduVocCsvReader::readDoc( KEduVocDocument *doc )
{
    m_doc = doc;

    QString separator = m_doc->csvDelimiter();

    QTextStream inputStream( m_inputFile );
    inputStream.setCodec( "UTF-8" );
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

    return true;
}
