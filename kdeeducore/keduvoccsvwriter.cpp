/***************************************************************************
                   export a KEduVocDocument to a delimited text file
    -----------------------------------------------------------------------
    copyright           : (C) 1999-2001 Ewald Arnold
                          (C) 2001 The KDE-EDU team
                          (C) 2007 Peter Hedlund <peter.hedlund@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QTextStream>

#include <KLocale>

#include "keduvoccsvwriter.h"
#include "keduvocdocument.h"
#include "keduvocexpression.h"

KEduVocCsvWriter::KEduVocCsvWriter(QFile *file)
{
  // the file must be already open
  m_outputFile = file;
}


bool KEduVocCsvWriter::writeDoc(KEduVocDocument *doc, const QString &generator)
{
  Q_UNUSED(generator);

  m_doc = doc;

  QString separator = m_doc->csvDelimiter();;

  QTextStream outputStream;
  outputStream.setDevice(m_outputFile);
  outputStream.setCodec("UTF-8");

  outputStream << i18n("! Title:")  << separator << m_doc->title() << "\n";
  outputStream << i18n("! Author:") << separator << m_doc->author() << "\n";

  KEduVocExpression *expression;
  int idCount = m_doc->identifierCount();
  QString currentRow;

  for (int e = 0; e < m_doc->entryCount(); e++)
  {
    expression = m_doc->entry(e);
    currentRow = "";
    bool sep = false;

    for (int i = 0; i < idCount; i++) {
      if (!sep)
        sep = true;
      else
        currentRow += separator;

      if (i == 0)
        currentRow += expression->original();
      else
        currentRow += expression->translation(i);
    }

    if (!currentRow.isEmpty())
      outputStream << currentRow << "\n";
  }

  return true;
}

