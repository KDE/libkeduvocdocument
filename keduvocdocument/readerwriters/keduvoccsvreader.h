/*
 * create a KEduVocDocument from a text file
 * SPDX-FileCopyrightText: 2007 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

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
    bool isParsable() Q_DECL_OVERRIDE
    {
        return true;
    }

    /** @brief returns the KEduVocDocument::FileType that this reader handles
        @return KEduVocDocument::FileType handled
     */
    KEduVocDocument::FileType fileTypeHandled() Q_DECL_OVERRIDE;

    /**  @brief Parse file and write into doc
     @param doc to be written
     @return error status of the read.*/
    KEduVocDocument::ErrorCode read(KEduVocDocument & doc ) Q_DECL_OVERRIDE;

    /** an error message.
        @return the error message
     */
    QString errorMessage() const Q_DECL_OVERRIDE
    {
        return m_errorMessage;
    }

private:
    QIODevice *m_inputFile;  ///< input device
    KEduVocDocument *m_doc;  ///< output doc
    QString m_errorMessage;  ///< error message
};

#endif
