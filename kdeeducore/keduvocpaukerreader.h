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

#include <QtXml>

class QIODevice;
class KEduVocDocument;

class KEduVocPaukerReader : public QObject
{
public:
  KEduVocPaukerReader(QIODevice *file);

  bool readDoc(KEduVocDocument *doc);

  QString errorMessage() const {return m_errorMessage;}

private:
  QString cardText(const QDomNode &entry, const QString &tagName) const;

  QIODevice *m_inputFile;
  KEduVocDocument *m_doc;
  QString m_errorMessage;
};

#endif
