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

#ifndef KEDUVOCREADONLYCONTAINERMODEL_H
#define KEDUVOCREADONLYCONTAINERMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>

#include <QObject>

#include <keduvocdocument.h>
#include <keduvoclesson.h>

/**
  * Model for the tree of containers (lessons, word types).
  * ReadonlyContainerModel is a read only tree model.
  * @see ContainerModel for its subclass that includes more options.
  */

class KEDUVOCDOCUMENT_EXPORT KEduVocReadonlyContainerModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit KEduVocReadonlyContainerModel(KEduVocContainer::EnumContainerType type, QObject *parent = 0);

    ~KEduVocReadonlyContainerModel();

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const;

    QModelIndex index(KEduVocContainer* container) const;

    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    // getter methods
    KEduVocContainer::EnumContainerType containerType() const;
    KEduVocDocument* document() const;

public slots:
    /** Set the new source kvtml file
     * @param doc the new file */
    void setDocument(KEduVocDocument *doc);

protected:
    virtual KEduVocContainer *rootContainer() const = 0;

private:
    class Private;
    Private * const d;
};


#endif
