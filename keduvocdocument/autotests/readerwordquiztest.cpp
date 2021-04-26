/*
 * SPDX-FileCopyrightText: 2014 Andreas Xavier <andxav at zoho dot com>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "keduvocdocument.h"
#include "readermanager.h"
#include "keduvocwqlreader.h"

#include "readerTestHelpers.h"

#include <QObject>
#include <QDebug>
#include <QTest>

namespace WordQuizReaderTests {

/**
 * @file readerwordquiztest.cpp
 * Unit Tests of parsing of WordQuiz WQL Files
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

    KEduVocDocument::FileType myType;

};

WQLGenerator::WQLGenerator()
    : headerText( QStringLiteral("WordQuiz\n5\n"))
    , fontTag( QStringLiteral("[Font Info]") )
    , gridTag( QStringLiteral("[Grid Info]") )
    , vocabTag( QStringLiteral("[Vocabulary]") )
    , lang0loc( QStringLiteral("en") )
    , lang1loc( QStringLiteral("de") )
    , word0Left( QStringLiteral("dog") )
    , word0Right( QStringLiteral("Hund") )
    , m_buffer( 0 )
    , m_string( QLatin1String("") )
    , myType( KEduVocDocument::Wql )

{
}
WQLGenerator::~WQLGenerator()
{
    if ( m_buffer ) {
        delete m_buffer;
    }
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
    m_string += vocabTag + '\n' + word0Left + "[\n" + word0Right +'\n';
    return *this;
}


void WqlReaderTest::testParseMinimalWQL()
{
    WQLGenerator gen;
    gen.preamble().minimalFontInfo().minimalGridInfo().minimalVocab();

    KVOCREADER_EXPECT( gen.m_string , KEduVocDocument::NoError , gen.myType );
}

void WqlReaderTest::testParseMissingFontInfo()
{
    WQLGenerator gen;
    gen.preamble();

    KVOCREADER_EXPECT( gen.m_string , KEduVocDocument::FileReaderFailed , gen.myType );
}


}

QTEST_MAIN( WordQuizReaderTests::WqlReaderTest )

#include "readerwordquiztest.moc"
