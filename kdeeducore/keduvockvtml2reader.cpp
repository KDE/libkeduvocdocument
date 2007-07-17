/***************************************************************************
                     read a KEduVocDocument from a KVTML file
    -----------------------------------------------------------------------
    copyright           : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                          (C) 2001 The KDE-EDU team
                          (C) 2005 Eric Pignet <eric at erixpage.com>
                          (C) 2007 Peter Hedlund <peter.hedlund@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvockvtml2reader.h"

#include <QTextStream>
#include <QList>
#include <QIODevice>

#include <kdebug.h>
#include <klocale.h>

#include "keduvocdocument.h"
#include "kvtml2defs.h"
#include "kvtmldefs.h"
#include "keduvockvtmlreader.h"

KEduVocKvtml2Reader::KEduVocKvtml2Reader(QIODevice *file)
: m_inputFile(file)
{
  // the file must be already open
  if (!m_inputFile->isOpen())
  {
    m_errorMessage = i18n("file must be opened first");
  }
}


bool KEduVocKvtml2Reader::readDoc(KEduVocDocument *doc)
{
  m_doc = doc;

  QDomDocument domDoc("KEduVocDocument");

  if (!domDoc.setContent(m_inputFile, &m_errorMessage))
    return false;

  QDomElement domElementKvtml = domDoc.documentElement();
  if (domElementKvtml.tagName() != KVTML_TAG)
  {
    m_errorMessage = i18n("This is not a KDE Vocabulary document.");
    return false;
  }
  
  if (domElementKvtml.attribute(KVTML_VERSION).toFloat() < 2.0)
  {
    // read the file with the old format
    
    // first reset the file to the beginning
    m_inputFile->seek(0);
    KEduVocKvtmlReader oldFormat(m_inputFile);
      
    // get the return value
    bool retval = oldFormat.readDoc(doc);
      
    // pass the errormessage up
    m_errorMessage = oldFormat.errorMessage();
    return retval;
  }

  //-------------------------------------------------------------------------
  // Information
  //-------------------------------------------------------------------------

  QDomElement info = domElementKvtml.firstChildElement(KVTML_INFORMATION);
  if (!info.isNull())
  {
    if (!readInformation(info))
      return false;
  }

  // possibly add lines support to information section of kvtml2 dtd?
  //documentAttribute = domElementKvtml.attributeNode(KV_LINES);
  //if (!documentAttribute.isNull())
  //  m_lines = documentAttribute.value().toInt();

  //-------------------------------------------------------------------------
  // Children
  //-------------------------------------------------------------------------

  bool result = readGroups(domElementKvtml);  // read sub-groups

  return result;
}

bool KEduVocKvtml2Reader::readInformation(QDomElement &informationElement)
{
  // read the generator
  QDomElement currentElement = informationElement.firstChildElement(KVTML_GENERATOR);
  if (!currentElement.isNull())
  {
    m_doc->setGenerator(currentElement.text());
    // add the version if it's there
    int pos = m_doc->generator().lastIndexOf(KVD_VERS_PREFIX);
    if (pos >= 0)
    {
      m_doc->setVersion(m_doc->generator().remove(0, pos + 2));
    }
  }
  
  // read the title
  currentElement = informationElement.firstChildElement(KVTML_TITLE);
  if (!currentElement.isNull())
  {
    m_doc->setTitle(currentElement.text());
  }
  
  // read the author
  currentElement = informationElement.firstChildElement(KVTML_AUTHOR);
  if (!currentElement.isNull())
  {
    m_doc->setAuthor(currentElement.text());
  }
  
  // read the license
  currentElement = informationElement.firstChildElement(KVTML_LICENSE);
  if (!currentElement.isNull())
  {
    m_doc->setLicense(currentElement.text());
  }

  // read the comment
  currentElement = informationElement.firstChildElement(KVTML_COMMENT);
  if (!currentElement.isNull())
  {
    m_doc->setDocumentRemark(currentElement.text());
  }

  return true;
}
  
bool KEduVocKvtml2Reader::readGroups(QDomElement &domElementParent)
{
  bool result = false;
  
  QDomElement currentElement;

  QDomElement groupElement = domElementParent.firstChildElement(KVTML_IDENTIFIERS);
  if (!groupElement.isNull())
  {
    QDomNodeList entryList = groupElement.elementsByTagName(KVTML_IDENTIFIER);
    if (entryList.length() <= 0)
    {
      m_errorMessage = i18n("missing identifiers from identifiers tag");
      return false;
    }

    for (int i = 0; i < entryList.count(); ++i) 
    {
      currentElement = entryList.item(i).toElement();
      if (currentElement.parentNode() == groupElement) 
      {
        result = readIdentifier(currentElement);
        if (!result)
          return false;
      }
    }    
  }
  
  groupElement = domElementParent.firstChildElement(KVTML_TYPES);
  if (!groupElement.isNull())
  {
    readTypes(groupElement);
  }
  
  groupElement = domElementParent.firstChildElement(KVTML_TENSES);
  if (!groupElement.isNull())
  {
    readTenses(groupElement);
  }
  
  groupElement = domElementParent.firstChildElement(KVTML_USAGES);
  if (!groupElement.isNull())
  {
    readUsages(groupElement);
  }

  groupElement = domElementParent.firstChildElement(KVTML_ENTRIES);
  if (!groupElement.isNull())
  {
	  QDomNodeList entryList = groupElement.elementsByTagName(KVTML_ENTRY);
    if (entryList.length() <= 0)
    {
      m_errorMessage = i18n("no entries found in entries tag");
      return false; // at least one entry is required
    }
    
    for (int i = 0; i < entryList.count(); ++i)
    {
      currentElement = entryList.item(i).toElement();
      if (currentElement.parentNode() == groupElement)
      {
        result = readEntry(currentElement);
        if (!result)
          return false;
      }
    }
  }

  groupElement = domElementParent.firstChildElement(KVTML_LESSONS);
  if (!groupElement.isNull())
  {
	  QDomNodeList entryList = groupElement.elementsByTagName(KVTML_LESSON);
    if (entryList.length() <= 0)
    {
      m_errorMessage = i18n("no lessons found in lessons tag");
      return false; // at least one entry is required
    }
    
    for (int i = 0; i < entryList.count(); ++i)
    {
      currentElement = entryList.item(i).toElement();
      if (currentElement.parentNode() == groupElement)
      {
        result = readLesson(currentElement);
        if (!result)
          return false;
      }
    }
  }

  return true;
}

bool KEduVocKvtml2Reader::readIdentifier(QDomElement &identifierElement)
{
  bool result = true;
  int id = identifierElement.attribute(KVTML_ID).toInt(&result);
  if (!result)
  {
    m_errorMessage = i18n("identifier missing id");
    return false;
  }
  
  QDomElement currentElement = identifierElement.firstChildElement(KVTML_TYPE);
  if (!currentElement.isNull())
  {
    
  }
  return result;
}

bool KEduVocKvtml2Reader::readEntry(QDomElement &entryElement)
{
  KEduVocExpression expr;
  QDomElement currentElement;
  bool result = true;

  // get entry id
  int id = entryElement.attribute(KVTML_ID).toInt(&result);
  if (!result)
  {
    m_errorMessage = i18n("entry missing id");
    return false;
  }
  
  // read info tags: inactive, inquery, and sizehint
  
  // read translation children
  QDomNodeList translationList = entryElement.elementsByTagName(KVTML_TRANSLATION);
  if (translationList.length() <= 0)
  {
    m_errorMessage = i18n("no translations found");
    return false; // at least one entry is required
  }
  
  for (int i = 0; i < translationList.count(); ++i)
  {
    currentElement = translationList.item(i).toElement();
    if (currentElement.parentNode() == entryElement)
    {
      result = readTranslation(currentElement, expr, i);
      if (!result)
        return false;
    }
  }
  
  // TODO: probably should insert at id position with a check to see if it exists
  m_doc->appendEntry(&expr);
  return result;
}

bool KEduVocKvtml2Reader::readTranslation(QDomElement &translationElement, 
                                          KEduVocExpression &expr, int index)
{
  QDomElement currentElement = translationElement.firstChildElement(KVTML_TEXT);
  if (!currentElement.isNull())
  {
    expr.translation(index).setTranslation(currentElement.text());
  }
  
  currentElement = translationElement.firstChildElement(KVTML_COMMENT);
  if (!currentElement.isNull())
  {
    expr.translation(index).setComment(currentElement.text());
  }
  
  currentElement = translationElement.firstChildElement(KVTML_TYPE);
  if (!currentElement.isNull())
  {
    expr.translation(index).setType(currentElement.text());
  }

// kvtml 1: we always have an original element (required)
//    currentElement = domElementParent.firstChildElement(KV_ORG);
//    if (currentElement.isNull()) { // sanity check
//        m_errorMessage = i18n("Data for original language missing");
//        return false;
//    }

//    while (!currentElement.isNull()) {
//      type = exprtype; // seems like type can be in the parent element and overwritten in the children here :(

//      //-----------
//      // Attributes
//      //-----------

//      // read attributes - the order of the query grades is interchanged!
//      if (i == 0 && !readExpressionChildAttributes( currentElement, lang, grade, r_grade, qcount, r_qcount, qdate, r_qdate, remark, bcount, r_bcount, query_id,
//                                          pronunciation, width, type, faux_ami_t, faux_ami_f, synonym, example, antonym, usage, paraphrase))
//        return false;

//      if (i != 0 && !readExpressionChildAttributes( currentElement, lang, grade, r_grade, qcount, r_qcount, qdate, r_qdate, remark, bcount, r_bcount, query_id,
//                                          pronunciation, width, type, faux_ami_f, faux_ami_t, synonym, example, antonym, usage, paraphrase))
//        return false;

//      if (m_doc->entryCount() == 0)
//      {
//        // only accept in first entry
//        if (width >= 0)
//          m_doc->setSizeHint(i, width);

//        if (query_id == KV_O)
//          q_org = lang;

//        if (query_id == KV_T)

//          q_trans = lang;
//      }
////kDebug() << " TranslationList.count(): " << translationList.count() << "  Entry count: " << m_doc->entryCount() << endl;
//        if (m_doc->entryCount() == 0) { // this is because in kvtml the languages are saved in the FIRST ENTRY ONLY.
//kDebug() << " Read Expression with identifiers: " << i << endl;
//            // new translation
//            if (lang.isEmpty()) {
//                if (i == 0) {
//                    lang = "original";
//                } else {
//                    // no definition in first entry ?
//                    lang.setNum(m_doc->identifierCount());
//                    lang.prepend("translation ");
//                }
//                m_doc->appendIdentifier(lang);
//            }
//        }
//        else
//        {
//            if (lang != m_doc->identifier(i) && !lang.isEmpty())
//            {
//            // different language ?
//            m_errorMessage = i18n("ambiguous definition of language code");
//            return false;
//            }
//        }

//      //---------
//      // Children

//      currentChild = currentElement.firstChildElement(KV_CONJUG_GRP);
//      if (!currentChild.isNull()) {
//          conjug.clear();
//          if (!readConjug(currentChild, conjug))
//            return false;
//      }

//      currentChild = currentElement.firstChildElement(KV_COMPARISON_GRP);
//      if (!currentChild.isNull()) {
//          comparison.clear();
//          if (!readComparison(currentChild, comparison))
//            return false;
//      }

//      currentChild = currentElement.firstChildElement(KV_MULTIPLECHOICE_GRP);
//      if (!currentChild.isNull()) {
//          mc.clear();
//          if (!readMultipleChoice(currentChild, mc))
//            return false;
//      }

//      textstr = currentElement.lastChild().toText().data();
//      if (textstr.isNull())
//        textstr = "";

//      if (i == 0) {
//        expr = KEduVocExpression(textstr);
//        expr.setLesson(lesson);
//        expr.setInQuery(inquery);
//        expr.setActive(active);
//      } else {
//        expr.setTranslation(i, textstr);
//      }

//      if (conjug.size() > 0) {
//        for ( int conjugationIndex = 0; conjugationIndex < conjug.size(); conjugationIndex++ ) {
//            expr.translation(i).setConjugation(conjug[conjugationIndex]);
//        }
//        //expr.setConjugation(i, conjug[0]); ///@todo check if this is better than the above!

//        conjug.clear();
//      }
//      if (!comparison.isEmpty())
//      {
//        expr.translation(i).setComparison(comparison);
//        comparison.clear();
//      }
//      if (!mc.isEmpty())
//      {
//        expr.translation(i).setMultipleChoice(mc);
//        mc.clear();
//      }
//      if (!type.isEmpty() )
//        expr.translation(i).setType (type);
//      if (!remark.isEmpty() )
//        expr.translation(i).setComment (remark);
//      if (!pronunciation.isEmpty() )
//        expr.translation(i).setPronunciation(pronunciation);
//      if (!faux_ami_f.isEmpty() )
//        expr.translation(i).setFalseFriend (0, faux_ami_f);
//      if (!faux_ami_t.isEmpty() )
//        expr.translation(0).setFalseFriend (i, faux_ami_t);
//      if (!synonym.isEmpty() )
//        expr.translation(i).setSynonym (synonym);
//      if (!example.isEmpty() )
//        expr.translation(i).setExample (example);
//      if (!usage.isEmpty() )
//        expr.translation(i).setUsageLabel (usage);
//      if (!paraphrase.isEmpty() )
//        expr.translation(i).setParaphrase (paraphrase);
//      if (!antonym.isEmpty() )
//        expr.translation(i).setAntonym (antonym);

//      if ( i != 0 ) {
//        expr.translation(i).gradeFrom(0).setQueryCount(qcount);
//        expr.translation(0).gradeFrom(i).setQueryCount(r_qcount);
//        expr.translation(i).gradeFrom(0).setBadCount(bcount);
//        expr.translation(0).gradeFrom(i).setBadCount(r_bcount);
//        expr.translation(i).gradeFrom(0).setQueryDate(qdate);
//        expr.translation(0).gradeFrom(i).setQueryDate(r_qdate);
//      }
////kDebug() << "KEduVocKvtml2Reader::readExpression(): id: " << i << " translation: " << textstr << endl;

//      // Next translation
//      currentElement = currentElement.nextSiblingElement(KV_TRANS);
//      i++;
//  }

  //if (m_doc->entryCount() == 0)
  //  m_doc->setQueryIdentifier(q_org, q_trans);
  return true;
}

bool KEduVocKvtml2Reader::readLesson(QDomElement &domElementParent)
{
  QString s;
  QStringList descriptions;
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

  descriptions.clear();
  QList<int> inQueryList;
  inQueryList.clear();

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
      descriptions.append(s);
    }
  }

  if (inQueryList.count() > 0)
    m_doc->setLessonsInQuery(inQueryList);
  m_doc->setLessonDescriptions(descriptions);

  return true;
}


bool KEduVocKvtml2Reader::readArticle(QDomElement &articleElement, int identifierNum)
/*
 <article>
  <definite>
    <male>der</male>
	<female>die</female>
	<neutral>das</neutral>
  </definite>
  <indefinite>
    <male>ein</male>
	<female>eine</female>
	<neutral>ein</neutral>
  </indefinite>
 </article>
*/
{ 
  QString fem_def = "";
  QString mal_def = "";
  QString nat_def = "";
  QString fem_indef = "";
  QString mal_indef = "";
  QString nat_indef = "";

  QDomElement currentElement = articleElement.firstChildElement(KVTML_DEFINITE);
  if (!currentElement.isNull())
  {
    QDomElement subElement = currentElement.firstChildElement(KVTML_MALE);
    if (!subElement.isNull())
    {
      mal_def = subElement.text();
    }
    
    subElement = currentElement.firstChildElement(KVTML_FEMALE);
    if (!subElement.isNull())
    {
      fem_def = subElement.text();
    }
    
    subElement = currentElement.firstChildElement(KVTML_NEUTRAL);
    if (!subElement.isNull())
    {
      nat_def = subElement.text();
    }
  }
  
  currentElement = articleElement.firstChildElement(KVTML_INDEFINITE);
  if (!currentElement.isNull())
  {
    QDomElement subElement = currentElement.firstChildElement(KVTML_MALE);
    if (!subElement.isNull())
    {
      mal_indef = subElement.text();
    }
    
    subElement = currentElement.firstChildElement(KVTML_FEMALE);
    if (!subElement.isNull())
    {
      fem_indef = subElement.text();
    }
    
    subElement = currentElement.firstChildElement(KVTML_NEUTRAL);
    if (!subElement.isNull())
    {
      nat_indef = subElement.text();
    }
  }
  m_doc->setArticle(identifierNum, KEduVocArticle(fem_def, fem_indef, mal_def, mal_indef, nat_def, nat_indef));

  return true;
}


