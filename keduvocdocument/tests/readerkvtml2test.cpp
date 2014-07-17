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
#include "keduvockvtml2reader.h"
#include "kvtml2defs.h"

#include <kdebug.h>
#include <qtest_kde.h>

namespace Kvtml2ReaderUnitTests
{

/**
 * @file Unit Tests of parsing of KVTML 2.0 XML strings
 *
 * Each test creates a QString of XML
 * reads, parses and verifies the parse.  The portions of the
 * XML generator object relevant to each test are defined in this file.
 * Refer to the dtd file to determine what should parse and what should
 * fail to parse.
 * */

/**
 * @brief Unit Tests of parsing of KVTML 2.0 strings
 *
 * Each test creates a string of XML
 * reads, parses and verifies the parse.
 * */

class Kvtml2ReaderTest : public QObject
{
    Q_OBJECT

private slots:
    /** Smallest passing KVTML 2 file according to DTD file.
     *  Bug 336780 - Missing Identifier causes segfault in Parley
     *  Bug 337352 - Missing Identifier causes segfault in kvocdoc
     * */
    void testParseExpectedMinimalXMLAccordingToDTD();
    /** Smallest passing KVTML 2 file according to code*/
    void testParseActualMinimalXML();
    /** Missing Info*/
    void testParseMissingInformation();
    /** Missing Ids*/
    void testParseMissingIdentifiers();
    /** Missing Entries*/
    void testParseMissingEntries();
    /** Missing Title*/
    void testParseMissingTitle();
    /** One Identifier With Locale*/
    void testParseWithLocale();
    /** Missing Locale*/
    void testParseMissingLocale();

private :

};

/**
 * @brief XMLGenerator builds the XML doc
 * This is to minimize repetition in the unit tests.
 * */
class XMLGenerator : public QDomDocument
{
public:
    XMLGenerator();
    ~XMLGenerator();

    //String Data
    const QString generator, author, license, comment, date, category, title;
    const QString lang0name, lang0loc, lang1name, lang1loc;

    //Generators of parts of the XML Doc
    XMLGenerator & preamble();
    XMLGenerator & minimalInfo();
    XMLGenerator & missingTitle();
    XMLGenerator & minimalIds();
    XMLGenerator & missingLocale();
    XMLGenerator & minimalEntries();
    XMLGenerator & minimalHeader();

    XMLGenerator & blankInfo();
    XMLGenerator & addTitle();

    XMLGenerator & blankIdentifier(const int ii);
    XMLGenerator & addLocale(const int ii);


    /** Convert to the QIODevice that the reader expects*/
    QIODevice * toQIODevice();
    QByteArray m_barray;
    QBuffer * m_buffer;

};

XMLGenerator::XMLGenerator()
    : QDomDocument()
    , generator( "Parse KVTML2 Unit Tests")
    , author( "Parse KVTML2 Test Author" )
    , license( "test license" )
    , comment( "comment" )
    , date( "2014-01-01" )
    , category( "test document" )
    , title( "Parse KVTML2 Test Title" )

    , lang0name( "English" )
    , lang0loc( "en" )
    , lang1name( "German" )
    , lang1loc( "de" )
    , m_buffer( 0 )

{
}
XMLGenerator::~XMLGenerator() {
    if ( m_buffer ) {
        delete m_buffer;
    }
}

QIODevice * XMLGenerator::toQIODevice()
{
    m_barray = this->toString(4).toLatin1();
    m_buffer = new QBuffer( & m_barray );
    return m_buffer;
}

XMLGenerator & XMLGenerator::preamble() {
    this -> setContent( QString( "<!DOCTYPE kvtml PUBLIC \"kvtml2.dtd\" \"http://edu.kde.org/kvtml/kvtml2.dtd\">\n" ) , true, 0, 0, 0);
    this->appendChild( this->createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"UTF-8\"" ) );

    QDomElement domElementKvtml = this->createElement( KVTML_TAG );
    this->appendChild( domElementKvtml );
    domElementKvtml.setAttribute( KVTML_VERSION, ( QString ) "2.0" );

    return *this;
}

XMLGenerator & XMLGenerator::blankInfo() {
    QDomElement info = this->createElement( KVTML_INFORMATION );
    elementsByTagName(KVTML_TAG).at( 0 ).appendChild( info );
    return *this;
}
XMLGenerator & XMLGenerator::addTitle() {
    QDomElement mtitle = this->createElement( KVTML_TITLE );
    QDomText mtitletext = this->createTextNode( title );
    mtitle.appendChild( mtitletext );
    elementsByTagName(KVTML_INFORMATION).at( 0 ).appendChild( mtitle );
    return *this;
}
XMLGenerator & XMLGenerator::minimalInfo() {
    return blankInfo().addTitle();
}

