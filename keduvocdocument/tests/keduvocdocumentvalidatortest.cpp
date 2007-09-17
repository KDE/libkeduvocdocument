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
    QVERIFY( 1==1 );
}

QTEST_KDEMAIN_CORE( KEduVocDocumentValidatorTest )

#include "keduvocdocumentvalidatortest.moc"
