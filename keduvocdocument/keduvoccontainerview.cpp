/***************************************************************************

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

// Own
#include "keduvoccontainerview.h"

// Qt
#include <QHeaderView>

// KEduVoc
#include <keduvoccontainermodel.h>

class KEduVocContainerView::Private
{
public:
    Private();

    // intentionally hide this to make sure a proper model is passed
    virtual void setModel( QAbstractItemModel *model ) {
        Q_UNUSED(model)
    }

    /** ContainerModel for this view. */
    KEduVocContainerModel *m_model;
};

KEduVocContainerView::Private::Private()
    : m_model(0)
{
}

KEduVocContainerView::KEduVocContainerView( QWidget *parent )
    : QTreeView( parent ),
      d( new Private )
{
    header()->setStretchLastSection( false );
    header()->setVisible( false );

    setAlternatingRowColors( true );

    // show the actions added by addAction() as right click menu.
    setContextMenuPolicy( Qt::ActionsContextMenu );

    setDragEnabled( true );
    setAcceptDrops( true );
    setDropIndicatorShown( true );
    setDragDropMode( QAbstractItemView::DragDrop );
}

KEduVocContainerView::~KEduVocContainerView()
{
    delete d;
}

KEduVocContainerModel* KEduVocContainerView::model()
{
    return d->m_model;
}

void KEduVocContainerView::setModel( KEduVocContainerModel *model )
{
    QTreeView::setModel( model );
    d->m_model = model;

    header()->setSectionResizeMode( 0, QHeaderView::Stretch );
    header()->setSectionResizeMode( 1, QHeaderView::ResizeToContents );
}

void KEduVocContainerView::slotRename()
{
    edit( selectionModel()->currentIndex() );
}
