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

#include <KRandomSequence>
#include <QList>

/** private class to store information about a lesson */
class KEduVocLesson::Private
{
public:
    // entries
    QList<KEduVocExpression*> m_entries;
};


KEduVocLesson::KEduVocLesson(const QString& name, KEduVocContainer *parent)
        : d( new Private ), KEduVocContainer(name, Lesson, parent)
{
}


KEduVocLesson::KEduVocLesson( const KEduVocLesson &other )
        : d( new Private ), KEduVocContainer(other)
{
    d->m_entries = other.d->m_entries;
}


KEduVocLesson::~KEduVocLesson()
{
    foreach (KEduVocExpression* entry, d->m_entries) {
        entry->removeLesson(this);
        if (entry->lessons().count() == 0) {
            delete entry;
        }
    }
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

void KEduVocLesson::appendEntry(KEduVocExpression* entry)
{
    d->m_entries.append( entry );
    entry->addLesson(this);
}

void KEduVocLesson::insertEntry(int index, KEduVocExpression * entry)
{
    d->m_entries.insert( index, entry );
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

void KEduVocLesson::randomizeEntries()
{
    KRandomSequence randomSequence(QDateTime::currentDateTime().toTime_t());
    randomSequence.randomize( d->m_entries );
}

