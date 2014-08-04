/*  This file is part of the KDE libraries
    Copyright (c) 2006 Jacob R Rideout <kde@jacobrideout.net>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "kautosavefiletest.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QSet>

#include <qtemporaryfile.h>
#include <../autosave/kautosavefile.h>

#include <QCoreApplication>
#include <QtTest/QtTest>

QTEST_MAIN(KAutoSaveFileTest)

using namespace LexiAutoSave;

void KAutoSaveFileTest::cleanupTestCase()
{
    Q_FOREACH (const QString &fileToRemove, filesToRemove) {
        QFile::remove(fileToRemove);
    }
}


void KAutoSaveFileTest::initTestCase()
{
    mainappname = QLatin1String("kautosavetest" );
    appname1 = QLatin1String("kautosavetest_app1" );
    appname2 = QLatin1String("kautosavetest_app2" );
    init();
}
void KAutoSaveFileTest::init()
{
    QCoreApplication::instance()->setApplicationName(mainappname);
    Q_FOREACH( KAutoSaveFile * afile,  KAutoSaveFile::staleFiles(QUrl() , mainappname )) {
        QVERIFY( afile->open(QIODevice::ReadWrite) );
        delete afile;
    }
    Q_FOREACH( KAutoSaveFile * afile,  KAutoSaveFile::staleFiles(QUrl() , appname1 )) {
        afile->open(QIODevice::ReadWrite);
        delete afile;
    }
    Q_FOREACH( KAutoSaveFile * afile,  KAutoSaveFile::staleFiles(QUrl() , appname2 )) {
        afile->open(QIODevice::ReadWrite);
        delete afile;
    }
}

void KAutoSaveFileTest::test_readWrite()
{
    QTemporaryFile file("test_readWrite");

    QVERIFY(file.open());

    QUrl normalFile(QFileInfo(file).absoluteFilePath());

    //Test basic functionality
    KAutoSaveFile saveFile(normalFile);

    QVERIFY(!QFile::exists(saveFile.fileName()));
    QVERIFY(saveFile.open(QIODevice::ReadWrite));

    QString inText = QString::fromLatin1("This is test data one.\n");

    {
        QTextStream ts(&saveFile);
        ts << inText;
        ts.flush();
    }

    saveFile.close();

    {
        QFile testReader(saveFile.fileName());
        testReader.open(QIODevice::ReadWrite);
        QTextStream ts(&testReader);

        QString outText = ts.readAll();

        QCOMPARE(outText, inText);
    }

    filesToRemove << file.fileName();
}

void KAutoSaveFileTest::test_fileStaleFiles()
{
    // TODO
}

void KAutoSaveFileTest::test_applicationStaleFiles()
{
    // TODO
}



void KAutoSaveFileTest::test_crossContaminateFilename()
{
    //  Create a temporary file with a unique filename
    QTemporaryFile file( "test_crossContaminateFilename" );
    QVERIFY( file.open() );
    QUrl fileQurl ( QUrl::fromLocalFile( file.fileName() ) );

    QVERIFY2( KAutoSaveFile::staleFiles(fileQurl).isEmpty()
              , "An unused filename has an autosave file." );

    // Create the autosave file lock
    KAutoSaveFile autoSave1( fileQurl );
    QVERIFY(autoSave1.open(QIODevice::ReadWrite));

    QVERIFY2( ! KAutoSaveFile::staleFiles(fileQurl).isEmpty()
              ,  "An expected autosave file does not exist" );

    // Create another temporary file with another unique filename
    QTemporaryFile file2( "test_crossContaminateFilename" );
    QVERIFY( file2.open() );
    QUrl file2Qurl ( QUrl::fromLocalFile( file2.fileName() ) );

    // Check for stale auto saves on a never used before filename
    QVERIFY2( KAutoSaveFile::staleFiles(file2Qurl).isEmpty()
              , "An unused filename has detected the autosave file of a different filename" );
}

void KAutoSaveFileTest::test_crossContaminateAppname()
{
    QString originalAppName( QCoreApplication::applicationName() );

    // Create two tempfiles
    QTemporaryFile file1( "test_crossContaminateAppname" );
    QVERIFY( file1.open() );
    QUrl file1Qurl ( QUrl::fromLocalFile( file1.fileName() ) );
    QTemporaryFile file2( "test_crossContaminateAppname" );
    QVERIFY( file2.open() );
    QUrl file2Qurl ( QUrl::fromLocalFile( file2.fileName() ) );


    //App1
    QCoreApplication::setApplicationName(appname1);

    // Create the autosave file lock
    KAutoSaveFile autoSaveApp1File1( file1Qurl );
    QVERIFY(autoSaveApp1File1.open(QIODevice::ReadWrite));
    KAutoSaveFile autoSaveApp1File2( file2Qurl );
    QVERIFY(autoSaveApp1File2.open(QIODevice::ReadWrite));

    QCoreApplication::setApplicationName(appname2);

    //App2 has nothing locked
    QCOMPARE( KAutoSaveFile::allStaleFiles().size() , 0 );
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl, appname2).size() , 0 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl, appname2).size() , 0 );
    QCOMPARE( KAutoSaveFile::allStaleFiles(appname2).size() , 0 );
    QCOMPARE( KAutoSaveFile::allStaleFiles(appname1).size() , 2 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl, appname1).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl, appname1).size() , 1 );

    // Create the autosave file lock
    KAutoSaveFile autoSaveApp2File1( file1Qurl );
    QVERIFY(autoSaveApp2File1.open(QIODevice::ReadWrite));
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).size() , 0 );
    QCOMPARE( KAutoSaveFile::allStaleFiles(appname2).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl, appname1).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl, appname1).size() , 1 );
    QCOMPARE( KAutoSaveFile::allStaleFiles(appname1).size() , 2 );

    QCoreApplication::setApplicationName(originalAppName);
}

void KAutoSaveFileTest::test_freeStaleOnRelease()
{
    // Create a temporary file with a unique filename
    QTemporaryFile file( "test_freeStaleOnRelease" );
    QVERIFY( file.open() );
    QUrl fileQurl ( QUrl::fromLocalFile( file.fileName() ) );

    QVERIFY2( KAutoSaveFile::staleFiles(fileQurl).isEmpty()
              , "An unused filename has an autosave file." );

    // Create the autosave file lock
    KAutoSaveFile autoSave1( fileQurl );
    QVERIFY(autoSave1.open(QIODevice::ReadWrite));

    QCOMPARE( KAutoSaveFile::staleFiles(fileQurl).size() , 1 );

    // Create another temporary file with another unique filename
    QTemporaryFile file2( "test_freeStaleOnRelease" );
    QVERIFY( file2.open() );
    QUrl file2Qurl ( QUrl::fromLocalFile( file2.fileName() ) );

    // Create the autosave file lock
    KAutoSaveFile autoSave2( file2Qurl );
    QVERIFY(autoSave2.open(QIODevice::ReadWrite));

    //Check that there is one of each autosave
    QCOMPARE( KAutoSaveFile::staleFiles(fileQurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(fileQurl).at( 0 )->fileName() , autoSave1.fileName() );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).at( 0 )->fileName() , autoSave2.fileName() );

    autoSave1.releaseLock();

    //Releasing the lock doesn't remove the autosave file
    QCOMPARE( KAutoSaveFile::staleFiles(fileQurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(fileQurl).at( 0 )->fileName() , autoSave1.fileName() );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).at( 0 )->fileName() , autoSave2.fileName() );

    //Check that double release doesn't change anything.
    autoSave1.releaseLock();

    //Check that nothing changed
    QCOMPARE( KAutoSaveFile::staleFiles(fileQurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(fileQurl).at( 0 )->fileName() , autoSave1.fileName() );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).at( 0 )->fileName() , autoSave2.fileName() );

}

void KAutoSaveFileTest::test_freeStaleOnSetManaged()
{
    // Create a temporary file with a unique filename
    QTemporaryFile file( "test_freeStaleOnSetManaged" );
    QVERIFY( file.open() );
    QUrl fileQurl ( QUrl::fromLocalFile( file.fileName() ) );

    QTemporaryFile file2( "test_freeStaleOnSetManaged" );
    QVERIFY( file2.open() );
    QUrl file2Qurl ( QUrl::fromLocalFile( file2.fileName() ) );

    QTemporaryFile file3( "test_freeStaleOnSetManaged" );
    QVERIFY( file3.open() );
    QUrl file3Qurl ( QUrl::fromLocalFile( file3.fileName() ) );

    QCOMPARE( KAutoSaveFile::staleFiles(fileQurl).size() , 0 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).size() , 0 );
    QCOMPARE( KAutoSaveFile::staleFiles(file3Qurl).size() , 0 );

    // Create the autosave1 file lock
    KAutoSaveFile autoSave1( fileQurl );
    QVERIFY(autoSave1.open(QIODevice::ReadWrite));

    // Create the autosave file lock
    KAutoSaveFile autoSave2( file2Qurl );
    QVERIFY(autoSave2.open(QIODevice::ReadWrite));

    //Check that there is one of each autosave
    QCOMPARE( autoSave1.managedFile() , fileQurl );
    QCOMPARE( KAutoSaveFile::staleFiles(fileQurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(file3Qurl).size() , 0 );
    QCOMPARE( KAutoSaveFile::staleFiles(fileQurl).at( 0 )->fileName() , autoSave1.fileName() );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).at( 0 )->fileName() , autoSave2.fileName() );

    //Check that repeated setManaged on the same locked file doesn't unlock the file
    autoSave1.setManagedFile(fileQurl );

    //Check that there is one of each autosave
    QCOMPARE( autoSave1.managedFile() , fileQurl );
    QCOMPARE( KAutoSaveFile::staleFiles(fileQurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(file3Qurl).size() , 0 );
    QCOMPARE( KAutoSaveFile::staleFiles(fileQurl).at( 0 )->fileName() , autoSave1.fileName() );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).at( 0 )->fileName() , autoSave2.fileName() );

    //Check that setManaged to a new file unlocks the old file
    //but doesn't lock the new file until it is opened for write
    autoSave1.setManagedFile(file3Qurl );

    QCOMPARE( autoSave1.managedFile() , file3Qurl );
    QVERIFY2( !autoSave1.isWritable(),  "autosave1 is still locked" );
    QCOMPARE( KAutoSaveFile::staleFiles(fileQurl).size() , 0 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(file3Qurl).size() , 0 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).at( 0 )->fileName() , autoSave2.fileName() );

    autoSave1.open(QIODevice::ReadWrite);

    QCOMPARE( autoSave1.managedFile() , file3Qurl );
    QCOMPARE( KAutoSaveFile::staleFiles(fileQurl).size() , 0 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(file3Qurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(file3Qurl).at( 0 )->fileName() , autoSave1.fileName() );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).at( 0 )->fileName() , autoSave2.fileName() );

}

void KAutoSaveFileTest::test_freeStaleOnDestroy()
{
    // Create a temporary file with a unique filename
    QTemporaryFile file( "test_freeStaleOnDestroy" );
    QVERIFY( file.open() );
    QUrl fileQurl ( QUrl::fromLocalFile( file.fileName() ) );

    QTemporaryFile file2( "test_freeStaleOnDestroy" );
    QVERIFY( file2.open() );
    QUrl file2Qurl ( QUrl::fromLocalFile( file2.fileName() ) );

    // Create the autosave1 file lock
    KAutoSaveFile * autoSave1 = new KAutoSaveFile( fileQurl );
    QVERIFY(autoSave1->open(QIODevice::ReadWrite));

    // Create the autosave2 file lock
    KAutoSaveFile * autoSave2 = new KAutoSaveFile( file2Qurl );
    QVERIFY(autoSave2->open(QIODevice::ReadWrite));

    QCOMPARE( KAutoSaveFile::staleFiles(fileQurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(fileQurl).at( 0 )->fileName() , autoSave1->fileName() );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).at( 0 )->fileName() , autoSave2->fileName() );

    //Destroying an autosave removes the file and doesn't remove any other file
    QString autoSave1FileName( autoSave1->fileName() );
    delete autoSave1;

    QCOMPARE( KAutoSaveFile::staleFiles(fileQurl).size() , 0 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).at( 0 )->fileName() , autoSave2->fileName() );

    delete autoSave2;

    QCOMPARE( KAutoSaveFile::staleFiles(fileQurl).size() , 0 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).size() , 0 );

}

void KAutoSaveFileTest::test_multipleAutoSavesSameFile()
{
    QSet<QString> expected,  actual;

    QCOMPARE( KAutoSaveFile::allStaleFiles().size() , 0 );

    // Create two tempfiles
    QTemporaryFile file1( "test_multipleAutoSavesSameFile" );
    QVERIFY( file1.open() );
    QUrl file1Qurl ( QUrl::fromLocalFile( file1.fileName() ) );
    QTemporaryFile file2( "test_multipleAutoSavesSameFile" );
    QVERIFY( file2.open() );
    QUrl file2Qurl ( QUrl::fromLocalFile( file2.fileName() ) );

    // Create the autosave file lock
    KAutoSaveFile autoSave1( file1Qurl );
    QVERIFY(autoSave1.open(QIODevice::ReadWrite));
    KAutoSaveFile autoSave2( file2Qurl );
    QVERIFY(autoSave2.open(QIODevice::ReadWrite));

    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 1 );
    // repeat doesn't cause error
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::allStaleFiles().size() , 2 );

    expected << autoSave2.fileName() << autoSave1.fileName();
    Q_FOREACH(const KAutoSaveFile * kasave,  KAutoSaveFile::allStaleFiles()) {
        actual << kasave->fileName();
    }

    QVERIFY2( actual.contains( expected ),  "All expected autosaves are not present" );

    //Duplicate autosaves are independent
    KAutoSaveFile * autoSave1p1 = new KAutoSaveFile( file1Qurl );
    QVERIFY(autoSave1p1->open(QIODevice::ReadWrite));

    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 2 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::allStaleFiles().size() , 3 );

    expected.clear();
    actual.clear();
    expected << autoSave1.fileName() << autoSave1p1->fileName();
    actual << KAutoSaveFile::staleFiles(file1Qurl).at( 0 )->fileName()
           << KAutoSaveFile::staleFiles(file1Qurl).at( 1 )->fileName();

    QVERIFY2( actual == expected,  "Both autosaves with same filename are not present" );

    //Releasing the lock on 1 autosave doesn't release the lock on both autosaves
    autoSave1p1->releaseLock();

    QVERIFY2( autoSave1.isWritable(),  "autosave1 is not locked" );
    QVERIFY2( !autoSave1p1->isWritable(),  "autosave1p1 is locked" );
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 2 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::allStaleFiles().size() , 3 );

    expected.clear();
    actual.clear();
    expected << autoSave1.fileName() << autoSave1p1->fileName();
    actual << KAutoSaveFile::staleFiles(file1Qurl).at( 0 )->fileName()
           << KAutoSaveFile::staleFiles(file1Qurl).at( 1 )->fileName();

    //remove doesn't release the lock on either autosave
    QVERIFY(autoSave1p1->open(QIODevice::ReadWrite));
    autoSave1p1->remove();
    QVERIFY2( autoSave1.isWritable(),  "autosave1 is not locked" );
    QVERIFY2( !autoSave1p1->isWritable(),  "autosave1p1 is locked" );
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 2 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::allStaleFiles().size() , 3 );

    expected.clear();
    actual.clear();
    expected << autoSave1.fileName() << autoSave1p1->fileName();
    actual << KAutoSaveFile::staleFiles(file1Qurl).at( 0 )->fileName()
           << KAutoSaveFile::staleFiles(file1Qurl).at( 1 )->fileName();

    //releaseLock on removed file removes the now staleFile
    autoSave1p1->releaseLock();
    QVERIFY2( !autoSave1p1->isWritable(),  "autosave1p1 is locked" );
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::staleFiles(file2Qurl).size() , 1 );
    QCOMPARE( KAutoSaveFile::allStaleFiles().size() , 2 );

    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).at( 0 )->fileName() , autoSave1.fileName() );

}

void KAutoSaveFileTest::test_requireExplicitRemoval()
{

    // Create tempfiles
    QTemporaryFile file1( "test_requireExplicitRemoval" );
    QVERIFY( file1.open() );
    QUrl file1Qurl ( QUrl::fromLocalFile( file1.fileName() ) );

    // Create the autosave file lock
    KAutoSaveFile * autoSave1 = new KAutoSaveFile( file1Qurl );

    QVERIFY(autoSave1->open(QIODevice::ReadWrite));

    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 1 );

    autoSave1->releaseLock();
    QVERIFY(!autoSave1->isWritable() );
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 1 );

    QVERIFY(autoSave1->open(QIODevice::ReadWrite));
    autoSave1->remove();
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 1 );

    delete autoSave1;
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 0 );

}

void KAutoSaveFileTest::test_stealAbandonedLock()
{
    // Create a tempfile
    QTemporaryFile file1( "test_stealAbandonedLock" );
    QVERIFY( file1.open() );
    QUrl file1Qurl ( QUrl::fromLocalFile( file1.fileName() ) );

    // Create the autosave file lock
    KAutoSaveFile * autoSave1 = new KAutoSaveFile( file1Qurl );

    QVERIFY(autoSave1->open(QIODevice::ReadWrite));
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 1 );

    autoSave1->releaseLock();
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 1 );

    delete autoSave1;
    KAutoSaveFile * autoSave2 = KAutoSaveFile::staleFiles(file1Qurl).at( 0 );

    //Manipulating a read only file,  you do not have the lock, you can't remove the lock
    QVERIFY(autoSave2->open(QIODevice::ReadOnly));
    autoSave2->close();
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 1 );
    QVERIFY(!autoSave2->remove());
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 1 );

    QVERIFY(autoSave2->open(QIODevice::ReadWrite));
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 1 );

    QTextStream out( autoSave2 );
    out << "A couple of lines\n of text.\n";

    //Closing a file doesn't remove the lock
    autoSave2->close();
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 1 );

    //resizing a file doesn't remove the lock
    QVERIFY(autoSave2->resize(0));
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 1 );

    //removing a file doesn't delete the lock
    QVERIFY(autoSave2->open(QIODevice::ReadWrite));
    QVERIFY(autoSave2->remove());
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 1 );

    //deleting a non-existent locked file will remove the lock
    delete autoSave2;
    QCOMPARE( KAutoSaveFile::staleFiles(file1Qurl).size() , 0 );

}


void KAutoSaveFileTest::test_locking()
{
    QUrl normalFile(QString::fromLatin1("fish://user@example.com/home/remote/test.txt"));

    KAutoSaveFile saveFile(normalFile);

    QVERIFY(!QFile::exists(saveFile.fileName()));
    QVERIFY(saveFile.open(QIODevice::ReadWrite));

    const QList<KAutoSaveFile *> staleFiles(KAutoSaveFile::staleFiles(normalFile));

    QVERIFY(!staleFiles.isEmpty());

    KAutoSaveFile *saveFile2 = staleFiles.at(0);

    const QString fn = saveFile2->fileName();
    // It looks like $XDG_DATA_HOME/stalefiles/qttest/test.txtXXXfish_%2Fhome%2FremoteXXXXXXX
    QVERIFY2(fn.contains(QLatin1String("stalefiles/" ) + mainappname
                         + QLatin1String("/test.txt")), qPrintable(fn));
    QVERIFY2(fn.contains(QLatin1String("fish_%2Fhome%2Fremote")), qPrintable(fn));

    QVERIFY(QFile::exists(saveFile.fileName()));
    QVERIFY(QFile::exists(saveFile2->fileName()));
    QVERIFY(!saveFile2->open(QIODevice::ReadWrite));

    saveFile.releaseLock();

    QVERIFY(saveFile2->open(QIODevice::ReadWrite));

    delete saveFile2;

}


/**
 * This code is the same as the example code other than the QVERIFY and setup statements.
 * **/
