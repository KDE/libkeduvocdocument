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
      m_errorMessage = i18n("missing identifier elements from identifiers tag");
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
    // TODO: do something with the type
  }
  
  currentElement = identifierElement.firstChildElement(KVTML_LOCALE);
  if (!currentElement.isNull())
  {
    // TODO: do we want to use this for the identifier, or the name?
    m_doc->setIdentifier(id, currentElement.text());
  }
  
  currentElement = identifierElement.firstChildElement(KVTML_NAME);
  if (!currentElement.isNull())
  {
    // TODO: do something with the name
  }
  
  currentElement = identifierElement.firstChildElement(KVTML_SIZEHINT);
  if (!currentElement.isNull())
  {
    // TODO: do something with the sizehint
  }
  
  // read sub-parts
  currentElement = identifierElement.firstChildElement(KVTML_ARTICLE);
  if (!currentElement.isNull())
  {
    readArticle(currentElement, id);
  }
  
  currentElement = identifierElement.firstChildElement(KVTML_PERSONALPRONOUNS);
  if (!currentElement.isNull())
  {
    KEduVocConjugation personalPronouns;
    readConjugation(currentElement, personalPronouns);
    m_doc->setConjugation(id, personalPronouns);
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
  currentElement = entryElement.firstChildElement(KVTML_INACTIVE);
  if (!currentElement.isNull())
  {
    // set the active state of the expression
    if (currentElement.text() == KVTML_TRUE)
    {
      expr.setActive(false);
    }
    else
    {
      expr.setActive(true);
    }
  }
  
  currentElement = entryElement.firstChildElement(KVTML_INQUERY);
  if (!currentElement.isNull())
  {
    // set the inquery information
    if (currentElement.text() == KVTML_TRUE)
    {
      expr.setInQuery(true);
    }
    else
    {
      expr.setInQuery(false);
    }
  }
  
  currentElement = entryElement.firstChildElement(KVTML_SIZEHINT);
  if (!currentElement.isNull())
  {
    // set the sizehint
    expr.setSizeHint(currentElement.text().toInt());
  }
  
  // read translation children
  QDomNodeList translationList = entryElement.elementsByTagName(KVTML_TRANSLATION);
  if (translationList.length() <= 0)
  {
    m_errorMessage = i18n("no translations found");
    return false; // at least one translation is required
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
  
  currentElement = translationElement.firstChildElement(KVTML_INQUERY);
  if (!currentElement.isNull())
  {
    // TODO: ask fregl what inquery is for, and do something with it
  }

  //<pronunciation></pronunciation>
  currentElement = translationElement.firstChildElement(KVTML_PRONUNCIATION);
  if (!currentElement.isNull())
  {
    expr.translation(index).setPronunciation(currentElement.text());
  }
  
  //<falsefriendfrom></falsefriendfrom>
  currentElement = translationElement.firstChildElement(KVTML_FALSEFRIENDFROM);
  if (!currentElement.isNull())
  {
    // TODO: figure out what to do with falsefriend information
  }

  //<falsefriendto></falsefriendto>
  currentElement = translationElement.firstChildElement(KVTML_FALSEFRIENDTO);
  if (!currentElement.isNull())
  {
  }

  //<falsefriend></falsefriend>
  currentElement = translationElement.firstChildElement(KVTML_FALSEFRIEND);
  if (!currentElement.isNull())
  {
    // TODO: figure out what to do with falsefriend information
  }

  //<antonym></antonym>
  currentElement = translationElement.firstChildElement(KVTML_ANTONYM);
  if (!currentElement.isNull())
  {
    // TODO: figure out what to do with falsefriend information
  }

  //<synonym></synonym>
  currentElement = translationElement.firstChildElement(KVTML_SYNONYM);
  if (!currentElement.isNull())
  {
    expr.translation(index).setSynonym(currentElement.text());
  }

  //<example></example>
  currentElement = translationElement.firstChildElement(KVTML_EXAMPLE);
  if (!currentElement.isNull())
  {
    expr.translation(index).setExample(currentElement.text());
  }

  //<usage></usage>
  currentElement = translationElement.firstChildElement(KVTML_USAGE);
  if (!currentElement.isNull())
  {
    expr.translation(index).setUsageLabel(currentElement.text());
  }

  //<paraphrase></paraphrase>
  currentElement = translationElement.firstChildElement(KVTML_PARAPHRASE);
  if (!currentElement.isNull())
  {
    expr.translation(index).setParaphrase(currentElement.text());
  }

  // conjugations
  currentElement = translationElement.firstChildElement(KVTML_CONJUGATION);
  KEduVocConjugation conjugation;
  while (!currentElement.isNull())
  {
    // read any conjugations (NOTE: this will overwrite any conjugations of the same type for this
    // translation, as the type is used as the key
    readConjugation(currentElement, conjugation);  
    currentElement = currentElement.nextSiblingElement(KVTML_CONJUGATION);
  }
  if (conjugation.entryCount() > 0)
  {
    expr.translation(index).setConjugation(conjugation);
  }

  // grade elements
  currentElement = translationElement.firstChildElement(KVTML_GRADE);
  while (!currentElement.isNull())
  {
    // TODO: read grade
    readGrade(currentElement, expr, index);
    currentElement = currentElement.nextSiblingElement(KVTML_GRADE);
  }

  // comparisons
  currentElement = translationElement.firstChildElement(KVTML_COMPARISON);
  if (!currentElement.isNull())
  {
    KEduVocComparison comparison;
    readComparison(currentElement, comparison);
    expr.translation(index).setComparison(comparison);
  }

  // multiple choice
  currentElement = translationElement.firstChildElement(KVTML_MULTIPLECHOICE);
  if (!currentElement.isNull())
  {
    KEduVocMultipleChoice mc;
    readMultipleChoice(currentElement, mc);
    expr.translation(index).setMultipleChoice(mc);
  }

  // image
  currentElement = translationElement.firstChildElement(KVTML_IMAGE);
  if (!currentElement.isNull())
  {
    // TODO: do something with the image
  }

  // sound
  currentElement = translationElement.firstChildElement(KVTML_SOUND);
  if (!currentElement.isNull())
  {
    // TODO: do something with the sound
  }

//        if (query_id == KV_O)
//          q_org = lang;

//        if (query_id == KV_T)

//          q_trans = lang;


//      if (!faux_ami_f.isEmpty() )
//        expr.translation(i).setFalseFriend (0, faux_ami_f);
//      if (!faux_ami_t.isEmpty() )
//        expr.translation(0).setFalseFriend (i, faux_ami_t);
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


bool KEduVocKvtml2Reader::readConjugation(QDomElement &conjugElement, KEduVocConjugation &curr_conjug)
/*
 <conjugation>
  <tenseid>1</tenseid>
  <type>regular</type>
  <singular>
    <first person></first person>
    <second person></second person>
    <third person>
      <male></male>
      <female></female>
      <neutral></neutral>
    </third person>
  </singular>
  <plural>
    <first person></first person>
    <second person></second person>
    <third person>
      <common></common>
    </third person>
  </plural>
 </conjugation>
*/
{
  bool p3_common;
  bool s3_common;
  QString singfirst;
  QString singsecond;
  QString singthirdmale;
  QString singthirdfemale;
  QString singthirdneutral;
  QString plurfirst;
  QString plursecond;
  QString plurthirdmale;
  QString plurthirdfemale;
  QString plurthirdneutral;
  QString type;
  
  QDomElement typeElement = conjugElement.firstChildElement(KVTML_TYPE);
  if (!typeElement.isNull())
  {
    type = typeElement.text();
  }
  
  // TODO: add something here to link and/or store tense information

  QDomElement currentGroup = conjugElement.firstChildElement(KVTML_SINGULAR);
  if (!currentGroup.isNull())
  {
    QDomElement currentElement = currentGroup.firstChildElement(KVTML_1STPERSON);
    if (!currentElement.isNull())
    {
      singfirst = currentElement.text();
    }
    
    currentElement = currentGroup.firstChildElement(KVTML_2NDPERSON);
    if (!currentElement.isNull())
    {
      singsecond = currentElement.text();
    }
    
    currentGroup = currentGroup.firstChildElement(KVTML_3RDPERSON);
    if (!currentGroup.isNull())
    {
      currentElement = currentGroup.firstChildElement(KVTML_COMMON);
      if (!currentElement.isNull())
      {
        s3_common = true;
        singthirdmale = currentElement.text();
        singthirdfemale = singthirdmale;
        singthirdneutral = singthirdmale;
      }
      else
      {
        s3_common = false;
        currentElement = currentGroup.firstChildElement(KVTML_MALE);
        if (!currentElement.isNull())
        {
          singthirdmale = currentElement.text();
        }
        
        currentElement = currentGroup.firstChildElement(KVTML_FEMALE);
        if (!currentElement.isNull())
        {
          singthirdfemale = currentElement.text();
        }
        
        currentElement = currentGroup.firstChildElement(KVTML_NEUTRAL);
        if (!currentElement.isNull())
        {
          singthirdneutral = currentElement.text();
        }
      }
      
    }
  }

  currentGroup = conjugElement.firstChildElement(KVTML_PLURAL);
  if (!currentGroup.isNull())
  {
    QDomElement currentElement = currentGroup.firstChildElement(KVTML_1STPERSON);
    if (!currentElement.isNull())
    {
      plurfirst = currentElement.text();
    }
    
    currentElement = currentGroup.firstChildElement(KVTML_2NDPERSON);
    if (!currentElement.isNull())
    {
      plursecond = currentElement.text();
    }
    
    currentGroup = currentGroup.firstChildElement(KVTML_3RDPERSON);
    if (!currentGroup.isNull())
    {
      currentElement = currentGroup.firstChildElement(KVTML_COMMON);
      if (!currentElement.isNull())
      {
        p3_common = true;
        plurthirdmale = currentElement.text();
        plurthirdfemale = singthirdmale;
        plurthirdneutral = singthirdmale;
      }
      else
      {
        p3_common = false;
        currentElement = currentGroup.firstChildElement(KVTML_MALE);
        if (!currentElement.isNull())
        {
          plurthirdmale = currentElement.text();
        }
        
        currentElement = currentGroup.firstChildElement(KVTML_FEMALE);
        if (!currentElement.isNull())
        {
          plurthirdfemale = currentElement.text();
        }
        
        currentElement = currentGroup.firstChildElement(KVTML_NEUTRAL);
        if (!currentElement.isNull())
        {
          plurthirdneutral = currentElement.text();
        }
      }
      
    }
  }
  
  curr_conjug.setPers3SingularCommon(type, s3_common);
  curr_conjug.setPers3PluralCommon(type, p3_common);
  curr_conjug.setPers1Singular(type, singfirst);
  curr_conjug.setPers2Singular(type, singsecond);
  curr_conjug.setPers3FemaleSingular(type, singthirdfemale);
  curr_conjug.setPers3MaleSingular(type, singthirdmale);
  curr_conjug.setPers3NaturalSingular(type, singthirdneutral);
  curr_conjug.setPers1Plural(type, plurfirst);
  curr_conjug.setPers2Plural(type, plursecond);
  curr_conjug.setPers3FemalePlural(type, plurthirdfemale);
  curr_conjug.setPers3MalePlural(type, plurthirdmale);
  curr_conjug.setPers3NaturalPlural(type, plurthirdneutral);

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
   <absolute>good</absolute>
   <comparative>better</comparative>
   <superlative>best</superlative>
 </comparison>
*/
{
  QDomElement currentElement;

  currentElement = domElementParent.firstChildElement(KVTML_ABSOLUTE);
  if (!currentElement.isNull()) {
    comp.setL1(currentElement.text());
  }

  currentElement = domElementParent.firstChildElement(KVTML_COMPARATIVE);
  if (!currentElement.isNull()) 
  {
    comp.setL2(currentElement.text());
  }

  currentElement = domElementParent.firstChildElement(KVTML_SUPERLATIVE);
  if (!currentElement.isNull()) 
  {
    comp.setL3(currentElement.text());
  }
  return true;
}


bool KEduVocKvtml2Reader::readMultipleChoice(QDomElement &multipleChoiceElement, KEduVocMultipleChoice &mc)
/*
 <multiplechoice>
   <choice>good</choice>
   <choice>better</choice>
   <choice>best</choice>
   <choice>best 2</choice>
   <choice>best 3</choice>
 </multiplechoice>
*/

{
  QDomElement currentElement;
  QString s;
  mc.clear();

  QDomNodeList choiceNodes = multipleChoiceElement.elementsByTagName(KVTML_CHOICE);
  for (int i = 0; i < choiceNodes.count(); ++i)
  {
    currentElement = choiceNodes.item(i).toElement();
    if (currentElement.parentNode() == multipleChoiceElement)
    {
      mc.appendChoice(currentElement.text());
    }
  }
  return true;
}

bool KEduVocKvtml2Reader::readGrade(QDomElement &gradeElement, KEduVocExpression &expr, int index)
{
  bool result = true;
  int id = gradeElement.attribute(KVTML_FROMID).toInt(&result);
  if (!result)
  {
    m_errorMessage = i18n("identifier missing id");
    return false;
  }
  
  QDomElement currentElement = gradeElement.firstChildElement(KVTML_CURRENTGRADE);
  if (!currentElement.isNull())
  {
    // TODO: find out how to use the current grade class for 
    // currentGradeInt and currentGradeFloat, or if this will always just have a float
  }
  
  currentElement = gradeElement.firstChildElement(KVTML_COUNT);
  if (!currentElement.isNull())
  {
    int value = currentElement.text().toInt();
    expr.translation(index).gradeFrom(id).setQueryCount(value);
  }
  
  currentElement = gradeElement.firstChildElement(KVTML_ERRORCOUNT);
  if (!currentElement.isNull())
  {
    int value = currentElement.text().toInt();
    expr.translation(index).gradeFrom(id).setBadCount(value);
  }
  
  currentElement = gradeElement.firstChildElement(KVTML_DATE);
  if (!currentElement.isNull())
  {
    QDateTime value = QDateTime::fromTime_t(currentElement.text().toUInt());
    expr.translation(index).gradeFrom(id).setQueryDate(value);
  }
}


//**** old code from kvtmlreader
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

//  faux_ami_f = "";
//  attribute = domElementExpressionChild.attributeNode(KV_FAUX_AMI_F);
//  if (!attribute.isNull())
//    faux_ami_f = attribute.value();

//  faux_ami_t = "";
//  attribute = domElementExpressionChild.attributeNode(KV_FAUX_AMI_T);
//  if (!attribute.isNull())
//    faux_ami_t = attribute.value();

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

#include "keduvockvtml2reader.moc"
