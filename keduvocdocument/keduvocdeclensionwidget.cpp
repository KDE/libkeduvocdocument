/***************************************************************************
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright 2011 Jan Gerrit Marker <jangerrit@weiler-marker.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvocdeclensionwidget.h"

#include "ui_keduvocdeclensionwidget.h"

#include <keduvocdocument.h>
#include <keduvocexpression.h>
#include <keduvocwordtype.h>
#include <keduvocdeclension.h>

#include <KMessageBox>
#include <KLocalizedString>

using namespace Editor;

class KEduVocDeclensionWidget::Private
{
public:
    Private();

    int m_identifier;
    KEduVocExpression* m_entry;
    KEduVocDocument* m_doc;

    /**
     * All line edits and labels, index corresponding to KEduVocWordFlag::indexOf
     */
    QMap< int, QLineEdit* > m_DeclensionLineEdits;

    Ui::KEduVocDeclensionWidget * m_ui;
};

KEduVocDeclensionWidget::Private::Private()
{
    m_doc = 0;
    m_entry = 0;
    m_identifier = -1;
    m_ui = new Ui::KEduVocDeclensionWidget();
}

KEduVocDeclensionWidget::KEduVocDeclensionWidget( QWidget *parent )
    : QWidget(parent)
    , d( new Private )
{
    d->m_ui->setupUi( this );

    d->m_ui->numberSelection->setItemData( 0, KEduVocWordFlag::Singular );
    d->m_ui->numberSelection->setItemData( 1, KEduVocWordFlag::Dual );
    d->m_ui->numberSelection->setItemData( 2, KEduVocWordFlag::Plural );
    connect( d->m_ui->numberSelection, static_cast<void (QComboBox::*)(int)>( &QComboBox::currentIndexChanged ), this, &KEduVocDeclensionWidget::updateEntries );

    connect( d->m_ui->nextButton, &QPushButton::clicked, this, &KEduVocDeclensionWidget::nextNumber );

    setupLineEdits();

    foreach( int index, d->m_DeclensionLineEdits.keys() ) {
        connect( d->m_DeclensionLineEdits.value( index ), SIGNAL( textChanged( const QString& ) ), SLOT( textChanged( const QString& ) ) );
    }
}

KEduVocDeclensionWidget::~KEduVocDeclensionWidget()
{
    delete d;
}

void KEduVocDeclensionWidget::textChanged( const QString& text )
{
    int valueIndex = d->m_DeclensionLineEdits.values().indexOf( qobject_cast<QLineEdit*>( sender() ) );
    int key = d->m_DeclensionLineEdits.keys().value( valueIndex ) | currentAdditionalWordFlag();
    d->m_entry->translation( d->m_identifier )->declension()->setDeclension( text, ( KEduVocWordFlag::Flags )key );
    emit sigModified();
}


void KEduVocDeclensionWidget::updateEntries()
{
    foreach( int key, d->m_DeclensionLineEdits.keys() ) {
        d->m_DeclensionLineEdits.value( key )->setText( d->m_entry->translation( d->m_identifier )->declension()->declension( ( KEduVocWordFlag::Flags )( key | currentAdditionalWordFlag() ) ).text() );
    }
}

void KEduVocDeclensionWidget::setTranslation( KEduVocExpression * entry, int identifier )
{
    if( !d->m_doc ) {
        return;
    }

    d->m_entry = entry;
    if( d->m_identifier != identifier ) {
        d->m_identifier = identifier;
    }

    if( !entry ) {
        setEnabled( false );
        return;
    }

    setupLineEdits();

    setEnabled(true);
    if( entry->translation( d->m_identifier )->wordType() ) {
        KEduVocWordFlags wordType = entry->translation( d->m_identifier )->wordType()->wordType();
        if( wordType & KEduVocWordFlag::Noun || wordType & KEduVocWordFlag::Adjective ) {

            // we create declensions on demand. if empty it will simply not be saved.
            // very little memory overhead, comfy to use ;)
            if( !entry->translation( d->m_identifier )->declension() ) {
                entry->translation( d->m_identifier )->setDeclension( new KEduVocDeclension );
            }

            updateEntries();
        }
    }
}

void KEduVocDeclensionWidget::setDocument( KEduVocDocument * doc )
{
    d->m_doc = doc;
}

int KEduVocDeclensionWidget::currentAdditionalWordFlag()
{
    if( d->m_entry->translation( d->m_identifier )->wordType()->wordType() & KEduVocWordFlag::Noun ) {
        ///@todo easier and better way to get gender?
        if( d->m_entry->translation( d->m_identifier )->wordType()->wordType() & KEduVocWordFlag::Feminine )
            return KEduVocWordFlag::Feminine;
        if( d->m_entry->translation( d->m_identifier )->wordType()->wordType() & KEduVocWordFlag::Masculine )
            return KEduVocWordFlag::Masculine;
        if( d->m_entry->translation( d->m_identifier )->wordType()->wordType() & KEduVocWordFlag::Neuter )
            return KEduVocWordFlag::Neuter;
    }

    return d->m_ui->numberSelection->itemData( d->m_ui->numberSelection->currentIndex() ).toInt();
}

void KEduVocDeclensionWidget::nextNumber()
{
    int newIndex = d->m_ui->numberSelection->currentIndex() + 1;
    if( newIndex >= d->m_ui->numberSelection->count() )
        newIndex = 0;

    d->m_ui->numberSelection->setCurrentIndex( newIndex );
}

