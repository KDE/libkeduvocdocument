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

#include <KTemporaryFile>

#include <qtest_kde.h>

#include <qobject.h>
#include <qvalidator.h>
#include <QtXml/QDomDocument>

/** Unit tests to verify that 2 kvocdocs can't access a single file at the same time.*/
class KEduVocDocumentFileLockingTest
  : public QObject
{
    Q_OBJECT

private slots:
    /**  Checks that the lock is released in the destructor*/
    void testVocFileLockReleaseOnDestruction();
    /**  Checks that the lock is released on close()*/
    void testVocFileLockReleaseOnClose();
    /**  Checks that the lock is released if another url is opened.*/
    void testVocFileLockReleaseOnSecondOpen();
    /** Checks that the lock is released when it saves as another filename*/
    void testVocFileLockReleaseOnSaveAs();

    /**  Checks that the kvocdoc doesn't lock itself out with a second open*/
    void testVocFileLockOpenOpenCycle();
    /**  Checks that the kvoc doc doesn't lock itself out with a second save*/
    void testVocFileLockOpenSaveSaveCycle();

    /**  Checks that the kvoc doc can steal the lock with an open function*/
    void testVocFileLockOpenStealWOpen();
    /**  Checks that the kvoc doc can steal the lock with a saveAs function*/
    void testVocFileLockOpenStealWSaveAs();

private :

    /** Class to manage creation/destruction of a kvtml temp doc*/
    class TemporaryVocDoc : public KTemporaryFile
    {
    public :
        /** Create the file, fix the suffix and instantiate it.*/
        TemporaryVocDoc() {
            this->setSuffix(".kvtml");
            this->open();
            this->close();
        }
        /** Destructor*/
        ~TemporaryVocDoc() {}
    };

    /** Creates a minimal doc that will save and load error free.*/
    class MinimalTempVocDoc : public TemporaryVocDoc
    {
    public :
        /** The minimal doc has generator, author, lang and local */
        MinimalTempVocDoc() {
            const QString generator = QString::fromLatin1( "File Locking Unit Tests" );
            const QString author = QString::fromLatin1( "File Locking Test" );
            const QString lang = QString::fromLatin1( "File Locking Language Name" );
            const QString locale = QString::fromLatin1( "en" ) ;

            KEduVocIdentifier lang0;
            lang0.setName( lang );
            lang0.setLocale( locale);

            KEduVocDocument *doc = new KEduVocDocument();
            doc->setAuthor( author );
            doc->appendIdentifier( lang0 );
            KUrl filename = this->fileName();
            doc->saveAs(filename, KEduVocDocument::Kvtml, generator);
            delete doc;
        }
        /** Destructor*/
        ~MinimalTempVocDoc() {}
    };
};


void KEduVocDocumentFileLockingTest::testVocFileLockReleaseOnDestruction()
{
    MinimalTempVocDoc tempfile;

    KEduVocDocument *doc1 = new KEduVocDocument();
    int docError = doc1->open(tempfile.fileName());
    QCOMPARE( docError, int( KEduVocDocument::NoError ) );

    const QString generator = QString::fromLatin1( "File Locking Unit Tests" );
    QCOMPARE( doc1->generator(), generator );

    KEduVocDocument *doc2 = new KEduVocDocument();
    docError = doc2->open(tempfile.fileName());
    QCOMPARE( docError, int( KEduVocDocument::FileLocked ) );
    delete doc2;

    delete doc1;
    KEduVocDocument *doc3 = new KEduVocDocument();
    docError = doc3->open(tempfile.fileName());
    QCOMPARE( docError, int( KEduVocDocument::NoError ) );
    QCOMPARE( doc3->generator(), generator );
    delete doc3;

}

void KEduVocDocumentFileLockingTest::testVocFileLockReleaseOnClose()
{
    MinimalTempVocDoc tempfile;

    KEduVocDocument *doc1 = new KEduVocDocument();
    int docError = doc1->open(tempfile.fileName());
    QCOMPARE( docError, int( KEduVocDocument::NoError ) );

    KEduVocDocument *doc2 = new KEduVocDocument();
    docError = doc2->open(tempfile.fileName());
    QCOMPARE( docError, int( KEduVocDocument::FileLocked ) );
    delete doc2;

    doc1->close();
    KEduVocDocument *doc3 = new KEduVocDocument();
    docError = doc3->open(tempfile.fileName());
    QCOMPARE( docError, int( KEduVocDocument::NoError ) );

    delete doc3;
    delete doc1;

}

void KEduVocDocumentFileLockingTest::testVocFileLockReleaseOnSecondOpen()
{
    MinimalTempVocDoc tempfile,  tempfile2;

    KEduVocDocument *doc1 = new KEduVocDocument();
    int docError = doc1->open(tempfile.fileName());
    QCOMPARE( docError, int( KEduVocDocument::NoError ) );

    KEduVocDocument *doc2 = new KEduVocDocument();
    docError = doc2->open(tempfile.fileName());
    QCOMPARE( docError, int( KEduVocDocument::FileLocked ) );
    delete doc2;

    doc1->open( tempfile2.fileName() );
    KEduVocDocument *doc3 = new KEduVocDocument();
    docError = doc3->open(tempfile.fileName());
    QCOMPARE( docError, int( KEduVocDocument::NoError ) );

    delete doc1;
    delete doc3;
}

