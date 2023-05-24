/*
 * SPDX-FileCopyrightText: 2014 Jeremy Whiting <jpwhiting@kde.org>
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

/** @file
 * \brief sharedkvtmlfiles test application.
 * @author Jeremy Whiting <jpwhiting@kde.org>
 */

#include "sharedkvtmlfiles.h"
#include "keduvocdocument.h"

#include <QCoreApplication>

#include <QDebug>

int main(int argc, char **argv)
{
    QCoreApplication::setApplicationName(QStringLiteral("sharedkvtmlfilestest"));
    QCoreApplication::setApplicationVersion(QStringLiteral("0.1"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));
    QCoreApplication app(argc, argv);

    const QStringList languages = SharedKvtmlFiles::languages();
    for (const QString &language : languages) {
        qDebug() << "Got language: " << language;
        const QStringList files = SharedKvtmlFiles::fileNames(language);
        for (const QString &file : files) {
            qDebug() << "Got filename " << file;
        }
    }

    return 0;
}
