/***************************************************************************
                     create a KEduVocDocument from a Vokabeln file
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

#ifndef KEDUVOCVOKABELNREADER_H
#define KEDUVOCVOKABELNREADER_H

#include <QString>
#include "keduvocdocument.h"
#include "readerbase.h"

class QIODevice;
class KEduVocDocument;

/** Vokabeln Reader*/
class KEduVocVokabelnReader : public ReaderBase
{
public:
    /** constructor */
    KEduVocVokabelnReader( );

    /**destructor*/
    virtual ~KEduVocVokabelnReader(){};

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

    /** an error message.
        @return the error message
    */
    virtual QString errorMessage() const
    {
        return m_errorMessage;
    }

private:
    QIODevice *m_inputFile;
    KEduVocDocument *m_doc;
    QString m_errorMessage;
};

#endif
