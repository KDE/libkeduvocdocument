/***************************************************************************
                     read a KEduVocDocument from a KVTML file
    -----------------------------------------------------------------------
    copyright           : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                          (C) 2001 The KDE-EDU team
                          (C) 2005 Eric Pignet <eric at erixpage.com>
                          (C) 2007 Peter Hedlund <peter.hedlund@kdemail.net>
                          (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvockvtmlreader.h"

#include <QTextStream>
#include <QList>
#include <QIODevice>

#include <kdebug.h>
#include <klocale.h>

#include "keduvocdocument.h"
#include "keduvoclesson.h"
#include "kvtmldefs.h"

KEduVocKvtmlReader::KEduVocKvtmlReader(QIODevice *file)
{
  // the file must be already open
  m_inputFile = file;
  m_errorMessage = "";
}


bool KEduVocKvtmlReader::readDoc(KEduVocDocument *doc)
{
  m_doc = doc;
  m_cols = 0;
  m_lines = 0;

  QDomDocument domDoc("KEduVocDocument");

  if (!domDoc.setContent(m_inputFile, &m_errorMessage))
    return false;

  QDomElement domElementKvtml = domDoc.documentElement();
  if (domElementKvtml.tagName() != KV_DOCTYPE)
  {
    m_errorMessage = i18n("This is not a KDE Vocabulary document.");
    return false;
  }

  //-------------------------------------------------------------------------
  // Attributes
  //-------------------------------------------------------------------------

  QDomAttr documentAttribute;
  documentAttribute = domElementKvtml.attributeNode(KV_ENCODING);
  if (!documentAttribute.isNull())
  {
    // TODO handle old encodings
    // Qt DOM API autodetects encoding, so is there anything to do ?
  }

  documentAttribute = domElementKvtml.attributeNode(KV_TITLE);
  if (!documentAttribute.isNull())
    m_doc->setTitle(documentAttribute.value());

  documentAttribute = domElementKvtml.attributeNode(KV_AUTHOR);
  if (!documentAttribute.isNull())
    m_doc->setAuthor(documentAttribute.value());

  documentAttribute = domElementKvtml.attributeNode(KV_LICENSE);
  if (!documentAttribute.isNull())
    m_doc->setLicense(documentAttribute.value());

  documentAttribute = domElementKvtml.attributeNode(KV_DOC_REM);
  if (!documentAttribute.isNull())
    m_doc->setDocumentRemark(documentAttribute.value());

  documentAttribute = domElementKvtml.attributeNode(KV_GENERATOR);
  if (!documentAttribute.isNull())
  {
    m_doc->setGenerator(documentAttribute.value());
    int pos = m_doc->generator().lastIndexOf(KVD_VERS_PREFIX);
    if (pos >= 0)
      m_doc->setVersion(m_doc->generator().remove(0, pos + 2));
  }

  documentAttribute = domElementKvtml.attributeNode(KV_COLS);
  if (!documentAttribute.isNull())
    m_cols = documentAttribute.value().toInt(); ///currently not used anywhere

  documentAttribute = domElementKvtml.attributeNode(KV_LINES);
  if (!documentAttribute.isNull())
    m_lines = documentAttribute.value().toInt();

  //-------------------------------------------------------------------------
  // Children
  //-------------------------------------------------------------------------

  bool result = readBody(domElementKvtml);  // read vocabulary

  return result;
}


bool KEduVocKvtmlReader::readBody(QDomElement &domElementParent)
{
  bool result = false;

  QDomElement currentElement;

  currentElement = domElementParent.firstChildElement(KV_LESS_GRP);
  if (!currentElement.isNull()) {
    result = readLesson(currentElement);
    if (!result)
      return false;
  }

  currentElement = domElementParent.firstChildElement(KV_ARTICLE_GRP);
  if (!currentElement.isNull()) {
    result = readArticle(currentElement);
    if (!result)
      return false;
  }

  currentElement = domElementParent.firstChildElement(KV_CONJUG_GRP);
  if (!currentElement.isNull()) {
    QList<KEduVocConjugation> conjugations;
    result = readConjug(currentElement, conjugations);
    if (result) {
      KEduVocConjugation conjug;
      for (int i = 0; i< conjugations.count(); i++) {
        conjug = conjugations[i];
        m_doc->setConjugation(i, conjug);
      }
    }
    else
      return false;
  }

  currentElement = domElementParent.firstChildElement(KV_OPTION_GRP);
  if (!currentElement.isNull()) {
    result = readOptions(currentElement);
    if (!result)
      return false;
  }

  // initialize the list of predefined types
  m_doc->wordTypes()->createOldWordTypeLists();
  currentElement = domElementParent.firstChildElement(KV_TYPE_GRP);
  if (!currentElement.isNull()) {
    result = readType(currentElement);
    if (!result)
      return false;
  }

  currentElement = domElementParent.firstChildElement(KV_TENSE_GRP);
  if (!currentElement.isNull()) {
    result = readTense(currentElement);
    if (!result)
      return false;
  }

  currentElement = domElementParent.firstChildElement(KV_USAGE_GRP);
  if (!currentElement.isNull()) {
    result = readUsage(currentElement);
    if (!result)
      return false;
  }

  QDomNodeList entryList = domElementParent.elementsByTagName(KV_EXPR);
  if (entryList.length() <= 0)
    return false;

  for (int i = 0; i < entryList.count(); ++i) {
    currentElement = entryList.item(i).toElement();
    if (currentElement.parentNode() == domElementParent) {
      result = readExpression(currentElement);
      if (!result)
        return false;
    }
  }

  return true;
}


bool KEduVocKvtmlReader::readLesson(QDomElement &domElementParent)
{
  QString s;
  QDomAttr attribute;
  QDomElement currentElement;

  //-------------------------------------------------------------------------
  // Attributes
  //-------------------------------------------------------------------------

  attribute = domElementParent.attributeNode(KV_SIZEHINT);
  if (!attribute.isNull())
    m_doc->setSizeHint(-1, attribute.value().toInt());

  //-------------------------------------------------------------------------
  // Children
  //-------------------------------------------------------------------------

  QDomNodeList entryList = domElementParent.elementsByTagName(KV_LESS_DESC);
  if (entryList.length() <= 0)
    return false;

  QList<int> inQueryList;

  for (int i = 0; i < entryList.count(); ++i) {
    currentElement = entryList.item(i).toElement();
    if (currentElement.parentNode() == domElementParent) {
      int no = 0;
      bool isCurr = false;

      attribute = currentElement.attributeNode(KV_LESS_NO);
      if (!attribute.isNull())
        no = attribute.value().toInt();

      attribute = currentElement.attributeNode(KV_LESS_CURR);
      if (!attribute.isNull())
        isCurr = attribute.value().toInt() != 0;

      if (isCurr && no != 0)
        m_doc->setCurrentLesson(no);

      attribute = currentElement.attributeNode(KV_LESS_QUERY);
      if (!attribute.isNull())
        if (attribute.value().toInt() != 0 && no > 0)
          inQueryList.append(no);

      s = currentElement.text();
      if (s.isNull())
        s = "";
      m_doc->addLesson(s, no);
    }
  }

  if (inQueryList.count() > 0)
    m_doc->setLessonsInQuery(inQueryList);

  return true;
}


bool KEduVocKvtmlReader::readArticle(QDomElement &domElementParent)
/*
 <article>
  <e l="de">        lang determines also lang order in entries !!
   <fi>eine</fi>    which must NOT differ
   <fd>die</fd>
   <mi>ein</mi>
   <md>der</md>
   <ni>ein</ni>
   <nd>das</nd>
  </e>
 </article>
*/
{

  QString s;
  QDomAttr attribute;
  QDomElement currentElement;
  QDomElement article;

  QDomNodeList entryList = domElementParent.elementsByTagName(KV_ART_ENTRY);
  if (entryList.length() <= 0)
    return false;

  for (int i = 0; i < entryList.count(); ++i) {

//kDebug() << "KEduVocKvtmlReader::readArticle() read " << entryList.count() << " articles. ";
    currentElement = entryList.item(i).toElement();
    if (currentElement.parentNode() == domElementParent) {
      QString lang;
      attribute = currentElement.attributeNode(KV_LANG);

      if (m_doc->identifierCount() <= i)
      {
        // first entry
        if (!attribute.isNull())         // no definition in first entry
          lang = attribute.value();
        else
          lang = "original";
        m_doc->appendIdentifier(lang);
//kDebug() << " Identifier " << i << " is " << lang;
      }
      else
      {
        if (!attribute.isNull() && attribute.value() != m_doc->identifier(i))
        {
          // different originals ?
          m_errorMessage = i18n("Ambiguous definition of language code");
          return false;
        }
      }

      //---------
      // Children

      QString fem_def = "";
      QString mal_def = "";
      QString nat_def = "";
      QString fem_indef = "";
      QString mal_indef = "";
      QString nat_indef = "";

      article = currentElement.firstChildElement(KV_ART_FD);
      if (!article.isNull()) {
        fem_def = article.text();
        if (fem_def.isNull())
          fem_def = "";
      }

      article = currentElement.firstChildElement(KV_ART_FI);
      if (!article.isNull()) {
        fem_indef = article.text();
        if (fem_indef.isNull())
          fem_indef = "";
      }

      article = currentElement.firstChildElement(KV_ART_MD);
      if (!article.isNull()) {
        mal_def = article.text();
        if (mal_def.isNull())
          mal_def = "";
      }

      article = currentElement.firstChildElement(KV_ART_MI);
      if (!article.isNull()) {
        mal_indef = article.text();
        if (mal_indef.isNull())
          mal_indef = "";
      }

      article = currentElement.firstChildElement(KV_ART_ND);
      if (!article.isNull()) {
        nat_def = article.text();
        if (nat_def.isNull())
          nat_def = "";
      }

      article = currentElement.firstChildElement(KV_ART_NI);
      if (!article.isNull()) {
        nat_indef = article.text();
        if (nat_indef.isNull())
          nat_indef = "";
      }

      m_doc->setArticle(i, KEduVocArticle(fem_def, fem_indef, mal_def, mal_indef, nat_def, nat_indef));
    }
  }

  return true;
}


