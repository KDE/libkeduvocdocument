/***************************************************************************
        scan a group of KVTML documents to get information from them
    -----------------------------------------------------------------------
    copyright      : (C) 2007 Jeremy Whiting <jpwhiting@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "sharedkvtmlfiles.h"

#include "keduvocdocument.h"

#include <kio/copyjob.h>
#include <kio/job.h>
#include <klocale.h>
#include <kstandarddirs.h>

#include <QDir>
#include <QList>
#include <QSet>

#include <kglobal.h>

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

K_GLOBAL_STATIC( SharedKvtmlFilesPrivate, sharedKvtmlFilesPrivate )

void SharedKvtmlFilesPrivate::rescan()
{
    this->m_titleList.clear();
    this->m_commentList.clear();
    this->m_filesByLang.clear();
    this->m_fileList.clear();

    QStringList locales;

    QStringList dataPaths = KGlobal::dirs()->findDirs( "data", "kvtml/" );
    for ( int i = 0; i < dataPaths.size(); ++i ) {
        locales += QDir( dataPaths[i] ).entryList( QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name );
    }

    // remove duplicates
    locales = locales.toSet().toList();

    for ( int i = 0; i < locales.size(); ++i ) {
        // get all files for this language
        QStringList thisLangFiles = KGlobal::dirs()->findAllResources( "data",
                                    QString( "kvtml/%1/*.kvtml" ).arg( QDir( locales[i] ).dirName() ) );
        // add them to the big list
        this->m_fileList << thisLangFiles;

        // then add them to their respective language maps
        for ( int j = 0; j < thisLangFiles.size(); ++j ) {
            this->m_filesByLang[locales[i]].append( thisLangFiles[j] );
        }
    }

    KEduVocDocument *doc = new KEduVocDocument();
    for ( int i = 0; i < this->m_fileList.size(); ++i ) {

        // open the file
        doc->open( KUrl::fromPath( this->m_fileList[i] ) );

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
    QStringList unsortedFiles = KGlobal::dirs()->findAllResources( "data",
                                QString( "kvtml/*.kvtml" ) );

    KEduVocDocument doc;

    while ( !unsortedFiles.isEmpty() ) {
        KUrl fileUrl( KUrl::fromPath( unsortedFiles.first() ) );
        // find the file's locale
        // open the file
        doc.open( fileUrl );

        if (doc.identifierCount() == 1) {
            QString locale = doc.identifier( 0 ).locale();

            // make sure the locale sub-folder exists
            KUrl pathUrl( fileUrl );
            pathUrl.setFileName( QString() );
            pathUrl.addPath( locale );
            KIO::mkdir( pathUrl );

            // move the file into the locale sub-folder
            KIO::move( fileUrl, pathUrl );
        }

        // take off the one we just did
        unsortedFiles.removeFirst();
    }
    
    QStringList khangmanFiles = KGlobal::dirs()->findAllResources( "data", 
                                QString( "kvtml/*.txt" ) );
    
    // move khangman files into
    while ( !khangmanFiles.isEmpty() ) {
        KUrl fileUrl( KUrl::fromPath( khangmanFiles.first() ) );
        KUrl destDir = KUrl::fromPath(KStandardDirs::locateLocal("appdata", "khangman/data/"));
        // do this better with KStandardDirs stuff
        KIO::move( fileUrl, destDir);
        khangmanFiles.removeFirst();
    }

    rescan();
}
