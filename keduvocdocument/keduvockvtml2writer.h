/***************************************************************************
                     export a KEduVocDocument to a KVTML file
    -----------------------------------------------------------------------
    copyright       : (C) 2007 Jeremy Whiting <jeremy@scitools.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCKVTML2WRITER_H
#define KEDUVOCKVTML2WRITER_H

#include <QtXml/QDomDocument>
#include <QList>
#include <QFile>

#include "keduvocgrammar.h"
#include "keduvocmultiplechoice.h"
#include "keduvoctranslation.h"

class KEduVocDocument;

class KEduVocKvtml2Writer
{
public:
  KEduVocKvtml2Writer(QFile *file);

  bool writeDoc(KEduVocDocument *doc, const QString &generator);
  
  /** write information entries
   * @param informationElement QDomElement <information> to write to
   * @param generator text describing generator
   */
  bool writeInformation(QDomElement &informationElement, const QString &generator);

  /** write identifiers group
   * @param identifiersElement QDomElement <identifiers> to write to
   */
  bool writeIdentifiers(QDomElement &identifiersElement);
  
  /** write article
   * @param articleElement QDomElement <article> to write to
   * @param article the article number to write
   */
  bool writeArticle(QDomElement &articleElement, int article);
  
  /** write conjugation
   * @param conjugationElement QDomElement <conjugation> or <personalpronouns> to write to
   * @param conjugation object to write
   * @param type conjugation type
   */
  bool writeConjugation(QDomElement &conjugationElement, const KEduVocConjugation &conjugation, 
                        const QString &type);

  /** write types
   * @param typesElement QDomElement <types> to write to
   */
  bool writeTypes(QDomElement &typesElement);
  
  /** write tenses
   * @param tensesElement QDomElement <tenses> to write to
   */
  bool writeTenses(QDomElement &tensesElement);
  
  /** write usages
   * @param usagesElement QDomElement <usages> to write to
   */
  bool writeUsages(QDomElement &usagesElement);

  /** write entries
   * @param entriesElement QDomElement <entries> to write to
   */
  bool writeEntries(QDomElement &entriesElement);
  
  /** write a translation
   * @param translationElement QDomElement <translation> to write to, with id pre-set
   * @param translation object to write
   */
  bool writeTranslation(QDomElement &translationElement, KEduVocTranslation &translation);
  
  /** write the lesson group
   * @param lessonsElement QDomElement <lessons> to write to
   */
  bool writeLessons(QDomElement &lessonsElement);
  
  bool writeConjugEntry(QDomDocument &domDoc, QDomElement &domElementParent, KEduVocConjugation &curr_conjug);
  
  /** write a comparison
   * @param comparisonElement QDomElement <comparison> to write to
   * @param comparison object to write
   * @returns success
   */
  bool writeComparison(QDomElement &comparisonElement, const KEduVocComparison &comparison);
  
  /** write multiple choice choices
   * @param multipleChoiceElement QDomElement <multiplechoice> to write to
   * @returns success
   */
  bool writeMultipleChoice(QDomElement &multipleChoiceElement, const KEduVocMultipleChoice &mc);

private:

  QDomElement newTextElement(const QString &elementName, const QString &text);
  
  QFile *m_outputFile;
  KEduVocDocument *m_doc;
  
  QDomDocument m_domDoc;
};

#endif
