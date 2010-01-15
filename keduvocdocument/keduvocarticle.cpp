/***************************************************************************

    Copyright 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    Copyright 2004, 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvocarticle.h"
#include "keduvocwordflags.h"

#include <QtCore/QMap>

class KEduVocArticle::Private
{
public:
    QMap <KEduVocWordFlags, QString>    m_articles;
};

KEduVocArticle::KEduVocArticle()
        :d( new Private )
{}

KEduVocArticle::KEduVocArticle( const KEduVocArticle &other )
        :d( new Private )
{
    d->m_articles = other.d->m_articles;
}

KEduVocArticle &KEduVocArticle::operator= ( const KEduVocArticle& other )
{
    d->m_articles = other.d->m_articles;
    return *this;
}

KEduVocArticle::KEduVocArticle( const QString &fem_def, const QString &fem_indef, const QString &mal_def, const QString &mal_indef, const QString &neu_def, const QString &neu_indef )
        :d( new Private )
{
    setArticle( mal_def, KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Masculine );
    setArticle( fem_def, KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Feminine );
    setArticle( neu_def, KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Neuter );

    setArticle( mal_indef, KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Masculine );
    setArticle( fem_indef, KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Feminine );
    setArticle( neu_indef, KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Neuter );
}

KEduVocArticle::~KEduVocArticle()
{
    delete d;
}


QString KEduVocArticle::article(const KEduVocWordFlags& flags)
{
    return d->m_articles.value(flags & (KEduVocWordFlag::genders | KEduVocWordFlag::numbers | KEduVocWordFlag::Definite | KEduVocWordFlag::Indefinite));
}

void KEduVocArticle::setArticle(const QString & article, const KEduVocWordFlags& flags)
{
    d->m_articles[flags & (KEduVocWordFlag::genders | KEduVocWordFlag::numbers | KEduVocWordFlag::Definite | KEduVocWordFlag::Indefinite)] = article;
}


bool KEduVocArticle::isArticle(const QString & article) const
{
    return d->m_articles.values().contains(article);
}

bool KEduVocArticle::isEmpty()
{
    // we don't count empty strings as articles
    foreach(const QString& s, d->m_articles)
    {
        if (!s.isEmpty())
            return false;
    }
    return true;
}