bool KEduVocKvtmlReader::readConjug(QDomElement &domElementParent, QList<KEduVocConjugation> &curr_conjug)
/*
 <conjugation>        used in header for definiton of "prefix"
  <e l="de">          lang determines also lang order in entries !!
   <s1>I</s1>         which must NOT differ
   <s2>you<2>
   <s3f>he</s3f>
   <s3m>she</s3m>
   <s3n>it</s3n>
   <p1>we</p1>
   <p2>you</p2>
   <p3f>they</p3f>
   <p3m>they</p3m>
   <p3n>they</p3n>
  </e>
 </conjugation>

 <conjugation>        and in entry for definition of tenses of (irreg.) verbs
  <t n="sipa">
   <s1>go</s1>
   <s2>go</s2>
   <s3f>goes</s3f>
   <s3m>goes</s3m>
   <s3n>goes</s3n>
   <p1>go</p1>
   <p2>go</p2>
   <p3f>go</p3f>
   <p3m>go</p3m>
   <p3n>go</p3n>
  </t>
 </conjugation>
*/
{
  QString s;
  bool p3_common;
  bool s3_common;
  QString pers1_sing;
  QString pers2_sing;
  QString pers3_m_sing;
  QString pers3_f_sing;
  QString pers3_n_sing;
  QString pers1_plur;
  QString pers2_plur;
  QString pers3_m_plur;
  QString pers3_f_plur;
  QString pers3_n_plur;
  QString lang;
  QString type;
  int count = 0;

  curr_conjug.clear();
  curr_conjug.append(KEduVocConjugation());

  QDomElement domElementConjugChild = domElementParent.firstChild().toElement();
  while (!domElementConjugChild.isNull())
  {
    if (domElementConjugChild.tagName() == KV_CON_ENTRY)
    {
      type = CONJ_PREFIX;

      //----------
      // Attribute

      QString lang;
      QDomAttr domAttrLang = domElementConjugChild.attributeNode(KV_LANG);

      if (m_doc->identifierCount() <= count)
      {
        // first entry
        if (!domAttrLang.isNull())            // no definition in first entry
          lang = domAttrLang.value();
        else
          lang = "original";
        m_doc->appendIdentifier(lang);
      }
      else
      {
        if (!domAttrLang.isNull() && domAttrLang.value() != m_doc->identifier(count))
        {
          // different originals ?
          m_errorMessage = i18n("Ambiguous definition of language code");
          return false;
        }
      }
    }
    else if (domElementConjugChild.tagName() == KV_CON_TYPE)
    {
      //----------
      // Attribute

      QDomAttr domAttrLang = domElementConjugChild.attributeNode(KV_CON_NAME);
      type = domAttrLang.value();
      if (type.isNull())
        type = "";

      if (type.length() != 0 && type.left(1) == UL_USER_TENSE)
      {
        int num = qMin(type.mid (1, 40).toInt(), 1000); // paranoia check
        if (num > m_doc->tenseDescriptions().count())
        {
          // description missing ?
          QString s;
          QStringList sl = m_doc->tenseDescriptions();
          for (int i = m_doc->tenseDescriptions().count(); i < num; i++)
          {
            s.setNum(i + 1);
            s.prepend("#");  // invent descr according to number
            sl.append(s);
          }
          m_doc->setTenseDescriptions(sl);
        }
      }
    }

    pers1_sing = "";
    pers2_sing = "";
    pers3_m_sing = "";
    pers3_f_sing = "";
    pers3_n_sing = "";
    pers1_plur = "";
    pers2_plur = "";
    pers3_m_plur = "";
    pers3_f_plur = "";
    pers3_n_plur = "";
    p3_common = false;
    s3_common = false;

    QDomElement domElementConjugGrandChild = domElementConjugChild.firstChild().toElement();
    while (!domElementConjugGrandChild.isNull())
    {
      if (domElementConjugGrandChild.tagName() == KV_CON_P1S)
      {
        pers1_sing = domElementConjugGrandChild.text();
        if (pers1_sing.isNull())
          pers1_sing = "";
      }
      else if (domElementConjugGrandChild.tagName() == KV_CON_P2S)
      {
        pers2_sing = domElementConjugGrandChild.text();
        if (pers2_sing.isNull())
          pers2_sing = "";
      }
      else if (domElementConjugGrandChild.tagName() == KV_CON_P3SF)
      {
        QDomAttr domAttrCommon = domElementConjugGrandChild.attributeNode(KV_CONJ_COMMON);
        if (!domAttrCommon.isNull())
          s3_common = domAttrCommon.value().toInt();  // returns 0 if the conversion fails

        pers3_f_sing = domElementConjugGrandChild.text();
        if (pers3_f_sing.isNull())
          pers3_f_sing = "";
      }
      else if (domElementConjugGrandChild.tagName() == KV_CON_P3SM)
      {
        pers3_m_sing = domElementConjugGrandChild.text();
        if (pers3_m_sing.isNull())
          pers3_m_sing = "";
      }
      else if (domElementConjugGrandChild.tagName() == KV_CON_P3SN)
      {
        pers3_n_sing = domElementConjugGrandChild.text();
        if (pers3_n_sing.isNull())
          pers3_n_sing = "";
      }
      else if (domElementConjugGrandChild.tagName() == KV_CON_P1P)
      {
        pers1_plur = domElementConjugGrandChild.text();
        if (pers1_plur.isNull())
          pers1_plur = "";
      }
      else if (domElementConjugGrandChild.tagName() == KV_CON_P2P)
      {
        pers2_plur = domElementConjugGrandChild.text();
        if (pers2_plur.isNull())
          pers2_plur = "";
      }
      else if (domElementConjugGrandChild.tagName() == KV_CON_P3PF)
      {
        QDomAttr domAttrCommon = domElementConjugGrandChild.attributeNode(KV_CONJ_COMMON);
        if (!domAttrCommon.isNull())
          p3_common = domAttrCommon.value().toInt();  // returns 0 if the conversion fails

        pers3_f_plur = domElementConjugGrandChild.text();
        if (pers3_f_plur.isNull())
          pers3_f_plur = "";
      }
      else if (domElementConjugGrandChild.tagName() == KV_CON_P3PM)
      {
        pers3_m_plur = domElementConjugGrandChild.text();
        if (pers3_m_plur.isNull())
          pers3_m_plur = "";
      }
      else if (domElementConjugGrandChild.tagName() == KV_CON_P3PN)
      {
        pers3_n_plur = domElementConjugGrandChild.text();
        if (pers3_n_plur.isNull())
          pers3_n_plur = "";
      }
      else
      {
        return false;
      }

      domElementConjugGrandChild = domElementConjugGrandChild.nextSibling().toElement();
    }

    if (domElementConjugChild.tagName() == KV_CON_ENTRY)
      while (count + 1 > (int) curr_conjug.size() )
        curr_conjug.append(KEduVocConjugation());

    curr_conjug[count].setPers3SingularCommon(type, s3_common);
    curr_conjug[count].setPers3PluralCommon(type, p3_common);
    curr_conjug[count].setPers1Singular(type, pers1_sing);
    curr_conjug[count].setPers2Singular(type, pers2_sing);
    curr_conjug[count].setPers3FemaleSingular(type, pers3_f_sing);
    curr_conjug[count].setPers3MaleSingular(type, pers3_m_sing);
    curr_conjug[count].setPers3NaturalSingular(type, pers3_n_sing);
    curr_conjug[count].setPers1Plural(type, pers1_plur);
    curr_conjug[count].setPers2Plural(type, pers2_plur);
    curr_conjug[count].setPers3FemalePlural(type, pers3_f_plur);
    curr_conjug[count].setPers3MalePlural(type, pers3_m_plur);
    curr_conjug[count].setPers3NaturalPlural(type, pers3_n_plur);

    if (domElementConjugChild.tagName() == KV_CON_ENTRY)
      count++;

    domElementConjugChild = domElementConjugChild.nextSibling().toElement();
  }

  return true;
}


