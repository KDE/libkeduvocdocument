/***************************************************************************

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvocmultiplechoicewidget.h"

#include "ui_keduvocmultiplechoicewidget.h"

#include <keduvoctranslation.h>
#include <keduvocexpression.h>
#include <QDebug>

#include <QStringListModel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

using namespace Editor;

class KEduVocMultipleChoiceWidget::Private
{
public:
    Private();

    KEduVocTranslation* m_translation;
    QStringListModel * m_choicesModel;

    Ui::KEduVocMultipleChoiceWidget * m_ui;
};

KEduVocMultipleChoiceWidget::Private::Private()
{
    m_ui = new Ui::KEduVocMultipleChoiceWidget();
}

KEduVocMultipleChoiceWidget::KEduVocMultipleChoiceWidget( QWidget *parent )
    : QWidget( parent )
    , d( new Private )
{
    d->m_ui->setupUi( this );

    connect( d->m_ui->addChoiceButton, &QPushButton::clicked, this, &KEduVocMultipleChoiceWidget::slotAddChoiceButton );
    connect( d->m_ui->removeChoiceButton, &QPushButton::clicked, this, &KEduVocMultipleChoiceWidget::slotRemoveChoiceButton);

    d->m_choicesModel = new QStringListModel( this );
    d->m_ui->multipleChoiceListView->setModel( d->m_choicesModel );

    connect( d->m_choicesModel, &QStringListModel::dataChanged, this, &KEduVocMultipleChoiceWidget::slotDataChanged );

    d->m_ui->multipleChoiceListView->setAcceptDrops( true );
    d->m_ui->multipleChoiceListView->installEventFilter( this );

    setEnabled( false );
}

KEduVocMultipleChoiceWidget::~KEduVocMultipleChoiceWidget()
{
    delete d;
}

void KEduVocMultipleChoiceWidget::slotDataChanged( const QModelIndex & topLeft, const QModelIndex & bottomRight )
{
    Q_UNUSED( topLeft )
    Q_UNUSED( bottomRight )
    d->m_translation->multipleChoice() = d->m_choicesModel->stringList();
    d->m_ui->removeChoiceButton->setEnabled( d->m_translation && d->m_translation->multipleChoice().count() > 0 );
}


void KEduVocMultipleChoiceWidget::setTranslation( KEduVocExpression * entry, int translation )
{
    if( entry ) {
        d->m_translation = entry->translation( translation );
    } else {
        d->m_translation = 0;
    }

    if( d->m_translation ) {
        setEnabled(true);
        d->m_choicesModel->setStringList( d->m_translation->multipleChoice() );
        d->m_ui->removeChoiceButton->setEnabled( d->m_translation->multipleChoice().count() > 0 );
    } else {
        setEnabled(false);
    }
    d->m_ui->removeChoiceButton->setEnabled( d->m_translation && d->m_translation->multipleChoice().count() > 0 );
}


void KEduVocMultipleChoiceWidget::slotAddChoiceButton()
{
    d->m_choicesModel->insertRow( d->m_choicesModel->rowCount() );
    QModelIndex index( d->m_choicesModel->index( d->m_choicesModel->rowCount() - 1 ) );
    d->m_choicesModel->setData( index, "" );
    d->m_ui->multipleChoiceListView->scrollTo( index );
    d->m_ui->multipleChoiceListView->setCurrentIndex( index );
    d->m_ui->multipleChoiceListView->edit( index );
}


void KEduVocMultipleChoiceWidget::slotRemoveChoiceButton()
{
    QModelIndex index = d->m_ui->multipleChoiceListView->selectionModel()->currentIndex();
    if( index.isValid() ) {
        d->m_choicesModel->removeRows( index.row(), 1, QModelIndex() );
    } else {
        d->m_choicesModel->removeRows( d->m_choicesModel->rowCount( QModelIndex() ) - 1, 1, QModelIndex() );
    }
    d->m_translation->multipleChoice() = d->m_choicesModel->stringList();
    d->m_ui->removeChoiceButton->setEnabled( d->m_translation && d->m_translation->multipleChoice().count() > 0 );
}


bool KEduVocMultipleChoiceWidget::eventFilter( QObject * obj, QEvent * event )
{
    if( obj == d->m_ui->multipleChoiceListView ) {
        if( event->type() == QEvent::DragEnter ) {
            QDragEnterEvent *dragEnterEvent = static_cast<QDragEnterEvent *>( event );
            //qDebug() << "DragEnter mime format: " << dragEnterEvent->format();
            if( ( dragEnterEvent->mimeData() != NULL ) && dragEnterEvent->mimeData()->hasText() ) {
                event->accept();
            }
            return true;
        }

        if( event->type() == QEvent::DragMove ) {
            event->accept();
            return true;
        }

        if( event->type() == QEvent::Drop ) {
            QDropEvent *dropEvent = static_cast<QDropEvent *>( event );
            //qDebug() << "You dropped onto me: " << dropEvent->mimeData()->text();
            if( ( dropEvent->mimeData() != NULL ) && dropEvent->mimeData()->hasText() ) {

                QStringList choices = dropEvent->mimeData()->text().split('\n');
                foreach( const QString & choice, choices ) {
                    d->m_choicesModel->insertRow( d->m_ui->multipleChoiceListView->model()->rowCount() );
                    d->m_choicesModel->setData( d->m_choicesModel->index( d->m_ui->multipleChoiceListView->model()->rowCount() - 1 ), choice );
                }

                return true;
            }
        }
    }
    return QObject::eventFilter( obj, event );
}