bool KEduVocKvtml2Reader::readConjug(QDomElement &domElementParent, QList<KEduVocConjugation> &curr_conjug, int)
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

bool KEduVocKvtml2Reader::readTypes(QDomElement &typesElement)
{
  QStringList types;

  QDomNodeList typeNodes = typesElement.elementsByTagName(KVTML_TYPE);
  for (int i = 0; i < typeNodes.count(); ++i)
  {
    QDomElement currentElement = typeNodes.item(i).toElement();
    if (currentElement.parentNode() == typesElement)
    {
      types.append(currentElement.text());
    }
  }

  m_doc->setTypeDescriptions(types);
  return true;
}


bool KEduVocKvtml2Reader::readTenses(QDomElement &tensesElement)
{
  QStringList tenses;

  QDomNodeList tenseNodes = tensesElement.elementsByTagName(KVTML_TENSE);
  for (int i = 0; i < tenseNodes.count(); ++i)
  {
    QDomElement currentElement = tenseNodes.item(i).toElement();
    if (currentElement.parentNode() == tensesElement)
    {
      tenses.append(currentElement.text());
    }
  }

  m_doc->setTenseDescriptions(tenses);
  return true;
}


bool KEduVocKvtml2Reader::readUsages(QDomElement &usagesElement)
{
  QStringList usages;

  QDomNodeList usageNodes = usagesElement.elementsByTagName(KVTML_USAGE);
  for (int i = 0; i < usageNodes.count(); ++i)
  {
    QDomElement currentElement = usageNodes.item(i).toElement();
    if (currentElement.parentNode() == usagesElement)
    {
      usages.append(currentElement.text());
    }
  }

  m_doc->setUsageDescriptions(usages);
  return true;
}