class Document : public QObject
{
    //  Q_OBJECT // INTENTIONALLY commented out to not generate a 2nd moc
public:
    /**Setup the example code to answer the userdialog @p userResponse**/
    void exampleCodeWrapper(bool userResponse);

    void exampleOpen(const QUrl &url
                     , bool userResponse
                     , const QString &recoverableName
                     , const QString &unrecoverableName );

    void recoverFiles( QList<KAutoSaveFile *> staleFiles
                       , const QString &recoverableName
                       , const QString &unrecoverableName );
    KAutoSaveFile * m_autosave;
};

void Document::exampleOpen(QUrl const & url
                           , bool userResponse
                           , const QString &recoverableName
                           , const QString &unrecoverableName)
{
    // check whether autosave files exist:
    QList<KAutoSaveFile *> staleFiles = KAutoSaveFile::staleFiles(url);
    QCOMPARE( staleFiles.size(),  2 );
    QVERIFY( !staleFiles.isEmpty() );
    if (!staleFiles.isEmpty()) {
        if ( userResponse ) {
            recoverFiles(staleFiles, recoverableName, unrecoverableName);
            return;
        } else {
            // remove the stale files
            foreach (KAutoSaveFile *stale, staleFiles) {
                if ( stale->open(QIODevice::WriteOnly) ) {
                    QCOMPARE( stale->fileName(),  recoverableName );
                } else {
                    QCOMPARE( stale->fileName(),  unrecoverableName );
                }
                delete stale;
            }
        }
    }

    // create new autosave object
    m_autosave = new KAutoSaveFile(url, this);
    QVERIFY( m_autosave );

    // continue the process of opening file 'url'
}

