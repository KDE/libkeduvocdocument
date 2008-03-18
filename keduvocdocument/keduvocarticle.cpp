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

#include <QtCore/QMap>
#include <KDebug>

class KEduVocArticle::Private
{
public:
    QMap <int, QString>    m_articles;
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
    setArticle( mal_def, Singular, Definite, Masculine );
    setArticle( fem_def, Singular, Definite, Feminine );
    setArticle( neu_def, Singular, Definite, Neutral );

    setArticle( mal_indef, Singular, Indefinite, Masculine );
    setArticle( fem_indef, Singular, Indefinite, Feminine );
    setArticle( neu_indef, Singular, Indefinite, Neutral );
}

KEduVocArticle::~KEduVocArticle()
{
    delete d;
}


QString KEduVocArticle::article(ArticleNumber number, ArticleDefiniteness definite, ArticleGender gender)
{
    if ( d->m_articles.contains( indexOf(number, definite, gender) ) ) {
        return d->m_articles.value( indexOf(number, definite, gender) );
    }
    return QString();
}

void KEduVocArticle::setArticle(const QString & article, ArticleNumber number, ArticleDefiniteness definite, ArticleGender gender)
{
    setArticle(article, indexOf(number, definite, gender));
}

void KEduVocArticle::setArticle(const QString & article, int index)
{
    d->m_articles[index] = article;
}

int KEduVocArticle::indexOf(ArticleNumber number, ArticleDefiniteness definite, ArticleGender gender)
{
    return number + (definite * (Plural+1)) + (gender * (Plural+1) * (Indefinite+1));
}

bool KEduVocArticle::isArticle(const QString & article) const
{
    return d->m_articles.values().contains(article);
}

bool KEduVocArticle::isEmpty()
{
    return d->m_articles.isEmpty();
}



