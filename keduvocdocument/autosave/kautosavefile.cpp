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

#include "kautosavefile.h"
#include "kautosavefileprivate.h"

#include <stdio.h> // for FILENAME_MAX

#include <QtCore/QLatin1Char>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include "qlockfile.h"
#include <QTemporaryFile>
#include "krandom.h"
#include "qstandardpaths.h"

#include "qdebug.h"

/**
 * @file
 * The implementation of the kAutoSaveFile is as follows:
 * Given a file, called filez. Find a directory that is writable.
 * In that directory create 3 files called
 * filez_mangled_nameXXXXX, filez_mangled_nameXXXXX.lock and filez_mangled_nameXXXXX.kalock
 * filez_mangled_nameXXXXX is the auto save file passed back to the user.
 * filez_mangled_nameXXXXX.lock is the QLock file used to preserve the lock
 * filez_mangled_nameXXXXX.kalock stores the meta information: real file URL and autosavefile location.
 *
 *  Only 4 of the 16 possible states of these 3 files existing/not existing are valid.
 *  Here is a table indicating which are good
 *
 *  autosave lock  kalock  valid
 *  0        0     0       good - no files
 *  1        0     0       bad  - no kalock to find autosave
 *  0        1     0       bad  - no kalock to find autosave
 *  1        1     0       bad  - no kalock to find autosave
 *  0        0     1       bad  - kalock but no file
 *  1        0     1       good - unlocked autosave
 *  0        1     1       good - locked empty autosave
 *  1        1     1       good - locked non-empty autosave
 *
 *
 * **/

namespace LexiAutoSave {

KAutoSaveFile::KAutoSaveFile(const QUrl &filename, QObject *parent)
    : QFile(parent),
      d(new KAutoSaveFilePrivate)
{
    d->managedFileNameChanged = true;
    d->managedFile = filename;
}

KAutoSaveFile::KAutoSaveFile(QObject *parent)
    : QFile(parent),
      d(new KAutoSaveFilePrivate)
{

}

KAutoSaveFile::~KAutoSaveFile()
{
    if ( d->isLocked() ) {
        QFile::remove();
        d->unkalock();
    }

    d->unlock();
    delete d;
}

void KAutoSaveFile::releaseLock()
{
    if (d->isLocked()) {
        d->unlock();

        if ( !exists() ) {
            d->unkalock();
        }
    }
    QFile::close();
}

bool KAutoSaveFile::remove()
{
    if (d->isLocked()) {
        bool ret = QFile::remove();
        return ret;
    } else {
        return false;
    }

}

bool KAutoSaveFile::open(OpenMode openmode)
{
    if (d->managedFile.isEmpty()) {
        return false;
    }

    if (d->managedFileNameChanged) {

        QString staleFilesDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
                                + QString::fromLatin1("/stalefiles/")
                                + QCoreApplication::instance()->applicationName();
        if (!QDir().mkpath(staleFilesDir)) {
            return false;
        }

        QString tempplate  = KAutoSaveFilePrivate::fileNameTemplate(d->managedFile, QString(),  true ) ;

        //Generate a file name
        QTemporaryFile myname( tempplate );
        if ( ! myname.open() ) {
            return false;
        }
        myname.setAutoRemove( false );

        setFileName(myname.fileName());

        d->autosaveRoot = myname.fileName();
        d->managedFileNameChanged = false;
    }

    if (openmode & QIODevice::WriteOnly) {

        if (!QFile::open(openmode) || ! d->kalock() || !d->tryLock()) {
            d->unkalock();
            QFile::close();
            return false;
        }

        return true;
    } else {
        releaseLock();
        return QFile::open(openmode);
    }
}

QUrl KAutoSaveFile::managedFile() const
{
    return d->managedFile;
}

//remove set managed it only makes the initialization overly complicated
void KAutoSaveFile::setManagedFile(const QUrl &filename)
{
    if ( filename == d->managedFile ) {
        return;
    }

    if ( ! filename.isEmpty() ) {
        if ( d->isLocked() ) {
            remove();
            d->unlock();
            d->unkalock();
        }
    }

    d->managedFileNameChanged = true;
    d->managedFile = filename;
}

QList<KAutoSaveFile *> KAutoSaveFile::staleFiles(const QUrl &filename, const QString &appName)
{

    // get stale files
    const QList< KAutoSaveFilePrivate::NamePair > files
        = KAutoSaveFilePrivate::findFilteredStales(filename, appName);

    QList<KAutoSaveFile *> list;
    KAutoSaveFile *asFile;

    // contruct a KAutoSaveFile for each stale file
    Q_FOREACH (const KAutoSaveFilePrivate::NamePair & filenames, files) {
        asFile = new KAutoSaveFile( QUrl( filenames.first ) );
        asFile -> setFileName( filenames.second );
        asFile->d->autosaveRoot = filenames.second;
        asFile->d->managedFileNameChanged = false;

        list.append(asFile);
    }

    return list;
}

QList<KAutoSaveFile *> KAutoSaveFile::allStaleFiles(const QString &applicationName)
{
    return staleFiles( QUrl(),  applicationName );
}

}

#include "moc_kautosavefile.cpp"
