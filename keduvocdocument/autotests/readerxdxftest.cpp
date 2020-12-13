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
