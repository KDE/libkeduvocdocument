/***************************************************************************
                   export a KEduVocDocument to a KVTML file
    -----------------------------------------------------------------------
    copyright           : (C) 1999-2001 Ewald Arnold
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

#include <kdebug.h>
#include <QtXml/QDomDocument>
#include <QTextStream>

#include "keduvockvtmlwriter.h"
#include "keduvocdocument.h"

KEduVocKvtmlWriter::KEduVocKvtmlWriter(QFile *file)
{
  // the file must be already open
  m_outputFile = file;
}


bool KEduVocKvtmlWriter::writeDoc(KEduVocDocument *doc, const QString &generator)
{
  bool first_expr = true;

  m_doc = doc;

  QDomDocument domDoc("kvtml SYSTEM \"kvoctrain.dtd\"");
  domDoc.appendChild(domDoc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""));
  QDomElement domElementKvtml = domDoc.createElement("kvtml");
  domDoc.appendChild(domElementKvtml);

  domElementKvtml.setAttribute(KV_ENCODING, (QString) "UTF-8");
  domElementKvtml.setAttribute(KV_GENERATOR, generator);
  domElementKvtml.setAttribute(KV_COLS, m_doc->identifierCount());
  domElementKvtml.setAttribute(KV_LINES, m_doc->numEntries());

  if (!m_doc->title().isEmpty())
    domElementKvtml.setAttribute(KV_TITLE, m_doc->title());

  if (!m_doc->author().isEmpty())
    domElementKvtml.setAttribute(KV_AUTHOR, m_doc->author());

  if (!m_doc->license().isEmpty())
    domElementKvtml.setAttribute(KV_LICENSE, m_doc->license());

  if (!m_doc->docRemark().isEmpty())
    domElementKvtml.setAttribute(KV_DOC_REM, m_doc->docRemark());

  if (!writeLesson(domDoc, domElementKvtml))
    return false;

  if (!writeArticle(domDoc, domElementKvtml))
    return false;

  QList<KEduVocConjugation> conjugations;
  for (int i = 0; i < m_doc->conjugationCount(); i++)
    conjugations.append(m_doc->conjugation(i));
  if (conjugations.count() > 0) {
    if (!writeConjugHeader(domDoc, domElementKvtml, conjugations))
      return false;
  }

  if (!writeOption(domDoc, domElementKvtml))
    return false;

  if (!writeType(domDoc, domElementKvtml))
    return false;

  if (!writeTense(domDoc, domElementKvtml))
    return false;

  if (!writeUsage(domDoc, domElementKvtml))
    return false;

  QString q_org, q_trans;
  m_doc->queryIdentifier(q_org, q_trans);

  int entryCount = m_doc->numEntries();

  for (int i = 0; i < entryCount; i++)
  {
    KEduVocExpression *entry = m_doc->entry(i);
    QDomElement domElementExpression = domDoc.createElement(KV_EXPR);

    if (entry->lesson() != 0)
    {
      int lm = entry->lesson();
      if (lm > m_doc->lessonDescriptions().count())
      {
        // should not be
        kError() << "index of lesson member too high: " << lm << endl;
        lm = 0;
      }
      domElementExpression.setAttribute(KV_LESS_MEMBER, lm);
    }

    if (entry->isInQuery())
      domElementExpression.setAttribute(KV_SELECTED, 1);

    if (!entry->isActive())
      domElementExpression.setAttribute (KV_INACTIVE, 1);

    if (entry->uniqueType() && !entry->type(0).isEmpty())
      domElementExpression.setAttribute (KV_EXPRTYPE, entry->type(0));

    QDomElement domElementOriginal = domDoc.createElement(KV_ORG);
    if (first_expr)
    {
      // save space, only tell language in first entry
      QString s;
      domElementOriginal.setAttribute(KV_SIZEHINT, m_doc->sizeHint(0));

      s = m_doc->originalIdentifier().simplified();
      if (s.isEmpty() )
        s = "original";
      domElementOriginal.setAttribute(KV_LANG, s);
      if (s == q_org)
        domElementOriginal.setAttribute(KV_QUERY, (QString) KV_O);
      else if (s == q_trans)
        domElementOriginal.setAttribute(KV_QUERY, (QString) KV_T);
    }

    if (!entry->remark(0).isEmpty() )
      domElementOriginal.setAttribute(KV_REMARK, entry->remark(0));

    if (!entry->synonym(0).isEmpty() )
      domElementOriginal.setAttribute(KV_SYNONYM, entry->synonym(0));

    if (!entry->example(0).isEmpty() )
      domElementOriginal.setAttribute(KV_EXAMPLE, entry->example(0));

    if (!entry->usageLabel(0).isEmpty() )
      domElementOriginal.setAttribute(KV_USAGE, entry->usageLabel(0));

    if (!entry->paraphrase(0).isEmpty() )
      domElementOriginal.setAttribute(KV_PARAPHRASE, entry->paraphrase(0));

    if (!entry->antonym(0).isEmpty() )
      domElementOriginal.setAttribute(KV_ANTONYM, entry->antonym(0));

    if (!entry->pronunciation(0).isEmpty() )
      domElementOriginal.setAttribute(KV_PRONUNCE, entry->pronunciation(0));

    if (!entry->uniqueType() && !entry->type(0).isEmpty())
      domElementOriginal.setAttribute(KV_EXPRTYPE, entry->type(0));

    if (!writeMultipleChoice(domDoc, domElementOriginal, entry->multipleChoice(0)))
      return false;

    QString s;
    QString entype = s = entry->type(0);
    int pos = s.indexOf(QM_TYPE_DIV);
    if (pos >= 0)
      entype = s.left(pos);
    else
      entype = s;

    if (entype == QM_VERB && entry->conjugation(0).numEntries() > 0)
    {
      KEduVocConjugation conj = entry->conjugation(0);
      if (!writeConjugEntry(domDoc, domElementOriginal, conj))
        return false;
    }
    else if (entype == QM_ADJ && !entry->comparison(0).isEmpty())
    {
      KEduVocComparison comp = entry->comparison(0);
      if (!writeComparison(domDoc, domElementOriginal, comp))
        return false;
    }

    QDomText domTextOriginal = domDoc.createTextNode(entry->original());
    domElementOriginal.appendChild(domTextOriginal);
    domElementExpression.appendChild(domElementOriginal);

    int trans = 1;
    while (trans < m_doc->identifierCount())
    {
      QDomElement domElementTranslation = domDoc.createElement(KV_TRANS);
      if (first_expr)
      {
        // save space, only tell language in first entry
        QString s;
        domElementTranslation.setAttribute(KV_SIZEHINT, m_doc->sizeHint(trans));

        s = m_doc->identifier(trans).simplified();
        if (s.isEmpty() )
        {
          s.setNum(trans);
          s.prepend("translation ");
        }
        domElementTranslation.setAttribute(KV_LANG, s);
        if (s == q_org)
          domElementTranslation.setAttribute(KV_QUERY, (QString) KV_O);
        else if (s == q_trans)
          domElementTranslation.setAttribute(KV_QUERY, (QString) KV_T);
      }

      if (entry->grade(trans, false) != 0 || entry->grade(trans, true) != 0)
        domElementTranslation.setAttribute(KV_GRADE, entry->gradeStr(trans, false) + ';' + entry->gradeStr(trans, true));

      if (entry->queryCount(trans, false) != 0 || entry->queryCount(trans, true) != 0)
        domElementTranslation.setAttribute(KV_COUNT, QString::number(entry->queryCount(trans, false)) + ';' + QString::number(entry->queryCount(trans, true)));

      if (entry->badCount(trans, false) != 0 || entry->badCount(trans, true) != 0)
        domElementTranslation.setAttribute(KV_BAD, QString::number(entry->badCount(trans, false)) + ';' + QString::number(entry->badCount(trans, true)));

      if (entry->queryDate(trans, false).toTime_t() != 0 || entry->queryDate(trans, true).toTime_t() != 0)
        domElementTranslation.setAttribute(KV_DATE, QString::number(entry->queryDate(trans, false).toTime_t()) + ';' + QString::number(entry->queryDate(trans, true).toTime_t()));

      if (!entry->remark(trans).isEmpty() )
        domElementTranslation.setAttribute(KV_REMARK, entry->remark(trans));

      if (!entry->fauxAmi(trans, false).isEmpty() )
        domElementTranslation.setAttribute(KV_FAUX_AMI_F, entry->fauxAmi(trans, false));

      if (!entry->fauxAmi(trans, true).isEmpty() )
        domElementTranslation.setAttribute(KV_FAUX_AMI_T, entry->fauxAmi(trans, true));

      if (!entry->synonym(trans).isEmpty() )
        domElementTranslation.setAttribute(KV_SYNONYM, entry->synonym(trans));

      if (!entry->example(trans).isEmpty() )
        domElementTranslation.setAttribute(KV_EXAMPLE, entry->example(trans));

      if (!entry->usageLabel(trans).isEmpty() )
        domElementTranslation.setAttribute(KV_USAGE, entry->usageLabel(trans));

      if (!entry->paraphrase(trans).isEmpty() )
        domElementTranslation.setAttribute(KV_PARAPHRASE, entry->paraphrase(trans));

      if (!entry->antonym(trans).isEmpty() )
        domElementTranslation.setAttribute(KV_ANTONYM, entry->antonym(trans));

      if (!entry->pronunciation(trans).isEmpty() )
        domElementTranslation.setAttribute(KV_PRONUNCE, entry->pronunciation(trans));

      if (!entry->uniqueType() && !entry->type(trans).isEmpty())
        domElementTranslation.setAttribute(KV_EXPRTYPE, entry->type(trans));

      if (!writeMultipleChoice(domDoc, domElementTranslation, entry->multipleChoice(trans)))
        return false;

      QString s;
      QString entype = s = entry->type(0);
      int pos = s.indexOf(QM_TYPE_DIV);
      if (pos >= 0)
        entype = s.left (pos);
      else
        entype = s;

      if (entype == QM_VERB && entry->conjugation(trans).numEntries() > 0)
      {
        KEduVocConjugation conj = entry->conjugation(trans);
        if (!writeConjugEntry(domDoc, domElementTranslation, conj))
          return false;
      }

      if (entype == QM_ADJ && !entry->comparison(trans).isEmpty())
      {
        KEduVocComparison comp = entry->comparison(trans);
        if (!writeComparison(domDoc, domElementTranslation, comp))
          return false;
      }

      QDomText domTextTranslation = domDoc.createTextNode(entry->translation(trans));
      domElementTranslation.appendChild(domTextTranslation);
      domElementExpression.appendChild(domElementTranslation);

      trans++;
    }

    domElementKvtml.appendChild(domElementExpression);

    first_expr = false;
  }

  domDoc.appendChild(domElementKvtml);

  QTextStream ts(m_outputFile);
  domDoc.save(ts, 2);

  return true;
}


bool KEduVocKvtmlWriter::writeLesson(QDomDocument &domDoc, QDomElement &domElementParent)
{
  if (m_doc->lessonDescriptions().count() == 0)
    return true;

  QDomElement domElementLesson = domDoc.createElement(KV_LESS_GRP);
  domElementLesson.setAttribute(KV_SIZEHINT, m_doc->sizeHint(-1));
  int count = 1;

  foreach(QString lesson, m_doc->lessonDescriptions())
  {
    if (!lesson.isNull())
    {
      QDomElement domElementDesc = domDoc.createElement(KV_LESS_DESC);
      QDomText domTextDesc = domDoc.createTextNode(lesson);

      domElementDesc.setAttribute(KV_LESS_NO, count);
      if (m_doc->currentLesson() == count)
        domElementDesc.setAttribute(KV_LESS_CURR, 1);
      bool inQuery = false;
      ///@todo check that this is working
      foreach(int liq, m_doc->lessonsInQuery())
        if (liq == count - 1)
          inQuery = true;

      if (count - 1 < m_doc->lessonsInQuery().count() && inQuery)
        domElementDesc.setAttribute(KV_LESS_QUERY, 1);

      domElementDesc.appendChild(domTextDesc);
      domElementLesson.appendChild(domElementDesc);
      count++;
    }
  }

  domElementParent.appendChild(domElementLesson);
  return true;
}


bool KEduVocKvtmlWriter::writeArticle(QDomDocument &domDoc, QDomElement &domElementParent)
/*
 <article>
  <e l="de">    lang determines also lang order in entries !!
   <fi>eine</fi>  which must NOT differ
   <fd>die</fd>
   <mi>ein</mi>
   <md>der</md>
   <ni>ein</ni>
   <nd>das</nd>
  </e>
 </article>
*/
{
  if (m_doc->articleCount() == 0)
    return true;

  QDomElement domElementArticle = domDoc.createElement(KV_ARTICLE_GRP);
  QString def;
  QString indef;
  QString s;

  for (int lfn = 0; lfn < qMin(m_doc->articleCount(), m_doc->identifierCount()); lfn++)
  {
    QDomElement domElementEntry = domDoc.createElement(KV_ART_ENTRY);
    if (lfn == 0)
    {
      s = m_doc->originalIdentifier().simplified();
      if (s.isEmpty())
        s = "original";
    }
    else
    {
      s = m_doc->identifier(lfn).simplified();
      if (s.isEmpty())
      {
        s.setNum(lfn);
        s.prepend("translation ");
      }
    }
    domElementEntry.setAttribute(KV_LANG, s);

    m_doc->article(lfn).female(def, indef);
    if (!def.isEmpty())
    {
      QDomElement domElementFD = domDoc.createElement(KV_ART_FD);
      QDomText domTextFD = domDoc.createTextNode(def);

      domElementFD.appendChild(domTextFD);
      domElementEntry.appendChild(domElementFD);
    }
    if (!indef.isEmpty())
    {
      QDomElement domElementFI = domDoc.createElement(KV_ART_FI);
      QDomText domTextFI = domDoc.createTextNode(indef);

      domElementFI.appendChild(domTextFI);
      domElementEntry.appendChild(domElementFI);
    }

    m_doc->article(lfn).male(def, indef);
    if (!def.isEmpty())
    {
      QDomElement domElementMD = domDoc.createElement(KV_ART_MD);
      QDomText domTextMD = domDoc.createTextNode(def);

      domElementMD.appendChild(domTextMD);
      domElementEntry.appendChild(domElementMD);
    }
    if (!indef.isEmpty())
    {
      QDomElement domElementMI = domDoc.createElement(KV_ART_MI);
      QDomText domTextMI = domDoc.createTextNode(indef);

      domElementMI.appendChild(domTextMI);
      domElementEntry.appendChild(domElementMI);
    }

    m_doc->article(lfn).natural(def, indef);
    if (!def.isEmpty())
    {
      QDomElement domElementND = domDoc.createElement(KV_ART_ND);
      QDomText domTextND = domDoc.createTextNode(def);

      domElementND.appendChild(domTextND);
      domElementEntry.appendChild(domElementND);
    }
    if (!indef.isEmpty())
    {
      QDomElement domElementNI = domDoc.createElement(KV_ART_NI);
      QDomText domTextNI = domDoc.createTextNode(indef);

      domElementNI.appendChild(domTextNI);
      domElementEntry.appendChild(domElementNI);
    }

    domElementArticle.appendChild(domElementEntry);
  }

  domElementParent.appendChild(domElementArticle);
  return true;
}


