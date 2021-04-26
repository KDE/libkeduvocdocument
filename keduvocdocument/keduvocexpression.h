/*
 * Vocabulary Expression for KDE Edu
 * SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
 * SPDX-FileCopyrightText: 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef KEDUVOCEXPRESSION_H
#define KEDUVOCEXPRESSION_H

#include "keduvocdocument_export.h"

#include <QDateTime>

#include "keduvoclesson.h"
#include "keduvocarticle.h"
#include "keduvocmultiplechoice.h"
#include "keduvoctranslation.h"

class KEduVocLesson;

/**
  This class contains one vocabulary expression as an original with one or more
  translations
  */
class KEDUVOCDOCUMENT_EXPORT KEduVocExpression
{
public:

    /** default constructor for an empty vocabulary expression
     */
    explicit KEduVocExpression();

    /** Constructor for a vocabulary expression with one translation
     *
     * @param expression       translation
     */
    explicit KEduVocExpression( const QString & expression );

    /** Constructor for a vocabulary expression with an original and one or more translations
     *
     * @param translations      translations
     */
    explicit KEduVocExpression( const QStringList & translations );

    KEduVocExpression(const KEduVocExpression& other);

    ~KEduVocExpression();

    /** return the lesson
     */
    KEduVocLesson * lesson() const;


    /** reset all grades of the entry
     * @param index     identifier (language)
     */
    void resetGrades( int index );

    /** returns flag if entry is activated for queries
     */
    bool isActive() const;

    /** set entry active (enabled for queries)
     */
    void setActive( bool flag = true );

    int sizeHint() const;
    void setSizeHint( int sizeHint );

    /**
     * Add a translation to this expression
     * @param index            number of translation = the identifier
     * @param expression       the translation
     */
    void setTranslation( int index, const QString &expression );

    /** removes a translation
     *
     * @param index            number of translation 1..x
     */
    void removeTranslation( int index );

    /**
     * Get a pointer to the translation
     * @param index of the language identifier
     * @return the translation
     */
    KEduVocTranslation* translation( int index );
    KEduVocTranslation* translation( int index ) const;

    QList<int> translationIndices() const;

    KEduVocExpression& operator= ( const KEduVocExpression &expression );
    bool operator== ( const KEduVocExpression &expression ) const;

private:
    class KEduVocExpressionPrivate;
    KEduVocExpressionPrivate* const d;

    /** only called by lesson to add itself to the lesson list
     */
    void setLesson( KEduVocLesson * l );

    friend class KEduVocLesson;
};

#endif // KEduVocExpression_H
