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

/*
PaukerDataItem::PaukerDataItem()
{
}

PaukerDataItem::PaukerDataItem(QDomElement &entry)
{
  domElement = entry;
}

PaukerDataItem::~PaukerDataItem()
{
}

QString PaukerDataItem::frontSide() const
{
  return getText("FrontSide");
}

QString PaukerDataItem::backSide() const
{
  return getText("BackSide");
}

QString PaukerDataItem::getText(const QString &tagName) const
{
  if(!domElement.isNull()) {

    QDomNodeList list = domElement.elementsByTagName(tagName);

    if(list.count() > 0) {

      QDomElement element = list.item(0).toElement();

      if(!element.isNull()) {
        return element.text();
      }
      else
        return QString::null;
    }
    else
      return QString::null;
  }
  else
    return QString::null;
}

PaukerData::PaukerData()
{
  document = new QDomDocument();
}

PaukerDataItemList PaukerData::parse(const QString &fileName)
{
  PaukerDataItemList list;

  QIODevice * file = KFilterDev::deviceForFile(fileName);
  document->setContent(file);

  QDomNodeList entries = document->elementsByTagName("Card");

  // loop through the "Card" tags looking for data
  for(uint i = 0 ; i < entries.count() ; i++) {

    // get an entry to operate on
    QDomElement entry = entries.item(i).toElement();

    // if the "node" is in fact an element -- i.e. not null
    if(!entry.isNull()) {
      PaukerDataItem item(entry);
      list.append(item);
    }
  }
  delete file;
  return list;
}
*/
KEduVocPaukerReader::KEduVocPaukerReader(QFile * file)
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

  QDomNodeList entries = domDoc.elementsByTagName("Card");

  if (entries.count() <= 0) {
    m_errorMessage = i18n("Error while reading file");
    return false;
  }

  for (int i = 0; entries.count() - 1; i++) {
    QDomElement entry = entries.at(i).toElement();
    if(!entry.isNull()) {
      front = cardText(entry, "FrontSide");
      back = cardText(entry, "BackSide");
      KEduVocExpression expr = KEduVocExpression(front);
      expr.setTranslation(1, back);
      m_doc->appendEntry(&expr);
    }
  }
}


QString KEduVocPaukerReader::cardText(const QDomElement & entry, const QString & tagName) const
{
  QDomNodeList list = entry.elementsByTagName(tagName);

  if(list.count() > 0) {
    QDomElement element = list.item(0).toElement();

    if(!element.isNull()) {
      return element.text();
    }
    else
      return QString::null;
  }
  else
    return QString::null;

}