bool KEduVocKvtml2Reader::readComparison(QDomElement &domElementParent, KEduVocComparison &comp)
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


bool KEduVocKvtml2Reader::readMultipleChoice(QDomElement &domElementParent, KEduVocMultipleChoice &mc)
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
    if (s.isNull())
      s = "";
    mc.setMC1(s);
  }

  currentElement = domElementParent.firstChildElement(KV_MC_2);
  if (!currentElement.isNull()) {
    s = currentElement.text();
    if (s.isNull())
      s = "";
    mc.setMC2(s);
  }

  currentElement = domElementParent.firstChildElement(KV_MC_3);
  if (!currentElement.isNull()) {
    s = currentElement.text();
    if (s.isNull())
      s = "";
    mc.setMC3(s);
  }

  currentElement = domElementParent.firstChildElement(KV_MC_4);
  if (!currentElement.isNull()) {
    s = currentElement.text();
    if (s.isNull())
      s = "";
    mc.setMC4(s);
  }

  currentElement = domElementParent.firstChildElement(KV_MC_5);
  if (!currentElement.isNull()) {
    s = currentElement.text();
    if (s.isNull())
      s = "";
    mc.setMC5(s);
  }

  mc.normalize();
  return true;
}


//bool KEduVocKvtml2Reader::readExpressionChildAttributes( QDomElement &domElementExpressionChild,
//                                                        QString &lang,
//                                                        grade_t &grade, grade_t &rev_grade,
//                                                        int &count, int &rev_count,
//                                                        QDateTime &date, QDateTime &rev_date,
//                                                        QString &remark,
//                                                        int &bcount, int &rev_bcount,
//                                                        QString &query_id,
//                                                        QString &pronunciation,
//                                                        int &width,
//                                                        QString &type,
//                                                        QString &faux_ami_f,
//                                                        QString &faux_ami_t,
//                                                        QString &synonym,
//                                                        QString &example,
//                                                        QString &antonym,
//                                                        QString &usage,
//                                                        QString &paraphrase)
//{
//  int pos;
//  QDomAttr attribute;

