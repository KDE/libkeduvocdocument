/*
 * SPDX-FileCopyrightText: 2016 Hartmut Riesenbeck <hartmut.riesenbeck@gmx.de>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KEDUVOCTEXTTEST_H
#define KEDUVOCTEXTTEST_H

#include <QObject>

class KEduVocText;

namespace KEduVocTextTests {
/** @file
    * \brief KeduVocText test application
    *        Various tests for KeduVocText class.
    * @author Hartmut Riesenbeck <hartmut.riesenbeck@gmx.de>
    */

class KEduVocTextTest : public QObject
{
Q_OBJECT

private slots:
    void testAssignmentOperator();
    void testEqualToOperator();

private:
    bool isEqual(const KEduVocText &a, const KEduVocText &b) const;
};
}

#endif // KEDUVOCTEXTTEST_H