bool KEduVocKvtmlWriter::writeOption(QDomDocument &domDoc, QDomElement &domElementParent)
{
  QDomElement domElementOption = domDoc.createElement(KV_OPTION_GRP);
  QDomElement domElementSort = domDoc.createElement(KV_OPT_SORT);

  domElementSort.setAttribute(KV_BOOL_FLAG, (m_doc->isSortingEnabled()?1:0));
  domElementOption.appendChild(domElementSort);

  domElementParent.appendChild(domElementOption);
  return true;
}


bool KEduVocKvtmlWriter::writeType(QDomDocument &domDoc, QDomElement &domElementParent)
{
  if (m_doc->typeDescriptions().count() == 0)
    return true;

  QDomElement domElementType = domDoc.createElement(KV_TYPE_GRP);
  int count = 1;

  foreach(QString type, m_doc->typeDescriptions())
  {
    if (!(type.isNull()) )
    {
      QDomElement domElementDesc = domDoc.createElement(KV_TYPE_DESC);
      QDomText domTextDesc = domDoc.createTextNode(type);

      domElementDesc.setAttribute(KV_TYPE_NO, count);
      domElementDesc.appendChild(domTextDesc);
      domElementType.appendChild(domElementDesc);
      count++;
    }
  }

  domElementParent.appendChild(domElementType);
  return true;
}


