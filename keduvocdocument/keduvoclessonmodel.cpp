/***************************************************************************

    Copyright 2008-2009 Frederik Gladhorn <gladhorn@kde.org>
    Copyright 2015 Rahul Chowdhury <rahul.chowdhury@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvoclessonmodel.h"

#include <KRandom>
#include <KLocalizedString>
#include <QtGui/QFont>

/** @file
  * Implementation of LessonModel.
  * Functions to create the model from the lessons of the vocabulary document.
  */


KEduVocLessonModel::KEduVocLessonModel(QObject * parent)
    : KEduVocContainerModel(KEduVocContainer::Lesson, parent)
    , d(0)
{
}

KEduVocLessonModel::~KEduVocLessonModel()
{
    delete d;
}

KEduVocContainer * KEduVocLessonModel::rootContainer() const
{
    if (!document()) {
        return 0;
    }
    return document()->lesson();
}

Qt::ItemFlags KEduVocLessonModel::flags(const QModelIndex &index) const
{
    if (index.isValid() && index.parent() == QModelIndex()) {
        return (Qt::ItemIsEnabled
                | Qt::ItemIsEditable
                | Qt::ItemIsSelectable);
    }

    // the name column should be checkable to select lessons for practice
    return  KEduVocContainerModel::flags(index);
}

QVariant KEduVocLessonModel::data(const QModelIndex & index, int role) const
{
    if (index.isValid() && !index.parent().isValid()) {
        if (index.column() == 0) {
            switch (role) {
            case Qt::DisplayRole:
                return i18nc("display of the name of the vocabulary collection", "Collection: %1", KEduVocContainerModel::data(index, role).toString());
            case Qt::FontRole:
                QFont f;
                f.setBold(true);
                return f;
            }
        }
    }
    return KEduVocContainerModel::data(index, role);
}

bool KEduVocLessonModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && !index.parent().isValid()) {
        if (index.column() == ContainerNameColumn && role == Qt::EditRole) {
            ///@todo decouple the root lesson and document title
            document()->setTitle(value.toString());
        }
    }
    return KEduVocContainerModel::setData(index, value, role);
}

void KEduVocLessonModel::splitLesson(const QModelIndex& containerIndex, int entriesPerLesson, SplitLessonOrder order)
{
    if (!containerIndex.isValid()) {
        return;
    }

    if (static_cast<KEduVocContainer*>(containerIndex.internalPointer())->containerType() != KEduVocContainer::Lesson) {
        return;
    }

    KEduVocLesson* parentLesson = static_cast<KEduVocLesson*>(containerIndex.internalPointer());

    int numNewLessons = parentLesson->entryCount() / entriesPerLesson;
    // modulo - fraction lesson if not 0 we need one more
    if (parentLesson->entryCount() % entriesPerLesson) {
        numNewLessons++;
    }

    while (parentLesson->entryCount() > 0) {
        beginInsertRows(containerIndex, parentLesson->entryCount(), parentLesson->entryCount());
        KEduVocLesson* child = new KEduVocLesson(parentLesson->name()
                + QString(" %1").arg(parentLesson->childContainerCount() + 1), parentLesson);
        parentLesson->appendChildContainer(child);
        endInsertRows();

        while (parentLesson->entryCount() > 0 && child->entryCount() < entriesPerLesson) {
            // next entry to be assigned to one of the new lessons
            int nextEntry = 0;
            if (order == Random) {
                nextEntry = KRandom::random() % parentLesson->entryCount();
                child->appendEntry(parentLesson->entry(nextEntry));
            }
        }
    }
}
