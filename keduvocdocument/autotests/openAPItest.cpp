/*
 * SPDX-FileCopyrightText: 2014-2014 Andreas Xavier <andxav at zoho dot com>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "keduvocdocument.h"
#include "dummyreader.h"

#include <KTemporaryFile>

#include <qtest_kde.h>

/** Unit tests to exercise each exit path of the KEdeVocDocment open() API.*/
class OpenAPITest
  : public QObject
{
    Q_OBJECT

private slots:
    /**  File Does not exist*/
    void fileDoesntExistTest();
    /**  File is Unreadable failure*/
    void unreadableFileTest();
    /**  File is locked failure*/
    void lockedFileFailureTest();
    /**  File is unknown format*/
    void unknownFormatTest();
    /**  FileReader error File*/
    void fileReaderFailedTest();
    /**  Good Read*/
    void goodTest();

private :

    /** Class to manage creation/destruction of a temp doc*/
    class TestDoc : public KTemporaryFile
    {
    public :
        /** Create the file, fix the suffix and instantiate it.*/
        explicit TestDoc(KEduVocDocument::ErrorCode err = KEduVocDocument::NoError) {
            this->open(QFile::WriteOnly );
            QTextStream out(this);
            out << DummyReader::makeDummyString( err );
            this->close();
        }
        /** Destructor*/
        ~TestDoc() {}
    };


};

void OpenAPITest::fileDoesntExistTest()
{
    KTemporaryFile tempfile;
    KEduVocDocument doc;

    KEduVocDocument::ErrorCode errcode( doc.open(tempfile.fileName(), KEduVocDocument::FileDefaultHandling) );
    QCOMPARE( int( errcode ),  int( KEduVocDocument::FileDoesNotExist ) );
}

void OpenAPITest::unreadableFileTest()
{
    TestDoc tempfile;
    QFile::setPermissions(
        tempfile.fileName()
        , ~QFile::Permissions( QFile::ReadOwner | QFile::ReadUser | QFile::ReadGroup | QFile::ReadOther) );

    KEduVocDocument doc;
    KEduVocDocument::ErrorCode errcode( doc.open(tempfile.fileName(), KEduVocDocument::FileDefaultHandling) );

    QCOMPARE( int( errcode ),  int( KEduVocDocument::FileDoesNotExist ) );
}

void OpenAPITest::lockedFileFailureTest()
{
    TestDoc tempfile;

    KEduVocDocument doc;
    QCOMPARE(doc.open(tempfile.fileName(), KEduVocDocument::FileDefaultHandling), KEduVocDocument::NoError);
    QCOMPARE(doc.open(tempfile.fileName(), KEduVocDocument::FileDefaultHandling), KEduVocDocument::NoError);
    KEduVocDocument doc2;
    QCOMPARE(doc2.open(tempfile.fileName(), KEduVocDocument::FileDefaultHandling), KEduVocDocument::FileLocked);

    KEduVocDocument doc3;
    QCOMPARE(doc3.open(tempfile.fileName(), KEduVocDocument::FileIgnoreLock), KEduVocDocument::NoError);

    doc.close();
    QCOMPARE(doc2.open(tempfile.fileName(), KEduVocDocument::FileDefaultHandling), KEduVocDocument::NoError);



}

void OpenAPITest::unknownFormatTest()
{
    TestDoc tempfile(KEduVocDocument::FileTypeUnknown);
    KEduVocDocument doc;
    QCOMPARE( doc.open(tempfile.fileName() ),  int( KEduVocDocument::FileTypeUnknown ) );
}

void OpenAPITest::fileReaderFailedTest()
{
    TestDoc tempfile(KEduVocDocument::FileReaderFailed);
    KEduVocDocument doc;
    QCOMPARE( doc.open(tempfile.fileName() ),  int( KEduVocDocument::FileReaderFailed ) );
}

void OpenAPITest::goodTest()
{
    TestDoc tempfile(KEduVocDocument::NoError);
    KEduVocDocument doc;
    QCOMPARE( doc.open(tempfile.fileName() ),  int( KEduVocDocument::NoError ) );
}


QTEST_MAIN( OpenAPITest, GUI )

#include "openapitest.moc"
