/*
 * SPDX-FileCopyrightText: 2014 Andreas Xavier <andxav at zoho dot com>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "keduvocdocument.h"
#include "readermanager.h"
#include "keduvockvtml2reader.h"
#include "kvtml2defs.h"
#include "readerTestHelpers.h"

#include <QDebug>
#include <QTest>

namespace Kvtml2ReaderUnitTests
{

/**
 * @file readerkvtml2test.cpp
 * Unit Tests of parsing of KVTML 2.0 XML strings
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
    /** Parsing Sound URL*/
    void testParseSoundUrl();
    void testParseSoundUrl_data();
    /** Parsing Image URL*/
    void testParseImageUrl();
    void testParseImageUrl_data();

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
    XMLGenerator &blankEntry(int entryId);
    XMLGenerator &blankTranslation(int translationId);
    XMLGenerator &addSoundUrl(const QString &soundUrl);
    XMLGenerator &addImageUrl(const QString &soundUrl);
    XMLGenerator &minimalLessons();
    XMLGenerator &blankContainer();
    XMLGenerator &addContainerEntry(int entryId);

    /** Convert to the QIODevice that the reader expects*/
    QIODevice * toQIODevice();
    QByteArray m_barray;
    QBuffer * m_buffer;

    KEduVocDocument::FileType myType;

};

XMLGenerator::XMLGenerator()
    : QDomDocument()
    , generator( QStringLiteral("Parse KVTML2 Unit Tests"))
    , author( QStringLiteral("Parse KVTML2 Test Author") )
    , license( QStringLiteral("test license") )
    , comment( QStringLiteral("comment") )
    , date( QStringLiteral("2014-01-01") )
    , category( QStringLiteral("test document") )
    , title( QStringLiteral("Parse KVTML2 Test Title") )

    , lang0name( QStringLiteral("English") )
    , lang0loc( QStringLiteral("en") )
    , lang1name( QStringLiteral("German") )
    , lang1loc( QStringLiteral("de") )
    , m_buffer( 0 )
    , myType( KEduVocDocument::Kvtml )

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
    m_buffer->open( QIODevice::ReadOnly );
    return m_buffer;
}

