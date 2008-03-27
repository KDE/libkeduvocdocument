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
    qDeleteAll(d->m_entries);
    delete d;
}

QList<KEduVocExpression*> KEduVocLesson::entries(EnumEntriesRecursive recursive)
{
    if (recursive == Recursive) {
        return entriesRecursive();
    }
    return d->m_entries;
}

int KEduVocLesson::entryCount(EnumEntriesRecursive recursive)
{
    if (recursive == Recursive) {
        return entriesRecursive().count();
    }
    return d->m_entries.count();
}

void KEduVocLesson::appendEntry(KEduVocExpression* entry)
{
    insertEntry(d->m_entries.size(), entry);
}

void KEduVocLesson::insertEntry(int index, KEduVocExpression * entry)
{
    Q_ASSERT(entry);
    d->m_entries.insert( index, entry );
    entry->setLesson(this);
    invalidateChildLessonEntries();
}

void KEduVocLesson::removeEntry(KEduVocExpression* entry)
{
    Q_ASSERT(entry);
    d->m_entries.removeAt( d->m_entries.indexOf(entry) );
    invalidateChildLessonEntries();
}

KEduVocExpression * KEduVocLesson::entry(int row, EnumEntriesRecursive recursive)
{
    if (recursive == Recursive) {
        return entriesRecursive().value(row);
    }
    return d->m_entries.value(row);
}


