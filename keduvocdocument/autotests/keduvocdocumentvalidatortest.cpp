/*
 * SPDX-FileCopyrightText: 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "keduvocdocument.h"
#include "keduvoclesson.h"
#include "keduvocexpression.h"
#include "keduvoctranslation.h"
#include "keduvocconjugation.h"
#include "keduvocdeclension.h"
#include "keduvocwordtype.h"


#include <QTemporaryFile>

#include <qtest_kde.h>

#include <QObject>
#include <QValidator>
#include <QDomDocument>

class KEduVocDocumentValidatorTest
  : public QObject
{
    Q_OBJECT

private slots:
    void testDocumentAboutInfo();
    void testLessons();
    void testWordTypes();
    void testTranslations();
    void testConjugations();
    void testDeclensions();
    void testAddRemoveLanguage();

};

void KEduVocDocumentValidatorTest::testDocumentAboutInfo()
{
    QTemporaryFile temp;
    temp.setSuffix(".kvtml");
    temp.open();
    QUrl fileName = QUrl::fromLocalFile(temp.fileName());
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
    doc.close();

    KEduVocDocument docRead;
    docRead.open(fileName);

    QCOMPARE( docRead.generator(), generator );
    QCOMPARE( docRead.author(), author );
    QCOMPARE( docRead.license(), license );
    QCOMPARE( docRead.documentComment(), comment );
    QCOMPARE( docRead.category(), category );
    QCOMPARE( docRead.title(), title );
}

void KEduVocDocumentValidatorTest::testLessons()
{
    const QString title = QString::fromLatin1( "Validator Test Title" );
    QString lesson0 = QString::fromLatin1( "Lesson Root" );
    QString lesson1 = QString::fromLatin1( "Lesson 1" );
    QString lesson1child1 = QString::fromLatin1( "Lesson 1.1" );
    QString lesson1child2 = QString::fromLatin1( "Lesson 1.2" );
    QString lesson2 = QString::fromLatin1( "Lesson 2" );
    QString lesson3 = QString::fromLatin1( "Lesson 3" );

    KEduVocDocument doc;
    doc.lesson()->appendChildContainer(new KEduVocLesson(lesson1, doc.lesson()));
    // Order here is significant as setTitle also sets the lesson name.
    doc.lesson()->setName( lesson0 );
    doc.setTitle( title );
    QCOMPARE(doc.lesson()->childContainerCount(), 1);
    QCOMPARE(doc.lesson()->childContainer(0)->containerType(), KEduVocContainer::Lesson);
    QCOMPARE(doc.lesson()->childContainer(0)->parent(), doc.lesson());
    QCOMPARE(doc.lesson()->childContainer(0)->name(), lesson1);

    ///@todo decouple document and root lesson title
    QEXPECT_FAIL("",  "Document and root lesson have the same name",  Continue);
    QCOMPARE(doc.lesson()->name(), lesson0);
    QCOMPARE( doc.title(), title );

    doc.lesson()->appendChildContainer(new KEduVocLesson(lesson2, doc.lesson()));
    doc.lesson()->appendChildContainer(new KEduVocLesson(lesson3, doc.lesson()));
    QCOMPARE(doc.lesson()->childContainerCount(), 3);

    doc.lesson()->childContainer(0)->appendChildContainer(new KEduVocLesson(lesson1child1, doc.lesson()->childContainer(0)));
    doc.lesson()->childContainer(0)->appendChildContainer(new KEduVocLesson(lesson1child2, doc.lesson()->childContainer(0)));
    QCOMPARE(doc.lesson()->childContainer(0)->childContainerCount(), 2);


}


void KEduVocDocumentValidatorTest::testWordTypes()
{
    KEduVocDocument doc;
    // create doc - has no word types yet
    QCOMPARE(doc.wordTypeContainer()->childContainerCount(), 0);

    KEduVocWordType *noun;
    KEduVocWordType *nounMale;
    KEduVocWordType *nounFemale;
    KEduVocWordType *verb;

    noun = new KEduVocWordType("Noun", doc.wordTypeContainer());
    doc.wordTypeContainer()->appendChildContainer(noun);
    QCOMPARE(doc.wordTypeContainer()->childContainerCount(), 1);
    nounMale = new KEduVocWordType("Male", noun);
    noun->appendChildContainer(nounMale);
    nounFemale = new KEduVocWordType("Female", noun);
    noun->appendChildContainer(nounFemale);
    verb = new KEduVocWordType("Verb", doc.wordTypeContainer());
    doc.wordTypeContainer()->appendChildContainer(verb);
    QCOMPARE(doc.wordTypeContainer()->childContainerCount(), 2);
    QCOMPARE(doc.wordTypeContainer()->childContainer(0)->childContainerCount(), 2);

    // create some entries
    for(int i = 0; i < 20; i++) {
        KEduVocExpression *e =
            new KEduVocExpression(QStringList() << QString("lang1 %1").arg(i) << QString("lang2 %1").arg(i));
        doc.lesson()->appendEntry(e);
        e->translation(0)->setWordType(noun);
        e->translation(1)->setWordType(noun);
    }
    QCOMPARE(doc.lesson()->entryCount(), 20);
    QCOMPARE(noun->entryCount(), 20);
    QCOMPARE(verb->entryCount(), 0);
    doc.lesson()->entry(0)->translation(0)->setWordType(verb);
    // translation 1 is still noun, so it needs to be in both now
    QCOMPARE(noun->entryCount(), 20);
    QCOMPARE(verb->entryCount(), 1);
    doc.lesson()->entry(0)->translation(1)->setWordType(verb);
    QCOMPARE(noun->entryCount(), 19);
    QCOMPARE(verb->entryCount(), 1);

    // delete word type
    doc.wordTypeContainer()->deleteChildContainer(1);
    // the word type is set to 0 when removed
    QVERIFY(doc.lesson()->entry(0)->translation(0)->wordType() == 0);
    QVERIFY(doc.lesson()->entry(0)->translation(1)->wordType() == 0);
    QCOMPARE(doc.wordTypeContainer()->childContainerCount(), 1);
}

void KEduVocDocumentValidatorTest::testTranslations()
{
    KEduVocTranslation *trans1 = new KEduVocTranslation(0, QString("My word"));
    QCOMPARE(trans1->text(), QString("My word"));

    // copy ctor
    KEduVocTranslation *trans2 = new KEduVocTranslation(*trans1);
    QCOMPARE(trans2->text(), QString("My word"));

    // operator =
    KEduVocTranslation *trans3 = new KEduVocTranslation(0);
    trans3 = new KEduVocTranslation(*trans1);
    QCOMPARE(trans3->text(), QString("My word"));

    QCOMPARE((int)trans1->grade(), 0);
    trans1->incGrade();
    QCOMPARE((int)trans1->grade(), 1);
    QCOMPARE((int)trans3->grade(), 0);
}

void KEduVocDocumentValidatorTest::testDeclensions()
{
    KEduVocTranslation translation(0);
    QVERIFY(translation.declension() == 0);
    KEduVocDeclension* declension = new KEduVocDeclension;
    translation.setDeclension(declension);
    QCOMPARE(translation.declension(), declension);
}

void KEduVocDocumentValidatorTest::testConjugations()
{
    KEduVocConjugation conjugation;
    conjugation.setConjugation(KEduVocText("first-singular"), KEduVocWordFlag::First | KEduVocWordFlag::Singular);
    QCOMPARE(conjugation.conjugation(KEduVocWordFlag::First | KEduVocWordFlag::Singular).text(), QString("first-singular"));

    QDomDocument doc = QDomDocument("test doc");
    QDomElement root = doc.createElement( "kvtml" );
    doc.appendChild(root);
    conjugation.toKVTML2(root, "tense");

    qDebug() << root.text();

    KEduVocConjugation *con2 = KEduVocConjugation::fromKVTML2(root);

    QCOMPARE(conjugation.conjugation(KEduVocWordFlag::First | KEduVocWordFlag::Singular).text(), con2->conjugation(KEduVocWordFlag::First | KEduVocWordFlag::Singular).text());
    delete con2;
}

void KEduVocDocumentValidatorTest::testAddRemoveLanguage()
{
    KEduVocDocument doc;
    // create some initial languages
    doc.appendIdentifier();
    doc.appendIdentifier();
    doc.appendIdentifier();
    doc.appendIdentifier();
    doc.identifier(0).setName("0");
    doc.identifier(1).setName("1");
    doc.identifier(2).setName("2");
    doc.identifier(3).setName("3");

    QCOMPARE(doc.identifierCount(), 4);
    KEduVocLesson* lesson = new KEduVocLesson("lesson", doc.lesson());
    doc.lesson()->appendChildContainer(lesson);
    lesson->appendEntry(new KEduVocExpression);
    lesson->entry(0)->setTranslation(0, "0");
    lesson->entry(0)->setTranslation(1, "1");
    lesson->entry(0)->setTranslation(2, "2");
    lesson->entry(0)->setTranslation(3, "3");
    QCOMPARE(lesson->entry(0)->translationIndices().size(), 4);

    // throw away the second language
    doc.removeIdentifier(1);

    QCOMPARE(doc.identifierCount(), 3);
    QCOMPARE(doc.identifier(0).name(), QString("0"));
    QCOMPARE(doc.identifier(1).name(), QString("2"));
    QCOMPARE(doc.identifier(2).name(), QString("3"));

    QCOMPARE(lesson->entry(0)->translationIndices().size(), 3);
    QCOMPARE(lesson->entry(0)->translation(0)->text(), QString("0"));
    QCOMPARE(lesson->entry(0)->translation(1)->text(), QString("2"));
    QCOMPARE(lesson->entry(0)->translation(2)->text(), QString("3"));
}


QTEST_KDEMAIN_CORE( KEduVocDocumentValidatorTest )