bool KEduVocKvtmlReader::readOptions(QDomElement &domElementParent)
{
  m_doc->setSortingEnabled(true);
  QDomElement currentElement = domElementParent.firstChildElement(KV_OPT_SORT);
  if (!currentElement.isNull()) {
    QDomAttr attribute = currentElement.attributeNode(KV_BOOL_FLAG);
    if (!attribute.isNull())
    {
      bool ok = true;
      m_doc->setSortingEnabled(attribute.value().toInt(&ok));  // returns 0 if the conversion fails
      if (!ok)
        m_doc->setSortingEnabled(true);
    }
  }

  return true;
}


bool KEduVocKvtmlReader::readType(QDomElement &domElementParent)
{
  QString s;
  QDomElement currentElement;

  QDomNodeList entryList = domElementParent.elementsByTagName(KV_TYPE_DESC);
  if (entryList.length() <= 0)
    return false;

  for (int i = 0; i < entryList.count(); ++i) {
    currentElement = entryList.item(i).toElement();
    if (currentElement.parentNode() == domElementParent) {
      // We need to even add empty elements since the old system relied on
      // the order. So "type1" "" "type2" should be just like that.

      kDebug() << "Adding old self defined type: " << currentElement.text();
      // add the type to the list of available types
      m_doc->wordTypes()->addType(currentElement.text());

      // from this the #1 are transformed to something sensible again
      m_oldSelfDefinedTypes.append(currentElement.text());
    }
  }

  return true;
}


