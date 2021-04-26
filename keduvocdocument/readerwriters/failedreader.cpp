/*
 * SPDX-FileCopyrightText: 2014 Andreas Xavier <andxav at zoho dot com>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "failedreader.h"

#include <KLocalizedString>
#include <QIODevice>


FailedReader::FailedReader(KEduVocDocument::ErrorCode inerror, const QString & msg)
    : m_error( KEduVocDocument::Unknown )
    , m_errorMessage( msg )
{
    if ( inerror != KEduVocDocument::NoError ) {
        m_error = inerror;
    }
    if ( msg.isNull() ) {
        m_errorMessage = i18n( "Error while reading file" );
    }
}

bool FailedReader::isParsable()
{
    return true;
}

KEduVocDocument::FileType FailedReader::fileTypeHandled()
{
    return KEduVocDocument::KvdNone;
}

KEduVocDocument::ErrorCode FailedReader::read(KEduVocDocument &) {
    return m_error;
}

QString FailedReader::errorMessage() const
{
    return m_errorMessage;
}
