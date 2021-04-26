/*
 * SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "keduvocpersonalpronoun.h"
#include "keduvoccommon_p.h"

#include <QMap>

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
        QString p = d->m_personalpronouns.value(flags & (KEduVocWordFlag::persons | KEduVocWordFlag::numbers| KEduVocWordFlag::genders));
        if (p.isEmpty() && !(flags & KEduVocWordFlag::genders) && d->m_maleFemaleDifferent && d->m_neutralExists)
        {
           //initial flag lookup failed, adding neuter gender flag
           flags |= KEduVocWordFlag::Neuter;
           p = d->m_personalpronouns.value(flags & (KEduVocWordFlag::persons | KEduVocWordFlag::numbers| KEduVocWordFlag::genders));
        }
        return p;
}

void KEduVocPersonalPronoun::setPersonalPronoun(const QString & personalpronoun, KEduVocWordFlags flags)
{
        d->m_personalpronouns[flags & (KEduVocWordFlag::persons | KEduVocWordFlag::numbers| KEduVocWordFlag::genders)] = personalpronoun;
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

