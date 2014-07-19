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

/** @brief CSV Reader, the default reader*/
class KEduVocCsvReader : public ReaderBase
{
public:
    /** constructor
     @param dev to parse*/
    explicit KEduVocCsvReader(QIODevice & dev);
    /** destructor */
    ~KEduVocCsvReader(){};

    /** @brief CSV can always parse a file
     @return true if parsable
    */
    virtual bool isParsable()
    {
        return true;
    }

    /** @brief returns the KEduVocDocument::FileType that this reader handles
        @return KEduVocDocument::FileType handled
     */
    virtual KEduVocDocument::FileType fileTypeHandled();

    /**  @brief Parse file and write into doc
     @param doc to be written
     @return error status of the read.*/
    virtual KEduVocDocument::ErrorCode read(KEduVocDocument & doc );

    /** an error message.
        @return the error message
     */
    virtual QString errorMessage() const
    {
        return m_errorMessage;
    }

private:
    QIODevice *m_inputFile;  ///< input device
    KEduVocDocument *m_doc;  ///< output doc
    QString m_errorMessage;  ///< error message
};

#endif
