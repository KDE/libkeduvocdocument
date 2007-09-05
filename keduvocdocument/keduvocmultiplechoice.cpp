/***************************************************************************

              manage multiple choice suggestions for queries

    -----------------------------------------------------------------------

    begin          : Mon Oct 29 18:09:29 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include "keduvocmultiplechoice.h"

class KEduVocMultipleChoice::KEduVocMultipleChoicePrivate
{
public:
    QStringList m_choices;
};

KEduVocMultipleChoice::KEduVocMultipleChoice()
        : d( new KEduVocMultipleChoicePrivate )
{}

KEduVocMultipleChoice::KEduVocMultipleChoice( const KEduVocMultipleChoice &other )
        : d( new KEduVocMultipleChoicePrivate )
{
    d->m_choices = other.choices();
}

KEduVocMultipleChoice::KEduVocMultipleChoice( const QStringList &choices )
        : d( new KEduVocMultipleChoicePrivate )
{
    setChoices( choices );
}

KEduVocMultipleChoice::~KEduVocMultipleChoice()
{
    delete d;
}

KEduVocMultipleChoice &KEduVocMultipleChoice::operator= ( const KEduVocMultipleChoice &other )
{
    d->m_choices = other.choices();
    return *this;
}

void KEduVocMultipleChoice::setChoices( const QStringList &choices )
{
    d->m_choices = choices;
}

QStringList KEduVocMultipleChoice::choices() const
{
    return d->m_choices;
}

bool KEduVocMultipleChoice::isEmpty() const
{
    return d->m_choices.isEmpty();
}


void KEduVocMultipleChoice::clear()
{
    d->m_choices.clear();
}


QString KEduVocMultipleChoice::choice( int index ) const
{
    QString choice;
    if ( d->m_choices.size() > index ) {
        choice = d->m_choices[index];
    }
    return choice;
}

void KEduVocMultipleChoice::setChoice( int index, const QString &s )
{
    while ( d->m_choices.size() <= index ) {
        d->m_choices.append( QString() );
    }
    d->m_choices[index] = s;
}

unsigned KEduVocMultipleChoice::size() const
{
    return d->m_choices.size();
}

bool KEduVocMultipleChoice::operator== ( const KEduVocMultipleChoice &choice ) const
{
    return d->m_choices == choice.choices();
}

void KEduVocMultipleChoice::appendChoice( const QString &s )
{
    d->m_choices.append( s );
}