bool KEduVocKvtmlReader::readTense(QDomElement &domElementParent)
{
  QString s;
  QDomElement currentElement;
  QStringList descriptions;

  QDomNodeList entryList = domElementParent.elementsByTagName(KV_TENSE_DESC);
  if (entryList.length() <= 0)
    return false;

  descriptions.clear();

  for (int i = 0; i < entryList.count(); ++i) {
    currentElement = entryList.item(i).toElement();
    if (currentElement.parentNode() == domElementParent) {
      int no = 0;

      QDomAttr attribute = currentElement.attributeNode(KV_TENSE_NO);
      if (!attribute.isNull())
        no = attribute.value().toInt();

      s = currentElement.text();
      if (s.isNull())
        s = "";
      descriptions.append(s);
    }
  }

  m_doc->setTenseDescriptions(descriptions);
  return true;
}


bool KEduVocKvtmlReader::readUsage(QDomElement &domElementParent)
{
  QString s;
  QDomElement currentElement;
  QStringList descriptions;

  QDomNodeList entryList = domElementParent.elementsByTagName(KV_USAGE_DESC);
  if (entryList.length() <= 0)
    return false;

  descriptions.clear();

  for (int i = 0; i < entryList.count(); ++i) {
    currentElement = entryList.item(i).toElement();
    if (currentElement.parentNode() == domElementParent) {
      int no = 0;

      QDomAttr attribute = currentElement.attributeNode(KV_USAGE_NO);
      if (!attribute.isNull())
        no = attribute.value().toInt();

      s = currentElement.text();
      if (s.isNull())
        s = "";
      descriptions.append(s);
    }
  }

  m_doc->setUsageDescriptions(descriptions);
  return true;
}