bool KEduVocKvtmlWriter::writeTense(QDomDocument &domDoc, QDomElement &domElementParent)
{
  if (m_doc->tenseDescriptions().count() == 0)
    return true;

  QDomElement domElementTense = domDoc.createElement(KV_TENSE_GRP);
  int count = 1;

  foreach(QString tense, m_doc->tenseDescriptions())
  {
    if (!(tense.isNull())) {
      QDomElement domElementDesc = domDoc.createElement(KV_TENSE_DESC);
      QDomText domTextDesc = domDoc.createTextNode(tense);

      domElementDesc.setAttribute(KV_TENSE_NO, count);
      domElementDesc.appendChild(domTextDesc);
      domElementTense.appendChild(domElementDesc);
      count++;
    }
  }

  domElementParent.appendChild(domElementTense);
  return true;
}


bool KEduVocKvtmlWriter::writeUsage(QDomDocument &domDoc, QDomElement &domElementParent)
{
  if (m_doc->usageDescriptions().count() == 0)
    return true;

  QDomElement domElementUsage = domDoc.createElement(KV_USAGE_GRP);
  int count = 1;

  foreach(QString usage, m_doc->usageDescriptions())
  {
    if (!(usage.isNull()))
    {
      QDomElement domElementDesc = domDoc.createElement(KV_USAGE_DESC);
      QDomText domTextDesc = domDoc.createTextNode(usage);

      domElementDesc.setAttribute(KV_USAGE_NO, count);
      domElementDesc.appendChild(domTextDesc);
      domElementUsage.appendChild(domElementDesc);
      count++;
    }
  }

  domElementParent.appendChild(domElementUsage);
  return true;
}


