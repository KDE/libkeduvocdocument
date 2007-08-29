/***************************************************************************
              manage lessons
    -----------------------------------------------------------------------

    begin        : August 11, 2007

    copyright    : (C) 2007 Jeremy Whiting <jeremywhiting@scitools.com>

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

#include "keduvoclesson.h"

#include <QSet>

/** private class to store information about a lesson */
class KEduVocLesson::Private
{
public:
    QSet<int> m_entries;
    QString m_name;
};

KEduVocLesson::KEduVocLesson()
        : d( new Private )
{}

KEduVocLesson::KEduVocLesson( const KEduVocLesson &other )
        : d( new Private )
{
    d->m_entries = other.d->m_entries;
    d->m_name = other.d->m_name;
}

KEduVocLesson::~KEduVocLesson()
{
    delete d;
}

KEduVocLesson& KEduVocLesson::operator= ( const KEduVocLesson &other )
{
    d->m_entries = other.d->m_entries;
    d->m_name = other.d->m_name;
    return *this;
}

void KEduVocLesson::setName( const QString &name )
{
    d->m_name = name;
}

QString KEduVocLesson::name()
{
    return d->m_name;
}

QList<int> KEduVocLesson::entries()
{
    return d->m_entries.toList();
}

void KEduVocLesson::addEntry( int entryid )
{
    d->m_entries.insert( entryid );
}

void KEduVocLesson::removeEntry( int entryid )
{
    d->m_entries.remove( entryid );
}
