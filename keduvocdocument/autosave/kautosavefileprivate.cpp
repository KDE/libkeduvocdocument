/*  This file is part of the KDE libraries
    Copyright (c) 2014 Andreas Xavier <andxav at zoho dot com>

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

#include "kautosavefileprivate.h"

#include <stdio.h> // for FILENAME_MAX

#include <QtCore/QDir>
#include <QtCore/QCoreApplication>

#include <QStringList>
#include <QTextStream>
#include <QDebug>

namespace LexiAutoSave {

    QList<KAutoSaveFilePrivate::NamePair > KAutoSaveFilePrivate::findFilteredStales(
    const QUrl & fileQurl,  const QString &applicationName) {

    QString appName(applicationName);
    if (appName.isEmpty()) {
        appName = QCoreApplication::instance()->applicationName();
    }

    QList<KAutoSaveFilePrivate::NamePair > files;
    QString rootname( fileNameTemplate( fileQurl , appName,  false) );

    const QStringList dirs = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);
    Q_FOREACH (const QString &dir, dirs) {
        QDir appDir(dir + QString::fromLatin1("/stalefiles/") + appName);

        Q_FOREACH (const QString &filename, appDir.entryList(QDir::Files)) {
            if ( !(filename.endsWith(QLatin1String(".kalock") ) ) ) {
                continue;
            }

            QFile file( (appDir.absolutePath() + QLatin1Char('/') + filename) );

            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                file.close();
                continue;
            }

            QTextStream in( &file );

            QString kaSavefile( in.readLine() );
            QString mfile( in.readLine() );
            file.close();

            NamePair package(qMakePair(mfile, kaSavefile));

            if ( ! fileQurl.isEmpty() ) {
                if ( kaSavefile.contains( rootname ) ){
                    files << package;
                }
            } else {
                files << package;
            }
        }
    }
    return files;
}

QString KAutoSaveFilePrivate::encoded( const QString & str ) {
    return QString::fromLatin1(QUrl::toPercentEncoding( str ).constData() );
}

QString KAutoSaveFilePrivate::fileNameTemplate(
    const QUrl & file, const QString &applicationName,  bool isPadded)
{
    static const int maxNameLength = FILENAME_MAX;

    // Note: we drop any query string and user/pass info
    const QString protocol(encoded( file.scheme() ));
    QString name1(encoded( file.fileName() ) );

    QString path(file.adjusted(QUrl::RemoveFilename | QUrl::StripTrailingSlash).path());
    QString pathEncoded( encoded( path ) );

    // Remove any part of the path to the right if it is longer than the max file size and
    // ensure that the max filesize takes into account the other parts of the tempFileName
    // Subtract 1 for the _ char, 7 is for the .kalock
    int trimNumber = qMax( 0,  - maxNameLength + pathEncoded.size() + paddingLength()
                           + name1.size() + protocol.size() + 1 + 7 );
    path.chop( trimNumber );
    QString pathEncodedFinal( encoded( path ) );

    // tempName = fileName + protocol + _ + path.truncated + padding
    // This is done so that the separation between the filename and path can be determined
    QString name2(protocol + QLatin1Char('_') + pathEncodedFinal);


    QString appName(applicationName);
    if (appName.isEmpty()) {
        appName = QCoreApplication::instance()->applicationName();
    }

    QString padding( QString::fromLatin1( "X" ) );
    padding.repeated( paddingLength() );

    QString tempplate = lockDir( appName ) + QChar::fromLatin1('/')
                        + name1 + name2 + ( isPadded ? padding : QString( ) );

    return tempplate;
}

bool KAutoSaveFilePrivate::tryLock()
{
    unlock();

    //Create a persistent long term lock
    m_lock = new QLockFile(autosaveRoot + QString::fromLatin1(".lock"));
    m_lock->setStaleLockTime(0);

    if (!m_lock->isLocked() && !m_lock->tryLock()) {
        //Try harder.  The only way the user found this file is using
        //staleFiles so them must have decided to steal the lock
        if ( ( m_lock->error() == QLockFile::LockFailedError ) ) {
            m_lock->removeStaleLockFile();
        }
        if (!m_lock->tryLock()) {
            unlock();
            return false;
        }
    }

    return true;
}

}