bool KEduVocKvtmlWriter::writeComparison(QDomDocument &domDoc, QDomElement &domElementParent, const KEduVocComparison &comp)
/*
 <comparison>
   <l1>good</l1>
   <l2>better</l2>
   <l3>best</l3>
 </comparison>
*/
{
  if (comp.isEmpty())
    return true;

  QDomElement domElementComparison = domDoc.createElement(KV_COMPARISON_GRP);

  if (!comp.l1().isEmpty() )
  {
    QDomElement domElementL1 = domDoc.createElement(KV_COMP_L1);
    QDomText domTextL1 = domDoc.createTextNode(comp.l1());

    domElementL1.appendChild(domTextL1);
    domElementComparison.appendChild(domElementL1);
  }

  if (!comp.l2().isEmpty() )
  {
    QDomElement domElementL2 = domDoc.createElement(KV_COMP_L2);
    QDomText domTextL2 = domDoc.createTextNode(comp.l2());

    domElementL2.appendChild(domTextL2);
    domElementComparison.appendChild(domElementL2);
  }

  if (!comp.l3().isEmpty() )
  {
    QDomElement domElementL3 = domDoc.createElement(KV_COMP_L3);
    QDomText domTextL3 = domDoc.createTextNode(comp.l3());

    domElementL3.appendChild(domTextL3);
    domElementComparison.appendChild(domElementL3);
  }

  domElementParent.appendChild(domElementComparison);
  return true;
}


