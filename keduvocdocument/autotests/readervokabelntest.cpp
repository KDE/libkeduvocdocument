/*
 * SPDX-FileCopyrightText: 2014 Andreas Xavier <andxav at zoho dot com>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "keduvocdocument.h"
#include "readermanager.h"
#include "keduvocvokabelnreader.h"

#include "readerTestHelpers.h"

#include <QDebug>
#include <QTest>

namespace VokabelnReaderUnitTests
{

/**
 * @file readervokabelntest.cpp
 * Unit Tests of parsing of Vokabeln strings
 *
 * Each test creates a QString of Vokabeln format
 * reads, parses and verifies the parse.  The portions of the
 * generator object relevant to each test are defined in this file.
 *
 * The current tests are cursory and the reader is fragile.
 * */

/**
 * @brief Unit Tests of parsing of Vokabeln strings
 *
 * Each test creates a string
 * reads, parses and verifies the parse.
 * */

class VokabelnReaderTest : public QObject
{
    Q_OBJECT

private slots:
    /** Initialize the string*/
    void init();
    /** Tow word doc*/
    void testParseTwoWord();
    /** InvalidDoc*/
    void testParseInvalid();
private :
    QString oneGoodDoc;
    QString oneBadDoc;
    KEduVocDocument::FileType myType;
};

void VokabelnReaderTest::init() {
    oneGoodDoc =
        QStringLiteral( "\"A Title of S0me Sort\n" ) \
        + "en - de\n" \
        + "Still more text\",3,456,789\n" \
        + "0\n" \
        + "Skipped Line\n" \
        + "\"en\",\"de\"\n" \
        + "skipped stuff\n" \
        + "8. Lernhilfe\n" \
        + "1\n" \
        + "2\n" \
        + "3\n" \
        + "4\n" \
        + "5\n" \
        + "6\n" \
        + "7\n" \
        + "8\n" \
        + "9\n" \
        + "0\n" \
        + "11\n" \
        + "12\n" \
        + "13\n" \
        + "14\n" \
        + "15\n" \
        + "\"dog\",\"Hund\",1\n" \
        + "skip 1\n" \
        + "skip 2\n" \
        + "\"cat\",\"Katze\",2\n" \
        + "skip 1\n" \
        + "skip 2\n" \
        + "skip a\n" \
        + "skip b\n" \
        + "skip c\n" \
        + "\"Lesson Title A\"\n" \
        + "skip a\n" \
        + "\"Lesson Title B\"\n" \
        + "skip b\n" \
        + "\"Extra Lesson Title\"\n";

    oneBadDoc =
                QStringLiteral( "\"A Title of S0me Sort\n" ) \
        + "en - de\n" \
        + "Still more text\",3,456,789\n" \
        + "0\n" \
        + "Skipped Line\n" ;

    myType = KEduVocDocument::Vokabeln;
}

void VokabelnReaderTest::testParseTwoWord()
{
    KVOCREADER_EXPECT( oneGoodDoc , KEduVocDocument::NoError , myType );
}

void VokabelnReaderTest::testParseInvalid()
{
    KVOCREADER_EXPECT( oneBadDoc , KEduVocDocument::FileReaderFailed, myType );
}

}

QTEST_MAIN( VokabelnReaderUnitTests::VokabelnReaderTest )



#include "readervokabelntest.moc"
