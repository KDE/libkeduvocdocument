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
    bool m_neutralExists;
    bool m_dualExists;
    QMap<KEduVocWordFlags, QString> m_personalpronouns;
};


KEduVocPersonalPronoun::Private::Private()
{
    m_maleFemaleDifferent = false;
    m_neutralExists = false;
    m_dualExists = false;
}

KEduVocPersonalPronoun::KEduVocPersonalPronoun()
        : d( new Private )
{}


KEduVocPersonalPronoun::KEduVocPersonalPronoun( const KEduVocPersonalPronoun& other )
        : d( new Private )
{
    d->m_maleFemaleDifferent = other.d->m_maleFemaleDifferent;
    d->m_neutralExists = other.d->m_neutralExists;
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
    d->m_neutralExists = other.d->m_neutralExists;
    d->m_personalpronouns = other.d->m_personalpronouns;
    d->m_dualExists = other.d->m_dualExists;
    return *this;
}


bool KEduVocPersonalPronoun::operator ==(const KEduVocPersonalPronoun& other) const
{
    return d->m_personalpronouns == other.d->m_personalpronouns &&
        d->m_maleFemaleDifferent == other.d->m_maleFemaleDifferent &&
        d->m_neutralExists == other.d->m_neutralExists &&
        d->m_dualExists == other.d->m_dualExists;
}


QString KEduVocPersonalPronoun::personalPronoun(KEduVocWordFlags flags) const
{
        return d->m_personalpronouns.value(flags & (KEduVocWordFlag::persons | KEduVocWordFlag::numbers));
}

void KEduVocPersonalPronoun::setPersonalPronoun(const QString & personalpronoun, KEduVocWordFlags flags)
{
        d->m_personalpronouns[flags & (KEduVocWordFlag::persons | KEduVocWordFlag::numbers)] = personalpronoun;
}


bool KEduVocPersonalPronoun::maleFemaleDifferent() const
{
    return d->m_maleFemaleDifferent;
}

void KEduVocPersonalPronoun::setMaleFemaleDifferent(bool different)
{
    d->m_maleFemaleDifferent = different;
}

bool KEduVocPersonalPronoun::neutralExists() const
{
    return d->m_neutralExists;
}

void KEduVocPersonalPronoun::setNeutralExists(bool exists)
{
    d->m_neutralExists = exists;
}

bool KEduVocPersonalPronoun::dualExists() const
{
    return d->m_dualExists;
}

void KEduVocPersonalPronoun::setDualExists(bool exists)
{
    d->m_dualExists = exists;
}

