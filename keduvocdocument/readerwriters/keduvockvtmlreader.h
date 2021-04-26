/*
 * read a KEduVocDocument from a KVTML file

 * SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
 * SPDX-FileCopyrightText: 2005 Eric Pignet <eric at erixpage.com>
 * SPDX-FileCopyrightText: 2007 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KEDUVOCKVTMLREADER_H
#define KEDUVOCKVTMLREADER_H

#include <QDomDocument>
#include <QList>


#include "keduvocexpression.h"
#include "keduvocarticle.h"
#include "keduvocmultiplechoice.h"
#include "keduvockvtmlcompability.h"
#include "keduvocpersonalpronoun.h"
#include "keduvocdocument.h"
#include "readerbase.h"

class QIODevice;
class KEduVocDocument;

/** @brief Reader for KVTML 1.0
@author Eric Pignet
*/
class KEduVocKvtmlReader : public QObject
{
    Q_OBJECT
public:
    /** constructor
        @param file an open device
    */
    explicit KEduVocKvtmlReader(QIODevice & file);
    /**destructor*/
    virtual ~KEduVocKvtmlReader(){};


    /** @brief Can this reader parse this file
     *
     Read a small portion of the header of the file
     and decide if it is a suitable type.
     @return true if parsable
     */
    virtual bool isParsable();

    /**  @brief Parse file and write into doc
     @param doc to be written
     @return error status of the read.*/
    virtual KEduVocDocument::ErrorCode read(KEduVocDocument & doc );

    /** an error message.
        @return the error message
    */
    virtual QString errorMessage() const
    {
        return m_errorMessage;
    }


    /**
     * Attempt to add a language/locale. Language/locale are set to the same value.
     * No error if already there with the same value.
     * @param languageId identifier number
     * @param language name
     * @return true if successful
     */
    bool addLanguage( int languageId, const QString& language);

    bool readLesson( QDomElement &domElementParent );
    bool readArticle( QDomElement &domElementParent );
    bool readPersonalPronouns( QDomElement &domElementParent, KEduVocPersonalPronoun& pronouns );
    bool readConjugation( QDomElement &domElementParent, KEduVocConjugation &conjugation );
    bool readTranslationConjugations( QDomElement &domElementParent, KEduVocTranslation* translation );
    bool readType( QDomElement &domElementParent );
    bool readTense( QDomElement &domElementParent );
    bool readUsage( QDomElement &domElementParent );
    bool readComparison( QDomElement &domElementParent, KEduVocTranslation* translation );
    bool readMultipleChoice( QDomElement &domElementParent, KEduVocTranslation* translation );
    bool readExpressionChildAttributes( QDomElement &domElementExpressionChild,
                                        QString &lang,
                                        grade_t &grade, grade_t &rev_grade,
                                        int &count, int &rev_count,
                                        QDateTime &date, QDateTime &rev_date,
                                        QString &remark,
                                        int &bcount, int &rev_bcount,
                                        QString &query_id,
                                        QString &pronunciation,
                                        int &width,
                                        QString &type,
                                        QString &faux_ami_f,
                                        QString &faux_ami_t,
                                        QString &synonym,
                                        QString &example,
                                        QString &antonym,
                                        QSet<QString> &usage,
                                        QString &paraphrase );
    bool readExpression( QDomElement &domElementParent );
    bool readBody( QDomElement &domElementParent );

private:
    QIODevice *m_inputFile;  ///< input device
    KEduVocDocument *m_doc;  ///< output doc
    QString m_errorMessage;  ///< error message
    int m_cols;
    int m_lines;
    QStringList m_oldSelfDefinedTypes;

    KEduVocKvtmlCompability m_compability;
};

#endif