void KEduVocDeclensionWidget::setupLineEdits()
{
    d->m_DeclensionLineEdits.clear();

    if( d->m_entry == 0 || !d->m_entry->translation( d->m_identifier ) || !d->m_entry->translation( d->m_identifier )->wordType()
            || d->m_entry->translation( d->m_identifier )->wordType()->wordType() & KEduVocWordFlag::Noun ) {
        d->m_ui->label_3->setText( i18n( "Singular" ) );
        d->m_ui->label_4->setText( i18n( "Dual" ) );
        d->m_ui->label_5->setText( i18n( "Plural" ) );

        /* Hide the GUI elements which are needed to switch the number */
        for( int i = 0; i < d->m_ui->numberLayout->count(); ++i ) {
            d->m_ui->numberLayout->itemAt( i )->widget()->hide();
        }

        d->m_DeclensionLineEdits[ KEduVocWordFlag::Singular | KEduVocWordFlag::Nominative ] = d->m_ui->masculine_1;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Singular | KEduVocWordFlag::Genitive ] = d->m_ui->masculine_2;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Singular | KEduVocWordFlag::Dative ] = d->m_ui->masculine_3;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Singular | KEduVocWordFlag::Accusative ] = d->m_ui->masculine_4;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Singular | KEduVocWordFlag::Ablative ] = d->m_ui->masculine_5;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Singular | KEduVocWordFlag::Locative ] = d->m_ui->masculine_6;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Singular | KEduVocWordFlag::Vocative ] = d->m_ui->masculine_7;

        d->m_DeclensionLineEdits[ KEduVocWordFlag::Dual | KEduVocWordFlag::Nominative ] = d->m_ui->feminine_1;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Dual | KEduVocWordFlag::Genitive ] = d->m_ui->feminine_2;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Dual | KEduVocWordFlag::Dative ] = d->m_ui->feminine_3;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Dual | KEduVocWordFlag::Accusative ] = d->m_ui->feminine_4;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Dual | KEduVocWordFlag::Ablative ] = d->m_ui->feminine_5;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Dual | KEduVocWordFlag::Locative ] = d->m_ui->feminine_6;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Dual | KEduVocWordFlag::Vocative ] = d->m_ui->feminine_7;

        d->m_DeclensionLineEdits[ KEduVocWordFlag::Plural | KEduVocWordFlag::Nominative ] = d->m_ui->neuter_1;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Plural | KEduVocWordFlag::Genitive ] = d->m_ui->neuter_2;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Plural | KEduVocWordFlag::Dative ] = d->m_ui->neuter_3;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Plural | KEduVocWordFlag::Accusative ] = d->m_ui->neuter_4;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Plural | KEduVocWordFlag::Ablative ] = d->m_ui->neuter_5;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Plural | KEduVocWordFlag::Locative ] = d->m_ui->neuter_6;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Plural | KEduVocWordFlag::Vocative ] = d->m_ui->neuter_7;
    } else {
        d->m_ui->label_3->setText( i18n( "Masculine" ) );
        d->m_ui->label_4->setText( i18n( "Feminine" ) );
        d->m_ui->label_5->setText( i18n( "Neuter" ) );

        /* Show the GUI elements which are needed to switch the number */
        for( int i = 0; i < d->m_ui->numberLayout->count(); ++i ) {
            d->m_ui->numberLayout->itemAt(i)->widget()->show();
        }

        d->m_DeclensionLineEdits[ KEduVocWordFlag::Masculine | KEduVocWordFlag::Nominative ] = d->m_ui->masculine_1;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Masculine | KEduVocWordFlag::Genitive ] = d->m_ui->masculine_2;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Masculine | KEduVocWordFlag::Dative ] = d->m_ui->masculine_3;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Masculine | KEduVocWordFlag::Accusative ] = d->m_ui->masculine_4;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Masculine | KEduVocWordFlag::Ablative ] = d->m_ui->masculine_5;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Masculine | KEduVocWordFlag::Locative ] = d->m_ui->masculine_6;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Masculine | KEduVocWordFlag::Vocative ] = d->m_ui->masculine_7;

        d->m_DeclensionLineEdits[ KEduVocWordFlag::Feminine | KEduVocWordFlag::Nominative ] = d->m_ui->feminine_1;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Feminine | KEduVocWordFlag::Genitive ] = d->m_ui->feminine_2;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Feminine | KEduVocWordFlag::Dative ] = d->m_ui->feminine_3;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Feminine | KEduVocWordFlag::Accusative ] = d->m_ui->feminine_4;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Feminine | KEduVocWordFlag::Ablative ] = d->m_ui->feminine_5;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Feminine | KEduVocWordFlag::Locative ] = d->m_ui->feminine_6;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Feminine | KEduVocWordFlag::Vocative ] = d->m_ui->feminine_7;

        d->m_DeclensionLineEdits[ KEduVocWordFlag::Neuter | KEduVocWordFlag::Nominative ] = d->m_ui->neuter_1;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Neuter | KEduVocWordFlag::Genitive ] = d->m_ui->neuter_2;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Neuter | KEduVocWordFlag::Dative ] = d->m_ui->neuter_3;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Neuter | KEduVocWordFlag::Accusative ] = d->m_ui->neuter_4;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Neuter | KEduVocWordFlag::Ablative ] = d->m_ui->neuter_5;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Neuter | KEduVocWordFlag::Locative ] = d->m_ui->neuter_6;
        d->m_DeclensionLineEdits[ KEduVocWordFlag::Neuter | KEduVocWordFlag::Vocative ] = d->m_ui->neuter_7;
    }
}
