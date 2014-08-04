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

#ifndef LKAUTOSAVEFILEPRIVATE_H
#define LKAUTOSAVEFILEPRIVATE_H

#include <QLockFile>
#include <QPair>
#include <QString>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QStandardPaths>

#include <QDebug>

namespace LexiAutoSave {

/**Private helper class for KAutoSaveFile*/
class KAutoSaveFilePrivate
{
public:
    /** constructor */
    KAutoSaveFilePrivate()
        : managedFile()
        , autosaveRoot()
        , m_lock(0)
        , managedFileNameChanged(false)
    {}

    typedef QPair<QString , QString > NamePair; ///< (managedfilename, autosavefilename) pair

    /** @brief Filter the stale file list by @p appname and @p file.
     * @details If file is QUrl() then grab all of the files.
     * @returns a list of the managed and autosave files.
     **/
    static QList< NamePair > findFilteredStales(const QUrl & file, const QString &appname);


    /** @brief The directory of the locks for this @p appname
     @return directory name **/
    static QString lockDir(const QString & appname ) {
        return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
            + QString::fromLatin1("/stalefiles/")
            + appname;
    }


    /** Constructs the root template of the three filenames from the @p qurl
     * The .lock file will be used to lock the other two.
     * The .kalock file will store the KAutoSaveFile meta information
     * The third file will be passed back to the user as the
     * autosavefile
     * @p isPadded determines if the padding is added.
     * @return the template for the filenames.**/
    static QString fileNameTemplate( const QUrl & qurl, const QString &appname , bool isPadded);

    /** safely encodes @p str **/
    static QString encoded( const QString & str );

    /** length of random padding added by QTemporaryFile **/
    static int paddingLength(){ return 8;}

    /** @return true if is locked  **/
    bool isLocked() const {
        return (m_lock && m_lock->isLocked());
    }

    /** @return true if was locked  **/
    bool tryLock();

    /** unlocks the lock
     and deletes the directory if empty **/
    void unlock() {
        if( m_lock != NULL ) {
            delete m_lock;
            m_lock = NULL;
            //this only succeeds it the directory is empty
            QDir().rmdir(QFileInfo(autosaveRoot).absolutePath());
        }
    }

    /** Write the meta information to the kalock file
     @return true on success.**/
    bool kalock()
    {
        QFile kalockFile(autosaveRoot + QString::fromLatin1(".kalock" ) );
        bool goodOpen( kalockFile.open( QIODevice::ReadWrite ));
        QTextStream out( & kalockFile );
        out << autosaveRoot << "\n"
            << managedFile.toEncoded(QUrl::RemoveUserInfo | QUrl::RemoveQuery) << "\n";
        kalockFile.close();
        return goodOpen;
    }

    /** Remove the kalock file
     This must also be accompanied with removing the autosave file
     and the directory if this is the last autosave file.
    **/
    void unkalock()
    {
        QFile kalockFile(autosaveRoot + QString::fromLatin1(".kalock" ) );
        kalockFile.remove();
        //this only succeeds it the directory is empty
        QDir().rmdir(QFileInfo(kalockFile).absolutePath());
    }

    QUrl managedFile;             ///< url of the real file
    QString autosaveRoot;         ///< root for the filenames of the autosave
    QLockFile *m_lock;            ///< lock on the autosave file
    bool managedFileNameChanged;  ///< an initialization dirty bit
                                  ///@todo remove this when setManaged is removed >/
};

}
#endif
