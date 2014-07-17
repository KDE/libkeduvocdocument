/***************************************************************************
                  create a KEduVocDocument from a text file
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

#ifndef KEDUVOCCSVREADER_H
#define KEDUVOCCSVREADER_H

#include <QString>
#include "keduvocdocument.h"
#include "readerbase.h"

class QIODevice;

class KEduVocDocument;

/** CSV Reader, the default reader*/
class KEduVocCsvReader : public ReaderBase
{
public:
    /** constructor */
    KEduVocCsvReader();
    /** destructor */
    ~KEduVocCsvReader(){};

    /**CSV can always parse a file
     @return true if parsable
    */
    virtual bool isParsable( QIODevice &) {
        return true;
    }

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