//  lang = "";
//  attribute = domElementExpressionChild.attributeNode(KV_LANG);
//  if (!attribute.isNull())
//    lang = attribute.value();

//  width = -1;
//  attribute = domElementExpressionChild.attributeNode(KV_SIZEHINT);
//  if (!attribute.isNull())
//    width = attribute.value().toInt();

//  grade = KV_NORM_GRADE;
//  rev_grade = KV_NORM_GRADE;
//  attribute = domElementExpressionChild.attributeNode(KV_GRADE);
//  if (!attribute.isNull())
//  {
//    QString s = attribute.value();
//    if ((pos = s.indexOf(';')) >= 1)
//    {
//      grade = s.left(pos).toInt();
//      rev_grade = s.mid(pos + 1, s.length()).toInt();
//    }
//    else
//      grade = s.toInt();
//  }

//  count = 0;
//  rev_count = 0;
//  attribute = domElementExpressionChild.attributeNode(KV_COUNT);
//  if (!attribute.isNull())
//  {
//    QString s = attribute.value();
//    if ((pos = s.indexOf(';')) >= 1)
//    {
//      count = s.left(pos).toInt();
//      rev_count = s.mid(pos + 1, s.length()).toInt();
//    }
//    else
//      count = s.toInt();
//  }

//  bcount = 0;
//  rev_bcount = 0;
//  attribute = domElementExpressionChild.attributeNode(KV_BAD);
//  if (!attribute.isNull())
//  {
//    QString s = attribute.value();
//    if ((pos = s.indexOf(';')) >= 1)
//    {
//      bcount = s.left(pos).toInt();
//      rev_bcount = s.mid(pos + 1, s.length()).toInt();
//    }
//    else
//      bcount = s.toInt();
//  }

