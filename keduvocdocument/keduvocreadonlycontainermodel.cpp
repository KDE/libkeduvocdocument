/***************************************************************************

    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
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

#include "keduvocreadonlycontainermodel.h"

#include <KLocalizedString>
#include <QDebug>

#include <keduvoccontainermimedata.h>
#include <keduvocvocabularymimedata.h>

#include <keduvocdocument.h>
#include <keduvoclesson.h>
#include <keduvocwordtype.h>
#include <keduvocexpression.h>

class KEduVocReadonlyContainerModel::Private
{
public:
    KEduVocContainer::EnumContainerType m_type;
    KEduVocDocument *m_doc;

    Private(KEduVocContainer::EnumContainerType type);
};

KEduVocReadonlyContainerModel::Private::Private(KEduVocContainer::EnumContainerType type)
    : m_type(type)
    , m_doc(0)
{
}

/** @file
  * Implementation of ReadonlyContainerModel.
  * Functions to create the model from the lessons of the vocabulary document.
  */


KEduVocReadonlyContainerModel::KEduVocReadonlyContainerModel(KEduVocContainer::EnumContainerType type,
                                                             QObject * parent)
    : QAbstractItemModel( parent )
    , d( new Private( type ))
{
}

KEduVocReadonlyContainerModel::~KEduVocReadonlyContainerModel()
{
    delete d;
}

KEduVocContainer::EnumContainerType KEduVocReadonlyContainerModel::containerType() const
{
    return d->m_type;
}

KEduVocDocument* KEduVocReadonlyContainerModel::document() const
{
    return d->m_doc;
}

void KEduVocReadonlyContainerModel::setDocument(KEduVocDocument * doc)
{
    beginResetModel();
    d->m_doc = doc;
    if (d->m_doc) {
        //qDebug() << "Set Document: " << d->m_doc->url();
    } else {
        //qDebug() << "Set Invalid Document";
    }
    endResetModel();
}

QModelIndex KEduVocReadonlyContainerModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!d->m_doc || !hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    KEduVocContainer *parentContainer;

    if (!parent.isValid()) {
        parentContainer = 0;
    } else {
        parentContainer = static_cast<KEduVocContainer*>(parent.internalPointer());
    }

    KEduVocContainer *childContainer = 0;
    if (!parentContainer) {
        childContainer = rootContainer();
        if (!childContainer) {
            return QModelIndex();
        }
    } else {
        childContainer = parentContainer->childContainer(row);
    }
    return createIndex(row, column, childContainer);
}

QModelIndex KEduVocReadonlyContainerModel::index(KEduVocContainer * container) const
{
    if (!container) {
        return QModelIndex();
    }

    QModelIndex currentIndex = index(container->row(), 0, index(container->parent()));
    Q_ASSERT(container == currentIndex.internalPointer());

    return currentIndex;
}

QModelIndex KEduVocReadonlyContainerModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    KEduVocContainer *childItem = static_cast<KEduVocContainer*>(index.internalPointer());
    if (!childItem) {
        return QModelIndex();
    }

    KEduVocContainer *parentItem = childItem->parent();

    if (!parentItem) {
        return QModelIndex();
    }

    QModelIndex parentIndex = createIndex(parentItem->row(), 0, parentItem);
    return parentIndex;
}

int KEduVocReadonlyContainerModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0) {
        return 0;
    }

    KEduVocContainer *parentItem;
    if (!parent.isValid()) {
        // root element
        if (!d->m_doc) {
            return 0;
        }
        return 1;
    } else {
        parentItem =  static_cast<KEduVocContainer*>(parent.internalPointer());
        return parentItem->childContainerCount();
    }
}

QVariant KEduVocReadonlyContainerModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    KEduVocContainer *container = static_cast<KEduVocContainer*>(index.internalPointer());

    switch (index.column()) {
    case 0: // Container name
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            if (index.parent() == QModelIndex()) {
                return i18n("None");
            }
            return container->name();
        }
        if (role == Qt::TextAlignmentRole) {
            return Qt::AlignLeft;
        }
        break;
    }

    return QVariant();
}

Qt::ItemFlags KEduVocReadonlyContainerModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        // the root element, not editable for now
        if (index.parent() == QModelIndex()) {
            return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }
        // the name column
        if (index.column() == 0) {
            return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        } else { // every other element
            return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }
    }
    return  0;
}

int KEduVocReadonlyContainerModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    if (!d->m_doc) {
        return 1;
    }

    return 1;
}