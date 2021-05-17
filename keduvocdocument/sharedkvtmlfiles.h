/*
 * scan a group of KVTML documents to get information from them
 * SPDX-FileCopyrightText: 2007 Jeremy Whiting <jpwhiting@kde.org>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SHAREDKVTMLFILES_H
#define SHAREDKVTMLFILES_H

#include "keduvocdocument_export.h"

#include <QStringList>

/**
 *namespace and singleton class to scan groups of kvtml files
 *from shared kvtml location, and give information
 *about files found there
 *@author Jeremy Whiting
 */
namespace SharedKvtmlFiles
{
    /** get list of all languages found in any files */
    KEDUVOCDOCUMENT_EXPORT QStringList languages();

    /** get list of filenames found of given language
     *@param language language requested QString() for all languages
     *@return a list of filenames with words in language
     */
    KEDUVOCDOCUMENT_EXPORT QStringList fileNames( const QString &language = QString() );

    /** get the list of document titles found of a given language
     *@param language requested language QString() for all titles
     *@return a list of document titles with words in language
     */
    KEDUVOCDOCUMENT_EXPORT QStringList titles( const QString &language = QString() );

    /** get the list of document remarks found of a given language
     *@param language requested language QString() for all comments
     *@return a list of document remarks with words in language
     */
    KEDUVOCDOCUMENT_EXPORT QStringList comments( const QString &language = QString() );

    /** rescan the shared kvtml locations */
    KEDUVOCDOCUMENT_EXPORT void rescan();

    /** sort files downloaded to kvtml top-level dir into locale sub-folders */
    KEDUVOCDOCUMENT_EXPORT void sortDownloadedFiles();
}

#endif

