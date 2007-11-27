/***************************************************************************

    Copyright 2007 Jeremy Whiting <jeremywhiting@scitools.com>
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

#include "keduvoclesson.h"

#include "keduvocexpression.h"

#include <QList>

/** private class to store information about a lesson */
class KEduVocLesson::Private
{
public:
    ~Private();

    // entries
    QList<KEduVocExpression*> m_entries;
};

KEduVocLesson::Private::~ Private()
{
    ///@todo delete children here???? is this a 1:1 mapping?
    qDeleteAll(m_entries);
}

KEduVocLesson::KEduVocLesson(const QString& name, KEduVocLesson *parent)
        : d( new Private ), KEduVocContainer(name, LessonContainer, parent)
{
}

KEduVocLesson::KEduVocLesson( const KEduVocLesson &other )
        : d( new Private ), KEduVocContainer(other)
{
    d->m_entries = other.d->m_entries;
}

KEduVocLesson::~KEduVocLesson()
{
    delete d;
}


QList<KEduVocExpression*> KEduVocLesson::entries()
{
    return d->m_entries;
}

int KEduVocLesson::entryCount()
{
    return d->m_entries.count();
}

void KEduVocLesson::addEntry(KEduVocExpression* entry)
{
    d->m_entries.append( entry );
    entry->addLesson(this);
}

void KEduVocLesson::removeEntry(KEduVocExpression* entry)
{
    d->m_entries.removeAt( d->m_entries.indexOf(entry) );
    entry->removeLesson(this);
}

KEduVocExpression * KEduVocLesson::entry(int row)
{
    return d->m_entries.value(row);
}