bool KEduVocKvtmlWriter::writeMultipleChoice(QDomDocument &domDoc, QDomElement &domElementParent, const KEduVocMultipleChoice &mc)
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
  if (mc.isEmpty())
    return true;

  QDomElement domElementMC = domDoc.createElement(KV_MULTIPLECHOICE_GRP);

  if (!mc.mc1().isEmpty() )
  {
    QDomElement domElementMC1 = domDoc.createElement(KV_MC_1);
    QDomText domTextMC1 = domDoc.createTextNode(mc.mc1());

    domElementMC1.appendChild(domTextMC1);
    domElementMC.appendChild(domElementMC1);
  }

  if (!mc.mc2().isEmpty() )
  {
    QDomElement domElementMC2 = domDoc.createElement(KV_MC_2);
    QDomText domTextMC2 = domDoc.createTextNode(mc.mc2());

    domElementMC2.appendChild(domTextMC2);
    domElementMC.appendChild(domElementMC2);
  }

  if (!mc.mc3().isEmpty() )
  {
    QDomElement domElementMC3 = domDoc.createElement(KV_MC_3);
    QDomText domTextMC3 = domDoc.createTextNode(mc.mc3());

    domElementMC3.appendChild(domTextMC3);
    domElementMC.appendChild(domElementMC3);
  }

  if (!mc.mc4().isEmpty() )
  {
    QDomElement domElementMC4 = domDoc.createElement(KV_MC_4);
    QDomText domTextMC4 = domDoc.createTextNode(mc.mc4());

    domElementMC4.appendChild(domTextMC4);
    domElementMC.appendChild(domElementMC4);
  }

  if (!mc.mc5().isEmpty() )
  {
    QDomElement domElementMC5 = domDoc.createElement(KV_MC_5);
    QDomText domTextMC5 = domDoc.createTextNode(mc.mc5());

    domElementMC5.appendChild(domTextMC5);
    domElementMC.appendChild(domElementMC5);
  }

  domElementParent.appendChild(domElementMC);
  return true;
}


