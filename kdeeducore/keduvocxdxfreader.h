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

#ifndef KEDUVOCXDXFREADER_H
#define KEDUVOCXDXFREADER_H

#include <QtXml>

class QIODevice;
class KEduVocDocument;

class KEduVocXdxfReader : public QObject
{
public:
  KEduVocXdxfReader(QIODevice *file);

  bool readDoc(KEduVocDocument *doc);

  QString errorMessage() const {return m_errorMessage;};

private:
  QIODevice *m_inputFile;
  KEduVocDocument *m_doc;
  QString m_errorMessage;
};

#endif
