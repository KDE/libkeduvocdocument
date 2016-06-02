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

#include "emptydocumenttest.h"

#include "keduvocdocument.h"
#include <QTest>

namespace EmptyDocumentTests{

QString EmptyDocumentTest::sm_cappturedMessageOutput = QString();

void EmptyDocumentTest::testConstuctionDestruction()
{
    sm_cappturedMessageOutput.clear();
    QtMessageHandler previouMsgHandler = qInstallMessageHandler(captureMessageOutput);
    KEduVocDocument *doc = new KEduVocDocument;
    delete doc;
    qInstallMessageHandler(previouMsgHandler);

    QCOMPARE(sm_cappturedMessageOutput, QString());
}

void EmptyDocumentTest::captureMessageOutput(QtMsgType /*type*/, const QMessageLogContext &/*context*/, const QString &msg)
{
    sm_cappturedMessageOutput = msg;
}

}

QTEST_MAIN(EmptyDocumentTests::EmptyDocumentTest)



