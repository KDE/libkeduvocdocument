/*
* read a KEduVocDocument from a KVTML2 file
* SPDX-FileCopyrightText: 2007 Jeremy Whiting <jpwhiting@kde.org>
* SPDX-FileCopyrightText: 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
* SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KEDUVOCKVTML2READER_H
#define KEDUVOCKVTML2READER_H

#include <QDomDocument>
#include <QMap>

#include "keduvocexpression.h"
#include "keduvocpersonalpronoun.h"
#include "keduvocarticle.h"
#include "keduvocmultiplechoice.h"
#include "keduvocdocument.h"
#include "readerbase.h"

class QIODevice;
class KEduVocDocument;
class KEduVocWordType;

/**
* @brief class to read kvtml2 data files into keduvocdocument
* @author Jeremy Whiting
*/
class KEduVocKvtml2Reader : public QObject, public ReaderBase
{
    Q_OBJECT
public:
    /** constructor */
    explicit KEduVocKvtml2Reader( QIODevice & file );
    /**destructor*/
    virtual ~KEduVocKvtml2Reader(){};


    /** @brief Can this reader parse this file
     *
     Read a small portion of the header of the file
     and decide if it is a suitable type.
     @return true if parsable
     */
    bool isParsable() Q_DECL_OVERRIDE;

    /** @brief returns the KEduVocDocument::FileType that this reader handles
        @return KEduVocDocument::FileType handled
     */
    KEduVocDocument::FileType fileTypeHandled() Q_DECL_OVERRIDE;

    /**  @brief Parse file and write into doc
     @param doc to be written
     @return error status of the read.*/
    KEduVocDocument::ErrorCode read(KEduVocDocument & doc ) Q_DECL_OVERRIDE;

    /** an error message.
        @return the error message
    */
    QString errorMessage() const Q_DECL_OVERRIDE
    {
        return m_errorMessage;
    }

private:
    /** read information entries
     * @param informationElement QDomElement information
     */
    bool readInformation( QDomElement &informationElement );

    /** read group elements: identifiers, entries, types, usages, lessons */
    bool readGroups( QDomElement &domElementParent );

    /** read an identifier
     * @param identifierElement QDomElement for the identifier to read
     */
    bool readIdentifier( QDomElement &identifierElement );

    /** read an identifiers articles
     * @param articleElement QDomElement for the article group
     * @param identifierNum number of the identifier this article is inside of
     */
    bool readArticle( QDomElement &articleElement, int identifierNum );

    bool readPersonalPronoun( QDomElement &conjugElement, KEduVocPersonalPronoun &pronoun );

    bool readPersonalPronounChild(QDomElement & personElement, KEduVocPersonalPronoun &pronoun, KEduVocWordFlags flags);

    /** read the types
     * @param typesElement QDomElement for the types group
     */
    bool readWordType( KEduVocWordType* parentContainer, QDomElement &typesElement );

    /**
     * Read a leitner box container.
     * This is a grading system where the vocabulary are kept in boxes and promoted/demoted during the learning.
     * Be aware that leitner boxes are a list only and no sub boxes will ever be read or written.
     * While reusing the lesson class is quite easy for this a proper subclass of KEduVocContainer would be the better solution.
     * @param parentContainer the parent to append the new leitner container to
     * @param leitnerElement the element in the dom
     * @return success
     */
    bool readLeitner( KEduVocLeitnerBox* parentContainer, QDomElement &leitnerElement );

    /**
     * Read all <container> tags within a word type definition.
     * @param parentContainer
     * @param lessonElement
     * @return
     */
    bool readChildWordTypes( KEduVocWordType* parentContainer, QDomElement &lessonElement );

    /** read the tenses
     * @param tensesElement QDomElement for the tenses group
     */
    QStringList readTenses( QDomElement &tensesElement );

    /** read the usages
     * @param usagesElement QDomElement for the usages group
     */
    bool readUsages( QDomElement &usagesElement );

    /** read an entry
     * @param entryElement QDomElement for the entry to read
     */
    bool readEntry( QDomElement &entryElement );

    /** read a translation
     * @param translationElement QDomElement for the translation to read
     */
    bool readTranslation( QDomElement &translationElement, KEduVocExpression *expr, int index );

    /** read a comparison
     * @param comparisonElement comparison group element
     * @param comp comparison object to read into
     */
    bool readComparison( QDomElement &comparisonElement, KEduVocTranslation *translation );

    /** read a multiple choice group
     * @param multipleChoiceElement element to read from
     * @param mc KEduVocMultipleChoice object to read to
     */
    bool readMultipleChoice( QDomElement &multipleChoiceElement, KEduVocTranslation* translation );

    /**
     * Read <lesson> tags.
     * @param parentLesson
     * @param lessonElement
     * @return
     */
    bool readChildLessons( KEduVocLesson* parentLesson, QDomElement &lessonElement );

    /** read a lesson, and append it to the document
     * @param lessonElement element to read from
     */
    bool readLesson( KEduVocLesson* parentLesson, QDomElement &lessonElement );

    bool readSynonymsAntonymsFalseFriends( QDomElement &rootElement );

    /** pre-opened QIODevice to read from */
    QIODevice *m_inputFile;

    /** KEduVocDocument to read to */
    KEduVocDocument *m_doc;

    /** because we read the entries first, we store them here temporarily.
     * later we read the lessons and put the entries there based on the key (their id) */
    QMap<int, KEduVocExpression*> m_allEntries;

    /** error message */
    QString m_errorMessage;
};

#endif
