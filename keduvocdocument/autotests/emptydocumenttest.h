/*
 * SPDX-FileCopyrightText: 2016 Hartmut Riesenbeck <hartmut.riesenbeck@gmx.de>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef EMPTYDOCUMENTTEST_H
#define EMPTYDOCUMENTTEST_H

#include <QObject>

namespace EmptyDocumentTests {
/** @file
    * \brief Empty KEduVocDocument test application
    *        The creation and destruction of a default constructed KEduVocDocument object shouldn't
    *        give any error message on stderr.
    * @author Hartmut Riesenbeck <hartmut.riesenbeck@gmx.de>
    */

class EmptyDocumentTest : public QObject
{
Q_OBJECT

private slots:
    void testConstuctionDestruction();

private:
    static void captureMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    static QString sm_cappturedMessageOutput;
};
}

#endif // EMPTYDOCUMENTTEST_H
