/*
 * SPDX-FileCopyrightText: 2014 Andreas Xavier <andxav at zoho dot com>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef READERTESTHELPERS_H
#define READERTESTHELPERS_H

#include "keduvocdocument.h"
#include <QTemporaryFile>
#include <QBuffer>

/** These are macros and functions common to all of the file reader tests*/
namespace ReaderTestHelpersUnitTest
{

    /** Class to manage creation/destruction of a temp doc*/
    class TestDoc : public QTemporaryFile
    {
    public :
        /** Create the file, fix the suffix and instantiate it.*/
        explicit TestDoc(const QString &str) {
            this->open(QFile::WriteOnly );
            QTextStream out(this);
            out << str;
            this->close();
        }
        /** Destructor*/
        ~TestDoc() {}
    };


// These macros are to force the QCOMPARE/QVERIFY to be in the test function.
// In order to produce useful out QCOMPARE must be in the test function
// since it is a macro

// Check that a parse returns errcode
#define KVOCREADER_EXPECT_CORE(str, eError, eType, isGood)              \
    do {                                                                \
        QByteArray array( str.toLatin1() );                             \
        QBuffer buffer( &array );                       \
        buffer.open( QIODevice::ReadOnly );                            \
        ReaderManager::ReaderPtr reader( ReaderManager::reader(buffer ) ); \
        KEduVocDocument docRead;                                        \
        KEduVocDocument::ErrorCode actualError(reader->read(docRead ) ); \
        KEduVocDocument::FileType actualType(reader->fileTypeHandled() ); \
        if (!isGood && (actualError != eError                           \
            || actualType != eType)) {                                  \
            qDebug() << str;                                            \
        }                                                               \
        QCOMPARE( int( actualError ), int( eError ) );                  \
        QCOMPARE( int( actualType ), int( eType ) );                    \
        ReaderTestHelpersUnitTest::TestDoc tempfile(str);               \
        KEduVocDocument doc;                                            \
        QCOMPARE( int(doc.detectFileType(tempfile.fileName() )), int(eType));     \
    }  while ( 0 )

// Check that a parse returns errcode.
#define KVOCREADER_EXPECT(str, expectedError, expectedType)             \
    do {                                                                \
        KVOCREADER_EXPECT_CORE( str , expectedError , expectedType, true);   \
    }  while ( 0 )

// Check that a parse returns errcode. This ignores the error.
#define KVOCREADER_DONT_EXPECT(str, expectedError, expectedType)        \
    do {                                                                \
        QEXPECT_FAIL("", " This is a known bug.", Continue);            \
        KVOCREADER_EXPECT_CORE( str , expectedError, expectedType,  false);  \
    }  while ( 0 )

}

#endif // READERTESTHELPERS_H
