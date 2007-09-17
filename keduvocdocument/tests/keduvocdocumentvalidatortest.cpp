/*
 Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#include "../keduvocdocument.h"

#include <KTemporaryFile>

#include <qtest_kde.h>

#include <qobject.h>
#include <qvalidator.h>

class KEduVocDocumentValidatorTest
  : public QObject
{
    Q_OBJECT

private slots:
    void testDocumentAboutInfo();
};

void KEduVocDocumentValidatorTest::testDocumentAboutInfo()
{
    KTemporaryFile temp;
    temp.setSuffix(".kvtml");
    temp.open();
    KUrl fileName = KUrl(temp.fileName());
    temp.close();

    KEduVocDocument doc;
    doc.setAuthor("Validator Test");
    doc.setLicense("test license");
    doc.setDocumentComment("comment");
    doc.setCategory("test document");
    doc.setTitle("Validator Test Title");

    doc.saveAs(fileName, KEduVocDocument::Kvtml, "Validator Unit Tests");

    KEduVocDocument docRead;
    docRead.open(fileName);

    QVERIFY( docRead.author() == "Validator Test" );
    QVERIFY( docRead.license() == "test license" );
    QVERIFY( docRead.documentComment() == "comment" );
    QVERIFY( docRead.category() == "test document" );
    QVERIFY( docRead.title() == "Validator Test Title" );
}

QTEST_KDEMAIN_CORE( KEduVocDocumentValidatorTest )

#include "keduvocdocumentvalidatortest.moc"
