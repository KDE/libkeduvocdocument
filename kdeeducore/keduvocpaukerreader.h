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

#ifndef KEDUVOCPAUKERREADER_H
#define KEDUVOCPAUKERREADER_H

#include <libkdeedu_core_export.h>

#include <QtXml>
#include <QFile>

class KEduVocDocument;

class KDEEDUCORE_EXPORT KEduVocPaukerReader : public QObject
{
public:
  KEduVocPaukerReader(QFile *file);

  bool readDoc(KEduVocDocument *doc);

  QString errorMessage() const {return m_errorMessage;};
private:
  QString cardText(const QDomElement &entry, const QString &tagName) const;

  QFile *m_inputFile;
  KEduVocDocument *m_doc;
  QString m_errorMessage;
};
/*
class PaukerDataItem
{
public:
  PaukerDataItem();
  PaukerDataItem(QDomElement &entry);
  virtual ~PaukerDataItem();

  QString frontSide() const;
  QString backSide() const;

protected:
  QString getText(const QString &tagName) const;

private:
  QDomElement domElement;
};

typedef QValueList<PaukerDataItem> PaukerDataItemList;

class PaukerData
{
public:
  PaukerData();
  PaukerDataItemList parse(const QString &fileName);
  int colWidth(int col);
  int numRows();
  QString language(int col);

private:
  QDomDocument* document;
};
*/
#endif
