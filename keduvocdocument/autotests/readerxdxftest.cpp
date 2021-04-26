/*
 * SPDX-FileCopyrightText: 2014 Andreas Xavier <andxav at zoho dot com>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "keduvocdocument.h"
#include "readermanager.h"
#include "keduvocxdxfreader.h"

#include "readerTestHelpers.h"

#include <QDebug>
#include <QTest>

namespace XdxfReaderUnitTests
{

/**
 * @file readerxdxftest.cpp
 * Unit Tests of parsing of Xdxf strings
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
    KEduVocDocument::FileType myType;
};

void XdxfReaderTest::init() {
    oneGoodDoc = \
                 QStringLiteral( "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n" )
                 + "<!DOCTYPE xdxf SYSTEM \"http://xdxf.sourceforge.net/xdxf_lousy.dtd\">\n"
                 + "<xdxf lang_from=\"GER\" lang_to=\"SPA\" format=\"visual\">\net"
                 + "<full_name>German Spanish</full_name>\n"
                 + "<description>Description of German and Spanish</description>\n"
                 + ""
                 + "<ar><k>Hund</k>el perro</ar>\n"
                 + "<ar><k>Schwein</k>el cerdo</ar>\n"
                 + "</xdxf>\n";
    myType = KEduVocDocument::Xdxf;
}


void XdxfReaderTest::testParseTwoWord()
{
    KVOCREADER_EXPECT( oneGoodDoc ,  KEduVocDocument::NoError ,  myType);
}

void XdxfReaderTest::testParseInvalid()
{
    QString invalid = oneGoodDoc +" bad parse";

    KVOCREADER_EXPECT( invalid ,  KEduVocDocument::FileReaderFailed ,  myType );
}

}

QTEST_MAIN( XdxfReaderUnitTests::XdxfReaderTest )



#include "readerxdxftest.moc"
