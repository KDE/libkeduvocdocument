/*
 * read a KEduVocDocument from a WQL file
 * SPDX-FileCopyrightText: 2004, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-FileCopyrightText: 2005 Eric Pignet
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KEDUVOCWQLREADER_H
#define KEDUVOCWQLREADER_H

#include <QString>
#include "keduvocdocument.h"
#include "readerbase.h"

class QIODevice;
class KEduVocDocument;

/**@brief Reader for WordQuiz files*/
class KEduVocWqlReader : public ReaderBase
{
public:
    /** constructor
        @param file an open device
    */
    explicit KEduVocWqlReader( QIODevice & file);
    /**destructor*/
    virtual ~KEduVocWqlReader(){};


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
