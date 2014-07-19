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

/**@brief Reader for the XDXF format*/
class KEduVocXdxfReader : public ReaderBase, private QXmlStreamReader
{
public:
    /** constructor
        @param file an device open for read
    */
    explicit KEduVocXdxfReader(QIODevice & file);

    /**destructor*/
    virtual ~KEduVocXdxfReader(){};

    /** @brief Can this reader parse this file
     *
     Read a small portion of the header of the file
     and decide if it is a suitable type.
     @return true if parsable
     */
    virtual bool isParsable();

    /** @brief returns the KEduVocDocument::FileType that this reader handles
        @return KEduVocDocument::FileType handled
     */
    virtual KEduVocDocument::FileType fileTypeHandled();

    /** @brief Parse file and write into doc
     @param doc to be written
     @return error status of the read.*/
    virtual KEduVocDocument::ErrorCode read(KEduVocDocument & doc);

    /** an error message.
        @return the error message
    */
    virtual QString errorMessage() const;

private:
    /** Skip unknown tags */
    void readUnknownElement();
    /** Read xdxf tag  */
    void readXdxf();
    /** Read ar tag which is the word pair with "<ar><k>key</k>text</ar>" format */
    void readEntry();

    KEduVocDocument *m_doc; ///< output doc
    QIODevice & m_dev; ///< input device
};

#endif