//  date.setTime_t(0);
//  rev_date.setTime_t(0);
//  attribute = domElementExpressionChild.attributeNode(KV_DATE);
//  if (!attribute.isNull())
//  {
//    QString s = attribute.value();
//    if ((pos = s.indexOf(';')) >= 1)
//    {
//      date.setTime_t(s.left(pos).toInt());
//      rev_date.setTime_t(s.mid(pos + 1, s.length()).toInt());
//    }
//    else
//      date.setTime_t(s.toInt());
//  }

//  attribute = domElementExpressionChild.attributeNode(KV_DATE2);
//  if (!attribute.isNull())
//  {
//    //this format is deprecated and ignored.
//  }

//  remark = "";
//  attribute = domElementExpressionChild.attributeNode(KV_REMARK);
//  if (!attribute.isNull())
//    remark = attribute.value();

//  faux_ami_f = "";
//  attribute = domElementExpressionChild.attributeNode(KV_FAUX_AMI_F);
//  if (!attribute.isNull())
//    faux_ami_f = attribute.value();

//  faux_ami_t = "";
//  attribute = domElementExpressionChild.attributeNode(KV_FAUX_AMI_T);
//  if (!attribute.isNull())
//    faux_ami_t = attribute.value();

//  synonym = "";
//  attribute = domElementExpressionChild.attributeNode(KV_SYNONYM);
//  if (!attribute.isNull())
//    synonym = attribute.value();

