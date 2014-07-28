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

#include "readermanager.h"
#include "keduvocdocument.h"

#include "readerwriters/readerbase.h"
#include "readerwriters/failedreader.h"
#include "readerwriters/dummyreader.h"

#include "readerwriters/keduvoccsvreader.h"
#include "readerwriters/keduvockvtml2reader.h"
#include "readerwriters/keduvocwqlreader.h"
#include "readerwriters/keduvocpaukerreader.h"
#include "readerwriters/keduvocvokabelnreader.h"
#include "readerwriters/keduvocxdxfreader.h"

#include "qdebug.h"

#include <QtCore/QIODevice>

ReaderManager::ReaderPtr ReaderManager::reader(QIODevice & dev)
{
    if ( !dev.isReadable() ) {
        //@todo add a useful error message
        return ReaderPtr( new FailedReader(KEduVocDocument::FileCannotRead) );
    }

    if ( dev.isSequential() ) {
        //@todo add a useful error message
        return ReaderPtr( new FailedReader(KEduVocDocument::FileCannotRead) );
    }

    ReaderPtr reader;

    reader = ReaderPtr( new DummyReader( dev ) );
    if ( reader->isParsable() ) {
       return reader;
    }
    dev.seek( 0 );

    reader = ReaderPtr( new KEduVocVokabelnReader( dev ) );
    if ( reader->isParsable() ) {
        return reader;
    }
    dev.seek( 0 );

    reader = ReaderPtr( new KEduVocPaukerReader( dev ) );
    if ( reader->isParsable() ) {
        return reader;
    }
    dev.seek( 0 );

    reader = ReaderPtr( new KEduVocXdxfReader( dev ) );
    if ( reader->isParsable() ) {
        return reader;
    }
    dev.seek( 0 );

    reader = ReaderPtr( new KEduVocKvtml2Reader( dev ) );
    if ( reader->isParsable() ) {
        return reader;
    }
    dev.seek( 0 );

    reader = ReaderPtr( new KEduVocWqlReader( dev ) );
    if ( reader->isParsable() ) {
        return reader;
    }
    dev.seek( 0 );

    reader = ReaderPtr( new KEduVocCsvReader( dev ) );
    if ( reader->isParsable() ) {
        return reader;
    }
    dev.seek( 0 );

    return ReaderPtr( new FailedReader(KEduVocDocument::FileTypeUnknown) );

}
