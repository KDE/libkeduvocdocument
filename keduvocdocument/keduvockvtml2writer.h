/*
 * export a KEduVocDocument to a KVTML file
 * SPDX-FileCopyrightText: 2007 Jeremy Whiting <jpwhiting@kde.org>
 * SPDX-FileCopyrightText: 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef KEDUVOCKVTML2WRITER_H
#define KEDUVOCKVTML2WRITER_H

#include <QDomDocument>
#include <QList>
#include <QFile>

#include "keduvocarticle.h"
#include "keduvocmultiplechoice.h"
#include "keduvoctranslation.h"
#include "keduvocpersonalpronoun.h"

class KEduVocDocument;
class KEduVocExpression;
class KEduVocLesson;
class KEduVocLeitnerBox;

/**
* @brief Class to write kvtml2 data files from KEduVocDocument
* @author Jeremy Whiting
*/
class KEduVocKvtml2Writer
{
public:
    explicit KEduVocKvtml2Writer( QFile *file );

    bool writeDoc( KEduVocDocument *doc, const QString &generator );

    QByteArray toByteArray( KEduVocDocument *doc, const QString &generator );

    /**
     * Helper function, appends a new element AND a text child to @p parent
     * Only appends if @p text is NOT empty.
     * @param parent
     * @param elementName
     * @param text
     */
    static void appendTextElement( QDomElement &parent, const QString &elementName, const QString &text );

private:

    bool createXmlDocument( KEduVocDocument *doc, const QString &generator );

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

    bool writePersonalPronoun( QDomElement &pronounElement, const KEduVocPersonalPronoun &pronoun);
    /** write types
     * @param typesElement QDomElement types to write to
     */
    bool writeWordTypes( QDomElement &typesElement, KEduVocWordType* parentContainer );

    /**
     * write the leitner boxes @see KEduVocLeitnerBox
     * @param leitnerParentElement parent dom element
     * @param parentContainer parent of the KEduVocLeitnerBoxes to write
     * @return success
     */
    bool writeLeitnerBoxes( QDomElement &leitnerParentElement, KEduVocLeitnerBox* parentContainer );

    /** write entries
     * @param entriesElement QDomElement entries to write to
     */
    bool writeEntries( QDomElement &entriesElement );

    /** write a translation
     * @param translationElement QDomElement translation to write to, with id pre-set
     * @param translation object to write
     */
    bool writeTranslation( QDomElement &translationElement, KEduVocTranslation* translation );

    /**
     * Used to write synonym, antonym and false friend lists
     * @param typesElement 
     * @param parentContainer 
     * @return 
     */
    bool writeRelated( QDomElement &parentElement, QList<KEduVocTranslation*> relatedList );

    /** write the lesson group
     * @param parentLesson the parent lesson of the current lesson
     * @param lessonsElement QDomElement lessons to write to
     */
    bool writeLessons( KEduVocLesson *parentLesson, QDomElement &lessonsElement );

    void writeSynonymAntonymFalseFriend(QDomElement & parentElement);

    /** write multiple choice choices
     * @param multipleChoiceElement QDomElement multiplechoice to write to
     * @returns success
     */
    bool writeMultipleChoice( QDomElement &multipleChoiceElement, KEduVocTranslation* translation );

    QDomElement newTextElement( const QString &elementName, const QString &text );

    QFile *m_outputFile;
    KEduVocDocument *m_doc;

    QList<KEduVocExpression*>  m_allEntries;
    QList<KEduVocTranslation*> m_synonyms;
    QList<KEduVocTranslation*> m_antonyms;
    QList<KEduVocTranslation*> m_falseFriends;

    QDomDocument m_domDoc;
};

#endif
