/*
 * manage multiple choice suggestions for queries
 * SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
 * SPDX-FileCopyrightText: 2005 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

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
    foreach ( const QString &choice, choices ) {
        // use appendChoice to check for empty entries
        appendChoice(choice);
    }
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
    if ( !s.isEmpty() ) {
        d->m_choices.append( s );
    }
}
