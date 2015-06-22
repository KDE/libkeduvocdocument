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

#include "keduvocimagechooserwidget.h"

#include "ui_keduvocimagechooserwidget.h"

#include <keduvocexpression.h>
#include <QDebug>

#include <QDragEnterEvent>
#include <QMimeData>

using namespace Editor;

class KEduVocImageChooserWidget::Private
{
public:
    Private();

    /// Column in the document - corresponds to the language (-KV_COL_TRANS)
    int m_currentTranslation;
    /// Selection in the doc - if more than one row is selected behavior is different
    KEduVocExpression * m_entry;
    
    Ui::KEduVocImageChooserWidget * m_ui;
};

KEduVocImageChooserWidget::Private::Private()
{
    m_currentTranslation = -1;
    m_entry = 0;
    m_ui = new Ui::KEduVocImageChooserWidget();
}

KEduVocImageChooserWidget::KEduVocImageChooserWidget( QWidget *parent )
    : QWidget(parent)
    , d( new Private )
{
    d->m_ui->setupUi( this );

    connect( d->m_ui->imageUrlRequester, &KUrlRequester::textChanged, this, &KEduVocImageChooserWidget::slotImageChanged );

    d->m_ui->imageWidget->setAcceptDrops( true );
    d->m_ui->imageWidget->installEventFilter( this );
    setEnabled(false);
}

KEduVocImageChooserWidget::~KEduVocImageChooserWidget()
{
    delete d;
}

void KEduVocImageChooserWidget::setTranslation( KEduVocExpression* entry, int translation )
{
    d->m_currentTranslation = translation;
    d->m_entry = entry;

    if( d->m_entry ) {
        setEnabled( true );
        d->m_ui->imageUrlRequester->setUrl( QUrl::fromLocalFile( d->m_entry->translation( d->m_currentTranslation )->imageUrl().toLocalFile() ) );
//         slotImageChanged( m_entry->translation(m_currentTranslation)->imageUrl().toLocalFile() );
    } else {
        setEnabled( false );
        d->m_ui->imageUrlRequester->clear();
//         imageLabel->setText(i18nc("@label image preview is empty", "No Preview"));
        d->m_ui->imageWidget->setPixmap( QPixmap() );
    }
}

void KEduVocImageChooserWidget::slotImageChanged( const QString & url )
{
    if( !url.isEmpty() ) {
        QPixmap pixmap( url );
        d->m_ui->imageWidget->setPixmap( pixmap );
    } else {
//         imageLabel->setText(i18nc("@label image preview is empty", "No Image"));
        d->m_ui->imageWidget->setPixmap( QPixmap() );
    }

    if( d->m_entry ) {
        d->m_entry->translation( d->m_currentTranslation )->setImageUrl( QUrl::fromLocalFile( url ) );
        foreach( int j, d->m_entry->translationIndices() ) {
            if( d->m_entry->translation( j )->imageUrl().isEmpty() ) {
                d->m_entry->translation( j )->setImageUrl( d->m_ui->imageUrlRequester->url() );
            }
        }
    }
}

bool KEduVocImageChooserWidget::eventFilter( QObject * obj, QEvent * event )
{
    if( obj == d->m_ui->imageWidget ) {
//         if (event->type() == QEvent::KeyPress) {
//              QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//              qDebug() << "Ate key press " << keyEvent->key();
//              return true;
//         }
//         if (event->type() == QEvent::Resize) {
//             QPixmap pixmap(imageUrlRequester->url().toLocalFile());
//             imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio));
//             return false;
//         }


        if( event->type() == QEvent::DragMove ) {
            event->accept();
            return true;
        }

        //Events with drop data
        if( event->type() == QEvent::DragEnter || event->type() == QEvent::Drop ) {
            if( !d->m_entry ) {
                return true;
            }
            QDropEvent *dropEvent = dynamic_cast <QDropEvent *> ( event );
            if( ( dropEvent!= NULL ) && ( dropEvent->mimeData()!= NULL ) ) {
                const QMimeData * mimeData( dropEvent->mimeData() );
                if( mimeData->hasUrls() && mimeData->hasUrls() && ( mimeData->urls().size() == 1 ) ) {
                    switch( event->type() ) {
                    case QEvent::DragEnter:
                        event->accept();
                        return true;
                    case QEvent::Drop:
                        d->m_ui->imageUrlRequester->setUrl( mimeData->urls().first() );
                        return true;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }

    return QObject::eventFilter( obj, event );
}
