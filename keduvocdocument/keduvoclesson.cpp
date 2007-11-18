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

#include "keduvocexpression.h"

#include <QList>

/** private class to store information about a lesson */
class KEduVocLesson::Private
{
public:
    ~Private();

    // properties for this lesson
    QString m_name;
    bool m_inPractice;

    // other lessons in the tree
    KEduVocLesson *m_parentLesson;
    QList<KEduVocLesson*> m_childLessons;

    // entries
    QList<KEduVocExpression*> m_entries;
};

KEduVocLesson::Private::~ Private()
{
    qDeleteAll(m_childLessons);

    ///@todo delete children here???? is this a 1:1 mapping?
    qDeleteAll(m_entries);
}

KEduVocLesson::KEduVocLesson(const QString& name, KEduVocLesson *parent)
        : d( new Private )
{
    d->m_parentLesson = parent;
    d->m_name = name;
}

KEduVocLesson::KEduVocLesson( const KEduVocLesson &other )
        : d( new Private )
{
    d->m_entries = other.d->m_entries;
    d->m_name = other.d->m_name;
    d->m_inPractice = other.d->m_inPractice;
}

KEduVocLesson::~KEduVocLesson()
{
    delete d;
}

void KEduVocLesson::appendChildLesson(KEduVocLesson * child)
{
    d->m_childLessons.append(child);
}

KEduVocLesson * KEduVocLesson::childLesson(int row)
{
    return d->m_childLessons.value(row);
}

int KEduVocLesson::childLessonCount() const
{
    return d->m_childLessons.count();
}

int KEduVocLesson::row() const
{
    if (d->m_parentLesson) {
        return d->m_parentLesson->d->m_childLessons.indexOf(const_cast<KEduVocLesson*>(this));
    }
    return 0;
}


KEduVocLesson& KEduVocLesson::operator= ( const KEduVocLesson &other )
{
    d->m_entries = other.d->m_entries;
    d->m_name = other.d->m_name;
    d->m_inPractice = other.d->m_inPractice;
    return *this;
}

bool KEduVocLesson::operator==(const KEduVocLesson &other)
{
    return d->m_entries == other.d->m_entries &&
        d->m_name == other.d->m_name &&
        d->m_inPractice == other.d->m_inPractice;;
}

void KEduVocLesson::setName( const QString &name )
{
    d->m_name = name;
}

QString KEduVocLesson::name()
{
    return d->m_name;
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


bool KEduVocLesson::inPractice()
{
    return d->m_inPractice;
}

void KEduVocLesson::setInPractice(bool inPractice)
{
    d->m_inPractice = inPractice;
}

void KEduVocLesson::removeTranslation(int translation)
{
    foreach(KEduVocLesson *childLesson, d->m_childLessons) {
        childLesson->removeTranslation(translation);
    }

    foreach(KEduVocExpression *entry, d->m_entries ) {
        entry->removeTranslation( translation );
    }
}

QList< KEduVocExpression * > KEduVocLesson::entriesRecursive()
{
    QList< KEduVocExpression * > entryList = entries();
    foreach(KEduVocLesson *childLesson, d->m_childLessons) {
        foreach(KEduVocExpression *childEntry, childLesson->entriesRecursive()) {
            if(!entryList.contains(childEntry)) {
                entryList.append(childEntry);
            }
        }
    }
}

QList< KEduVocLesson * > KEduVocLesson::childLessons()
{
    return d->m_childLessons;
}

KEduVocExpression * KEduVocLesson::entry(int row)
{
    return d->m_entries.value(row);
}

KEduVocLesson * KEduVocLesson::parent()
{
    return d->m_parentLesson;
}

