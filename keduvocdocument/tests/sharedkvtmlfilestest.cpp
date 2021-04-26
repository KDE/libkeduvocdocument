/*
 * SPDX-FileCopyrightText: 2014 Jeremy Whiting <jpwhiting@kde.org>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

/** @file
 * \brief sharedkvtmlfiles test application.
 * @author Jeremy Whiting <jpwhiting@kde.org>
 */

#include "keduvocdocument.h"
#include "sharedkvtmlfiles.h"

#include <QCoreApplication>

#include <QDebug>


int main( int argc, char ** argv )
{
    QCoreApplication::setApplicationName(QStringLiteral("sharedkvtmlfilestest"));
    QCoreApplication::setApplicationVersion(QStringLiteral("0.1"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));
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
