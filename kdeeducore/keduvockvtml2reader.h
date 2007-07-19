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

/**
* @brief class to read kvtml2 data files into keduvocdocument
* @author Jeremy Whiting
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
  
  /** read an identifiers articles
   * @param articleElement QDomElement for the article group
   * @param identifierNum number of the identifier this article is inside of
   */
  bool readArticle(QDomElement &articleElement, int identifierNum);
  
  /** read in a conjugation
   * @param conjugElement QDomElement for the conjugation group
   * @param curr_conjug conjugation object to populate
   */
  bool readConjug(QDomElement &conjugElement, KEduVocConjugation &curr_conjug);

  /** read the types
   * @param typesElement QDomElement for the types group
   */
  bool readTypes(QDomElement &typesElement);
  
  /** read the tenses
   * @param tensesElement QDomElement for the tenses group
   */
  bool readTenses(QDomElement &tensesElement);
  
  /** read the usages
   * @param usagesElement QDomElement for the usages group
   */
  bool readUsages(QDomElement &usagesElement);

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
