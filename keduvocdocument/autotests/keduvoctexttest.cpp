/***************************************************************************
 *   Copyright (C) 2016 by Hartmut Riesenbeck <hartmut.riesenbeck@gmx.de>  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#include "keduvoctexttest.h"

#include "keduvoctext.h"
#include <QTest>

namespace KEduVocTextTests{

void KEduVocTextTest::testAssignmentOperator()
{
    // This list should be extended when new data members are added to
    // KEduVocTextPrivate.  Appropriate getter/setter methods needs to be
    // provided by KEduVocText.

    KEduVocText t1;
    KEduVocText t2;

    QCOMPARE(isEqual(t1, t2), true);

    t1.setText(QStringLiteral("foo"));
    QCOMPARE(isEqual(t1, t2), false);
    t2 = t1;
    QCOMPARE(isEqual(t1, t2), true);

    t1.setPreGrade(1);
    QCOMPARE(isEqual(t1, t2), false);
    t2 = t1;
    QCOMPARE(isEqual(t1, t2), true);

    t1.setGrade(1);
    QCOMPARE(isEqual(t1, t2), false);
    t2 = t1;
    QCOMPARE(isEqual(t1, t2), true);

    t1.setPracticeCount(1);
    QCOMPARE(isEqual(t1, t2), false);
    t2 = t1;
    QCOMPARE(isEqual(t1, t2), true);

    t1.setBadCount(1);
    QCOMPARE(isEqual(t1, t2), false);
    t2 = t1;
    QCOMPARE(isEqual(t1, t2), true);

    t1.setPracticeDate(QDateTime::currentDateTime());
    QCOMPARE(isEqual(t1, t2), false);
    t2 = t1;
    QCOMPARE(isEqual(t1, t2), true);

    t1.setInterval(1);
    QCOMPARE(isEqual(t1, t2), false);
    t2 = t1;
    QCOMPARE(isEqual(t1, t2), true);
}

void KEduVocTextTest::testEqualToOperator()
{
    // This list should be extended when new data members are added to
    // KEduVocTextPrivate.  Appropriate getter/setter methods needs to be
    // provided by KEduVocText.

    KEduVocText t1;
    KEduVocText t2;

    QCOMPARE(t1 == t2, true);

    t1.setText(QStringLiteral("foo"));
    QCOMPARE(t1 == t2, false);
    t2.setText(QStringLiteral("foo"));
    QCOMPARE(t1 == t2, true);

    t1.setPreGrade(1);
    QCOMPARE(t1 == t2, false);
    t2.setPreGrade(1);
    QCOMPARE(t1 == t2, true);

    t1.setGrade(1);
    QCOMPARE(t1 == t2, false);
    t2.setGrade(1);
    QCOMPARE(t1 == t2, true);

    t1.setPracticeCount(1);
    QCOMPARE(t1 == t2, false);
    t2.setPracticeCount(1);
    QCOMPARE(t1 == t2, true);

    t1.setBadCount(1);
    QCOMPARE(t1 == t2, false);
    t2.setBadCount(1);
    QCOMPARE(t1 == t2, true);

    QDateTime testTime(QDateTime::currentDateTime());
    t1.setPracticeDate(testTime);
    QCOMPARE(t1 == t2, false);
    t2.setPracticeDate(testTime);
    QCOMPARE(t1 == t2, true);

    t1.setInterval(1);
    QCOMPARE(t1 == t2, false);
    t2.setInterval(1);
    QCOMPARE(t1 == t2, true);
}

bool KEduVocTextTest::isEqual(const KEduVocText &a, const KEduVocText &b) const
{
    // This list should be extended when new data members are added to
    // KEduVocTextPrivate.  Appropriate getter/setter methods needs to be
    // provided by KEduVocText.

    return a.text() == b.text()
        &&  a.preGrade() == b.preGrade()
        &&  a.grade() == b.grade()
        &&  a.practiceCount() == b.practiceCount()
        &&  a.badCount() == b.badCount()
        &&  a.practiceDate() == b.practiceDate()
        &&  a.interval() == b.interval();

}

}

QTEST_MAIN(KEduVocTextTests::KEduVocTextTest)

