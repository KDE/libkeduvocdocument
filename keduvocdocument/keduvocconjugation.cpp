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
    KEduVocGrade m_grade;
    QMap<int, QString> m_conjugations;
};


KEduVocConjugation::KEduVocConjugation()
        : d( new Private )
{}


KEduVocConjugation::KEduVocConjugation( const KEduVocConjugation& other )
        : d( new Private )
{
    d->m_grade = other.d->m_grade;
    d->m_conjugations = other.d->m_conjugations;
}


KEduVocConjugation::~KEduVocConjugation()
{
    delete d;
}


KEduVocConjugation& KEduVocConjugation::operator = ( const KEduVocConjugation& other )
{
    d->m_grade = other.d->m_grade;
    d->m_conjugations = other.d->m_conjugations;
    return *this;
}

bool KEduVocConjugation::operator ==(const KEduVocConjugation& other) const
{
    return d->m_conjugations == other.d->m_conjugations &&
           d->m_grade == other.d->m_grade;
}


QString KEduVocConjugation::conjugation(int index) const
{
    if ( d->m_conjugations.contains(index) ) {
        return d->m_conjugations.value(index);
    }
    return QString();
}


QString KEduVocConjugation::conjugation(ConjugationPerson person, ConjugationNumber number) const
{
    return conjugation(indexOf(person, number));
}

void KEduVocConjugation::setConjugation(const QString & conjugation, ConjugationPerson person, ConjugationNumber number)
{
    setConjugation(conjugation, indexOf(person, number));
}


void KEduVocConjugation::setConjugation(const QString & conjugation, int index)
{
    if ( !conjugation.isEmpty() ) {
        d->m_conjugations[index] = conjugation;
    } else {
        // if we received an empty string, remove the element.
        if ( d->m_conjugations.contains(index) ) {
            d->m_conjugations.remove(index);
        }
    }
}


int KEduVocConjugation::indexOf(ConjugationPerson person, ConjugationNumber number)
{
    return person + PersonMAX * number;
}


bool KEduVocConjugation::isEmpty()
{
    return d->m_conjugations.count() == 0;
}


KEduVocGrade & KEduVocConjugation::grade()
{
    return d->m_grade;
}

QList< int > KEduVocConjugation::keys()
{
    return d->m_conjugations.keys();
}



