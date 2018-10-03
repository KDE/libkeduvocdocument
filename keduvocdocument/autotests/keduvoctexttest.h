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

