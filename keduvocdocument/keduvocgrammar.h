/***************************************************************************

              manage grammar parts (articles, conjugation)

    -----------------------------------------------------------------------

    begin        : Sat Nov 27 09:50:53 MET 1999

    copyright    : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                   (C) 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
                   (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCGRAMMAR_H
#define KEDUVOCGRAMMAR_H

#include "libkeduvocdocument_export.h"

#include <QtCore/QStringList>

/**
 * Class representing the articles of a language
 *
 * It contains all available articles of the language as QString
 */
class KEDUVOCDOCUMENT_EXPORT KEduVocArticle
{
public:

    enum ArticleNumber {
        Singular,
        Dual,
        Plural,
        NumberMAX
    };

    enum ArticleGender {
        Masculine,
        Feminine,
        Neutral,
        GenderMAX
    };

    enum ArticleDefiniteness {
        Definite,
        Indefinite,
        DefinitenessMAX
    };


    /**
     * The constructor without arguments
     */
    explicit KEduVocArticle();

    /** copy constructor for d-pointer safety
     * @param other article object to copy
     */
    KEduVocArticle( const KEduVocArticle &other );

    /**
     * DEPRECATED The constructor with arguments, assumes singular!
     * @param fem_def reference to a QString with the definite female article
     * @param fem_indef reference to a QString with the indefinite female article
     * @param mal_def reference to a QString with the definite male article
     * @param mal_indef reference to a QString with the indefinite male article
     * @param nat_def reference to a QString with the definite neutral article
     * @param nat_indef reference to a QString with the indefinite neutral article
     */
    KDE_DEPRECATED KEduVocArticle( const QString &fem_def, const QString &fem_indef, const QString &mal_def, const QString &mal_indef, const QString &nat_def, const QString &nat_indef );

    /**
     * default destructor, deletes the d pointer
     */
    ~KEduVocArticle();


    /**
     * assignment operator for d-pointer copying
     */
    KEduVocArticle &operator= ( const KEduVocArticle& other );


    QString article(ArticleNumber number, ArticleDefiniteness definite, ArticleGender gender);

    void setArticle(const QString& article, ArticleNumber number, ArticleDefiniteness definite, ArticleGender gender);

    bool isArticle(const QString& article) const;

private:
    int indexOf(ArticleNumber number, ArticleDefiniteness definite, ArticleGender gender);

    class Private;
    Private * const d;
};


// class KEDUVOCDOCUMENT_EXPORT KEduVocComparison
// {
// public:
// 
//     /**
//      * The constructor without arguments
//      */
//     explicit KEduVocComparison();
// 
//     /** copy constructor
//      * @param other comparison object to copy
//      */
//     KEduVocComparison( const KEduVocComparison &other );
// 
//     /**
//      * The constructor with arguments
//      * @param l1
//      * @param l2
//      * @param l3
//      */
//     KEduVocComparison( const QString &l1, const QString &l2, const QString &l3 );
// 
//     /** default destructor, deletes the d-pointer */
//     ~KEduVocComparison();
// 
//     /** set the first comparison
//      @param s value to set
//      */
//     void setL1( const QString &s );
// 
//     /** set the second comparison
//      @param s value to set
//      */
//     void setL2( const QString &s );
// 
//     /** set the third comparison
//      @param s value to set
//      */
//     void setL3( const QString &s );
// 
//     /** get the first comparison
//      * @returns the first comparison
//      */
//     QString l1() const;
// 
//     /** get the second comparison
//      * @returns the second comparison
//      */
//     QString l2() const;
// 
//     /** get the third comparison
//      * @returns the third comparison
//      */
//     QString l3() const;
// 
//     /** is the comparison empty
//      * @returns true if empty, false otherwise
//      */
//     bool isEmpty() const;
// 
//     /** clear the comparison */
//     void clear();
// 
//     /** equality operator
//      * @param a object to compare to
//      * @returns true if comparisons are the same, false otherwise
//      */
//     bool operator == ( const KEduVocComparison& a ) const;
// 
//     /** assignment operator for d-pointer copying
//      * @param other object to copy from
//      * @returns reference to this object
//      */
//     KEduVocComparison &operator= ( const KEduVocComparison& other );
// 
// private:
//     class Private;
//     Private * const d;
// };


#endif // KEDUVOCGRAMMAR_H
