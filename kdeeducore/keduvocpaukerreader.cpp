/***************************************************************************
                     create a KEduVocDocument from a Pauker file
    -----------------------------------------------------------------------
    copyright     : (C) 2004, 2007 Peter Hedlund <peter.hedlund@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <klocale.h>

#include "keduvocpaukerreader.h"
#include "keduvocexpression.h"
#include "keduvocdocument.h"

KEduVocPaukerReader::KEduVocPaukerReader(QIODevice * file)
{
  // the file must be already open
  m_inputFile = file;
  m_errorMessage = "";
}


bool KEduVocPaukerReader::readDoc(KEduVocDocument * doc)
{
  m_doc = doc;
  QString front;
  QString back;

  QDomDocument domDoc("Pauker");
  if (!domDoc.setContent(m_inputFile, &m_errorMessage))
    return false;

  QDomElement description = domDoc.documentElement().firstChildElement("Description");
  if(!description.isNull())
    m_doc->setDocumentRemark(description.text());

  QDomNodeList entries = domDoc.elementsByTagName("Card");

  if (entries.count() <= 0) {
    m_errorMessage = i18n("Error while reading file");
    return false;
  }

  m_doc->setAuthor("http://pauker.sf.net");
  ///Pauker does not provide any column titles
  m_doc->appendIdentifier(i18n("Front Side"));
  m_doc->appendIdentifier(i18n("Reverse Side"));

  for (int i = 0; i < entries.count(); i++) {
    QDomNode entry = entries.at(i);
    if(!entry.isNull()) {
      front = cardText(entry, "FrontSide");
      back = cardText(entry, "ReverseSide");
      KEduVocExpression expr = KEduVocExpression(front);
      expr.setTranslation(1, back);
      m_doc->appendEntry(&expr);
    }
  }
  return true;
}


QString KEduVocPaukerReader::cardText(const QDomNode & entry, const QString & tagName) const
{
  QDomElement element = entry.firstChildElement(tagName);

  if(!element.isNull())
    return element.text();
  else
    return QString();
}

