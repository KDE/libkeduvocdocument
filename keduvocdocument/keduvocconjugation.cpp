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
    struct conjug_t
    {
        conjug_t()
        {
            p3common = false;
            s3common = false;
        }

        bool operator==(const conjug_t& other) const;

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

    QMap <QString, conjug_t> m_conjugations;
};


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


KEduVocConjugation& KEduVocConjugation::operator = ( const KEduVocConjugation& a )
{
    d->m_conjugations = a.d->m_conjugations;
    return *this;
}


bool KEduVocConjugation::operator == ( const KEduVocConjugation& a ) const
{
///@todo conjugations: rewrite operator==
kDebug() <<"Implement KEduVocConjugation::operator==";
//     return d->m_conjugations[0].type == a.d->m_conjugations[0].type;
    return d->m_conjugations == a.d->m_conjugations;
}


int KEduVocConjugation::entryCount() const
{
    return d->m_conjugations.count();
}


QString KEduVocConjugation::getType( int idx )
{
    kDebug() << "KEduVocConjugation::getType()" << idx;
    if ( idx >= d->m_conjugations.count() )
        return QString();

    return d->m_conjugations.keys().value(idx);
}


bool KEduVocConjugation::pers3SingularCommon( const QString &type ) const
{
    return d->m_conjugations.value(type).s3common;
}


bool KEduVocConjugation::pers3PluralCommon( const QString &type ) const
{
    return d->m_conjugations.value(type).p3common;
}


QString KEduVocConjugation::pers1Singular( const QString &type ) const
{
    return d->m_conjugations.value(type).pers1_sing;
}


QString KEduVocConjugation::pers2Singular( const QString &type ) const
{
    return d->m_conjugations.value(type).pers2_sing;
}


QString KEduVocConjugation::pers3FemaleSingular( const QString &type ) const
{
    return d->m_conjugations.value(type).pers3_f_sing;
}


QString KEduVocConjugation::pers3MaleSingular( const QString &type ) const
{
    return d->m_conjugations.value(type).pers3_m_sing;
}


QString KEduVocConjugation::pers3NaturalSingular( const QString &type ) const
{
    return d->m_conjugations.value(type).pers3_n_sing;
}


QString KEduVocConjugation::pers1Plural( const QString &type ) const
{
    return d->m_conjugations.value(type).pers1_plur;
}


QString KEduVocConjugation::pers2Plural( const QString &type ) const
{
    return d->m_conjugations.value(type).pers2_plur;
}


QString KEduVocConjugation::pers3FemalePlural( const QString &type ) const
{
    return d->m_conjugations.value(type).pers3_f_plur;
}


QString KEduVocConjugation::pers3MalePlural( const QString &type ) const
{
    return d->m_conjugations.value(type).pers3_m_plur;
}


QString KEduVocConjugation::pers3NaturalPlural( const QString &type ) const
{
    return d->m_conjugations.value(type).pers3_n_plur;
}


void KEduVocConjugation::setPers3PluralCommon( const QString &type, bool f )
{
    d->m_conjugations[type].p3common = f;
}


void KEduVocConjugation::setPers3SingularCommon( const QString &type, bool f )
{
    d->m_conjugations[type].s3common = f;
}


void KEduVocConjugation::setPers1Singular( const QString &type, const QString &str )
{
    d->m_conjugations[type].pers1_sing = str;
}


void KEduVocConjugation::setPers2Singular( const QString &type, const QString &str )
{
    d->m_conjugations[type].pers2_sing = str;
}


void KEduVocConjugation::setPers3FemaleSingular( const QString &type, const QString &str )
{
    d->m_conjugations[type].pers3_f_sing = str;
}


void KEduVocConjugation::setPers3MaleSingular( const QString &type, const QString &str )
{
    d->m_conjugations[type].pers3_m_sing = str;
}


void KEduVocConjugation::setPers3NaturalSingular( const QString &type, const QString &str )
{
    d->m_conjugations[type].pers3_n_sing = str;
}


void KEduVocConjugation::setPers1Plural( const QString &type, const QString &str )
{
    d->m_conjugations[type].pers1_plur = str;
}


void KEduVocConjugation::setPers2Plural( const QString &type, const QString &str )
{
    d->m_conjugations[type].pers2_plur = str;
}


void KEduVocConjugation::setPers3FemalePlural( const QString &type, const QString &str )
{
    d->m_conjugations[type].pers3_f_plur = str;
}


void KEduVocConjugation::setPers3MalePlural( const QString &type, const QString &str )
{
    d->m_conjugations[type].pers3_m_plur = str;
}


void KEduVocConjugation::setPers3NaturalPlural( const QString &type, const QString &str )
{
    d->m_conjugations[type].pers3_n_plur = str;
}


bool KEduVocConjugation::Private::conjug_t::operator ==(const conjug_t & other) const
{
    return s3common == other.s3common &&
        p3common == other.p3common &&
        pers1_sing == other.pers1_sing &&
        pers2_sing == other.pers2_sing &&
        pers3_m_sing == other.pers3_m_sing &&
        pers3_f_sing == other.pers3_f_sing &&
        pers3_n_sing == other.pers3_n_sing &&
        pers1_plur == other.pers1_plur &&
        pers2_plur == other.pers2_plur &&
        pers3_m_plur == other.pers3_m_plur &&
        pers3_f_plur == other.pers3_f_plur &&
        pers3_n_plur == other.pers3_n_plur;
}


