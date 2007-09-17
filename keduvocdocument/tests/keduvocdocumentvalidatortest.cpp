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

    const QString generator = QString::fromLatin1( "Validator Unit Tests" );
    const QString author = QString::fromLatin1( "Validator Test" );
    const QString license = QString::fromLatin1( "test license" );
    const QString comment = QString::fromLatin1( "comment" );
    const QString category = QString::fromLatin1( "test document" );
    const QString title = QString::fromLatin1( "Validator Test Title" );

    KEduVocDocument doc;
    doc.setAuthor( author );
    doc.setLicense( license );
    doc.setDocumentComment( comment );
    doc.setCategory( category );
    doc.setTitle( title );

    doc.saveAs(fileName, KEduVocDocument::Kvtml, generator);

    KEduVocDocument docRead;
    docRead.open(fileName);

    QCOMPARE( docRead.generator(), generator );
    QCOMPARE( docRead.author(), author );
    QCOMPARE( docRead.license(), license );
    QCOMPARE( docRead.documentComment(), comment );
    QCOMPARE( docRead.category(), category );
    QCOMPARE( docRead.title(), title );
}

QTEST_KDEMAIN_CORE( KEduVocDocumentValidatorTest )

#include "keduvocdocumentvalidatortest.moc"
