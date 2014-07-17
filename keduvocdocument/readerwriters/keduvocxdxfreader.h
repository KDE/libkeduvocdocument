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

#include <QXmlStreamReader>
#include "keduvocdocument.h"
#include "readerbase.h"

class QIODevice;
class KEduVocDocument;

/**Reader for the XDXF format*/
class KEduVocXdxfReader : public ReaderBase, private QXmlStreamReader
{
public:
    /** constructor */
    KEduVocXdxfReader( );

    /**destructor*/
    virtual ~KEduVocXdxfReader(){};

    /** @brief Can this reader parse this file
     *
     Read a small portion of the header of the file
     and decide if it is a suitable type.
     @param file an device open for read
     @return true if parsable
     */
    virtual bool isParsable( QIODevice & file);

    /** Parse file and write into doc
     @param file an open device
     @param doc to be written
     @return error status of the read.*/
    virtual KEduVocDocument::ErrorCode read(QIODevice & file, KEduVocDocument & doc);

    /** an error message.*/
    virtual QString errorMessage() const;

private:
    void readUnknownElement();
    void readXdxf();
    void readEntry();

    KEduVocDocument *m_doc;
};

#endif