XMLGenerator & XMLGenerator::preamble() {
    this -> setContent( QStringLiteral( "<!DOCTYPE kvtml PUBLIC \"kvtml2.dtd\" \"http://edu.kde.org/kvtml/kvtml2.dtd\">\n" ) , true, 0, 0, 0);
    this->appendChild( this->createProcessingInstruction( QStringLiteral("xml"), QStringLiteral("version=\"1.0\" encoding=\"UTF-8\"") ) );

    QDomElement domElementKvtml = this->createElement( KVTML_TAG );
    this->appendChild( domElementKvtml );
    domElementKvtml.setAttribute( KVTML_VERSION, ( QString ) QStringLiteral("2.0") );

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
    id.setAttribute(QStringLiteral("id"), str.setNum( ii ) );
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

XMLGenerator &XMLGenerator::blankEntry(int entryId)
{
    QDomElement elem = this->createElement(KVTML_ENTRY);
    elem.setAttribute(QStringLiteral("id"), QString::number(entryId));
    elementsByTagName(KVTML_ENTRIES).at(0).appendChild(elem);
    return *this;
}

XMLGenerator &XMLGenerator::blankTranslation(int translationId)
{
    QDomElement elem = this->createElement(KVTML_TRANSLATION);
    elem.setAttribute(QStringLiteral("id"), QString::number(translationId));
    elementsByTagName(KVTML_ENTRY).at(0).appendChild(elem);
    return *this;
}

XMLGenerator &XMLGenerator::addSoundUrl(const QString &soundUrl)
{
    QDomElement elem = this->createElement(KVTML_SOUND);
    elem.appendChild(this->createTextNode(soundUrl));
    elementsByTagName(KVTML_TRANSLATION).at(0).appendChild(elem);
    return *this;
}

XMLGenerator &XMLGenerator::addImageUrl(const QString &imageUrl)
{
    QDomElement elem = this->createElement(KVTML_IMAGE);
    elem.appendChild(this->createTextNode(imageUrl));
    elementsByTagName(KVTML_TRANSLATION).at(0).appendChild(elem);
    return *this;
}

XMLGenerator &XMLGenerator::minimalLessons()
{
    QDomElement elem = this->createElement(KVTML_LESSONS);
    elementsByTagName(KVTML_TAG).at(0).appendChild(elem);
    return *this;
}

XMLGenerator &XMLGenerator::blankContainer()
{
    QDomElement elem = this->createElement(KVTML_CONTAINER);
    elementsByTagName(KVTML_LESSONS).at(0).appendChild(elem);
    return *this;
}

XMLGenerator &XMLGenerator::addContainerEntry(int entryId)
{
    QDomElement elem = this->createElement(KVTML_ENTRY);
    elem.setAttribute(QStringLiteral("id"), QString::number(entryId));
    elementsByTagName(KVTML_CONTAINER).at(0).appendChild(elem);
    return *this;
}


void Kvtml2ReaderTest::testParseExpectedMinimalXMLAccordingToDTD()
{
    XMLGenerator gen;
    gen.preamble().minimalInfo().minimalIds().minimalEntries();

    ///@todo Either Fix the DTD to agree with the code or fix the code to agree with the DTD
    KVOCREADER_DONT_EXPECT( gen.toString(4),  KEduVocDocument::NoError, gen.myType );
}
void Kvtml2ReaderTest::testParseActualMinimalXML()
{
    XMLGenerator gen;
    gen.preamble().minimalInfo().minimalIds().blankIdentifier(0).addLocale(0).minimalEntries();

    KVOCREADER_EXPECT( gen.toString(4) ,  KEduVocDocument::NoError, gen.myType );
}

void Kvtml2ReaderTest::testParseMissingInformation()
{
    XMLGenerator gen;
    gen.preamble().minimalIds().minimalEntries();

    KVOCREADER_EXPECT( gen.toString(4) ,  KEduVocDocument::FileReaderFailed, gen.myType );
}
void Kvtml2ReaderTest::testParseMissingIdentifiers()
{
    XMLGenerator gen;
    gen.preamble().minimalInfo().minimalEntries();

    ///@todo Either Fix the DTD to agree with the code or fix the code to agree with the DTD
    KVOCREADER_DONT_EXPECT( gen.toString(4) ,  KEduVocDocument::FileReaderFailed, gen.myType );
}
void Kvtml2ReaderTest::testParseMissingEntries()
{
    XMLGenerator gen;
    gen.preamble().minimalInfo().minimalIds();

    KVOCREADER_EXPECT( gen.toString(4) ,  KEduVocDocument::FileReaderFailed, gen.myType );
}
void Kvtml2ReaderTest::testParseMissingTitle()
{
    XMLGenerator gen;
    gen.preamble().blankInfo().minimalIds().minimalEntries();

    KVOCREADER_EXPECT( gen.toString(4) ,  KEduVocDocument::FileReaderFailed, gen.myType );
}

void Kvtml2ReaderTest::testParseWithLocale()
{
    XMLGenerator gen;
    gen.preamble().blankInfo().minimalIds().minimalEntries().blankIdentifier(0).addLocale( 0 );

    KVOCREADER_EXPECT( gen.toString(4) ,  KEduVocDocument::NoError, gen.myType );
}

void Kvtml2ReaderTest::testParseMissingLocale()
{
    XMLGenerator gen;
    gen.preamble().blankInfo().minimalIds().minimalEntries().blankIdentifier(0);

    ///@todo Either Fix the DTD to agree with the code or fix the code to agree with the DTD
    KVOCREADER_DONT_EXPECT( gen.toString(4) ,  KEduVocDocument::FileReaderFailed, gen.myType );
}

void Kvtml2ReaderTest::testParseSoundUrl()
{
    QFETCH(QString, urlStoredInKvtmlFile);
    QFETCH(QString, expectedParsedUrl);
    QFETCH(QString, kvtmlFileUrl);

    // Create a minimal KVTML doc with a sound url
    XMLGenerator gen;
    gen.minimalHeader().blankIdentifier(0).blankEntry(0).blankTranslation(0);
    gen.addSoundUrl(urlStoredInKvtmlFile);
    gen.minimalLessons().blankContainer().addContainerEntry(0);

    // Parse KVTML doc with KEduVocDocument
    QByteArray array(gen.toString(4).toLatin1());
    QScopedPointer<QBuffer> buffer(new QBuffer(&array));
    buffer->open(QIODevice::ReadOnly);
    ReaderManager::ReaderPtr reader(ReaderManager::reader(*buffer));
    KEduVocDocument testDoc;
    testDoc.setUrl(QUrl(kvtmlFileUrl));
    KEduVocDocument::ErrorCode errorCode(reader->read(testDoc));
    if (errorCode != KEduVocDocument::NoError) {
        QFAIL("Test document could not be filled from buffer.");
    }

    QUrl urlReadFromDoc = testDoc.lesson()->childContainer(0)->entry(0)->translation(0)->soundUrl();
    QCOMPARE(urlReadFromDoc.toString(), expectedParsedUrl);
}

void Kvtml2ReaderTest::testParseSoundUrl_data()
{
    QTest::addColumn<QString>("urlStoredInKvtmlFile");
    QTest::addColumn<QString>("expectedParsedUrl");
    QTest::addColumn<QString>("kvtmlFileUrl");

    QTest::newRow("Relative Path")
    << "file:sounds/bar.mp3"                // Url stored in KVTML file
    << "file:///home/foo/sounds/bar.mp3"    // Expected parsed url
    << "file:///home/foo/bar.kvtml";        // KVTML file url

    QTest::newRow("Absolute Path")
    << "file:///data/sounds/bar.mp3"        // Url stored in KVTML file
    << "file:///data/sounds/bar.mp3"        // Expected parsed url
    << "file:///home/foo/bar.kvtml";        // KVTML file url

    QTest::newRow("Remote Path")
    << "http://example.com/sounds/bar.mp3"  // Url stored in KVTML file
    << "http://example.com/sounds/bar.mp3"  // Expected parsed url
    << "file:///home/foo/bar.kvtml";        // KVTML file url
}

void Kvtml2ReaderTest::testParseImageUrl()
{
    QFETCH(QString, urlStoredInKvtmlFile);
    QFETCH(QString, expectedParsedUrl);
    QFETCH(QString, kvtmlFileUrl);

    // Create a minimal KVTML doc with a image url
    XMLGenerator gen;
    gen.minimalHeader().blankIdentifier(0).blankEntry(0).blankTranslation(0);
    gen.addImageUrl(urlStoredInKvtmlFile);
    gen.minimalLessons().blankContainer().addContainerEntry(0);

    // Parse KVTML doc with KEduVocDocument
    QByteArray array(gen.toString(4).toLatin1());
    QScopedPointer<QBuffer> buffer(new QBuffer(&array));
    buffer->open(QIODevice::ReadOnly);
    ReaderManager::ReaderPtr reader(ReaderManager::reader(*buffer));
    KEduVocDocument testDoc;
    testDoc.setUrl(QUrl(kvtmlFileUrl));
    KEduVocDocument::ErrorCode errorCode(reader->read(testDoc));
    if (errorCode != KEduVocDocument::NoError) {
        QFAIL("Test document could not be filled from buffer.");
    }

    QUrl urlReadFromDoc = testDoc.lesson()->childContainer(0)->entry(0)->translation(0)->imageUrl();
    QCOMPARE(urlReadFromDoc.toString(), expectedParsedUrl);
}

void Kvtml2ReaderTest::testParseImageUrl_data()
{
    QTest::addColumn<QString>("urlStoredInKvtmlFile");
    QTest::addColumn<QString>("expectedParsedUrl");
    QTest::addColumn<QString>("kvtmlFileUrl");

    QTest::newRow("Relative Path")
    << "file:images/bar.png"                // Url stored in KVTML file
    << "file:///home/foo/images/bar.png"    // Expected parsed url
    << "file:///home/foo/bar.kvtml";        // KVTML file url

    QTest::newRow("Absolute Path")
    << "file:///data/images/bar.png"        // Url stored in KVTML file
    << "file:///data/images/bar.png"        // Expected parsed url
    << "file:///home/foo/bar.kvtml";        // KVTML file url

    QTest::newRow("Remote Path")
    << "http://example.com/images/bar.png"  // Url stored in KVTML file
    << "http://example.com/images/bar.png"  // Expected parsed url
    << "file:///home/foo/bar.kvtml";        // KVTML file url
}


}

QTEST_MAIN( Kvtml2ReaderUnitTests::Kvtml2ReaderTest )



#include "readerkvtml2test.moc"