void KEduVocDocumentFileLockingTest::testVocFileLockReleaseOnSaveAs()
{
    MinimalTempVocDoc tempfile;
    TemporaryVocDoc tempfile2;

    KEduVocDocument *doc1 = new KEduVocDocument();
    int docError = doc1->open(tempfile.fileName());
    QCOMPARE( docError, int( KEduVocDocument::NoError ) );

    KEduVocDocument *doc2 = new KEduVocDocument();
    docError = doc2->open(tempfile.fileName());
    QCOMPARE( docError, int( KEduVocDocument::FileLocked ) );
    delete doc2;

    const QString generator = QString::fromLatin1( "File Locking Unit Tests" );
    docError = doc1->saveAs( tempfile2.fileName() ,KEduVocDocument::Kvtml, generator);
    QCOMPARE( docError, int( KEduVocDocument::NoError ) );

    KEduVocDocument *doc3 = new KEduVocDocument();
    docError = doc3->open(tempfile.fileName());
    QCOMPARE( docError, int( KEduVocDocument::NoError ) );

    delete doc1;
    delete doc3;
}

void KEduVocDocumentFileLockingTest::testVocFileLockOpenOpenCycle()
{
    MinimalTempVocDoc tempfile;
    const QString generator = QString::fromLatin1( "File Locking Unit Tests" );

    KEduVocDocument *doc = new KEduVocDocument();
    int docError = doc->open(tempfile.fileName());
    docError = doc->open(tempfile.fileName());
    QCOMPARE( docError, int( KEduVocDocument::NoError ) );

    delete doc;
}

void KEduVocDocumentFileLockingTest::testVocFileLockOpenSaveSaveCycle()
{
    MinimalTempVocDoc tempfile;
    const QString generator = QString::fromLatin1( "File Locking Unit Tests" );

    KEduVocDocument *doc = new KEduVocDocument();
    int docError = doc->open(tempfile.fileName());

    doc->setAuthor( QString::fromLatin1( "Author1" ) );
    docError = doc->saveAs( tempfile.fileName() ,KEduVocDocument::Kvtml, generator);
    QCOMPARE( docError, int( KEduVocDocument::NoError ) );

    const QString author2 = QString::fromLatin1( "Author2" );
    doc->setAuthor( author2 );
    docError = doc->saveAs( tempfile.fileName() ,KEduVocDocument::Kvtml, generator);
    QCOMPARE( docError, int( KEduVocDocument::NoError ) );

    delete doc;

    KEduVocDocument docRead;
    docRead.open(tempfile.fileName());
    QCOMPARE( docRead.author(), author2 );

}

void KEduVocDocumentFileLockingTest::testVocFileLockOpenStealWOpen()
{
    MinimalTempVocDoc tempfile;
    const QString generator = QString::fromLatin1( "File Locking Unit Tests" );

    KEduVocDocument *doc1 = new KEduVocDocument();
    int docError = doc1->open(tempfile.fileName());
    QCOMPARE( docError, int( KEduVocDocument::NoError ) );

    KEduVocDocument *doc2 = new KEduVocDocument();
    docError = doc2->open(tempfile.fileName());
    QCOMPARE( docError, int( KEduVocDocument::FileLocked ) );

    docError = doc2->open(tempfile.fileName(),  KEduVocDocument::FileIgnoreLock);
    QCOMPARE( docError, int( KEduVocDocument::NoError ) );

    docError = doc1->saveAs(tempfile.fileName(), KEduVocDocument::Kvtml, generator );
    QCOMPARE( docError, int( KEduVocDocument::FileCannotLock ) );

    delete doc2;
    delete doc1;
}

void KEduVocDocumentFileLockingTest::testVocFileLockOpenStealWSaveAs()
{
    MinimalTempVocDoc tempfile,  tempfile2;
    const QString generator = QString::fromLatin1( "File Locking Unit Tests Save w Stolen Lock" );

    KEduVocDocument *doc1 = new KEduVocDocument();
    int docError = doc1->open(tempfile.fileName());
    QCOMPARE( docError, int( KEduVocDocument::NoError ) );

    KEduVocDocument *doc2 = new KEduVocDocument();
    docError = doc2->open(tempfile2.fileName());
    QCOMPARE( docError, int( KEduVocDocument::NoError ) );

    docError = doc2->saveAs(tempfile.fileName(), KEduVocDocument::Kvtml, generator );
    QCOMPARE( docError, int( KEduVocDocument::FileLocked ) );

    docError = doc2->saveAs(tempfile.fileName(), KEduVocDocument::Kvtml, KEduVocDocument::FileIgnoreLock );
    QCOMPARE( docError, int( KEduVocDocument::NoError ) );

    docError = doc1->saveAs(tempfile.fileName(), KEduVocDocument::Kvtml, generator );
    QCOMPARE( docError, int( KEduVocDocument::FileCannotLock ) );

    delete doc2;
    delete doc1;
}



QTEST_KDEMAIN_CORE( KEduVocDocumentFileLockingTest )

#include "keduvocdocumentfilelockingtest.moc"
