/*
 * SPDX-FileCopyrightText: 2007-2008 Frederik Gladhorn <gladhorn@kde.org>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KEDUVOCIDENTIFIER_H
#define KEDUVOCIDENTIFIER_H

#include "keduvocdocument_export.h"

#include "keduvocpersonalpronoun.h"
#include "keduvocarticle.h"

#include <QList>
#include <QString>
#include <QSet>


/**
Class to store meta information about a language or any other category in the vocabulary.
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
    KEduVocArticle& article() const;

    /**
     * Sets the articles for this identifier
     * @param article              article block
     */
    void setArticle( const KEduVocArticle& article );

    /**
     * Get the personal pronouns for this identifier
     * @returns a KEduVocPersonalPronoun containing the personal pronouns
     */
    KEduVocPersonalPronoun& personalPronouns() const;

    /**
     * Sets personal pronouns
     * @param pronouns a KEduVocConjugation containing the personal pronouns
     */
    void setPersonalPronouns( const KEduVocPersonalPronoun &pronouns );

    /**
     * Returns the name of tense number @p tenseIndex
     * @param tenseIndex desired tense
     * @return name of the tense
     */
    QString tense(int tenseIndex) const;

    /**
     * Sets the name of a tense for this language
     * @param tenseIndex 
     * @param tense 
     */
    void setTense(int tenseIndex, const QString& tense);

    QStringList tenseList() const;

    void setTenseList(const QStringList& tenses);

private:
    class Private;
    Private * const d;
};

#endif
