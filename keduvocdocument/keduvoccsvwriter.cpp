/*
 * export a KEduVocDocument to a delimited text file
 * SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
 * SPDX-FileCopyrightText: 2007 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "keduvoccsvwriter.h"

#include <KLocalizedString>
#include <QFile>
#include <QTextStream>

#include "keduvocdocument.h"
#include "keduvoclesson.h"
#include "keduvocexpression.h"

KEduVocCsvWriter::KEduVocCsvWriter( QFile *file )
    : m_outputFile(file) // the file must be already open
    , m_doc(nullptr)
{
}

bool KEduVocCsvWriter::writeDoc( KEduVocDocument *doc, const QString &generator )
{
    Q_UNUSED( generator );

    m_doc = doc;

    QString separator = m_doc->csvDelimiter();

    QTextStream outputStream;
    outputStream.setDevice( m_outputFile );
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    outputStream.setCodec( "UTF-8" );
#endif

    outputStream << i18nc( "@item:intable the title of the document will be written here", "Title:" )  << separator << m_doc->title() << "\n";
    outputStream << i18nc( "@item:intable the author will be written here", "Author:" ) << separator << m_doc->author() << "\n";

    KEduVocExpression *expression;
    int idCount = m_doc->identifierCount();
    QString currentRow;

    for ( int e = 0; e < m_doc->lesson()->entryCount(KEduVocLesson::Recursive); e++ ) {
        expression = m_doc->lesson()->entries(KEduVocLesson::Recursive).value( e );
        currentRow = QLatin1String("");
        bool sep = false;

        for ( int i = 0; i < idCount; i++ ) {
            if ( !sep )
                sep = true;
            else
                currentRow += separator;

            currentRow += expression->translation( i )->text();
        }

        if ( !currentRow.isEmpty() )
            outputStream << currentRow << "\n";
    }

    return true;
}
