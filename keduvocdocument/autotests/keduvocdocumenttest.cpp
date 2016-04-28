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

#include "keduvocdocument.h"
#include <QTemporaryFile>
#include <QTest>

namespace KEduVocDocumentTests {
/** @file
 * \brief Various KEduVocDocument tests
 * This test class contends various tests for public KEduVocDocument methods.
 * They are used to proof expected behavior before and after apply changes to the tested methods.
 * @author Hartmut Riesenbeck <hartmut.riesenbeck@gmx.de>
 */

class KEduVocDocumentTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testDetectFileType();
    void testOpen();

private:
    void createTmpTestFileOfType(KEduVocDocument::FileType fileType);
    QString tmpTestFileName() const;
    QUrl tmpTestFileUrl() const;

private:
    QTemporaryFile m_tempTestFile;
    KEduVocDocument m_documentTemplate;
};

void KEduVocDocumentTest::initTestCase()
{
    if (!m_tempTestFile.open()) {
        QFAIL("Temporary test file could not be created.");
        return;
    }
    m_tempTestFile.close();

    KEduVocIdentifier lang;
    lang.setName(QStringLiteral("KEduVocDocument Language Name"));
    lang.setLocale(QStringLiteral("en"));

    m_documentTemplate.setAuthor(QStringLiteral("KEduVocDocument Tests"));
    m_documentTemplate.appendIdentifier(lang);
    m_documentTemplate.setGenerator(QStringLiteral("KEduVocDocument Unit Tests"));
}

void KEduVocDocumentTest::testDetectFileType()
{
    KEduVocDocument::FileType resultFileType(KEduVocDocument::KvdNone);

    createTmpTestFileOfType(KEduVocDocument::Kvtml);
    resultFileType = KEduVocDocument::detectFileType(tmpTestFileName());
    QCOMPARE(resultFileType, KEduVocDocument::Kvtml);

    createTmpTestFileOfType(KEduVocDocument::Csv);
    resultFileType = KEduVocDocument::detectFileType(tmpTestFileName());
    QCOMPARE(resultFileType, KEduVocDocument::Csv);
}

void KEduVocDocumentTest::testOpen()
{
    KEduVocDocument::ErrorCode openResult(KEduVocDocument::NoError);

    createTmpTestFileOfType(KEduVocDocument::Kvtml);
    KEduVocDocument docKvtml;
    openResult = docKvtml.open(tmpTestFileUrl(), KEduVocDocument::FileOpenReadOnly);
    QCOMPARE(openResult, KEduVocDocument::NoError);

    createTmpTestFileOfType(KEduVocDocument::Csv);
    KEduVocDocument docCsv;
    openResult = docCsv.open(tmpTestFileUrl(), KEduVocDocument::FileOpenReadOnly);
    QCOMPARE(openResult, KEduVocDocument::NoError);
}

void KEduVocDocumentTest::createTmpTestFileOfType(KEduVocDocument::FileType fileType)
{
    KEduVocDocument::ErrorCode result = m_documentTemplate.saveAs(tmpTestFileUrl(), fileType);
    if (result != KEduVocDocument::NoError) {
        QStringList errorStrings = QStringList() << "NoError" << "Unknown" << "InvalidXml"
            << "FileTypeUnknown" << "FileCannotWrite" << "FileWriterFailed" << "FileCannotRead"
            << "FileReaderFailed"<< "FileDoesNotExist" << "FileLocked" << "FileCannotLock"
            << "FileIsReadOnly";
        QString failText =QString("Temporary test file could not be saved. (Error code = %1)");
        failText = failText.arg(errorStrings.value(result));
        QFAIL(failText.toLocal8Bit().constData());
    }
}

QString KEduVocDocumentTest::tmpTestFileName() const
{
    return m_tempTestFile.fileName();
}

QUrl KEduVocDocumentTest::tmpTestFileUrl() const
{
    return QUrl::fromLocalFile(m_tempTestFile.fileName());
}

}

QTEST_MAIN(KEduVocDocumentTests::KEduVocDocumentTest)

#include "keduvocdocumenttest.moc"


