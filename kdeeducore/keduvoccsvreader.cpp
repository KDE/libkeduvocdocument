/***************************************************************************

                    create a KEduVocDocument from a text file

    -----------------------------------------------------------------------

    begin        : Sat Sep 06 11:00:53 MET 1999

    copyright    : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                   (C) 2001 The KDE-EDU team
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

#include <QTextStream>

#include <klocale.h>

#include "keduvocdocument.h"
#include "keduvoccsvreader.h"

KEduVocCsvReader::KEduVocCsvReader(QIODevice *file)
{
  // the file must be already open
  m_inputFile = file;
  m_errorMessage = "";
}


bool KEduVocCsvReader::readDoc(KEduVocDocument *doc)
{
  m_doc = doc;

  QString separator = QString(",");

  QTextStream inputStream(m_inputFile);
  inputStream.setCodec("UTF-8");
  inputStream.setAutoDetectUnicode(true);
  inputStream.seek(0);

  int languageCount = 0;

  while (!inputStream.atEnd()) {
    QString s = inputStream.readLine();

    if (!s.simplified().isEmpty()) {
      KEduVocExpression expression(s, separator);
      languageCount = qMax(languageCount, expression.numTranslations() + 1);
      m_doc->appendEntry(&expression);
    }
  }

  for (int j = 0; j < languageCount; j++)
    if (j == 0)
      m_doc->appendIdentifier(i18n("Original"));
    else
      m_doc->appendIdentifier(i18n("Translation %1", j));

  return true;
}
