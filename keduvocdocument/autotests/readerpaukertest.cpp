/*
 * SPDX-FileCopyrightText: 2014 Andreas Xavier <andxav at zoho dot com>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "keduvocdocument.h"
#include "readermanager.h"
#include "keduvocpaukerreader.h"

#include "readerTestHelpers.h"

#include <QDebug>
#include <QTest>

namespace PaukerReaderUnitTests
{

/**
 * @file readerpaukertest.cpp
 * Unit Tests of parsing of Pauker strings
 *
 * Each test creates a QString of Pauker format
 * reads, parses and verifies the parse.  The portions of the
 * XML generator object relevant to each test are defined in this file.
 *
 *This is the supported format
 * @verbatim
 <?xml version="1.0" encoding="UTF-8"?>
 <!--This is a lesson file for Pauker (http://pauker.sourceforge.net)-->
 <Lesson LessonFormat="1.7">
   <Description>Some Description</Description>
   <Batch>
     <Card>
       <FrontSide>
         <Text>Front text</Text>
       </FrontSide>
       <ReverseSide>
         <Text>Back Text</Text>
       </ReverseSide>
     </Card>
   ... more cards
    </Batch>
  </Lesson>
  * @endverbatim
  * */

/**
 * @brief Unit Tests of parsing of KVTML 2.0 strings
 *
 * Each test creates a string of XML
 * reads, parses and verifies the parse.
 * */

class PaukerReaderTest : public QObject
{
    Q_OBJECT

private slots:
    /** Initialize the string*/
    void init();
    /** One word doc*/
    void testParseOneWord();
    /** InvalidDoc*/
    void testParseInvalid();
private :
    QString oneGoodDoc;
    QString oneBadDoc;
    KEduVocDocument::FileType myType;
};

void PaukerReaderTest::init() {
    oneGoodDoc =                                    \
    QStringLiteral("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n \
     <!--This is a lesson file for Pauker (http://pauker.sourceforge.net)-->\n \
     <Lesson LessonFormat=\"1.7\">\n \
       <Description>Some Description</Description>\n \
       <Batch>\n \
         <Card>\n \
           <FrontSide>\n \
             <Text>Front text</Text>\n \
           </FrontSide>\n \
           <ReverseSide>\n \
             <Text>Back Text</Text>\n \
           </ReverseSide>\n \
         </Card>\n \
        </Batch>\n \
      </Lesson>\n") ;
    oneBadDoc = oneGoodDoc + "\ninvalid XML characters ";

    myType = KEduVocDocument::Pauker;
}


void PaukerReaderTest::testParseOneWord()
{
    KVOCREADER_EXPECT( oneGoodDoc ,  KEduVocDocument::NoError , myType );
}

void PaukerReaderTest::testParseInvalid()
{
    KVOCREADER_EXPECT( oneBadDoc ,  KEduVocDocument::FileReaderFailed , myType );
}

}

QTEST_MAIN( PaukerReaderUnitTests::PaukerReaderTest )



#include "readerpaukertest.moc"