bool KEduVocKvtmlReader::readComparison(QDomElement &domElementParent, KEduVocComparison &comp)
/*
 <comparison>
   <l1>good</l1>
   <l2>better</l2>
   <l3>best</l3>
 </comparison>
*/
{
  QString s;
  comp.clear();

  QDomElement currentElement;

  currentElement = domElementParent.firstChildElement(KV_COMP_L1);
  if (!currentElement.isNull()) {
    s = currentElement.text();
    if (s.isNull())
      s = "";
    comp.setL1(s);
  }

  currentElement = domElementParent.firstChildElement(KV_COMP_L2);
  if (!currentElement.isNull()) {
    s = currentElement.text();
    if (s.isNull())
      s = "";
    comp.setL2(s);
  }

  currentElement = domElementParent.firstChildElement(KV_COMP_L3);
  if (!currentElement.isNull()) {
    s = currentElement.text();
    if (s.isNull())
      s = "";
    comp.setL3(s);
  }
  return true;
}


bool KEduVocKvtmlReader::readMultipleChoice(QDomElement &domElementParent, KEduVocMultipleChoice &mc)
/*
 <multiplechoice>
   <mc1>good</mc1>
   <mc2>better</mc2>
   <mc3>best</mc3>
   <mc4>best 2</mc4>
   <mc5>best 3</mc5>
 </multiplechoice>
*/

{
  QString s;
  mc.clear();

  QDomElement currentElement;

  currentElement = domElementParent.firstChildElement(KV_MC_1);
  if (!currentElement.isNull()) {
    s = currentElement.text();
    mc.appendChoice(s);
  }

  currentElement = domElementParent.firstChildElement(KV_MC_2);
  if (!currentElement.isNull()) {
    s = currentElement.text();
    mc.appendChoice(s);
  }

  currentElement = domElementParent.firstChildElement(KV_MC_3);
  if (!currentElement.isNull()) {
    s = currentElement.text();
    mc.appendChoice(s);
  }

  currentElement = domElementParent.firstChildElement(KV_MC_4);
  if (!currentElement.isNull()) {
    s = currentElement.text();
    mc.appendChoice(s);
  }

  currentElement = domElementParent.firstChildElement(KV_MC_5);
  if (!currentElement.isNull()) {
    s = currentElement.text();
    mc.appendChoice(s);
  }

  return true;
}


