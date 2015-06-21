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

#include "keduvoccomparisonwidget.h"
#include "ui_keduvoccomparisonwidget.h"


#include <keduvocwordtype.h>
#include <keduvoctranslation.h>
#include <keduvocexpression.h>
#include <keduvocdocument.h>

#include <KMessageBox>
#include <KLocalizedString>

using namespace Editor;

class KEduVocComparisonWidget::Private
{
public:
    Private();

    KEduVocTranslation* m_translation;
    KEduVocDocument* m_doc;
    Ui::KEduVocComparisonWidget * m_ui;
};

KEduVocComparisonWidget::Private::Private()
{
    m_translation = 0;
    m_doc = 0;
    m_ui = new Ui::KEduVocComparisonWidget();
}

KEduVocComparisonWidget::KEduVocComparisonWidget( QWidget *parent )
    : QWidget( parent )
    , d( new Private )
{
    d->m_ui->setupUi(this);

    connect( d->m_ui->makeAdjectiveButton, &QPushButton::clicked, this, &KEduVocComparisonWidget::slotMakeAdjectiveButton );
    connect( d->m_ui->makeAdverbButton, &QPushButton::clicked, this, &KEduVocComparisonWidget::slotMakeAdverbButton );

    connect( d->m_ui->comparativeLineEdit, &QLineEdit::editingFinished, this, &KEduVocComparisonWidget::slotComparativeChanged );
    connect( d->m_ui->superlativeLineEdit, &QLineEdit::editingFinished, this, &KEduVocComparisonWidget::slotSuperlativeChanged );
}

KEduVocComparisonWidget::~KEduVocComparisonWidget()
{
    delete d;
}

void KEduVocComparisonWidget::setTranslation( KEduVocExpression * entry, int translation )
{
    d->m_ui->comparativeLineEdit->setText( QString() );
    d->m_ui->superlativeLineEdit->setText( QString() );
    d->m_ui->comparativeLineEdit->setEnabled( false );
    d->m_ui->superlativeLineEdit->setEnabled( false );

    if( entry ) {
        d->m_translation = entry->translation( translation );
    } else {
        d->m_translation = 0;
    }

    if( d->m_translation ) {
        d->m_ui->absoluteEntryLabel->setText( d->m_translation->text() );
        setEnabled( true );

        if( d->m_translation->wordType() ) {
            if( d->m_translation->wordType()->wordType() & KEduVocWordFlag::Adjective
                    || d->m_translation->wordType()->wordType() & KEduVocWordFlag::Adverb ) {
                d->m_ui->comparativeLineEdit->setEnabled( true );
                d->m_ui->superlativeLineEdit->setEnabled( true );

                d->m_ui->comparativeLineEdit->setText( d->m_translation->comparativeForm().text() );
                d->m_ui->superlativeLineEdit->setText( d->m_translation->superlativeForm().text() );
            }
        }
    } else {
        setEnabled( false );
        d->m_ui->absoluteEntryLabel->setText( QString() );
    }
}

void KEduVocComparisonWidget::slotMakeAdjectiveButton()
{
    if( !d->m_doc ) {
        return;
    }

    // find an adjective container
    KEduVocWordType* container = d->m_doc->wordTypeContainer()->childOfType( KEduVocWordFlag::Adjective );
    if( container ) {
        d->m_translation->setWordType( container );
        d->m_ui->comparativeLineEdit->setEnabled( true );
        d->m_ui->superlativeLineEdit->setEnabled( true );
        d->m_ui->comparativeLineEdit->setFocus();
    } else {
        ///@todo better message
        KMessageBox::information( this, i18n( "Could not determine word type of adjectives" ) );
    }
}

void KEduVocComparisonWidget::slotMakeAdverbButton()
{
    if( !d->m_doc ) {
        return;
    }

    // find an adverb container
    KEduVocWordType* container = d->m_doc->wordTypeContainer()->childOfType( KEduVocWordFlag::Adverb );
    if( container ) {
        d->m_translation->setWordType( container );
        d->m_ui->comparativeLineEdit->setEnabled( true );
        d->m_ui->superlativeLineEdit->setEnabled( true );
        d->m_ui->comparativeLineEdit->setFocus();
    } else {
        ///@todo better message
        KMessageBox::information( this, i18n( "Could not determine word type of adverbs" ) );
    }
}

void KEduVocComparisonWidget::setDocument( KEduVocDocument * doc )
{
    d->m_doc = doc;
}

void KEduVocComparisonWidget::slotComparativeChanged()
{
    d->m_translation->setComparativeForm( d->m_ui->comparativeLineEdit->text() );
}

void KEduVocComparisonWidget::slotSuperlativeChanged()
{
    d->m_translation->setSuperlativeForm( d->m_ui->superlativeLineEdit->text() );
}
