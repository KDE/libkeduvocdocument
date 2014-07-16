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
#include "keduvocxdxfreader.h"

#include <kdebug.h>
#include <qtest_kde.h>

namespace XdxfReaderUnitTests
{

/**
 * @file Unit Tests of parsing of Xdxf strings
 *
 * Each test creates a QString of Xdxf format
 * reads, parses and verifies the parse.  The portions of the
 * generator object relevant to each test are defined in this file.
 *
 * The current tests are cursory.
 * @todo add check to verify that the words are actually in the kvocdocument
 * */

/**
 * @brief Unit Tests of parsing of Xdxf strings
 *
 * Each test creates a string
 * reads, parses and verifies the parse.
 * */

class XdxfReaderTest : public QObject
{
    Q_OBJECT

private slots:
    /** Initialize the string*/
    void init();
    /** Two word doc*/
    void testParseTwoWord();
    /** InvalidDoc*/
    void testParseInvalid();
private :
    QString oneGoodDoc;
};

void XdxfReaderTest::init() {
    oneGoodDoc = \
                 QString( "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" ) \
                 + "<!DOCTYPE xdxf SYSTEM \"http://xdxf.sourceforge.net/xdxf_lousy.dtd\">" \
                 + "<xdxf lang_from=\"GER\" lang_to=\"SPA\" format=\"visual\">" \
                 + "<full_name>German Spanish</full_name>"              \
                 + "<description>Description of German and Spanish</description>" \
                 + ""                                                   \
                 + "<ar><k>Hund</k>el perro</ar>"                       \
                 + "<ar><k>Schwein</k>el cerdo</ar>"                    \
                 + "</xdxf>";
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
        KEduVocXdxfReader reader( &docRead );                           \
        KEduVocDocument::ErrorCode actual(reader.read( buffer ) );      \
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



void XdxfReaderTest::testParseTwoWord()
{
    PARSE_EXPECT( oneGoodDoc ,  KEduVocDocument::NoError );
}

void XdxfReaderTest::testParseInvalid()
{
    QString invalid = oneGoodDoc +" bad parse";

    PARSE_EXPECT( invalid ,  KEduVocDocument::FileReaderFailed );
}

}

QTEST_KDEMAIN_CORE( XdxfReaderUnitTests::XdxfReaderTest )



#include "xdxfreadertest.moc"
