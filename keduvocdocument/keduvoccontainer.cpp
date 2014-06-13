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

#include "keduvoccontainer.h"

#include "keduvocexpression.h"

#include <QDebug>
#include <QList>

/** private class to store information about a lesson */
class KEduVocContainer::Private
{
public:
    ~Private();

    // properties for this lesson
    QString m_name;
    bool m_inPractice;

    // other lessons in the tree
    KEduVocContainer *m_parentContainer;
    QList < KEduVocContainer * > m_childContainers;

    EnumContainerType m_type;

    QList < KEduVocExpression* > m_childLessonEntries;
    bool m_childLessonEntriesValid;

    /// Image url
    QUrl m_imageUrl;
};

KEduVocContainer::Private::~Private()
{
    qDeleteAll(m_childContainers);
}

KEduVocContainer::KEduVocContainer(const QString& name, EnumContainerType type, KEduVocContainer *parent)
        : d( new Private )
{
    d->m_parentContainer = parent;
    d->m_name = name;
    d->m_inPractice = true;
    d->m_type = type;
    d->m_childLessonEntriesValid = false;
}

KEduVocContainer::KEduVocContainer( const KEduVocContainer &other )
        : d( new Private )
{
    d->m_name = other.d->m_name;
    d->m_inPractice = other.d->m_inPractice;
    d->m_type = other.d->m_type;
    d->m_parentContainer = other.d->m_parentContainer;
    d->m_childLessonEntriesValid = false;
}

KEduVocContainer::~KEduVocContainer()
{
    delete d;
}

void KEduVocContainer::appendChildContainer(KEduVocContainer * child)
{
    d->m_childContainers.append(child);
    child->d->m_parentContainer = this;

    invalidateChildLessonEntries();
}

KEduVocContainer * KEduVocContainer::childContainer(int row)
{
    return d->m_childContainers.value(row);
}


KEduVocContainer * KEduVocContainer::childContainer(const QString & name)
{
    if (d->m_name == name) {
        return this;
    }

    foreach (KEduVocContainer *container, d->m_childContainers) {
        KEduVocContainer *found = container->childContainer(name);
        if (found) {
            return found;
        }
    }
    return 0;
}


void KEduVocContainer::deleteChildContainer(int row)
{
    qDebug() << "Delete of container - check entry deletion!";
    delete d->m_childContainers.takeAt(row);

    invalidateChildLessonEntries();
}

void KEduVocContainer::removeChildContainer(int row)
{
    d->m_childContainers.removeAt(row);
    invalidateChildLessonEntries();
}


int KEduVocContainer::childContainerCount() const
{
    return d->m_childContainers.count();
}

int KEduVocContainer::row() const
{
    if (d->m_parentContainer) {
        return d->m_parentContainer->d->m_childContainers.indexOf(const_cast<KEduVocContainer*>(this));
    }
    return 0;
}


KEduVocContainer& KEduVocContainer::operator= ( const KEduVocContainer &other )
{
    d->m_name = other.d->m_name;
    d->m_inPractice = other.d->m_inPractice;
    return *this;
}

bool KEduVocContainer::operator==(const KEduVocContainer &other)
{
    return  d->m_name == other.d->m_name &&
            d->m_inPractice == other.d->m_inPractice
/// @todo make this return something useful
            ;
}

void KEduVocContainer::setName( const QString &name )
{
    d->m_name = name;
}

QString KEduVocContainer::name()
{
    return d->m_name;
}

bool KEduVocContainer::inPractice()
{
    return d->m_inPractice;
}

void KEduVocContainer::setInPractice(bool inPractice)
{
    d->m_inPractice = inPractice;
}

void KEduVocContainer::removeTranslation(int translation)
{
    foreach(KEduVocContainer *childContainer, d->m_childContainers) {
        childContainer->removeTranslation(translation);
    }

    foreach(KEduVocExpression *entry, entries() ) {
        entry->removeTranslation( translation );
    }
}

QList< KEduVocExpression * > KEduVocContainer::entriesRecursive()
{
    if (!d->m_childLessonEntriesValid) {
        updateChildLessonEntries();
    }
    return d->m_childLessonEntries;
}

QList< KEduVocContainer * > KEduVocContainer::childContainers()
{
    return d->m_childContainers;
}

KEduVocContainer * KEduVocContainer::parent()
{
    return d->m_parentContainer;
}

void KEduVocContainer::setContainerType(KEduVocContainer::EnumContainerType type)
{
    d->m_type = type;
}

KEduVocContainer::EnumContainerType KEduVocContainer::containerType()
{
    return d->m_type;
}


QUrl KEduVocContainer::imageUrl()
{
    return d->m_imageUrl;
}

void KEduVocContainer::setImageUrl(const QUrl &url)
{
    d->m_imageUrl = url;
}

void KEduVocContainer::insertChildContainer(int row, KEduVocContainer * child)
{
    d->m_childContainers.insert(row, child);
    child->d->m_parentContainer = this;

    invalidateChildLessonEntries();
}

void KEduVocContainer::updateChildLessonEntries()
{
    QList < KEduVocExpression* > entriesRecursive = entries();

    foreach(KEduVocContainer *childContainer, d->m_childContainers)
        foreach(KEduVocExpression * expr, childContainer->entries(Recursive))
            entriesRecursive.append(expr);

    d->m_childLessonEntries = entriesRecursive;
    d->m_childLessonEntriesValid = true;
}

void KEduVocContainer::invalidateChildLessonEntries()
{
    d->m_childLessonEntriesValid = false;
    // propagate to parent
    if (d->m_parentContainer) {
        d->m_parentContainer->invalidateChildLessonEntries();
    }
}

double KEduVocContainer::averageGrade(int translation, EnumEntriesRecursive recursive)
{
    // grades range from 0..7 right now
    int sum = 0;
    foreach (KEduVocExpression *entry, entries(recursive)) {
        sum += entry->translation(translation)->grade();
    }
    // make that a percentage
    return (sum * 100.0/7.0)/entryCount(recursive);
}

int KEduVocContainer::expressionsOfGrade(int translation, grade_t grade, EnumEntriesRecursive recursive)
{
    int sum = 0;
    foreach (KEduVocExpression *entry, entries(recursive)) {
        if (entry->translation(translation)->grade() == grade) {
            sum++;
        }
    }
    return sum;
}

void KEduVocContainer::resetGrades(int translation, EnumEntriesRecursive recursive)
{
    foreach (KEduVocExpression *entry, entries(recursive)) {
        entry->resetGrades(translation);
    }
}

