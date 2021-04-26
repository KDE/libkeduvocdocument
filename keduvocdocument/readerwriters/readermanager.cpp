/*
 * SPDX-FileCopyrightText: 2014 Andreas Xavier <andxav at zoho dot com>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "readermanager.h"

#include "readerwriters/readerbase.h"
#include "readerwriters/failedreader.h"
#include "readerwriters/dummyreader.h"

#include "readerwriters/keduvoccsvreader.h"
#include "readerwriters/keduvockvtml2reader.h"
#include "readerwriters/keduvocwqlreader.h"
#include "readerwriters/keduvocpaukerreader.h"
#include "readerwriters/keduvocvokabelnreader.h"
#include "readerwriters/keduvocxdxfreader.h"

#include <QDebug>


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
