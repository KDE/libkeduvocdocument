/***************************************************************************

    C++ Implementation: keduvocconjugation

    -----------------------------------------------------------------------

    begin         : Di Aug 28 2007

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

#include "keduvocconjugation.h"
#include "keduvoccommon_p.h"
#include <KLocalizedString>
#include <KDebug>

class KEduVocConjugation::Private
{
public:
    Private();

//     bool operator==(const conjug_t& other) const;

    bool    p3common;
    bool    s3common;
    QString pers1_sing;
    QString pers2_sing;
    QString pers3_m_sing;
    QString pers3_f_sing;
    QString pers3_n_sing;
    QString pers1_plur;
    QString pers2_plur;
    QString pers3_m_plur;
    QString pers3_f_plur;
    QString pers3_n_plur;

};


KEduVocConjugation::Private::Private()
{
    p3common = false;
    s3common = false;
}

KEduVocConjugation::KEduVocConjugation()
        : d( new Private )
{}


KEduVocConjugation::KEduVocConjugation( const KEduVocConjugation& rhs )
        : d( new Private( *rhs.d ) )
{
}


KEduVocConjugation::~KEduVocConjugation()
{
    delete d;
}


KEduVocConjugation& KEduVocConjugation::operator = ( const KEduVocConjugation& other )
{
    d->s3common = other.d->s3common;
    d->p3common = other.d->p3common;
    d->pers1_sing = other.d->pers1_sing;
    d->pers2_sing = other.d->pers2_sing;
    d->pers3_m_sing = other.d->pers3_m_sing;
    d->pers3_f_sing = other.d->pers3_f_sing;
    d->pers3_n_sing = other.d->pers3_n_sing;
    d->pers1_plur = other.d->pers1_plur;
    d->pers2_plur = other.d->pers2_plur;
    d->pers3_m_plur = other.d->pers3_m_plur;
    d->pers3_f_plur = other.d->pers3_f_plur;
    d->pers3_n_plur = other.d->pers3_n_plur;
    return *this;
}

bool KEduVocConjugation::operator ==(const KEduVocConjugation& other) const
{
    return d->s3common == other.d->s3common &&
        d->p3common == other.d->p3common &&
        d->pers1_sing == other.d->pers1_sing &&
        d->pers2_sing == other.d->pers2_sing &&
        d->pers3_m_sing == other.d->pers3_m_sing &&
        d->pers3_f_sing == other.d->pers3_f_sing &&
        d->pers3_n_sing == other.d->pers3_n_sing &&
        d->pers1_plur == other.d->pers1_plur &&
        d->pers2_plur == other.d->pers2_plur &&
        d->pers3_m_plur == other.d->pers3_m_plur &&
        d->pers3_f_plur == other.d->pers3_f_plur &&
        d->pers3_n_plur == other.d->pers3_n_plur;
}

bool KEduVocConjugation::pers3SingularCommon() const
{
    return d->s3common;
}


bool KEduVocConjugation::pers3PluralCommon() const
{
    return d->p3common;
}


QString KEduVocConjugation::pers1Singular() const
{
    return d->pers1_sing;
}


QString KEduVocConjugation::pers2Singular() const
{
    return d->pers2_sing;
}


QString KEduVocConjugation::pers3FemaleSingular() const
{
    return d->pers3_f_sing;
}


QString KEduVocConjugation::pers3MaleSingular() const
{
    return d->pers3_m_sing;
}


QString KEduVocConjugation::pers3NaturalSingular() const
{
    return d->pers3_n_sing;
}


QString KEduVocConjugation::pers1Plural() const
{
    return d->pers1_plur;
}


QString KEduVocConjugation::pers2Plural() const
{
    return d->pers2_plur;
}


QString KEduVocConjugation::pers3FemalePlural() const
{
    return d->pers3_f_plur;
}


QString KEduVocConjugation::pers3MalePlural() const
{
    return d->pers3_m_plur;
}


QString KEduVocConjugation::pers3NaturalPlural() const
{
    return d->pers3_n_plur;
}


void KEduVocConjugation::setPers3PluralCommon( bool f )
{
    d->p3common = f;
}


void KEduVocConjugation::setPers3SingularCommon( bool f )
{
    d->s3common = f;
}


void KEduVocConjugation::setPers1Singular( const QString &str )
{
    d->pers1_sing = str;
}


void KEduVocConjugation::setPers2Singular( const QString &str )
{
    d->pers2_sing = str;
}


void KEduVocConjugation::setPers3FemaleSingular( const QString &str )
{
    d->pers3_f_sing = str;
}


void KEduVocConjugation::setPers3MaleSingular( const QString &str )
{
    d->pers3_m_sing = str;
}


void KEduVocConjugation::setPers3NaturalSingular( const QString &str )
{
    d->pers3_n_sing = str;
}


void KEduVocConjugation::setPers1Plural( const QString &str )
{
    d->pers1_plur = str;
}


void KEduVocConjugation::setPers2Plural( const QString &str )
{
    d->pers2_plur = str;
}


void KEduVocConjugation::setPers3FemalePlural( const QString &str )
{
    d->pers3_f_plur = str;
}


void KEduVocConjugation::setPers3MalePlural( const QString &str )
{
    d->pers3_m_plur = str;
}


void KEduVocConjugation::setPers3NaturalPlural( const QString &str )
{
    d->pers3_n_plur = str;
}

