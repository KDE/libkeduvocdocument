/***************************************************************************

    C++ Interface: keduvocidentifier

    -----------------------------------------------------------------------

    begin         : Mi Aug 29 2007

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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
#ifndef KEDUVOCIDENTIFIER_H
#define KEDUVOCIDENTIFIER_H

#include "libkeduvocdocument_export.h"

#include "keduvocconjugation.h"
#include "keduvocgrammar.h"

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QSet>


/**
Class to store meta information about a language or any other category in the vocabulary.

 @author
*/
class KEDUVOCDOCUMENT_EXPORT KEduVocIdentifier
{
public:
    /**
     * Default ctor.
     */
    explicit KEduVocIdentifier();

    /**
     * Copy ctor.
     * @param other
     */
    KEduVocIdentifier( const KEduVocIdentifier &other );

    /**
     * dtor
     */
    ~KEduVocIdentifier();

    /**
     * assignment operator
     * @param other
     * @return
     */
    KEduVocIdentifier& operator= ( const KEduVocIdentifier &other );
public:

    /**
     * Name of this identifier. (English, Anatomy, Fruit salad...)
     * @return name
     */
    QString name() const;
    /**
     * Set the name
     * @param name
     */
    void setName( const QString& name );

    /**
     * The locale of the contents: en, de, es, ...
     * @return locale
     */
    QString locale() const;
    /**
     * Set the locale
     * @param name
     */
    void setLocale( const QString& name );


    /**
     * Articles (a, the in English, el, la,... in Spanish)
     * @returns articles
     */
    KEduVocArticle article() const;

    /**
     * Sets the articles for this identifier
     * @param art              article block
     */
    void setArticle( const KEduVocArticle& article );


    /**
     * Get the personal pronouns for this identifier
     * @returns a KEduVocConjugation containing the personal pronouns
     */
    KEduVocConjugation personalPronouns() const;

    /**
     * Sets personal pronouns
     * @param pronouns a KEduVocConjugation containing the personal pronouns
     */
    void setPersonalPronouns( const KEduVocConjugation &pronouns );

private:
    class Private;
    Private * const d;

};

#endif