void Document::recoverFiles( QList<KAutoSaveFile *> staleFiles
                             , const QString &recoverableName
                             , const QString &unrecoverableName
    ) {
    foreach (KAutoSaveFile *stale, staleFiles) {
        if (!stale->open(QIODevice::ReadWrite)) {
            // show an error message; we could not steal the lockfile
            // maybe another application got to the file before us?
            QCOMPARE( stale->fileName() ,  unrecoverableName );
            delete stale;
            continue;
        }
        QCOMPARE( stale->fileName() ,  recoverableName );

        Document *doc = new Document;
        doc->m_autosave = stale;

        QCOMPARE( QString( stale->readAll() ), QString( "Auto Save1 File\n" ) );

        // NOT part of example code, just cleanup
        delete stale;
    }
}

void Document::exampleCodeWrapper(bool userResponse)
{
    // Create a tempfile
    QTemporaryFile managedFile;
    QVERIFY( managedFile.open() );
    QTextStream out( &managedFile );
    out << "Managed File\n";

    QUrl managedFileQurl ( QUrl::fromLocalFile( managedFile.fileName() ) );

    // Create the recoverable locked file
    KAutoSaveFile * autoSaveRecoverable = new KAutoSaveFile( managedFileQurl );

    QVERIFY(autoSaveRecoverable->open(QIODevice::ReadWrite));
    QUrl recoverableQurl( autoSaveRecoverable->fileName() );
    QString recoverableFileName( autoSaveRecoverable->fileName() );
    QTextStream out2( autoSaveRecoverable );
    out2 << "Auto Save1 File\n";
    autoSaveRecoverable->releaseLock();
    delete autoSaveRecoverable;

    KAutoSaveFile autoSaveHard( managedFileQurl );

    QVERIFY(autoSaveHard.open(QIODevice::ReadWrite));
    QUrl unrecoverableQurl( autoSaveHard.fileName() );

    exampleOpen( managedFileQurl, userResponse
                 , recoverableFileName, autoSaveHard.fileName());

}

