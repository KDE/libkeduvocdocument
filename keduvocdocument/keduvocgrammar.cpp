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
    QString fem_def;
    QString fem_indef;
    QString mal_def;
    QString mal_indef;
    QString nat_def;
    QString nat_indef;
};

KEduVocArticle::KEduVocArticle()
        :d( new Private )
{}

KEduVocArticle::KEduVocArticle( const KEduVocArticle &other )
        :d( new Private )
{
    operator= ( other );
}

KEduVocArticle &KEduVocArticle::operator= ( const KEduVocArticle& other )
{
    d->fem_def = other.d->fem_def;
    d->fem_indef = other.d->fem_indef;
    d->mal_def = other.d->mal_def;
    d->mal_indef = other.d->mal_indef;
    d->nat_def = other.d->nat_def;
    d->nat_indef = other.d->nat_indef;

    return *this;
}

KEduVocArticle::KEduVocArticle( const QString &fem_def, const QString &fem_indef, const QString &mal_def, const QString &mal_indef, const QString &nat_def, const QString &nat_indef )
        :d( new Private )
{
    setFemale( fem_def, fem_indef );
    setMale( mal_def, mal_indef );
    setNeutral( nat_def, nat_indef );
}

KEduVocArticle::~KEduVocArticle()
{
    delete d;
}

void KEduVocArticle::setFemale( const QString &def, const QString &indef )
{
    d->fem_def = def;
    d->fem_indef = indef;
}


void KEduVocArticle::setMale( const QString &def, const QString &indef )
{
    d->mal_def = def;
    d->mal_indef = indef;
}


void KEduVocArticle::setNeutral( const QString &def, const QString &indef )
{
    d->nat_def = def;
    d->nat_indef = indef;
}


void KEduVocArticle::getFemale( QString *def, QString *indef ) const
{
    if ( def )
        *def = d->fem_def;
    if ( indef )
        *indef = d->fem_indef;
}


void KEduVocArticle::getMale( QString *def, QString *indef ) const
{
    if ( def )
        *def = d->mal_def;
    if ( indef )
        *indef = d->mal_indef;
}


void KEduVocArticle::getNeutral( QString *def, QString *indef ) const
{
    if ( def )
        *def = d->nat_def;
    if ( indef )
        *indef = d->nat_indef;
}


