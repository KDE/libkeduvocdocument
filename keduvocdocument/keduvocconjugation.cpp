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

    bool m_thirdCommonSingular;
    bool m_thirdCommonDual;
    bool m_thirdCommonPlural;
    QMap<int, QString> m_conjugations;
};


KEduVocConjugation::Private::Private()
{
    m_thirdCommonSingular = false;
    m_thirdCommonDual = false;
    m_thirdCommonPlural = false;
}

KEduVocConjugation::KEduVocConjugation()
        : d( new Private )
{}


KEduVocConjugation::KEduVocConjugation( const KEduVocConjugation& other )
        : d( new Private )
{
    d->m_thirdCommonSingular = other.d->m_thirdCommonSingular;
    d->m_thirdCommonDual = other.d->m_thirdCommonDual;
    d->m_thirdCommonPlural = other.d->m_thirdCommonPlural;
    d->m_conjugations = other.d->m_conjugations;
}


KEduVocConjugation::~KEduVocConjugation()
{
    delete d;
}


KEduVocConjugation& KEduVocConjugation::operator = ( const KEduVocConjugation& other )
{
    d->m_thirdCommonSingular = other.d->m_thirdCommonSingular;
    d->m_thirdCommonDual = other.d->m_thirdCommonDual;
    d->m_thirdCommonPlural = other.d->m_thirdCommonPlural;
    d->m_conjugations = other.d->m_conjugations;
    return *this;
}

bool KEduVocConjugation::operator ==(const KEduVocConjugation& other) const
{
    return d->m_conjugations == other.d->m_conjugations &&
        d->m_thirdCommonSingular == other.d->m_thirdCommonSingular &&
        d->m_thirdCommonDual == other.d->m_thirdCommonDual &&
        d->m_thirdCommonPlural == other.d->m_thirdCommonPlural;
    ;
}


QString KEduVocConjugation::conjugation(ConjugationPerson person, ConjugationNumber number) const
{
    int index = indexOf(person, number);
    if ( d->m_conjugations.contains(index) ) {
        return d->m_conjugations.value(index);
    }
    return QString();
}

void KEduVocConjugation::setConjugation(const QString & conjugation, ConjugationPerson person, ConjugationNumber number)
{
    d->m_conjugations[indexOf(person, number)] = conjugation;
}


int KEduVocConjugation::indexOf(ConjugationPerson person, ConjugationNumber number) const
{
    return person + PersonMAX * number;
}

