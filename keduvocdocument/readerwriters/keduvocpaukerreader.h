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
#include "keduvocdocument.h"
#include "readerbase.h"

class QIODevice;
class KEduVocDocument;

/** Reader for the Pauker format*/
class KEduVocPaukerReader : public ReaderBase, private QXmlStreamReader
{
public:
    /** constructor */
    KEduVocPaukerReader();
    /**destructor*/
    virtual ~KEduVocPaukerReader(){};


    /** @brief Can this reader parse this file
     *
     Read a small portion of the header of the file
     and decide if it is a suitable type.
     @param file an device open for read
     @return true if parsable
     */
    virtual bool isParsable( QIODevice & file);

    /** @brief Can this reader parse this file
     *
     Read a small portion of the header of the file
     and decide if it is a suitable type.
     @param file an device open for read
     @param doc document object to store the data in
     @return true if parsable
     */
    virtual KEduVocDocument::ErrorCode read( QIODevice & file, KEduVocDocument & doc );

    /** an error message.
        @return the error message
    */
    virtual QString errorMessage() const;

private:
    void readUnknownElement();
    void readPauker();
    void readBatch();
    void readCard();
    QString readText();

    KEduVocDocument *m_doc;
};

#endif
