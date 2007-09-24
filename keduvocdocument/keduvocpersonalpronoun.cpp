/***************************************************************************
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvocpersonalpronoun.h"
#include "keduvoccommon_p.h"
#include <KLocalizedString>
#include <KDebug>

class KEduVocPersonalPronoun::Private
{
public:
    Private();

    bool m_maleFemaleDifferent;
    bool m_neuterExists;
    bool m_dualExists;
    QMap<int, QString> m_personalpronouns;
};


KEduVocPersonalPronoun::Private::Private()
{
    m_maleFemaleDifferent = false;
    m_neuterExists = false;
    m_dualExists = false;
}

KEduVocPersonalPronoun::KEduVocPersonalPronoun()
        : d( new Private )
{}


KEduVocPersonalPronoun::KEduVocPersonalPronoun( const KEduVocPersonalPronoun& other )
        : d( new Private )
{
    d->m_maleFemaleDifferent = other.d->m_maleFemaleDifferent;
    d->m_neuterExists = other.d->m_neuterExists;
    d->m_personalpronouns = other.d->m_personalpronouns;
    d->m_dualExists = other.d->m_dualExists;
}


KEduVocPersonalPronoun::~KEduVocPersonalPronoun()
{
    delete d;
}


KEduVocPersonalPronoun& KEduVocPersonalPronoun::operator = ( const KEduVocPersonalPronoun& other )
{
    d->m_maleFemaleDifferent = other.d->m_maleFemaleDifferent;
    d->m_neuterExists = other.d->m_neuterExists;
    d->m_personalpronouns = other.d->m_personalpronouns;
    d->m_dualExists = other.d->m_dualExists;
    return *this;
}


bool KEduVocPersonalPronoun::operator ==(const KEduVocPersonalPronoun& other) const
{
    return d->m_personalpronouns == other.d->m_personalpronouns &&
        d->m_maleFemaleDifferent == other.d->m_maleFemaleDifferent &&
        d->m_neuterExists == other.d->m_neuterExists;
        d->m_dualExists == other.d->m_dualExists;
}


QString KEduVocPersonalPronoun::personalPronoun(KEduVocConjugation::ConjugationPerson person, KEduVocConjugation::ConjugationNumber number) const
{
    int index = indexOf(person, number);
    if ( d->m_personalpronouns.contains(index) ) {
        return d->m_personalpronouns.value(index);
    }
    return QString();
}

void KEduVocPersonalPronoun::setPersonalPronoun(const QString & personalpronoun, KEduVocConjugation::ConjugationPerson person, KEduVocConjugation::ConjugationNumber number)
{
    d->m_personalpronouns[indexOf(person, number)] = personalpronoun;
}


int KEduVocPersonalPronoun::indexOf(KEduVocConjugation::ConjugationPerson person, KEduVocConjugation::ConjugationNumber number) const
{
    return person + KEduVocConjugation::PersonMAX * number;
}

bool KEduVocPersonalPronoun::maleFemaleDifferent() const
{
    return d->m_maleFemaleDifferent;
}

void KEduVocPersonalPronoun::setMaleFemaleDifferent(bool different)
{
    d->m_maleFemaleDifferent = different;
}

bool KEduVocPersonalPronoun::neuterExists() const
{
    return d->m_neuterExists;
}

void KEduVocPersonalPronoun::setNeuterExists(bool exists)
{
    d->m_neuterExists = exists;
}

bool KEduVocPersonalPronoun::dualExists() const
{
    return d->m_dualExists;
}

void KEduVocPersonalPronoun::setDualExists(bool exists)
{
    d->m_dualExists = exists;
}