bool KEduVocKvtmlReader::readExpressionChildAttributes( QDomElement &domElementExpressionChild,
                                                        QString &lang,
                                                        grade_t &grade, grade_t &rev_grade,
                                                        int &count, int &rev_count,
                                                        QDateTime &date, QDateTime &rev_date,
                                                        QString &remark,
                                                        int &bcount, int &rev_bcount,
                                                        QString &query_id,
                                                        QString &pronunciation,
                                                        int &width,
                                                        QString &type,
                                                        QString &subType,
                                                        QString &faux_ami_f,
                                                        QString &faux_ami_t,
                                                        QString &synonym,
                                                        QString &example,
                                                        QString &antonym,
                                                        QString &usage,
                                                        QString &paraphrase)
{
  int pos;
  QDomAttr attribute;

  lang = "";
  attribute = domElementExpressionChild.attributeNode(KV_LANG);
  if (!attribute.isNull())
    lang = attribute.value();

  width = -1;
  attribute = domElementExpressionChild.attributeNode(KV_SIZEHINT);
  if (!attribute.isNull())
    width = attribute.value().toInt();

  grade = KV_NORM_GRADE;
  rev_grade = KV_NORM_GRADE;
  attribute = domElementExpressionChild.attributeNode(KV_GRADE);
  if (!attribute.isNull())
  {
    QString s = attribute.value();
    if ((pos = s.indexOf(';')) >= 1)
    {
      grade = s.left(pos).toInt();
      rev_grade = s.mid(pos + 1, s.length()).toInt();
    }
    else
      grade = s.toInt();
  }

  count = 0;
  rev_count = 0;
  attribute = domElementExpressionChild.attributeNode(KV_COUNT);
  if (!attribute.isNull())
  {
    QString s = attribute.value();
    if ((pos = s.indexOf(';')) >= 1)
    {
      count = s.left(pos).toInt();
      rev_count = s.mid(pos + 1, s.length()).toInt();
    }
    else
      count = s.toInt();
  }

  bcount = 0;
  rev_bcount = 0;
  attribute = domElementExpressionChild.attributeNode(KV_BAD);
  if (!attribute.isNull())
  {
    QString s = attribute.value();
    if ((pos = s.indexOf(';')) >= 1)
    {
      bcount = s.left(pos).toInt();
      rev_bcount = s.mid(pos + 1, s.length()).toInt();
    }
    else
      bcount = s.toInt();
  }

  date.setTime_t(0);
  rev_date.setTime_t(0);
  attribute = domElementExpressionChild.attributeNode(KV_DATE);
  if (!attribute.isNull())
  {
    QString s = attribute.value();
    if ((pos = s.indexOf(';')) >= 1)
    {
      date.setTime_t(s.left(pos).toInt());
      rev_date.setTime_t(s.mid(pos + 1, s.length()).toInt());
    }
    else
      date.setTime_t(s.toInt());
  }

  attribute = domElementExpressionChild.attributeNode(KV_DATE2);
  if (!attribute.isNull())
  {
    //this format is deprecated and ignored.
  }

  remark = "";
  attribute = domElementExpressionChild.attributeNode(KV_REMARK);
  if (!attribute.isNull())
    remark = attribute.value();

  faux_ami_f = "";
  attribute = domElementExpressionChild.attributeNode(KV_FAUX_AMI_F);
  if (!attribute.isNull())
    faux_ami_f = attribute.value();

  faux_ami_t = "";
  attribute = domElementExpressionChild.attributeNode(KV_FAUX_AMI_T);
  if (!attribute.isNull())
    faux_ami_t = attribute.value();

  synonym = "";
  attribute = domElementExpressionChild.attributeNode(KV_SYNONYM);
  if (!attribute.isNull())
    synonym = attribute.value();

  example = "";
  attribute = domElementExpressionChild.attributeNode(KV_EXAMPLE);
  if (!attribute.isNull())
    example = attribute.value();

  usage = "";
  attribute = domElementExpressionChild.attributeNode(KV_USAGE);
  if (!attribute.isNull())
  {
    usage = attribute.value();
    if (usage.length() != 0 && usage.left(1) == UL_USER_USAGE)
    {
      int num = qMin(usage.mid (1, 40).toInt(), 1000); // paranioa check
      if (num > m_doc->usageDescriptions().count())
      {
        // description missing ?
        QStringList sl = m_doc->usageDescriptions();
        QString s;
        for (int i = m_doc->usageDescriptions().count(); i < num; i++)
        {
          s.setNum(i + 1);
          s.prepend("#");  // invent descr according to number
          sl.append(s);
        }
        m_doc->setUsageDescriptions(sl);
      }
    }
  }

  paraphrase = "";
  attribute = domElementExpressionChild.attributeNode(KV_PARAPHRASE);
  if (!attribute.isNull())
    paraphrase = attribute.value();

  antonym = "";
  attribute = domElementExpressionChild.attributeNode(KV_ANTONYM);
  if (!attribute.isNull())
    antonym = attribute.value();

    // this is all done by reference - so we have to care about "type" :(
    attribute = domElementExpressionChild.attributeNode(KV_EXPRTYPE);
    if (!attribute.isNull())
    {
        QString oldType = attribute.value();
        if (oldType.length() >= 2 && type.left(1) == QM_USER_TYPE)
        {
            // they started counting at 1
            int selfDefinedTypeIndex = oldType.right(type.count()-1).toInt() -1;
            // append invented types (do we not trust our own writer?)
            if ( selfDefinedTypeIndex >= m_oldSelfDefinedTypes.count() )
            {
                while (selfDefinedTypeIndex >= m_oldSelfDefinedTypes.count()) {
                    m_oldSelfDefinedTypes.append( i18n("User defined word type %1", m_oldSelfDefinedTypes.count() - 1) );
                }
            }
            type = m_oldSelfDefinedTypes.value(selfDefinedTypeIndex);
        } else { // not user defined - preset types
                // convert from pre-0.5 versions (I guess we can just leave that in here.
            // I seriously doubt that any such documents exist...
            if (oldType == "1")
                oldType = QM_VERB;
            else if (oldType == "2")
                oldType = QM_NOUN;
            else if (oldType == "3")
                oldType = QM_NAME;

            type = m_doc->wordTypes()->mainTypeFromOldFormat(oldType);
            subType = m_doc->wordTypes()->subTypeFromOldFormat(oldType);
        } // not user defined - preset types
    }

  pronunciation = "";
  attribute = domElementExpressionChild.attributeNode(KV_PRONUNCE);
  if (!attribute.isNull())
    pronunciation = attribute.value();

  query_id = "";
  attribute = domElementExpressionChild.attributeNode(KV_QUERY);
  if (!attribute.isNull())
    query_id = attribute.value();

  return true;
}


