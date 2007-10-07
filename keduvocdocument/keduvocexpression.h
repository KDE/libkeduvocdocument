/***************************************************************************
                        Vocabulary Expression for KDE Edu
    -----------------------------------------------------------------------
    copyright            : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                           (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
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

#include "keduvocgrammar.h"
#include "keduvocmultiplechoice.h"
#include "keduvoctranslation.h"

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
    explicit KEduVocExpression( const QString & expression, int lesson = -1 );

    /** Constructor for a vocabulary expression with an original and one or more translations
     *
     * @param expression       expression
     * @param separator        expression will be split into an original and one or more translations using separator
     * @param lesson           lesson number, 0 for none
     */
    explicit KEduVocExpression( const QStringList & translations, int lesson = -1 );

    KEduVocExpression( const KEduVocExpression &expression );

    ~KEduVocExpression();

    /** returns index of lesson (-1 = none)
     */
    int lesson() const;

    /** sets index of lesson (-1 = none)
     */
    void setLesson( int l );

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

    /** returns this translation
     *
     * @return                 expression or "" if no translation available
     */
    QString translationString( int index ) const;

    void setTranslation( int index, const KEduVocTranslation & translation );
    /**
     * Add a translation to this expression
     * @param index            number of translation = the identifier
     * @param expression       the translation
     */
    void setTranslation( int index, const QString &expression );

//     void setTranslation( const QString &identifier, const QString & expression );

    /** removes a translation
     *
     * @param index            number of translation 1..x
     */
    void removeTranslation( int index );


    /**
     * Get a mutable reference to the translation
     * @param index of the language identifier
     * @return the translation
     */
    KEduVocTranslation & translation( int index ) const;

    QList<int> translationIndices() const;

    /** sets the box of the Leitner system which actually contains the expression
     *
     * @param box              the box's name
     */
    void setLeitnerBox( const QString & box );

    /** returns the name of the Leitner system's box actually containing the expression
     *
     * @return                 the box's name
     */
    QString leitnerBox() const;

    KEduVocExpression& operator= ( const KEduVocExpression &expression );
    bool operator== ( const KEduVocExpression &expression ) const;

private:
    class KEduVocExpressionPrivate;
    KEduVocExpressionPrivate* const d;
};

#endif // KEduVocExpression_H
