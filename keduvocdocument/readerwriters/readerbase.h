/*
 Copyright 2014 Andreas Xavier <andxav at zoho dot com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 USA
*/

#ifndef READERBASE_H
#define READERBASE_H

#include "keduvocdocument.h"

#include <kdebug.h>

class QIODevice;
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
