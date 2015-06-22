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

#include "keduvocinflectionwidget.h"

#include <keduvocdocument.h>
#include <keduvocexpression.h>
#include <keduvocwordtype.h>
#include <keduvocdeclension.h>
#include <keduvocdeclensionwidget.h>
#include <keduvocconjugationwidget.h>

#include <KLocalizedString>
#include <KComboBox>

#include <QDialogButtonBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

using namespace Editor;

class KEduVocInflectionWidget::Private
{
public:
    Private();

    QWidget *m_wordTypeWidget;
    KEduVocDeclensionWidget *m_declensionWidget;
    KEduVocConjugationWidget *m_conjugationWidget;

    KEduVocDocument *m_doc;
    KEduVocExpression *m_entry;
    int m_translation;
};

KEduVocInflectionWidget::Private::Private()
{
    m_wordTypeWidget = 0;
    m_declensionWidget = 0;
    m_conjugationWidget = 0;
    m_doc = 0;
    m_entry = 0;
}

KEduVocInflectionWidget::KEduVocInflectionWidget( QWidget* parent )
    : QStackedWidget(parent)
    , d( new Private )
{
    d->m_wordTypeWidget = new QWidget( this );
    QVBoxLayout *wordTypeLayout = new QVBoxLayout();

    QLabel *label = new QLabel();
    wordTypeLayout->addWidget( label );

    QPushButton *toVerb = new QPushButton( i18n( "Verb" ) );
    toVerb->setObjectName( "toVerb" );
    connect( toVerb, &QPushButton::pressed, this, &KEduVocInflectionWidget::setWordType );
    wordTypeLayout->addWidget( toVerb, 1 );

    QPushButton *toNoun = new QPushButton( i18n( "Noun" ) );
    toNoun->setObjectName( "toNoun" );
    connect( toNoun, &QPushButton::pressed, this, &KEduVocInflectionWidget::setWordType );
    wordTypeLayout->addWidget( toNoun, 1 );

    QPushButton *toAdjective = new QPushButton( i18n( "Adjective" ) );
    toAdjective->setObjectName( "toAdjective" );
    connect( toAdjective, &QPushButton::pressed, this, &KEduVocInflectionWidget::setWordType );
    wordTypeLayout->addWidget( toAdjective, 1 );

    wordTypeLayout->addStretch();

    d->m_wordTypeWidget->setDisabled( true );
    d->m_wordTypeWidget->setLayout( wordTypeLayout );

    d->m_conjugationWidget = new KEduVocConjugationWidget( this );
    d->m_declensionWidget = new KEduVocDeclensionWidget( this );

    addWidget( d->m_wordTypeWidget );
    addWidget( d->m_conjugationWidget );
    addWidget( d->m_declensionWidget );
}

KEduVocInflectionWidget::~KEduVocInflectionWidget()
{
    delete d;
}

void KEduVocInflectionWidget::setDocument( KEduVocDocument* doc )
{
    d->m_doc = doc;
    d->m_conjugationWidget->setDocument( doc );
    d->m_declensionWidget->setDocument( doc );
}

void KEduVocInflectionWidget::setTranslation( KEduVocExpression* entry, int translation )
{
    d->m_entry = entry;
    d->m_translation = translation;

    if( !entry || !entry->translation( translation ) ) {
        d->m_wordTypeWidget->setDisabled( true );
        setCurrentWidget( d->m_wordTypeWidget );
        return;
    }
    if( !entry->translation( translation )->wordType() ||
            !( entry->translation( translation )->wordType()->wordType() & KEduVocWordFlag::Verb
              || entry->translation( translation )->wordType()->wordType() & KEduVocWordFlag::Noun
              || entry->translation( translation )->wordType()->wordType() & KEduVocWordFlag::Adjective
             )
       ) {
        qobject_cast <QLabel*> ( d->m_wordTypeWidget->layout()->itemAt( 0 )->widget() )->setText( i18n( "\"%1\" is a:", entry->translation( translation )->text() ) );
        d->m_wordTypeWidget->setEnabled(true);
        setCurrentWidget( d->m_wordTypeWidget );
        return;
    }

    d->m_conjugationWidget->setTranslation( entry, translation );
    d->m_declensionWidget->setTranslation( entry, translation );

    if( entry->translation( translation )->wordType()->wordType() & KEduVocWordFlag::Verb ) {
        setCurrentWidget( d->m_conjugationWidget );
    } else {
        setCurrentWidget( d->m_declensionWidget );
    }
}

void KEduVocInflectionWidget::setWordType()
{
    if( !sender() ) {
        return;
    }

    KEduVocWordType* container = 0;

    if( sender()->objectName() == "toVerb" ) {
        container = d->m_doc->wordTypeContainer()->childOfType( KEduVocWordFlag::Verb );
    } else if( sender()->objectName() == "toNoun" ) {
        KEduVocWordFlags type = KEduVocWordFlag::Noun;

        QPointer <QDialogButtonBox> getGenderDialog = new QDialogButtonBox;
        getGenderDialog->setStandardButtons( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );

        KComboBox *genderComboBox = new KComboBox;
        genderComboBox->addItem( i18n( "Masculine" ), KEduVocWordFlag::Masculine );
        genderComboBox->addItem( i18n( "Neuter" ), KEduVocWordFlag::Neuter );
        genderComboBox->addItem( i18n( "Feminine" ), KEduVocWordFlag::Feminine );

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget( genderComboBox );
        layout->addWidget( getGenderDialog );

        QPointer <QDialog> dialog = new QDialog( this );
        dialog->setLayout( layout );
        dialog->setWindowTitle( i18n( "Please select the noun's gender" ) );

        connect( getGenderDialog.data(), &QDialogButtonBox::accepted, dialog.data(), &QDialog::accept );
        connect( getGenderDialog.data(), &QDialogButtonBox::rejected, dialog.data(), &QDialog::reject );

        if( dialog->exec() == QDialog::Accepted ) {
            type = ( KEduVocWordFlags )( type | genderComboBox->itemData( genderComboBox->currentIndex() ).toInt() );
        }

        delete dialog;

        container = d->m_doc->wordTypeContainer()->childOfType( type );
    } else if( sender()->objectName() == "toAdjective" ) {
        container = d->m_doc->wordTypeContainer()->childOfType( KEduVocWordFlag::Adjective );
    }

    if (container) {
        d->m_entry->translation( d->m_translation )->setWordType( container );
        setTranslation( d->m_entry, d->m_translation);
    }
}