XMLGenerator & XMLGenerator::minimalIds() {
    QDomElement elem = this->createElement( KVTML_IDENTIFIERS ) ;
    elementsByTagName(KVTML_TAG).at( 0 ).appendChild( elem );
    return *this;
}

XMLGenerator & XMLGenerator::minimalEntries() {
    QDomElement elem = this->createElement( KVTML_ENTRIES ) ;
    elementsByTagName(KVTML_TAG).at( 0 ).appendChild( elem );
    return *this;
}

XMLGenerator & XMLGenerator::minimalHeader() {
    return preamble().minimalInfo().minimalIds().minimalEntries();
}


XMLGenerator & XMLGenerator::blankIdentifier(const int ii) {
    QDomElement id = this->createElement( KVTML_IDENTIFIER );
    QString str;
    id.setAttribute("id", str.setNum( ii ) );
    elementsByTagName(KVTML_IDENTIFIERS).at( 0 ).appendChild( id );
    return *this;
}
XMLGenerator & XMLGenerator::addLocale(const int ii) {
    QDomElement x = this->createElement( KVTML_LOCALE );
    QDomText loctext = this->createTextNode( ii ? lang1loc : lang0loc);
    x.appendChild( loctext );
    elementsByTagName(KVTML_IDENTIFIER).at( ii ).appendChild( x );
    return *this;
}


// These macros are to force the QCOMPARE/QVERIFY to be in the test function.
// QCOMPARE must be in the test function.

// Check that a parse returns errcode
#define PARSE_EXPECT_CORE(gen, expected,  verbose)                      \
    do {                                                                \
        KEduVocKvtml2Reader reader;                                     \
        KEduVocDocument docRead;                                        \
        KEduVocDocument::ErrorCode actual(reader.read( *gen.toQIODevice(), docRead ) ); \
        if (verbose && actual != expected) {                            \
            kDebug() << gen.toString( 4 );                              \
            kDebug() << "Actual="<<int( actual );                       \
        }                                                               \
        QCOMPARE( int( actual ), int( expected ) ); \
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



void Kvtml2ReaderTest::testParseExpectedMinimalXMLAccordingToDTD()
{
    XMLGenerator gen;
    gen.preamble().minimalInfo().minimalIds().minimalEntries();

    ///@todo Either Fix the DTD to agree with the code or fix the code to agree with the DTD
    PARSE_DONT_EXPECT( gen ,  KEduVocDocument::NoError );
}
void Kvtml2ReaderTest::testParseActualMinimalXML()
{
    XMLGenerator gen;
    gen.preamble().minimalInfo().minimalIds().blankIdentifier(0).addLocale(0).minimalEntries();

    PARSE_EXPECT( gen ,  KEduVocDocument::NoError );
}

void Kvtml2ReaderTest::testParseMissingInformation()
{
    XMLGenerator gen;
    gen.preamble().minimalIds().minimalEntries();

    PARSE_EXPECT( gen ,  KEduVocDocument::FileReaderFailed );
}
void Kvtml2ReaderTest::testParseMissingIdentifiers()
{
    XMLGenerator gen;
    gen.preamble().minimalInfo().minimalEntries();

    ///@todo Either Fix the DTD to agree with the code or fix the code to agree with the DTD
    PARSE_DONT_EXPECT( gen ,  KEduVocDocument::FileReaderFailed );
}
void Kvtml2ReaderTest::testParseMissingEntries()
{
    XMLGenerator gen;
    gen.preamble().minimalInfo().minimalIds();

    PARSE_EXPECT( gen ,  KEduVocDocument::FileReaderFailed );
}
void Kvtml2ReaderTest::testParseMissingTitle()
{
    XMLGenerator gen;
    gen.preamble().blankInfo().minimalIds().minimalEntries();

    PARSE_EXPECT( gen ,  KEduVocDocument::FileReaderFailed );
}

void Kvtml2ReaderTest::testParseWithLocale()
{
    XMLGenerator gen;
    gen.preamble().blankInfo().minimalIds().minimalEntries().blankIdentifier(0).addLocale( 0 );

    PARSE_EXPECT( gen ,  KEduVocDocument::NoError );
}

void Kvtml2ReaderTest::testParseMissingLocale()
{
    XMLGenerator gen;
    gen.preamble().blankInfo().minimalIds().minimalEntries().blankIdentifier(0);

    ///@todo Either Fix the DTD to agree with the code or fix the code to agree with the DTD
    PARSE_DONT_EXPECT( gen ,  KEduVocDocument::FileReaderFailed );
}



}

QTEST_KDEMAIN_CORE( Kvtml2ReaderUnitTests::Kvtml2ReaderTest )



#include "kvtml2readertest.moc"
