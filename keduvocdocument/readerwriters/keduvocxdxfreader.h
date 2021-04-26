/*
 * create a KEduVocDocument from a XDXF file
 * SPDX-FileCopyrightText: 2007 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

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
    QString errorMessage() const Q_DECL_OVERRIDE;

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
