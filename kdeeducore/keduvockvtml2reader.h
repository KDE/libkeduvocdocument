/***************************************************************************
                     read a KEduVocDocument from a KVTML2 file
    -----------------------------------------------------------------------
    copyright      : (C) 2007 Jeremy Whiting <jeremy@scitools.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCKVTML2READER_H
#define KEDUVOCKVTML2READER_H

#include <QtXml/QDomDocument>
#include <QList>

#include "keduvocexpression.h"
#include "keduvocgrammar.h"
#include "keduvocmultiplechoice.h"

class QIODevice;
class KEduVocDocument;

// internal types, indented are subtypes

#define QM_VERB           "v"    // go
#define   QM_VERB_IRR     "ir"
#define   QM_VERB_REG     "re"
#define QM_NOUN           "n"    // table, coffee
#define   QM_NOUN_F       "f"
#define   QM_NOUN_M       "m"
#define   QM_NOUN_S       "s"
#define QM_NAME           "nm"
#define QM_ART            "ar"   // article
#define   QM_ART_DEF      "def"  // definite    a/an
#define   QM_ART_IND      "ind"  // indefinite  the
#define QM_ADJ            "aj"   // adjective   expensive, good
#define QM_ADV            "av"   // adverb      today, strongly
#define QM_PRON           "pr"   // pronoun     you, she
#define   QM_PRON_POS     "pos"  // possessive  my, your
#define   QM_PRON_PER     "per"  // personal
#define QM_PHRASE         "ph"
#define QM_NUM            "num"  // numeral
#define   QM_NUM_ORD      "ord"  // ordinal     first, second
#define   QM_NUM_CARD     "crd"  // cardinal    one, two
#define QM_INFORMAL       "ifm"
#define QM_FIG            "fig"
#define QM_CON            "con"  // conjuncton  and, but
#define QM_PREP           "pre"  // preposition behind, between
#define QM_QUEST          "qu"   // question    who, what

// type delimiters

#define QM_USER_TYPE  "#"   // designates number of user type
#define QM_TYPE_DIV   ":"   // divide main from subtype

// usage delimiters (also declared in UsageManager.h)

#define UL_USER_USAGE  "#"   // designates number of user type

/**
* @brief class to read kvtml2 data files into keduvocdocument
@author Jeremy Whiting
*/
class KEduVocKvtml2Reader : public QObject
{
  Q_OBJECT
public:
  /** default constructor 
   * @param file file to read from
   */
  KEduVocKvtml2Reader(QIODevice *file);

  /** read the document
   * @param doc document object to store the data in
   */
  bool readDoc(KEduVocDocument *doc);
  
  /** read information entries
   * @param informationElement QDomElement <information>
   */
  bool readInformation(QDomElement &informationElement);

  /** read group elements: identifiers, entries, types, usages, lessons */
  bool readGroups(QDomElement &domElementParent);

  /** read an identifier
   * @param identifierElement QDomElement for the identifier to read
   */
  bool readIdentifier(QDomElement &identifierElement);
  
  bool readArticle(QDomElement &articleElement);
  bool readConjug(QDomElement &conjugElement, QList<KEduVocConjugation> &curr_conjug);

  bool readType(QDomElement &typeElement);
  bool readTense(QDomElement &tenseElement);
  bool readUsage(QDomElement &usageElement);

  /** read an entry
   * @param entryElement QDomElement for the entry to read
   */
  bool readEntry(QDomElement &entryElement);

  /** read a translation
   * @param translationElement QDomElement for the translation to read
   */
  bool readTranslation(QDomElement &translationElement, KEduVocExpression &expr, int index);

  bool readComparison(QDomElement &comparisonElement, KEduVocComparison &comp);
  bool readMultipleChoice(QDomElement &multipleChoiceElement, KEduVocMultipleChoice &mc);
  bool readGrade(QDomElement &gradeElement);

  bool readLesson(QDomElement &lessonElement);

  QString errorMessage() const { return m_errorMessage; }

private:

  /** pre-opened QIODevice to read from */
  QIODevice *m_inputFile;
  
  /** KEduVocDocument to read to */
  KEduVocDocument *m_doc;
  
  /** error message */
  QString m_errorMessage;
};

#endif
