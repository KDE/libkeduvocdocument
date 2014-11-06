/*
 Copyright 2014 Andreas Xavier <andxav at zoho dot com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 USA
*/

#include "failedreader.h"

#include <klocalizedstring.h>
#include <QtCore/QIODevice>


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