bool KEduVocKvtmlWriter::writeConjugHeader(QDomDocument &domDoc, QDomElement &domElementParent, QList<KEduVocConjugation> &curr_conjug)
{
/*
 <conjugation>    used in header for definiton of "prefix"
  <e l="de">      lang determines also lang order in entries !!
   <s1>I</s1>     which must NOT differ
   <s2>you<2>
   <s3f common="0">he</s3f>
   <s3m>she</s3m>
   <s3n>it</s3n>
   <p1>we</p1>
   <p2>you</p2>
   <p3f common="1">they</p3f>
   <p3m>they</p3m>
   <p3n>they</p3n>
  </e>
 </conjugation>

*/
  if (curr_conjug.size() == 0)
    return true;

  QDomElement domElementConjug = domDoc.createElement(KV_CONJUG_GRP);
  QString s;

  for (int ent = 0; ent < qMin(curr_conjug.count(), m_doc->identifierCount()); ent++)
  {
    QDomElement domElementEntry = domDoc.createElement(KV_CON_ENTRY);

    if (ent == 0)
    {
      s = m_doc->originalIdentifier().simplified();
      if (s.isEmpty())
        s = "original";
    }
    else
    {
      s = m_doc->identifier(ent).simplified();
      if (s.isEmpty())
      {
        s.setNum(ent);
        s.prepend("translation ");
      }
    }
    domElementEntry.setAttribute(KV_LANG, s);

    if (!writeConjug(domDoc, domElementEntry, curr_conjug[ent], CONJ_PREFIX))
      return false;

    domElementConjug.appendChild(domElementEntry);
  }

  domElementParent.appendChild(domElementConjug);
  return true;
}