//  example = "";
//  attribute = domElementExpressionChild.attributeNode(KV_EXAMPLE);
//  if (!attribute.isNull())
//    example = attribute.value();

//  usage = "";
//  attribute = domElementExpressionChild.attributeNode(KV_USAGE);
//  if (!attribute.isNull())
//  {
//    usage = attribute.value();
//    if (usage.length() != 0 && usage.left(1) == UL_USER_USAGE)
//    {
//      int num = qMin(usage.mid (1, 40).toInt(), 1000); // paranioa check
//      if (num > m_doc->usageDescriptions().count())
//      {
//        // description missing ?
//        QStringList sl = m_doc->usageDescriptions();
//        QString s;
//        for (int i = m_doc->usageDescriptions().count(); i < num; i++)
//        {
//          s.setNum(i + 1);
//          s.prepend("#");  // invent descr according to number
//          sl.append(s);
//        }
//        m_doc->setUsageDescriptions(sl);
//      }
//    }
//  }

//  paraphrase = "";
//  attribute = domElementExpressionChild.attributeNode(KV_PARAPHRASE);
//  if (!attribute.isNull())
//    paraphrase = attribute.value();

//  antonym = "";
//  attribute = domElementExpressionChild.attributeNode(KV_ANTONYM);
//  if (!attribute.isNull())
//    antonym = attribute.value();

//  attribute = domElementExpressionChild.attributeNode(KV_EXPRTYPE);
//  if (!attribute.isNull())
//  {
//    type = attribute.value();
//    if (type == "1")
//      type = QM_VERB;
//    else if (type == "2")  // convert from pre-0.5 versions
//      type = QM_NOUN;
//    else if (type == "3")
//      type = QM_NAME;

//    if (type.length() != 0 && type.left(1) == QM_USER_TYPE)
//    {
//      int num = qMin(type.mid (1, 40).toInt(), 1000); // paranoia check
//      if (num > m_doc->typeDescriptions().count())
//      {
//        // description missing ?
//        QString s;
//        QStringList sl = m_doc->typeDescriptions();
//        for (int i = m_doc->typeDescriptions().count(); i < num; i++)
//        {
//          s.setNum(i + 1);
//          s.prepend("#");  // invent descr according to number
//          sl.append(s);
//        }
//        m_doc->setTypeDescriptions(sl);
//      }
//    }
//  }

//  pronunciation = "";
//  attribute = domElementExpressionChild.attributeNode(KV_PRONUNCE);
//  if (!attribute.isNull())
//    pronunciation = attribute.value();

//  query_id = "";
//  attribute = domElementExpressionChild.attributeNode(KV_QUERY);
//  if (!attribute.isNull())
//    query_id = attribute.value();

//  return true;
//}



#include "keduvockvtml2reader.moc"
