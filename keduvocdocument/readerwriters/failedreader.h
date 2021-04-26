/*
 * SPDX-FileCopyrightText: 2014 Andreas Xavier <andxav at zoho dot com>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef FAILEDREADER_H
#define FAILEDREADER_H

#include "readerbase.h"

#include <QDebug>


/**
 * @brief A fallback reader when the device can't be read or no other reader can parse.
 @details FailedReader always returns the error message that it was initialized with
 , unless the error code was NoError.  In which case it returns Unknown.
 *
 * */
class FailedReader : public ReaderBase
{
public:
    /** @brief When constructed with error code FailedReader will return this error code
     @param error the error code or defaults to Unknown
     @param msg a custom message or defaults to \"Error while reading file\"*/
    explicit FailedReader(KEduVocDocument::ErrorCode error = KEduVocDocument::Unknown
                          , const QString & msg = QString());

    /**destructor*/
    virtual ~FailedReader(){};

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
    KEduVocDocument::ErrorCode read(KEduVocDocument & doc) Q_DECL_OVERRIDE;

    /** an error message.
        @return the error message
    */
    QString errorMessage() const Q_DECL_OVERRIDE;
private :
    KEduVocDocument::ErrorCode m_error; ///< The error code to always return;
    QString m_errorMessage; ///< The error message
};

#endif // FAILEDREADER_H
