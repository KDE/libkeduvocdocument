/***************************************************************************
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvocsynonymwidget.h"

#include "ui_keduvocsynonymwidget.h"

#include <keduvocdocument.h>
#include <keduvoctranslation.h>
#include <keduvocexpression.h>

#include <QStringListModel>
#include <QDragEnterEvent>
#include <KLocalizedString>

using namespace Editor;

class KEduVocSynonymWidget::Private
{
public:
    Private( SynonymWidgetType type );

    KEduVocDocument  *m_doc;
    // the one that was selected before
    KEduVocTranslation* m_lastTranslation;
    // the one the user clicked on latest
    KEduVocTranslation* m_currentTranslation;
    QStringListModel * m_listModel;
    SynonymWidgetType m_type;
    Ui::KEduVocSynonymWidget * m_ui;
};

KEduVocSynonymWidget::Private::Private( SynonymWidgetType type )
{
    m_doc = 0;
    m_type = type;
    m_currentTranslation = 0;
    m_lastTranslation = 0;
    m_ui = new Ui::KEduVocSynonymWidget();
}

KEduVocSynonymWidget::KEduVocSynonymWidget( SynonymWidgetType type, QWidget *parent )
    : QWidget( parent )
    , d( new Private( type ) )
{
    d->m_ui->setupUi( this );

    connect( d->m_ui->synonymButton, &QPushButton::clicked, this, &KEduVocSynonymWidget::togglePair );
    d->m_listModel = new QStringListModel( this );
    d->m_ui->listView->setModel( d->m_listModel );

    updateList();
}

KEduVocSynonymWidget::~KEduVocSynonymWidget()
{
    delete d;
}

void KEduVocSynonymWidget::setDocument( KEduVocDocument *doc )
{
    d->m_doc = doc;
}

void KEduVocSynonymWidget::setTranslation( KEduVocExpression * entry, int translation )
{
    // ignore zeros
    if( entry ) {
        d->m_lastTranslation = d->m_currentTranslation;
        d->m_currentTranslation = entry->translation( translation );
        updateList();
    } else {
        // better play save, the entry most likely has been deleted.
        d->m_lastTranslation = 0;
        d->m_currentTranslation = 0;
        updateList();
    }
}

void KEduVocSynonymWidget::updateList()
{
    // clear the list
    d->m_listModel->removeRows( 0, d->m_listModel->rowCount() );

    // set the button text
    if( !( d->m_lastTranslation && d->m_currentTranslation ) ) {
        d->m_ui->synonymButton->setEnabled( false );
        d->m_ui->synonymButton->setText( i18n( "Select Synonyms" ) );
    } else {
        d->m_ui->synonymButton->setEnabled( true );
        switch( d->m_type ) {
        case Synonym:
            if( d->m_currentTranslation->synonyms().contains( d->m_lastTranslation ) ) {
                d->m_ui->synonymButton->setText( i18n( "%1 and %2 are not Synonyms", d->m_currentTranslation->text(), d->m_lastTranslation->text() ) );
            } else {
                d->m_ui->synonymButton->setText( i18n( "%1 and %2 are Synonyms", d->m_currentTranslation->text(), d->m_lastTranslation->text() ) );
            }
            break;
        case Antonym:
            if( d->m_currentTranslation->antonyms().contains( d->m_lastTranslation ) ) {
                d->m_ui->synonymButton->setText( i18n( "%1 and %2 are not Antonyms", d->m_currentTranslation->text(), d->m_lastTranslation->text() ) );
            } else {
                d->m_ui->synonymButton->setText( i18n( "%1 and %2 are Antonyms", d->m_currentTranslation->text(), d->m_lastTranslation->text() ) );
            }
            break;
        case FalseFriend:
            if( d->m_currentTranslation->falseFriends().contains( d->m_lastTranslation ) ) {
                d->m_ui->synonymButton->setText( i18n( "%1 and %2 are not False Friends", d->m_currentTranslation->text(), d->m_lastTranslation->text() ) );
            } else {
                d->m_ui->synonymButton->setText( i18n( "%1 and %2 are False Friends", d->m_currentTranslation->text(), d->m_lastTranslation->text() ) );
            }
            break;
        }
    }

    if( d->m_currentTranslation ) {
        switch( d->m_type ) {
        case Synonym:
            d->m_ui->synonymLabel->setText( i18nc( "Title for a list of synonyms for a word", "Synonyms of %1:", d->m_currentTranslation->text() ) );
            break;
        case Antonym:
            d->m_ui->synonymLabel->setText( i18nc( "Title for a list of antonyms (opposites) for a word", "Antonyms of %1:", d->m_currentTranslation->text() ) );
            break;
        case FalseFriend:
            d->m_ui->synonymLabel->setText( i18nc( "Title for a list of false friend (things that sound similar but have different meanings) for a word", "False Friends of %1:", d->m_currentTranslation->text() ) );
            break;
        }

        // load list of synonyms/antonyms/ffs
        QList< KEduVocTranslation* > list;
        switch( d->m_type ) {
        case Synonym:
            list = d->m_currentTranslation->synonyms();
            break;
        case Antonym:
            list = d->m_currentTranslation->antonyms();
            break;
        case FalseFriend:
            list = d->m_currentTranslation->falseFriends();
            break;
        }
        foreach( KEduVocTranslation * translation, list ) {
            int row = d->m_listModel->rowCount();
            d->m_listModel->insertRow( row );
            d->m_listModel->setData( d->m_listModel->index( row ), translation->text() );
        }
    } else {
        d->m_ui->synonymLabel->clear();
    }
}

void KEduVocSynonymWidget::togglePair()
{
    // pair them up
    switch( d->m_type ) {
    case Synonym:
        if( d->m_currentTranslation->synonyms().contains( d->m_lastTranslation ) ) {
            d->m_currentTranslation->removeSynonym( d->m_lastTranslation );
            d->m_lastTranslation->removeSynonym( d->m_currentTranslation );
        } else {
            d->m_currentTranslation->addSynonym( d->m_lastTranslation );
            d->m_lastTranslation->addSynonym( d->m_currentTranslation );
        }
        break;
    case Antonym:
        if( d->m_currentTranslation->antonyms().contains( d->m_lastTranslation ) ) {
            d->m_currentTranslation->removeAntonym( d->m_lastTranslation );
            d->m_lastTranslation->removeAntonym( d->m_currentTranslation );
        } else {
            d->m_currentTranslation->addAntonym( d->m_lastTranslation );
            d->m_lastTranslation->addAntonym( d->m_currentTranslation );
        }
        break;
    case FalseFriend:
        if( d->m_currentTranslation->falseFriends().contains( d->m_lastTranslation ) ) {
            d->m_currentTranslation->removeFalseFriend( d->m_lastTranslation );
            d->m_lastTranslation->removeFalseFriend( d->m_currentTranslation );
        } else {
            d->m_currentTranslation->addFalseFriend( d->m_lastTranslation );
            d->m_lastTranslation->addFalseFriend( d->m_currentTranslation );
        }
        break;
    }

    d->m_doc->setModified( true );

    updateList();
}
