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

#ifndef DUMMYREADER_H
#define DUMMYREADER_H

#include "readerbase.h"

#include <QXmlStreamReader>

#include <qdebug.h>

class QIODevice;

/**
 * @brief A dummy reader  to be used in unit tests
 * @details DummyReader always returns the error message corresponding to the
 * file generated with makeDummyString()
 *
 * */
class DummyReader : public ReaderBase, private QXmlStreamReader
{
public:
    /** @brief Given an error produce a file that the dummy reader can detect and return the error.
        @param error The error to return
        @param msg the message to return
        @return string of the file that will generate the error */
    static QString makeDummyString(KEduVocDocument::ErrorCode error
                                   , const QString & msg = QStringLiteral("Dummy Reader Error"))
    //Note: This should be defined in the cpp file, but I was having linker problems
    {
        QString out;
        QXmlStreamWriter stream(&out);
        stream.setAutoFormatting(true);
        stream.writeStartDocument();
        stream.writeStartElement(mTag());
        stream.writeTextElement( makeErrorTag(error), msg);
        stream.writeEndElement(); // m_tag
        stream.writeEndDocument();
        qDebug() << "The file" <<out;
        return out;
    }

    /** constructor
     @param dev device to parse*/
    explicit DummyReader(QIODevice &dev);
    /**destructor*/
    virtual ~DummyReader(){};

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
    /**
        @brief XML tag to identify a dummy file
        @return tagname that indicates this is a dummy document
    */
    static QString mTag()
    //Note: This should be defined in the cpp file, but I was having linker problems
    {
        return QStringLiteral( "kvocdocdummyreadertestelement" );
    }

    /**
        @brief Make error into a tabname
        @param err Error code to convert
        @return tagname
    */
    static QString makeErrorTag(KEduVocDocument::ErrorCode err)
    //Note: This should be defined in the cpp file, but I was having linker problems
    {
        return "errnum-" +QString::number( int( err));
    }

    KEduVocDocument::ErrorCode m_error; ///< The error code to always return;
    QString m_errorMessage; ///< The error message
    QIODevice & m_dev; ///< input device

};

#endif // DUMMYREADER_H
