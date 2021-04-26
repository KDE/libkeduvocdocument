/*
 * export a KEduVocDocument to a delimited text file
 * SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
 * SPDX-FileCopyrightText: 2007 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef KEDUVOCCSVWRITER_H
#define KEDUVOCCSVWRITER_H

class QFile;
class QString;
class KEduVocDocument;

class KEduVocCsvWriter
{
public:
    explicit KEduVocCsvWriter( QFile *file );

    bool writeDoc( KEduVocDocument *doc, const QString &generator );

private:
    QFile *m_outputFile;
    KEduVocDocument *m_doc;
};

#endif
