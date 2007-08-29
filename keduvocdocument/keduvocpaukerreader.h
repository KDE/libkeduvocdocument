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

#include <QXmlStreamReader>

class QIODevice;
class KEduVocDocument;

class KEduVocPaukerReader : public QXmlStreamReader
{
public:
    KEduVocPaukerReader( KEduVocDocument *doc );

    bool read( QIODevice *device );

private:
    void readUnknownElement();
    void readPauker();
    void readBatch();
    void readCard();
    QString readText();

    KEduVocDocument *m_doc;
};

#endif
