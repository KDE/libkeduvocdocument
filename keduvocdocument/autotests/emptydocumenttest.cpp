/*
 * SPDX-FileCopyrightText: 2016 Hartmut Riesenbeck <hartmut.riesenbeck@gmx.de>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

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



