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

        QString type;
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

    typedef QList<conjug_t> conjug_tList;
    conjug_tList m_conjugations;
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
    return d->m_conjugations[0].type == a.d->m_conjugations[0].type;
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

    return d->m_conjugations[idx].type;
}


void KEduVocConjugation::setType( int idx, const QString & type )
{
    if ( idx >= d->m_conjugations.size() )
        return;

    d->m_conjugations[idx].type = type;
}


#define _GET_CON_(elem, type, default) \
   for (int i = 0; i < d->m_conjugations.size(); i++) \
     if (d->m_conjugations[i].type == type) \
        return d->m_conjugations[i].elem; \
   return default;


bool KEduVocConjugation::pers3SingularCommon( const QString &type ) const
{
    _GET_CON_( s3common, type, false );
}


bool KEduVocConjugation::pers3PluralCommon( const QString &type ) const
{
    _GET_CON_( p3common, type, false );
}


QString KEduVocConjugation::pers1Singular( const QString &type ) const
{
    _GET_CON_( pers1_sing, type, "" );
}


QString KEduVocConjugation::pers2Singular( const QString &type ) const
{
    _GET_CON_( pers2_sing, type, "" );
}


QString KEduVocConjugation::pers3FemaleSingular( const QString &type ) const
{
    _GET_CON_( pers3_f_sing, type, "" );
}


QString KEduVocConjugation::pers3MaleSingular( const QString &type ) const
{
    _GET_CON_( pers3_m_sing, type, "" );
}


QString KEduVocConjugation::pers3NaturalSingular( const QString &type ) const
{
    _GET_CON_( pers3_n_sing, type, "" );
}


QString KEduVocConjugation::pers1Plural( const QString &type ) const
{
    _GET_CON_( pers1_plur, type, "" );
}


QString KEduVocConjugation::pers2Plural( const QString &type ) const
{
    _GET_CON_( pers2_plur, type, "" );
}


QString KEduVocConjugation::pers3FemalePlural( const QString &type ) const
{
    _GET_CON_( pers3_f_plur, type, "" );
}


QString KEduVocConjugation::pers3MalePlural( const QString &type ) const
{
    _GET_CON_( pers3_m_plur, type, "" );
}


QString KEduVocConjugation::pers3NaturalPlural( const QString &type ) const
{
    _GET_CON_( pers3_n_plur, type, "" );
}


#undef _GET_CON_


#define _SET_CON_(elem, type, str) \
   for (int i = 0; i < d->m_conjugations.size(); i++) \
     if (d->m_conjugations[i].type == type) { \
       d->m_conjugations[i].elem = str; \
       return; \
     } \
   Private::conjug_t ct; \
   ct.type = type; \
   ct.elem = str; \
   d->m_conjugations.append(ct);


void KEduVocConjugation::setPers3PluralCommon( const QString &type, bool f )
{
    _SET_CON_( p3common, type, f );
}


void KEduVocConjugation::setPers3SingularCommon( const QString &type, bool f )
{
    _SET_CON_( s3common, type, f );
}


void KEduVocConjugation::setPers1Singular( const QString &type, const QString &str )
{
    _SET_CON_( pers1_sing, type, str );
}


void KEduVocConjugation::setPers2Singular( const QString &type, const QString &str )
{
    _SET_CON_( pers2_sing, type, str );
}


void KEduVocConjugation::setPers3FemaleSingular( const QString &type, const QString &str )
{
    _SET_CON_( pers3_f_sing, type, str );
}


void KEduVocConjugation::setPers3MaleSingular( const QString &type, const QString &str )
{
    _SET_CON_( pers3_m_sing, type, str );
}


void KEduVocConjugation::setPers3NaturalSingular( const QString &type, const QString &str )
{
    _SET_CON_( pers3_n_sing, type, str );
}


void KEduVocConjugation::setPers1Plural( const QString &type, const QString &str )
{
    _SET_CON_( pers1_plur, type, str );
}


void KEduVocConjugation::setPers2Plural( const QString &type, const QString &str )
{
    _SET_CON_( pers2_plur, type, str );
}


void KEduVocConjugation::setPers3FemalePlural( const QString &type, const QString &str )
{
    _SET_CON_( pers3_f_plur, type, str );
}


void KEduVocConjugation::setPers3MalePlural( const QString &type, const QString &str )
{
    _SET_CON_( pers3_m_plur, type, str );
}


void KEduVocConjugation::setPers3NaturalPlural( const QString &type, const QString &str )
{
    _SET_CON_( pers3_n_plur, type, str );
}

#undef _SET_CON_


