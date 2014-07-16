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

#include "keduvocdocument.h"
#include "keduvocvokabelnreader.h"

#include <kdebug.h>
#include <qtest_kde.h>

namespace VokabelnReaderUnitTests
{

/**
 * @file Unit Tests of parsing of Vokabeln strings
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
};

void VokabelnReaderTest::init() {
    oneGoodDoc =
        QString( "\"A Title of S0me Sort\n" ) \
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
        + "\"cat\",\"Katze\",21\n" \
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
    }


// These macros are to force the QCOMPARE/QVERIFY to be in the test function.
// QCOMPARE must be in the test function.

// Check that a parse returns errcode
#define PARSE_EXPECT_CORE(instring, expected,  verbose)                 \
    do {                                                                \
        QByteArray array( instring.toLatin1() );                        \
        QBuffer * buffer = new QBuffer( &array );                       \
        buffer->open( QIODevice::ReadOnly );                            \
        KEduVocDocument docRead;                                        \
        KEduVocVokabelnReader reader( buffer );                         \
        KEduVocDocument::ErrorCode actual(reader.readDoc( &docRead ) ); \
        if (verbose && actual != expected) {                            \
        }                                                               \
        QCOMPARE( int( actual ), int( expected ) );                     \
    }  while ( 0 )

// Check that a parse returns errcode. This ignores the error.
#define PARSE_EXPECT(gen, expected)                                   \
    do {                                                              \
        PARSE_EXPECT_CORE( gen , expected , true);                    \
    }  while ( 0 )

// Check that a parse returns errcode. This ignores the error.
#define PARSE_DONT_EXPECT(gen, expected)                                \
    do {                                                                \
        QEXPECT_FAIL("", " This is a known bug.", Continue);            \
        PARSE_EXPECT_CORE( gen , expected ,  false);                    \
    }  while ( 0 )



void VokabelnReaderTest::testParseTwoWord()
{
    PARSE_EXPECT( oneGoodDoc ,  KEduVocDocument::NoError );
}

void VokabelnReaderTest::testParseInvalid()
{
    QString invalid = "First character is not a double quote " + oneGoodDoc;

    PARSE_EXPECT( invalid ,  KEduVocDocument::FileReaderFailed );
}

}

QTEST_KDEMAIN_CORE( VokabelnReaderUnitTests::VokabelnReaderTest )



#include "vokabelnreadertest.moc"
