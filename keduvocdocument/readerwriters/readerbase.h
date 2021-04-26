/*
 * SPDX-FileCopyrightText: 2014 Andreas Xavier <andxav at zoho dot com>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef READERBASE_H
#define READERBASE_H

#include "keduvocdocument.h"

#include <QDebug>

class KEduVocDocument;

/**
 * @brief a base class for readers of various lexicon formats
 *
 * */
class ReaderBase
{
public:

    /** destructor */
    virtual ~ReaderBase(){};

    /** @brief Can this reader parse the file
     *
     * Read a small portion of the header of the file
     * and decide if it is a suitable type.
     * @return true if parsable
     */
    virtual bool isParsable() = 0;

    /** @brief returns the KEduVocDocument::FileType that this reader handles
        @return KEduVocDocument::FileType handled
     */
    virtual KEduVocDocument::FileType fileTypeHandled() = 0;

    /**  @brief Parse file and write into doc
     @param doc to be written
     @return error status of the read.*/
    virtual KEduVocDocument::ErrorCode read(KEduVocDocument & doc) = 0;

    /** an error message.
        @return the error message
    */
    virtual QString errorMessage() const = 0;

};

#endif // READERBASE_H
