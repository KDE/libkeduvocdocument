/***************************************************************************
                        Vocabulary Expression for KDE Edu
    -----------------------------------------------------------------------
    copyright            : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                           (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCEXPRESSION_H
#define KEDUVOCEXPRESSION_H

#include "libkeduvocdocument_export.h"

#include <QtCore/QDateTime>

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
     * @param lesson           lesson number
     */
    explicit KEduVocExpression( const QString & expression );

    /** Constructor for a vocabulary expression with an original and one or more translations
     *
     * @param expression       expression
     * @param separator        expression will be split into an original and one or more translations using separator
     * @param lesson           lesson number, 0 for none
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

    void setTranslation( int index, KEduVocTranslation* translation );
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
