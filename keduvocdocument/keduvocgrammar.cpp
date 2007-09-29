/***************************************************************************

             manage grammar parts (articles, conjugation)

    -----------------------------------------------------------------------

    begin     : Sat Nov 27 09:50:53 MET 1999

    copyright : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                (C) 2004, 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
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

#include "keduvocgrammar.h"

#include <QtCore/QMap>
#include <KDebug>

class KEduVocComparison::Private
{
public:
    QString ls1;
    QString ls2;
    QString ls3;
};

KEduVocComparison::KEduVocComparison()
        : d( new Private )
{}

KEduVocComparison::KEduVocComparison( const KEduVocComparison &other )
        : d( new Private )
{
    setL1( other.l1() );
    setL2( other.l2() );
    setL3( other.l3() );
}

KEduVocComparison::KEduVocComparison( const QString &l1, const QString &l2, const QString &l3 )
        : d( new Private )
{
    setL1( l1 );
    setL2( l2 );
    setL3( l3 );
}

KEduVocComparison::~KEduVocComparison()
{
    delete d;
}

KEduVocComparison &KEduVocComparison::operator= ( const KEduVocComparison& other )
{
    setL1( other.l1() );
    setL2( other.l2() );
    setL3( other.l3() );

    return *this;
}

bool KEduVocComparison::isEmpty() const
{
    return d->ls1.simplified().isEmpty() && d->ls2.simplified().isEmpty() && d->ls3.simplified().isEmpty();
}


void KEduVocComparison::clear()
{
    d->ls1 = "";
    d->ls2 = "";
    d->ls3 = "";
}

bool KEduVocComparison::operator == ( const KEduVocComparison& a ) const
{
    return ( d->ls1 == a.l1() && d->ls2 == a.l2() && d->ls3 == a.l3() );
}

void KEduVocComparison::setL1( const QString &s )
{
    d->ls1 = s;
}

void KEduVocComparison::setL2( const QString &s )
{
    d->ls2 = s;
}

void KEduVocComparison::setL3( const QString &s )
{
    d->ls3 = s;
}

QString KEduVocComparison::l1() const
{
    return d->ls1;
}
QString KEduVocComparison::l2() const
{
    return d->ls2;
}

QString KEduVocComparison::l3() const
{
    return d->ls3;
}




//=================================================================

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
    kDebug() << article << "#" << number << "def" << definite << "indef"  << gender << "index" << indexOf(number, definite, gender);
    d->m_articles[indexOf(number, definite, gender)] = article;
}

int KEduVocArticle::indexOf(ArticleNumber number, ArticleDefiniteness definite, ArticleGender gender)
{
    return number + (definite * NumberMAX) + (gender * NumberMAX * DefinitenessMAX);
}
