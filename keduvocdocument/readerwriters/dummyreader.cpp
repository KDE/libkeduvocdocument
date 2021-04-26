/*
 * SPDX-FileCopyrightText: 2014 Andreas Xavier <andxav at zoho dot com>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "dummyreader.h"

#include <KLocalizedString>

#include <QXmlStreamWriter>
#include <QIODevice>


//@todo These static functions should be defined here.  Find the syntax bug.
// QString DummyReader::mTag()
// {
//     return QString( "kvocdocdummyreadertestelement" );
// }

// QString DummyReader::makeErrorTag(KEduVocDocument::ErrorCode err)
// {
//     return "errnum-" +QString::number( int( err));
// }

// QString DummyReader::makeDummyString(KEduVocDocument::ErrorCode error
//                                    , const QString & msg )
// {
//     QString out;
//     QXmlStreamWriter stream(&out);
//     stream.setAutoFormatting(true);
//     stream.writeStartDocument();
//     stream.writeStartElement(mTag());
//     stream.writeTextElement( makeErrorTag(error), msg);
//     stream.writeEndElement(); // m_tag
//     stream.writeEndDocument();
//     kDebug() << "The file" <<out;
//     return out;
// }


DummyReader::DummyReader(QIODevice & dev)
    :m_dev( dev )
{
}

bool DummyReader::isParsable()
{
    bool isparsable = false;
    setDevice( &m_dev );
    if ( !atEnd() ) {
        readNextStartElement();
        if ( isStartElement() ) {
            if ( name() == mTag() ) {
                isparsable=true;
            }
        }
    }
    m_dev.seek( 0 );
    return isparsable;
}

KEduVocDocument::FileType DummyReader::fileTypeHandled()
{
    return KEduVocDocument::KvdNone;
}

KEduVocDocument::ErrorCode DummyReader::read(KEduVocDocument &) {
    setDevice( &m_dev );
    if ( !atEnd() ) {
        readNextStartElement();
        if ( isStartElement() ) {
            // kDebug() << "Reading Dummy File is start "<< name() <<" text "<<text();
            if ( name() == mTag() ) {
                readNext();
                readNext();
                //kDebug() << "Reading Dummy File"<<name() << "text" << text();
                if ( isStartElement() ) {
                    m_errorMessage = readElementText();
                    // kDebug() << "Reading Dummy File is " <<name() <<" with "<<m_errorMessage;
                    if ( makeErrorTag( KEduVocDocument::NoError ) == name() )
                        return KEduVocDocument::NoError;
                    if ( makeErrorTag( KEduVocDocument::Unknown ) == name() )
                        return KEduVocDocument::Unknown;
                    if ( makeErrorTag( KEduVocDocument::InvalidXml ) == name() )
                        return KEduVocDocument::InvalidXml;
                    if ( makeErrorTag( KEduVocDocument::FileTypeUnknown ) == name() )
                        return KEduVocDocument::FileTypeUnknown;
                    if ( makeErrorTag( KEduVocDocument::FileCannotWrite ) == name() )
                        return KEduVocDocument::FileCannotWrite;
                    if ( makeErrorTag( KEduVocDocument::FileWriterFailed ) == name() )
                        return KEduVocDocument::FileWriterFailed;
                    if ( makeErrorTag( KEduVocDocument::FileCannotRead ) == name() )
                        return KEduVocDocument::FileCannotRead;
                    if ( makeErrorTag( KEduVocDocument::FileReaderFailed ) == name() )
                        return KEduVocDocument::FileReaderFailed;
                    if ( makeErrorTag( KEduVocDocument::FileDoesNotExist ) == name() )
                        return KEduVocDocument::FileDoesNotExist;
                    if ( makeErrorTag( KEduVocDocument::FileLocked ) == name() )
                        return KEduVocDocument::FileLocked;
                    if ( makeErrorTag( KEduVocDocument::FileCannotLock ) == name() )
                        return KEduVocDocument::FileCannotLock;
                }
            }
        }
    }
    return KEduVocDocument::Unknown;
}


QString DummyReader::errorMessage() const
{
    return m_errorMessage;
}
