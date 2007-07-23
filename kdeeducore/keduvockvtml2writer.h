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


  bool writeLesson(QDomDocument &domDoc, QDomElement &domElementParent);
  bool writeType(QDomDocument &domDoc, QDomElement &domElementParent);
  bool writeTense(QDomDocument &domDoc, QDomElement &domElementParent);
  bool writeUsage(QDomDocument &domDoc, QDomElement &domElementParent);
  bool writeOption(QDomDocument &domDoc, QDomElement &domElementParent);
  bool writeArticle (QDomDocument &domDoc, QDomElement &domElementParent);
  bool writeConjugHeader(QDomDocument &domDoc, QDomElement &domElementParent, QList<KEduVocConjugation> &curr_conjug);
  bool writeConjug(QDomDocument &domDoc, QDomElement &domElementParent, const KEduVocConjugation &curr_conjug, const QString &type);
  bool writeConjugEntry(QDomDocument &domDoc, QDomElement &domElementParent, KEduVocConjugation &curr_conjug);
  bool writeComparison(QDomDocument &domDoc, QDomElement &domElementParent, const KEduVocComparison &comp);
  bool writeMultipleChoice(QDomDocument &domDoc, QDomElement &domElementParent, const KEduVocMultipleChoice &mc);

private:
  QFile *m_outputFile;
  KEduVocDocument *m_doc;
  
  QDomDocument m_domDoc;
};

#endif
