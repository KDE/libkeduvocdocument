/***************************************************************************

    Copyright 2007 Jeremy Whiting <jpwhiting@kde.org>
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

#include <QtCore/QList>

/** private class to store information about a lesson */
class KEduVocLesson::Private
{
public:
    // entries
    QList<KEduVocExpression*> m_entries;
};


KEduVocLesson::KEduVocLesson(const QString& name, KEduVocContainer *parent)
        : KEduVocContainer(name, Lesson, parent), d( new Private )
{
}

// Private constructor only used by KEduVocDocument when creating the top level lesson.
KEduVocLesson::KEduVocLesson(const QString& name, KEduVocDocument *document)
   : KEduVocContainer(name, Lesson, document)
   , d(new Private)
{
}


KEduVocLesson::KEduVocLesson( const KEduVocLesson &other )
        : KEduVocContainer(other), d( new Private )
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
    if (d->m_entries.indexOf(entry) == -1) {
        //attempting to remove entry from lesson that does not contain it!
        return;
    }
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


