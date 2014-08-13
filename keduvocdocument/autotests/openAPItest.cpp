/*
 Copyright 2014-2014 Andreas Xavier <andxav at zoho dot com>

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
