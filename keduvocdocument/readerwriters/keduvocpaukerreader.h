/***************************************************************************
                  create a KEduVocDocument from a Pauker file
    -----------------------------------------------------------------------
    copyright     : (C) 2004, 2007 Peter Hedlund <peter.hedlund@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCPAUKERREADER_H
#define KEDUVOCPAUKERREADER_H

#include <QXmlStreamReader>
#include "keduvocdocument.h"
#include "readerbase.h"

class QIODevice;
class KEduVocDocument;

/**  @brief Reader for the Pauker format*/
class KEduVocPaukerReader : public ReaderBase, private QXmlStreamReader
{
public:
    /** constructor
     @param file an device open for read
    */
    explicit KEduVocPaukerReader( QIODevice & file);
    /**destructor*/
    virtual ~KEduVocPaukerReader(){};


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
    QString errorMessage() const Q_DECL_OVERRIDE;

private:
    /** Skip unknown tags*/
    void readUnknownElement();
    /** Read Pauker tag*/
    void readPauker();
    /** Read a batch tag*/
    void readBatch();
    /** Read a card tag*/
    void readCard();
    /** Read a Text tag
     @return the tag */
    QString readText();

    KEduVocDocument *m_doc; ///< output doc
    QIODevice & m_dev; ///< input device/file
};

#endif