void KAutoSaveFileTest::test_exampleCode()
{
    Document doc1, doc2;
    doc1.exampleCodeWrapper( false);
    doc2.exampleCodeWrapper( true);
}


void KAutoSaveFileTest::test_deleteDirectory()
{
    QTemporaryFile file( "test_freeStaleOnDestroy" );
    QVERIFY( file.open() );
    QUrl fileQurl ( QUrl::fromLocalFile( file.fileName() ) );

    // Create the autosave1 file lock
    KAutoSaveFile * autoSave1 = new KAutoSaveFile( fileQurl );
    QVERIFY(autoSave1->open(QIODevice::ReadWrite));

    delete autoSave1;

    //The last autosave is gone and so should be the directory.
    QString staleFilesDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
                            + QString::fromLatin1("/stalefiles/")
                            + mainappname;

    QVERIFY2( ! QDir( staleFilesDir ).exists()
              , "application directory in stalefiles still exists after last kautosave is deleted" );

}

// void KAutoSaveFileTest::test_housekeeping()
// {
//     /*Implementation specific:
//      * There are 4 bad combinations of autosavefile, kalock and lock files*/

//     QString housekeepFile( "houseKeep" );
//     QString tempplate = KAutoSaveFilePrivate::fileNameTemplate(housekeepFile, true );



//     //* Only autosave file
//     QTemporaryFile asaveOnlyfile( tempplate );
//     QVERIFY( asaveOnlyfile.open() );

//     //* Only lock file
//     //* autosave with lock file
//     //* Only kalock file


//     // Create a tempfile

//     // Create the autosave file lock
//     KAutoSaveFile * autoSave1 = new KAutoSaveFile( file1Qurl );
//     filesToRemove << autoSave1->fileName();

//     autoSave1->setAutoRemove( false );
//     QVERIFY(autoSave1->open(QIODevice::ReadWrite));



//     QTemporaryFile randfile1("test_exampleCode");
//     QVERIFY( randfile1.open() );
//     QUrl randomQurl ( QUrl::fromLocalFile( randfile1.fileName() ) );

//     QCOMPARE( KAutoSaveFile::staleFiles(randomQurl).size() , 0 );



// }
