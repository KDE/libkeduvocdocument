/***************************************************************************

    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvoccontainermodel.h"
#include "keduvoccontainer.h"

#include "keduvoccontainermimedata.h"
#include "keduvocvocabularymimedata.h"

#include <keduvocdocument.h>
#include <keduvoclesson.h>
#include <keduvocwordtype.h>
#include <keduvocexpression.h>

#include <QDebug>
#include <KLocalizedString>

/** @file
  * Implementation of ContainerModel.
  * Functions to create the model from the lessons of the vocabulary document.
  */

KEduVocContainerModel::KEduVocContainerModel(KEduVocContainer::EnumContainerType type, QObject * parent)
    : KEduVocReadonlyContainerModel(type, parent)
    , d(0)
{
}

KEduVocContainerModel::~KEduVocContainerModel()
{
    delete d;
}

QModelIndex KEduVocContainerModel::appendContainer(const QModelIndex& parent, const QString & containerName)
{
    KEduVocContainer* parentContainer;
    if (parent.isValid()) {
        parentContainer = static_cast<KEduVocContainer*>(parent.internalPointer());
    } else {
        return QModelIndex();
    }

    beginInsertRows(parent, parentContainer->childContainerCount(),
                    parentContainer->childContainerCount());
    switch (containerType()) {
        case (KEduVocContainer::Lesson):
            parentContainer->appendChildContainer(new KEduVocLesson(containerName, static_cast<KEduVocLesson*>(parentContainer)));
            break;
        case (KEduVocContainer::WordType): {
            KEduVocWordType* parentWordType = static_cast<KEduVocWordType*>(parentContainer);
            KEduVocWordType* wordTypeContainer = new KEduVocWordType(containerName, parentWordType);
            wordTypeContainer->setWordType(parentWordType->wordType());
            parentContainer->appendChildContainer(wordTypeContainer);
            break;
        }
        default:
            break;
    }
    endInsertRows();

    return index(parentContainer->childContainerCount() - 1, 0, parent);
}


QVariant KEduVocContainerModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    KEduVocContainer *container = static_cast<KEduVocContainer*>(index.internalPointer());

    switch (index.column()) {
    case ContainerNameColumn:
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            return container->name();
        }

        if (role == Qt::TextAlignmentRole) {
            return Qt::AlignLeft;
        }
        break;
    case TotalCountColumn:
        if (role == Qt::DisplayRole) {
            return container->entryCount(KEduVocContainer::Recursive);
        }
        if (role == Qt::TextAlignmentRole) {
            return Qt::AlignRight;
        }
        break;
    default:
        if (role == Qt::TextAlignmentRole) {
            return Qt::AlignRight;
        }
    }

    return QVariant();
}

bool KEduVocContainerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    if (index.column() == ContainerNameColumn) {
        KEduVocContainer *container = static_cast<KEduVocContainer*>(index.internalPointer());
        // rename a lesson
        if (role == Qt::EditRole) {
            container->setName(value.toString());
            emit dataChanged(index, index);
            emit documentModified();
            return true;
        }

        // checkboxes
        if (role == Qt::CheckStateRole) {
            bool newState = value.toBool();
            for (int i = 0; i < rowCount(index); i++) {
                setData(index.child(i, 0), newState, Qt::CheckStateRole);
            }
            container->setInPractice(newState);
            emit dataChanged(index, index);
            emit documentModified();
            return true;
        }
    }
    return false;
}


Qt::ItemFlags KEduVocContainerModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        // the root element, not editable for now
        if (index.parent() == QModelIndex()) {
            return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }
        // the name column
        if (index.column() == ContainerNameColumn) {
            return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable
                    | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
        } else { // every other element
            return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled |  Qt::ItemIsDropEnabled);
        }
    }
    return  Qt::ItemIsDropEnabled;
}


QVariant KEduVocContainerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // statically two columns for now
    if (orientation == Qt::Horizontal) {
        switch (section) {
        case ContainerNameColumn:
            if (role == Qt::DisplayRole) {
                return i18n("Unit");
            }
            break;
        case TotalCountColumn:
            if (role == Qt::DisplayRole) {
                return QVariant();
            }
            if (role == Qt::ToolTipRole) {
                return i18n("Number of entries in this lesson.");
            }
            break;
        }
    }
    return QVariant();
}


int KEduVocContainerModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    if (!document()) {
        return FirstDataColumn;
    }

    // for now one grade per language
    return FirstDataColumn;
}


void KEduVocContainerModel::deleteContainer(const QModelIndex & containerIndex)
{
    KEduVocContainer* container = static_cast<KEduVocContainer*>(containerIndex.internalPointer());
    KEduVocContainer* parent = container->parent();

    if (!parent) {
        // never delete the root container
        return;
    }

    beginRemoveRows(containerIndex.parent(), containerIndex.row(), containerIndex.row());
    parent->deleteChildContainer(container->row());
    endRemoveRows();
}

Qt::DropActions KEduVocContainerModel::supportedDropActions() const
{
    return Qt::MoveAction | Qt::CopyAction;
}

QStringList KEduVocContainerModel::mimeTypes() const
{
    return QStringList() << "text/plain";
}


QMimeData * KEduVocContainerModel::mimeData(const QModelIndexList &indexes) const
{
    KEduVocContainerMimeData *mimeData = new KEduVocContainerMimeData();

    foreach(const QModelIndex & index, indexes) {
        mimeData->addContainer(static_cast<KEduVocContainer*>(index.internalPointer()));
    }

    mimeData->setText("Lesson");

    return mimeData;
}


bool KEduVocContainerModel::dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent)
{
    Q_UNUSED(column)

    if (action == Qt::IgnoreAction) {
        return true;
    }

    // if it's internal, get the pointers
    const KEduVocContainerMimeData * containerData =
        qobject_cast<const KEduVocContainerMimeData *>(data);

    if (containerData) {
        foreach(KEduVocContainer * container, containerData->containerList()) {
            // no way to move a word type to a lesson for now
            if (container->containerType() != containerType()) {
                return false;
            }

            if (action == Qt::MoveAction || action == Qt::CopyAction) {
                //qDebug() << "Move container: " << container->name();
                KEduVocContainer* parentContainer = 0;

                if (parent.isValid()) {
                    parentContainer = static_cast<KEduVocContainer*>(parent.internalPointer());
                }
                if (!parentContainer) {
                    // drop into root
                    parentContainer = rootContainer();
                } else {
                    //make sure a container cannot be dropped into one of its child containers!
                    KEduVocContainer* childTest = parentContainer;
                    while (childTest != 0) {
                        if (childTest == container) {
                            //qDebug() << "Cannot drop a container into one of its child containers!";
                            return false;
                        }
                        childTest = childTest->parent();
                    }
                }

                QModelIndex oldParent = index(container->parent());

                beginRemoveRows(oldParent, container->row(), container->row());
                container->parent()->removeChildContainer(container->row());
                endRemoveRows();

                // if we get to choose, append seems sane.
                if (row < 0) {
                    row = parentContainer->childContainerCount();
                }

                // use index because we sometimes reparent to the root container instead of dropping into nowhere
                beginInsertRows(index(parentContainer), row, row);
                parentContainer->insertChildContainer(row, container);
                endInsertRows();

                return true;
            }
        }
    }

    // if it's a translation, get the pointers
    const KEduVocVocabularyMimeData * translationData =
        qobject_cast<const KEduVocVocabularyMimeData *>(data);

    if (translationData) {
        if (!parent.isValid()) {
            return false;
        }
        if (containerType() == KEduVocContainer::Lesson) {
            // Create a list of the entries associated with the translations being copied. This prevents duplicates if they highlighted several columns.
            QList<KEduVocExpression*> entries;

            foreach(KEduVocTranslation * translation, translationData->translationList()) {
                if (!entries.contains(translation->entry())) {
                    entries << translation->entry();
                }
            }

            foreach(KEduVocExpression * entry, entries) {
                static_cast<KEduVocLesson*>(parent.internalPointer())->appendEntry(new KEduVocExpression(*entry));
            }
        }

        if (containerType() == KEduVocContainer::WordType) {
            foreach(KEduVocTranslation * translation, translationData->translationList()) {
                translation->setWordType(
                    static_cast<KEduVocWordType*>(parent.internalPointer()));
            }
        }
        return false;
    }

//     qDebug() << data->formats();

    return false;
}

Qt::DropActions KEduVocContainerModel::supportedDragActions() const
{
    return (Qt::CopyAction | Qt::MoveAction);
}