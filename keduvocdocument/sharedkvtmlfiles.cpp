/*
 * scan a group of KVTML documents to get information from them
 * SPDX-FileCopyrightText: 2007 Jeremy Whiting <jpwhiting@kde.org>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "sharedkvtmlfiles.h"

#include "keduvocdocument.h"

#include <KLocalizedString>
#include <kio/copyjob.h>
#include <kio/job.h>

#include <QDir>
#include <QStandardPaths>

#include <QDebug>

class SharedKvtmlFilesPrivate
{
public:
    /** default constructor calls rescan*/
    SharedKvtmlFilesPrivate()
    {
        rescan();
    }

    /** default destructor */
    ~SharedKvtmlFilesPrivate()
    {}

    /** scan the folder for documents, and record what is found */
    void rescan();

    /** list of all files found */
    QStringList m_fileList;

    /** list of all files titles found */
    QStringList m_titleList;

    /** list of file comments */
    QStringList m_commentList;

    /** map of files by language key */
    QMap<QString, QStringList> m_filesByLang;
};

Q_GLOBAL_STATIC( SharedKvtmlFilesPrivate, sharedKvtmlFilesPrivate )

void SharedKvtmlFilesPrivate::rescan()
{
    this->m_titleList.clear();
    this->m_commentList.clear();
    this->m_filesByLang.clear();
    this->m_fileList.clear();

    QStringList locales;

    // Get all kvtml paths
    QStringList nameFilter;
    nameFilter.append(QStringLiteral("*.kvtml"));
    QStringList dataPaths = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, QStringLiteral("apps/kvtml"), QStandardPaths::LocateDirectory);
    Q_FOREACH (const QString &path, dataPaths) {
        qDebug() << "Checking path " << path << " for kvtml files";
        QStringList locales = QDir( path ).entryList( QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name );
        Q_FOREACH (const QString &locale, locales) {
            QStringList files = QDir( path + '/' + locale ).entryList(nameFilter, QDir::Files );
            Q_FOREACH (const QString &filename, files) {
                QString filePath = path + '/' + locale + '/' + filename;
                this->m_fileList << filePath;
                this->m_filesByLang[locale].append( filePath );
            }
        }
    }

    KEduVocDocument *doc = new KEduVocDocument();
    for ( int i = 0; i < this->m_fileList.size(); ++i ) {

        // open the file
        doc->open( QUrl::fromLocalFile( this->m_fileList[i] ) );

        // add it's title to the title list
        this->m_titleList.append( doc->title() );

        // add it's comment to the comment list
        this->m_commentList.append( doc->documentComment() );
    }
    delete doc;
}

void SharedKvtmlFiles::rescan()
{
    sharedKvtmlFilesPrivate->rescan();
}

QStringList SharedKvtmlFiles::languages()
{
    return sharedKvtmlFilesPrivate->m_filesByLang.keys();
}

QStringList SharedKvtmlFiles::fileNames( const QString &language )
{
    // return files by language for given language if it's not empty
    // otherwise return all filenames
    return language.isEmpty() ? sharedKvtmlFilesPrivate->m_fileList : sharedKvtmlFilesPrivate->m_filesByLang.value( language );
}

QStringList SharedKvtmlFiles::titles( const QString &language )
{
    QStringList retlist;

    if ( language.isEmpty() ) {
        retlist = sharedKvtmlFilesPrivate->m_titleList;
    } else {
        QStringList filenames = sharedKvtmlFilesPrivate->m_filesByLang.value( language );
        for ( int i = 0; i < filenames.size(); ++i ) {
            retlist.append( sharedKvtmlFilesPrivate->m_titleList[sharedKvtmlFilesPrivate->m_fileList.indexOf( filenames[i] )] );
        }
    }

    return retlist;
}

QStringList SharedKvtmlFiles::comments( const QString &language )
{
    QStringList retlist;

    if ( language.isEmpty() ) {
        retlist = sharedKvtmlFilesPrivate->m_commentList;
    } else {
        QStringList filenames = sharedKvtmlFilesPrivate->m_filesByLang.value( language );
        for ( int i = 0; i < filenames.size(); ++i ) {
            retlist.append( sharedKvtmlFilesPrivate->m_commentList[sharedKvtmlFilesPrivate->m_fileList.indexOf( filenames[i] )] );
        }
    }

    return retlist;
}

void SharedKvtmlFiles::sortDownloadedFiles()
{
    QStringList nameFilter;
    nameFilter.append(QStringLiteral("*.kvtml"));
    QStringList dataPaths = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, QStringLiteral("apps/kvtml"), QStandardPaths::LocateDirectory);
    QStringList unsortedFiles;
    Q_FOREACH (const QString &path, dataPaths) {
        QStringList files = QDir( path ).entryList(nameFilter, QDir::Files | QDir::NoDotAndDotDot );
        Q_FOREACH (const QString &filename, files) {
            unsortedFiles.append( path + '/' + filename);
        }
    }

    KEduVocDocument doc;

    while ( !unsortedFiles.isEmpty() ) {
        QUrl fileUrl( QUrl::fromLocalFile( unsortedFiles.first() ) );
        // find the file's locale
        // open the file
        doc.open( fileUrl );

        if (doc.identifierCount() == 1) {
            QString locale = doc.identifier( 0 ).locale();

            // make sure the locale sub-folder exists
            QUrl pathUrl = QUrl( fileUrl );
            pathUrl = QUrl( pathUrl.toString(QUrl::RemoveFilename) + '/' + locale );
            KIO::mkdir( pathUrl );

            // move the file into the locale sub-folder
            KIO::move( fileUrl, pathUrl );
        }

        // take off the one we just did
        unsortedFiles.removeFirst();
    }

    nameFilter = QStringList(QStringLiteral("*.txt"));
    QStringList khangmanFiles;
    Q_FOREACH (const QString &path, dataPaths) {
        QStringList files = QDir( path ).entryList(nameFilter, QDir::Files );
        Q_FOREACH (const QString &filename, files) {
            khangmanFiles.append( path + '/' + filename);
        }
    }

    // move khangman files into
    while ( !khangmanFiles.isEmpty() ) {
        QUrl fileUrl( QUrl::fromLocalFile( khangmanFiles.first() ) );
        QUrl destDir = QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/khangman/data/");
        // do this better with KStandardDirs stuff
        KIO::move( fileUrl, destDir);
        khangmanFiles.removeFirst();
    }

    rescan();
}
