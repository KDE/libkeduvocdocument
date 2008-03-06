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

#include "keduvocwordtype.h"

#include "keduvocexpression.h"

#include <KDebug>

#include <QtCore/QList>
#include <QtCore/QSet>

class KEduVocWordType::Private
{
public:
    EnumWordType m_wordType;
    // entries
    QList<KEduVocTranslation*> m_translations;
};

KEduVocWordType::KEduVocWordType(const QString& name, KEduVocWordType *parent)
        : d( new Private ), KEduVocContainer(name, WordType, parent)
{
    d->m_wordType = General;
}

KEduVocWordType::~KEduVocWordType()
{
    foreach(KEduVocTranslation* translation, d->m_translations) {
        translation->setWordType(0);
    }
    delete d;
}

QList<KEduVocExpression*> KEduVocWordType::entries(EnumEntriesRecursive recursive)
{
    if (recursive == Recursive) {
        return entriesRecursive();
    }
    /// FIXME this is recalculated every time, very inefficient!
    /// cache entries here, only update on dirty
    QSet<KEduVocExpression*> entries;
    foreach(KEduVocTranslation* translation, d->m_translations) {
        entries.insert(translation->entry());
    }
    return entries.toList();
}

int KEduVocWordType::entryCount(EnumEntriesRecursive recursive)
{
    if (recursive == Recursive) {
        return entriesRecursive().count();
    }
    return entries().count();
}

void KEduVocWordType::addTranslation(KEduVocTranslation* translation)
{
    d->m_translations.append( translation );
    invalidateChildLessonEntries();
}

void KEduVocWordType::removeTranslation(KEduVocTranslation* translation)
{
    d->m_translations.removeAt( d->m_translations.indexOf(translation));
    invalidateChildLessonEntries();
}

KEduVocTranslation * KEduVocWordType::translation(int row)
{

    return d->m_translations.value(row);
}

KEduVocExpression * KEduVocWordType::entry(int row, EnumEntriesRecursive recursive)
{
    if (recursive == Recursive) {
        return entriesRecursive().value(row);
    }
    return entries().value(row);
}

void KEduVocWordType::setWordType(EnumWordType type)
{
    d->m_wordType = type;
}

KEduVocWordType::EnumWordType KEduVocWordType::wordType() const
{
    return d->m_wordType;
}

KEduVocWordType* KEduVocWordType::childOfType(KEduVocWordType::EnumWordType type)
{
    if(wordType()==type) {
        return this;
    }
    foreach(KEduVocContainer* child, childContainers()) {
        KEduVocWordType* result = static_cast<KEduVocWordType*>(child)->childOfType(type);
        if(result) {
            return result;
        }
    }
    return 0;
}

