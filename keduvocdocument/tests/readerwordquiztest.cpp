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
#include "keduvocwqlreader.h"
#include <qobject.h>

#include <kdebug.h>
#include <qtest_kde.h>

namespace WordQuizReaderTests {

/**
 * @file Unit Tests of parsing of WordQuiz WQL Files
 *
 * Each test creates a QString in WQL format
 * */

/**
 * @brief Unit Tests of parsing of WQL
 *
 * Each test creates a string
 * reads, parses and verifies the parse.
 *
 * Most of the WQL file is ignored in the parser
 * , so it is replaced in test with a=1 etc.
 * */

class WqlReaderTest : public QObject
{
    Q_OBJECT

private slots:
    /** Smallest passing File
     * */
    void testParseMinimalWQL();
    /** Missing Font Info*/
    void testParseMissingFontInfo();

private :

};

/**
 * @brief WQLGenerator builds the WQL doc
 * This is to minimize repetition in the unit tests.
 * */
class WQLGenerator
{
public:
    WQLGenerator();
    ~WQLGenerator();

    //String Data
    const QString headerText,  fontTag,  gridTag,  vocabTag;
    const QString lang0loc, lang1loc;
    const QString word0Left,  word0Right;

    //Generators of parts of the WQL Doc
    WQLGenerator & preamble();
    WQLGenerator & minimalFontInfo();
    WQLGenerator & minimalGridInfo();
    WQLGenerator & minimalVocab();

    /** Convert to the QIODevice that the reader expects*/
    QIODevice * toQIODevice();
    QByteArray m_barray;
    QBuffer * m_buffer;
    QString m_string;

};

WQLGenerator::WQLGenerator()
    : headerText( "WordQuiz\n5\n")
    , fontTag( "[Font Info]" )
    , gridTag( "[Grid Info]" )
    , vocabTag( "[Vocabulary]" )
    , lang0loc( "en" )
    , lang1loc( "de" )
    , word0Left( "dog" )
    , word0Right( "Hund" )
    , m_buffer( 0 )
    , m_string( "" )

{
}
WQLGenerator::~WQLGenerator()
{
    if ( m_buffer ) {
        delete m_buffer;
    }
}

QIODevice * WQLGenerator::toQIODevice()
{
    m_barray.append(  m_string );
    m_buffer = new QBuffer( & m_barray );
    m_buffer->open( QIODevice::ReadOnly );
    return m_buffer;
}

WQLGenerator & WQLGenerator::preamble() {
    m_string = headerText;
    return *this;
}

WQLGenerator & WQLGenerator::minimalFontInfo() {
    m_string += fontTag + "\na=1\nb=1\nc=1\nd=1\n";
    return *this;
}

WQLGenerator & WQLGenerator::minimalGridInfo() {
    m_string += gridTag + "\na=1\nb=1\nc=1\n";
    return *this;
}

WQLGenerator & WQLGenerator::minimalVocab() {
    m_string += vocabTag + "\n" + word0Left + "[\n" + word0Right +"\n";
    return *this;
}

// These macros are to force the QCOMPARE/QVERIFY to be in the test function.
// QCOMPARE must be in the test function.

// Check that a parse returns errcode
#define PARSE_EXPECT_CORE(gen, expected,  verbose)                      \
    do {                                                                \
        KEduVocWqlReader reader( gen.toQIODevice() );                   \
        KEduVocDocument docRead;                                        \
        KEduVocDocument::ErrorCode actual(reader.readDoc( &docRead ) ); \
        if (verbose && actual != expected) {                            \
            kDebug() << gen.m_string;                                   \
            kDebug() << reader.errorMessage();                          \
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



void WqlReaderTest::testParseMinimalWQL()
{
    WQLGenerator gen;
    gen.preamble().minimalFontInfo().minimalGridInfo().minimalVocab();

    PARSE_EXPECT( gen , KEduVocDocument::NoError );
}

void WqlReaderTest::testParseMissingFontInfo()
{
    WQLGenerator gen;
    gen.preamble();

    PARSE_EXPECT( gen , KEduVocDocument::FileReaderFailed );
}


}

QTEST_KDEMAIN_CORE( WordQuizReaderTests::WqlReaderTest )



#include "wordquizreadertest.moc"