bool KEduVocKvtmlReader::readExpression(QDomElement &domElementParent)
{
  grade_t                   grade;
  grade_t                   r_grade;
  int                       qcount;
  int                       r_qcount;
  int                       bcount;
  int                       r_bcount;
  QString                   remark;
  QString                   pronunciation;
  QDateTime                 qdate;
  QDateTime                 r_qdate;
  bool                      inquery;
  bool                      active;
  QString                   lang;
  QString                   textstr;
  QString                   q_org;
  QString                   q_trans;
  QString                   query_id;
  int                       lesson = 0;
  int                       width;
  QString                   type;
  QString                   subType;
  QString                   faux_ami_f;
  QString                   faux_ami_t;
  QString                   synonym;
  QString                   example;
  QString                   antonym;
  QString                   usage;
  QString                   paraphrase;
  QList<KEduVocConjugation> conjug;
  KEduVocComparison         comparison;
  KEduVocMultipleChoice     mc;
  KEduVocExpression         expr;

  QDomAttr                  attribute;
  QDomElement               currentElement;
  QDomElement               currentChild;

  //-------------------------------------------------------------------------
  // Attributes
  //-------------------------------------------------------------------------

  attribute = domElementParent.attributeNode(KV_LESS_MEMBER);
  if (!attribute.isNull())
  {
    lesson = attribute.value().toInt();
  }

  if (lesson && lesson > m_doc->lessonCount())
  {
    // it's from a lesson that hasn't been added yet
    // so make sure this lesson is in the document
    m_doc->addLesson(QString("#") + QString::number(lesson), lesson);
  }

  attribute = domElementParent.attributeNode(KV_SELECTED);
  if (!attribute.isNull())
    inquery = attribute.value() == "1" ? true : false;
  else
    inquery = false;

  attribute = domElementParent.attributeNode(KV_INACTIVE);
  if (!attribute.isNull())
    active = attribute.value() == "1" ? false : true;
  else
    active = true;

    // this is all done by reference - so we have to care about "type" :(
    attribute = domElementParent.attributeNode(KV_EXPRTYPE);
    if (!attribute.isNull())
    {
        QString oldType = attribute.value();

        if (oldType.length() >= 2 && type.left(1) == QM_USER_TYPE)
        {
            // they started counting at 1
            int selfDefinedTypeIndex = oldType.right(type.count()-1).toInt() -1;
            // append invented types (do we not trust our own writer?)
            if ( selfDefinedTypeIndex >= m_oldSelfDefinedTypes.count() )
            {
                while (selfDefinedTypeIndex >= m_oldSelfDefinedTypes.count()) {
                    m_oldSelfDefinedTypes.append( i18n("User defined word type %1", m_oldSelfDefinedTypes.count() - 1) );
                }
            }
            type = m_oldSelfDefinedTypes.value(selfDefinedTypeIndex);
        } else { // not user defined - preset types
                // convert from pre-0.5 versions (I guess we can just leave that in here.
            // I seriously doubt that any such documents exist...
            if (oldType == "1")
                oldType = QM_VERB;
            else if (oldType == "2")
                oldType = QM_NOUN;
            else if (oldType == "3")
                oldType = QM_NAME;

            type = m_doc->wordTypes()->mainTypeFromOldFormat(oldType);
            subType = m_doc->wordTypes()->subTypeFromOldFormat(oldType);
        } // not user defined - preset types
    }


  //-------------------------------------------------------------------------
  // Children 'Translation'
  //-------------------------------------------------------------------------

  //QDomNodeList translationList = domElementParent.elementsByTagName(KV_TRANS);

    // count which translation we are on
    int i=0;

    // kvtml 1: we always have an original element (required)
    currentElement = domElementParent.firstChildElement(KV_ORG);
    if (currentElement.isNull()) { // sanity check
        m_errorMessage = i18n("Data for original language missing");
        return false;
    }

    while (!currentElement.isNull()) {

      //-----------
      // Attributes
      //-----------

      // read attributes - the order of the query grades is interchanged!
      if (i == 0 && !readExpressionChildAttributes( currentElement, lang, grade, r_grade, qcount, r_qcount, qdate, r_qdate, remark, bcount, r_bcount, query_id,
                                          pronunciation, width, type, subType, faux_ami_t, faux_ami_f, synonym, example, antonym, usage, paraphrase))
        return false;

      if (i != 0 && !readExpressionChildAttributes( currentElement, lang, grade, r_grade, qcount, r_qcount, qdate, r_qdate, remark, bcount, r_bcount, query_id,
                                          pronunciation, width, type, subType, faux_ami_f, faux_ami_t, synonym, example, antonym, usage, paraphrase))
        return false;

      if (m_doc->entryCount() == 0)
      {
        // only accept in first entry
        if (width >= 0)
          m_doc->setSizeHint(i, width);

        if (query_id == KV_O)
          q_org = lang;

        if (query_id == KV_T)

          q_trans = lang;
      }
//kDebug() << " TranslationList.count(): " << translationList.count() << "  Entry count: " << m_doc->entryCount();
        if (m_doc->entryCount() == 0) { // this is because in kvtml the languages are saved in the FIRST ENTRY ONLY.
//kDebug() << " Read Expression with identifiers: " << lang;
            // new translation
            if (lang.isEmpty()) {
                if (i == 0) {
                    lang = "original";
                } else {
                    // no definition in first entry ?
                    lang.setNum(m_doc->identifierCount());
                    lang.prepend("translation ");
                }

            }
            if (m_doc->identifierCount() <= i)
              m_doc->appendIdentifier(lang);
        }
        else
        {
            if (lang != m_doc->identifier(i) && !lang.isEmpty())
            {
            // different language ?
            m_errorMessage = i18n("ambiguous definition of language code");
            return false;
            }
        }

      //---------
      // Children

      currentChild = currentElement.firstChildElement(KV_CONJUG_GRP);
      if (!currentChild.isNull()) {
          conjug.clear();
          if (!readConjug(currentChild, conjug))
            return false;
      }

      currentChild = currentElement.firstChildElement(KV_COMPARISON_GRP);
      if (!currentChild.isNull()) {
          comparison.clear();
          if (!readComparison(currentChild, comparison))
            return false;
      }

      currentChild = currentElement.firstChildElement(KV_MULTIPLECHOICE_GRP);
      if (!currentChild.isNull()) {
          mc.clear();
          if (!readMultipleChoice(currentChild, mc))
            return false;
      }

      textstr = currentElement.lastChild().toText().data();
      if (textstr.isNull())
        textstr = "";

      if (i == 0) {
		  expr = KEduVocExpression(textstr);
		  expr.setLesson(lesson);
		  expr.setInQuery(inquery);
		  expr.setActive(active);
	  } else {
		  expr.setTranslation(i, textstr);
	  }

      if (conjug.size() > 0) {
        for ( int conjugationIndex = 0; conjugationIndex < conjug.size(); conjugationIndex++ ) {
            expr.translation(i).setConjugation(conjug[conjugationIndex]);
        }
        //expr.setConjugation(i, conjug[0]); ///@todo check if this is better than the above!

        conjug.clear();
      }
      if (!comparison.isEmpty())
      {
        expr.translation(i).setComparison(comparison);
        comparison.clear();
      }
      if (!mc.isEmpty())
      {
        expr.translation(i).setMultipleChoice(mc);
        mc.clear();
      }

      if (!type.isEmpty()) {
        expr.translation(i).setType(type);
        if (!subType.isEmpty()) {
            expr.translation(i).setSubType(subType);
        }
      }

      if (!remark.isEmpty() )
        expr.translation(i).setComment (remark);
      if (!pronunciation.isEmpty() )
        expr.translation(i).setPronunciation(pronunciation);
      if (!faux_ami_f.isEmpty() )
        expr.translation(i).setFalseFriend (0, faux_ami_f);
      if (!faux_ami_t.isEmpty() )
        expr.translation(0).setFalseFriend (i, faux_ami_t);
      if (!synonym.isEmpty() )
        expr.translation(i).setSynonym (synonym);
      if (!example.isEmpty() )
        expr.translation(i).setExample (example);
      if (!usage.isEmpty() )
        expr.translation(i).setUsageLabel (usage);
      if (!paraphrase.isEmpty() )
        expr.translation(i).setParaphrase (paraphrase);
      if (!antonym.isEmpty() )
        expr.translation(i).setAntonym (antonym);

      if ( i != 0 ) {
		expr.translation(i).gradeFrom(0).setGrade(grade);
		expr.translation(0).gradeFrom(i).setGrade(r_grade);
        expr.translation(i).gradeFrom(0).setQueryCount(qcount);
        expr.translation(0).gradeFrom(i).setQueryCount(r_qcount);
        expr.translation(i).gradeFrom(0).setBadCount(bcount);
        expr.translation(0).gradeFrom(i).setBadCount(r_bcount);
        expr.translation(i).gradeFrom(0).setQueryDate(qdate);
        expr.translation(0).gradeFrom(i).setQueryDate(r_qdate);
      }

      // Next translation
      currentElement = currentElement.nextSiblingElement(KV_TRANS);
      i++;
  }

  if (m_doc->entryCount() == 0)
    m_doc->setQueryIdentifier(q_org, q_trans);
  m_doc->appendEntry(&expr);

  // also add this entryid to the lesson it's part of
  if (m_doc->lesson(lesson) != NULL)
  {
	m_doc->lesson(lesson)->addEntry(m_doc->entryCount());
  }

  return true;
}

#include "keduvockvtmlreader.moc"
