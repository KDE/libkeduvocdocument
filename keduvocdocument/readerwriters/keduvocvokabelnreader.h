/*
                     create a KEduVocDocument from a Vokabeln file
    -----------------------------------------------------------------------
 * SPDX-FileCopyrightText: 2007 Peter Hedlund <peter.hedlund@kdemail.net>


 * SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef KEDUVOCVOKABELNREADER_H
#define KEDUVOCVOKABELNREADER_H

#include <QString>
#include "keduvocdocument.h"
#include "readerbase.h"

class QIODevice;
class KEduVocDocument;

/** @brief Vokabeln Reader*/
class KEduVocVokabelnReader : public ReaderBase
{
public:
    /** constructor
        @param file an device open for read
    */
    explicit KEduVocVokabelnReader( QIODevice & file);

    /**destructor*/
    virtual ~KEduVocVokabelnReader(){};

    /** @brief Can this reader parse this file
     *
     Read a small portion of the header of the file
     and decide if it is a suitable type.
     @return true if parsable
     */
    bool isParsable() Q_DECL_OVERRIDE;

    /** @brief returns the KEduVocDocument::FileType that this reader handles
        @return KEduVocDocument::FileType handled
     */
    KEduVocDocument::FileType fileTypeHandled() Q_DECL_OVERRIDE;

    /** @brief Parse file and write into doc
     @param doc to be written
     @return error status of the read.*/
    KEduVocDocument::ErrorCode read(KEduVocDocument & doc) Q_DECL_OVERRIDE;

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
