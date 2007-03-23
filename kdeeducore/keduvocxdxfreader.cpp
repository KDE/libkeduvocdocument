/***************************************************************************
                     create a KEduVocDocument from a XDXF file
    -----------------------------------------------------------------------
    copyright     : (C) 2007 Peter Hedlund <peter.hedlund@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QIODevice>

#include <klocale.h>

#include "keduvocxdxfreader.h"
#include "keduvocexpression.h"
#include "keduvocdocument.h"

KEduVocXdxfReader::KEduVocXdxfReader(QIODevice * file)
{
  // the file must be already open
  m_inputFile = file;
  m_errorMessage = "";
}


bool KEduVocXdxfReader::readDoc(KEduVocDocument * doc)
{
  m_doc = doc;
  QString front;
  QString back;

  QDomDocument domDoc("XDXF");
  if (!domDoc.setContent(m_inputFile, &m_errorMessage))
    return false;

  if (domDoc.doctype().name() != "xdxf") {
    m_errorMessage = i18n("This is not a XDXF document");
    return false;
  }

  QDomElement description = domDoc.documentElement().firstChildElement("description");
  if(!description.isNull())
    m_doc->setDocumentRemark(description.text());

  QDomElement title = domDoc.documentElement().firstChildElement("full_name");
  if(!title.isNull())
    m_doc->setTitle(title.text());

  ///The language attributes are required and should be ISO 639-2 codes, but you never know...
  QDomAttr id1 = domDoc.documentElement().attributeNode("lang_from");
  if(!id1.isNull())
    m_doc->appendIdentifier(id1.value().toLower());
  else
    m_doc->appendIdentifier(i18n("Original"));

  QDomAttr id2 = domDoc.documentElement().attributeNode("lang_to");
  if(!id2.isNull())
    m_doc->appendIdentifier(id2.value().toLower());
  else
    m_doc->appendIdentifier(i18n("Translation"));

  QDomNodeList entries = domDoc.elementsByTagName("ar");

  if (entries.count() <= 0) {
    m_errorMessage = i18n("Error while reading file");
    return false;
  }

  m_doc->setAuthor("http://xdxf.sf.net");

  for (int i = 0; i < entries.count(); i++) {
    QDomNode entry = entries.at(i);
    if(!entry.isNull()) {
      QDomElement from = entry.firstChildElement("k");
      if(!from.isNull())
        front = from.text();
      else
        front = QString(); 

      back = entry.toElement().lastChild().toText().data();
      KEduVocExpression expr = KEduVocExpression(front);
      expr.setTranslation(1, back);
      m_doc->appendEntry(&expr);
    }
  }
  return true;
}
