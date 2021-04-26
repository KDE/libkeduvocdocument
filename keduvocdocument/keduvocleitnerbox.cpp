/*
 * SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "keduvocleitnerbox.h"

#include "keduvocexpression.h"

#include <QSet>

class KEduVocLeitnerBox::Private
{
public:
    // cache the entries
    QList<KEduVocExpression*> m_expressions;
    // list of translations
    QList<KEduVocTranslation*> m_translations;
};

KEduVocLeitnerBox::KEduVocLeitnerBox(const QString& name, KEduVocLeitnerBox *parent)
        : KEduVocContainer(name, Leitner, parent), d( new Private )
{
    // only one top level and children, this is only a list
    Q_ASSERT(!parent || !parent->parent());
}

KEduVocLeitnerBox::~KEduVocLeitnerBox()
{
    foreach(KEduVocTranslation* translation, d->m_translations) {
        translation->setLeitnerBox(0);
    }
    delete d;
}

QList<KEduVocExpression*> KEduVocLeitnerBox::entries(EnumEntriesRecursive recursive)
{
    Q_UNUSED(recursive)
    return d->m_expressions;
}

int KEduVocLeitnerBox::entryCount(EnumEntriesRecursive recursive)
{
    Q_UNUSED(recursive)
    return d->m_expressions.count();
}

void KEduVocLeitnerBox::addTranslation(KEduVocTranslation* translation)
{
    // add to expression - if not already there because another translation of the same word is there.
    bool found = false;
    foreach(int i, translation->entry()->translationIndices()) {
        if (translation->entry()->translation(i)->leitnerBox() == this) {
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

void KEduVocLeitnerBox::removeTranslation(KEduVocTranslation* translation)
{
    int index = d->m_translations.indexOf(translation);
    d->m_translations.removeAt(index);

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
        if (translation->entry()->translation(i)->leitnerBox() == this) {
            found = true;
            break;
        }
    }
    if (!found) {
        d->m_expressions.removeAt(d->m_expressions.indexOf(translation->entry()));
    }

    invalidateChildLessonEntries();
}

KEduVocTranslation * KEduVocLeitnerBox::translation(int row)
{
    return d->m_translations.value(row);
}

KEduVocExpression * KEduVocLeitnerBox::entry(int row, EnumEntriesRecursive recursive)
{
    Q_UNUSED(recursive)
    return entries().value(row);
}

