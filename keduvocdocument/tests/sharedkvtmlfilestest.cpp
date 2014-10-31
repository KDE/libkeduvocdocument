/***************************************************************************
 *   Copyright (C) 2014 by Jeremy Whiting <jpwhiting@kde.org>              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

/** @file
 * \brief sharedkvtmlfiles test application.
 * @author Jeremy Whiting <jpwhiting@kde.org>
 */

#include "keduvocdocument.h"
#include "sharedkvtmlfiles.h"

#include <QCoreApplication>

#include <QDebug>
#include <QUrl>


int main( int argc, char ** argv )
{
    QCoreApplication::setApplicationName("sharedkvtmlfilestest");
    QCoreApplication::setApplicationVersion("0.1");
    QCoreApplication::setOrganizationDomain("kde.org");
    QCoreApplication app( argc, argv );

    QStringList languages = SharedKvtmlFiles::languages();
    Q_FOREACH (const QString &language, languages) {
        qDebug() << "Got language: " << language;
        QStringList files = SharedKvtmlFiles::fileNames(language);
        Q_FOREACH (const QString &file, files) {
            qDebug() << "Got filename " << file;
        }
    }

    return 0;
}
