/***************************************************************************
                     read a KEduVocDocument from a KVTML file
    -----------------------------------------------------------------------
    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                     (C) 2005 Eric Pignet <eric at erixpage.com>
                     (C) 2007 Peter Hedlund <peter.hedlund@kdemail.net>
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

#ifndef KEDUVOCKVTMLREADER_H
#define KEDUVOCKVTMLREADER_H

#include <QtXml/QDomDocument>
#include <QtCore/QList>


#include "keduvocexpression.h"
#include "keduvocarticle.h"
#include "keduvocmultiplechoice.h"
#include "keduvockvtmlcompability.h"
#include "keduvocpersonalpronoun.h"

class QIODevice;
class KEduVocDocument;

/**
@author Eric Pignet
*/
class KEduVocKvtmlReader : public QObject
{
    Q_OBJECT
public:
    KEduVocKvtmlReader( QIODevice *file );

    bool readDoc( KEduVocDocument *doc );

    /**
     * Attempt to add a language/locale. Language/locale are set to the same value.
     * No error if already there with the same value.
     * @param languageId idenifier number
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

    QString errorMessage() const
    {
        return m_errorMessage;
    }

private:
    QIODevice *m_inputFile;
    KEduVocDocument *m_doc;
    QString m_errorMessage;
    int m_cols;
    int m_lines;
    QStringList m_oldSelfDefinedTypes;

    KEduVocKvtmlCompability m_compability;
};

#endif