bool KEduVocKvtmlWriter::writeConjugEntry(QDomDocument &domDoc, QDomElement &domElementParent, KEduVocConjugation &curr_conjug)
/*
 <conjugation>    in entry for definition of tenses of (irreg.) verbs
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
  curr_conjug.cleanUp();
  if (curr_conjug.numEntries() == 0 )
    return true;

  QDomElement domElementConjug = domDoc.createElement(KV_CONJUG_GRP);
  QString type;

  for (int lfn = 0; lfn < curr_conjug.numEntries(); lfn++)
  {
    QDomElement domElementType = domDoc.createElement(KV_CON_TYPE);

    type = curr_conjug.getType(lfn);
    domElementType.setAttribute(KV_CON_NAME, type);

    if (!writeConjug(domDoc, domElementType, curr_conjug, curr_conjug.getType(lfn)))
      return false;

    domElementConjug.appendChild(domElementType);
  }

  domElementParent.appendChild(domElementConjug);
  return true;
}


bool KEduVocKvtmlWriter::writeConjug(QDomDocument &domDoc, QDomElement &domElementParent, const KEduVocConjugation &curr_conjug, const QString &type)
{
  if (!curr_conjug.pers1Singular(type).isEmpty())
  {
    QDomElement domElementP1s = domDoc.createElement(KV_CON_P1S);
    QDomText domTextP1s = domDoc.createTextNode(curr_conjug.pers1Singular(type));

    domElementP1s.appendChild(domTextP1s);
    domElementParent.appendChild(domElementP1s);
  }

  if (!curr_conjug.pers2Singular(type).isEmpty())
  {
    QDomElement domElementP2s = domDoc.createElement(KV_CON_P2S);
    QDomText domTextP2s = domDoc.createTextNode(curr_conjug.pers2Singular(type));

    domElementP2s.appendChild(domTextP2s);
    domElementParent.appendChild(domElementP2s);
  }

  if (!curr_conjug.pers3FemaleSingular(type).isEmpty() || curr_conjug.pers3SingularCommon(type))
  {
    QDomElement domElementP3sf = domDoc.createElement(KV_CON_P3SF);
    QDomText domTextP3sf = domDoc.createTextNode(curr_conjug.pers3FemaleSingular(type));

    if (curr_conjug.pers3SingularCommon(type))
      domElementP3sf.setAttribute(KV_CONJ_COMMON, 1);

    domElementP3sf.appendChild(domTextP3sf);
    domElementParent.appendChild(domElementP3sf);
  }

  if (!curr_conjug.pers3MaleSingular(type).isEmpty())
  {
    QDomElement domElementP3sm = domDoc.createElement(KV_CON_P3SM);
    QDomText domTextP3sm = domDoc.createTextNode(curr_conjug.pers3MaleSingular(type));

    domElementP3sm.appendChild(domTextP3sm);
    domElementParent.appendChild(domElementP3sm);
  }

  if (!curr_conjug.pers3NaturalSingular(type).isEmpty())
  {
    QDomElement domElementP3sn = domDoc.createElement(KV_CON_P3SN);
    QDomText domTextP3sn = domDoc.createTextNode(curr_conjug.pers3NaturalSingular(type));

    domElementP3sn.appendChild(domTextP3sn);
    domElementParent.appendChild(domElementP3sn);
  }

  if (!curr_conjug.pers1Plural(type).isEmpty())
  {
    QDomElement domElementP1p = domDoc.createElement(KV_CON_P1P);
    QDomText domTextP1p = domDoc.createTextNode(curr_conjug.pers1Plural(type));

    domElementP1p.appendChild(domTextP1p);
    domElementParent.appendChild(domElementP1p);
  }

  if (!curr_conjug.pers2Plural(type).isEmpty())
  {
    QDomElement domElementP2p = domDoc.createElement(KV_CON_P2P);
    QDomText domTextP2p = domDoc.createTextNode(curr_conjug.pers2Plural(type));

    domElementP2p.appendChild(domTextP2p);
    domElementParent.appendChild(domElementP2p);
  }

  if (!curr_conjug.pers3FemalePlural(type).isEmpty() || curr_conjug.pers3PluralCommon(type))
  {
    QDomElement domElementP3pf = domDoc.createElement(KV_CON_P3PF);
    QDomText domTextP3pf = domDoc.createTextNode(curr_conjug.pers3FemalePlural(type));

    if (curr_conjug.pers3PluralCommon(type))
      domElementP3pf.setAttribute(KV_CONJ_COMMON, 1);

    domElementP3pf.appendChild(domTextP3pf);
    domElementParent.appendChild(domElementP3pf);
  }

  if (!curr_conjug.pers3MalePlural(type).isEmpty())
  {
    QDomElement domElementP3pm = domDoc.createElement(KV_CON_P3PM);
    QDomText domTextP3pm = domDoc.createTextNode(curr_conjug.pers3MalePlural(type));

    domElementP3pm.appendChild(domTextP3pm);
    domElementParent.appendChild(domElementP3pm);
  }

  if (!curr_conjug.pers3NaturalPlural(type).isEmpty())
  {
    QDomElement domElementP3pn = domDoc.createElement(KV_CON_P3PN);
    QDomText domTextP3pn = domDoc.createTextNode(curr_conjug.pers3NaturalPlural(type));

    domElementP3pn.appendChild(domTextP3pn);
    domElementParent.appendChild(domElementP3pn);
  }

  return true;
}
