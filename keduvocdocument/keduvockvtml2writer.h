/***************************************************************************
                     export a KEduVocDocument to a KVTML file
    -----------------------------------------------------------------------
    copyright       : (C) 2007 Jeremy Whiting <jeremy@scitools.com>
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

#ifndef KEDUVOCKVTML2WRITER_H
#define KEDUVOCKVTML2WRITER_H

#include <QtXml/QDomDocument>
#include <QtCore/QList>
#include <QtCore/QFile>

#include "keduvocgrammar.h"
#include "keduvocmultiplechoice.h"
#include "keduvoctranslation.h"
#include "keduvocpersonalpronoun.h"

class KEduVocDocument;
class KEduVocExpression;
class KEduVocLesson;

/**
* @brief Class to write kvtml2 data files from KEduVocDocument
* @author Jeremy Whiting
*/
class KEduVocKvtml2Writer
{
public:
    KEduVocKvtml2Writer( QFile *file );

    bool writeDoc( KEduVocDocument *doc, const QString &generator );

    /** write information entries
     * @param informationElement QDomElement information to write to
     * @param generator text describing generator
     */
    bool writeInformation( QDomElement &informationElement, const QString &generator );

    /** write identifiers group
     * @param identifiersElement QDomElement identifiers to write to
     */
    bool writeIdentifiers( QDomElement &identifiersElement );

    /** write article
     * @param articleElement QDomElement article to write to
     * @param article the article number to write
     */
    bool writeArticle( QDomElement &articleElement, int article );

    /** write conjugation
     * @param conjugationElement QDomElement conjugation or personalpronouns to write to
     * @param conjugation object to write
     * @param type conjugation type
     */
    bool writeConjugation( QDomElement &conjugationElement, const KEduVocConjugation &conjugation,
                           const QString &tense );

    bool writePersonalPronoun( QDomElement &pronounElement, const KEduVocPersonalPronoun &pronoun);
    /** write types
     * @param typesElement QDomElement types to write to
     */
    bool writeWordTypes( QDomElement &typesElement, KEduVocLesson* parentContainer );

    /** write tenses
     * @param tensesElement QDomElement tenses to write to
     */
    bool writeTenses( QDomElement &tensesElement );

    /** write usages
     * @param usagesElement QDomElement usages to write to
     */
    bool writeUsages( QDomElement &usagesElement );

    /** write entries
     * @param entriesElement QDomElement entries to write to
     */
    bool writeEntries( QDomElement &entriesElement );

    /** write a translation
     * @param translationElement QDomElement translation to write to, with id pre-set
     * @param translation object to write
     */
    bool writeTranslation( QDomElement &translationElement, KEduVocTranslation* translation );

    /** write the lesson group
     * @param parentLesson the parent lesson of the current lesson
     * @param lessonsElement QDomElement lessons to write to
     */
    bool writeLessons( KEduVocLesson *parentLesson, QDomElement &lessonsElement );

    /** write a comparison
     * @param comparisonElement QDomElement comparison to write to
     * @param comparison object to write
     * @returns success
     */
    bool writeComparison( QDomElement &comparisonElement, const KEduVocComparison &comparison );

    /** write multiple choice choices
     * @param multipleChoiceElement QDomElement multiplechoice to write to
     * @returns success
     */
    bool writeMultipleChoice( QDomElement &multipleChoiceElement, KEduVocTranslation* translation );

private:
    QDomElement newTextElement( const QString &elementName, const QString &text );

    QFile *m_outputFile;
    KEduVocDocument *m_doc;

    ///@todo which of the following is in use?
    QList<KEduVocExpression*>  m_allEntries;
    QDomElement m_wordTypeElement;

    QDomDocument m_domDoc;
};

#endif
