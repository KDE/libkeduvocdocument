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
    // bitvector of word type flags
    KEduVocWordFlags m_flags;
    QList<KEduVocExpression*> m_expressions;
    // list of translations
    QList<KEduVocTranslation*> m_translations;
};

KEduVocWordType::KEduVocWordType(const QString& name, KEduVocWordType *parent)
        : KEduVocContainer(name, WordType, parent), d( new Private )
{}

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

    return d->m_expressions;
}

int KEduVocWordType::entryCount(EnumEntriesRecursive recursive)
{
    if (recursive == Recursive) {
        return entriesRecursive().count();
    }
    return d->m_expressions.count();
}

void KEduVocWordType::addTranslation(KEduVocTranslation* translation)
{
    // add to expression - if not already there because another translation of the same word is there.
    bool found = false;
    foreach(int i, translation->entry()->translationIndices()) {
        if (translation->entry()->translation(i)->wordType() == this) {
            found = true;
            break;
        }
    }
    if (!found) {
        d->m_expressions.append(translation->entry());
    }
    d->m_translations.append( translation );
    invalidateChildLessonEntries();
}

void KEduVocWordType::removeTranslation(KEduVocTranslation* translation)
{
    d->m_translations.removeAt( d->m_translations.indexOf(translation) );

    // no lesson found - this entry is being deleted. remove all its siblings.
    if (!translation->entry()->lesson()) {
        int index = d->m_expressions.indexOf(translation->entry());
        if (index != -1) {
            d->m_expressions.removeAt(index);
        }
    }

    // remove from cache
    bool found = false;
    foreach(int i, translation->entry()->translationIndices()) {
        if (translation->entry()->translation(i)->wordType() && translation->entry()->translation(i)->wordType() == this) {
            found = true;
            break;
        }
    }
    if (!found) {
        d->m_expressions.removeAt(d->m_expressions.indexOf(translation->entry()));
    }

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

KEduVocWordFlags KEduVocWordType::wordType() const
{
    return d->m_flags;
}

void KEduVocWordType::setWordType(KEduVocWordFlags flags)
{
    d->m_flags = flags;
}

KEduVocWordType* KEduVocWordType::childOfType(const KEduVocWordFlags& flags)
{
    if(d->m_flags == flags) {
        return this;
    }
    foreach(KEduVocContainer* child, childContainers()) {
        KEduVocWordType* result = static_cast<KEduVocWordType*>(child)->childOfType(flags);
        if(result) {
            return result;
        }
    }
    return 0;
}

