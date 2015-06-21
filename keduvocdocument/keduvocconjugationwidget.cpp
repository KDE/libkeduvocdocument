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

#include "keduvocconjugationwidget.h"

#include "ui_keduvocconjugationwidget.h"


#include <keduvocdocument.h>
#include <keduvocexpression.h>
#include <keduvocwordtype.h>
#include <KMessageBox>

using namespace Editor;

class KEduVocConjugationWidget::Private
{
public:
    Private();

    /**
     * Fill the line edits
     */
    void updateEntries();

    QString m_lastTenseSelection;
    int m_identifier;
    KEduVocExpression* m_entry;
    KEduVocDocument* m_doc;

    /**
     * All line edits and labels, index corresponding to KEduVocWordFlag::indexOf
     */
    QMap< KEduVocWordFlags, QLineEdit* > m_conjugationLineEdits;

    Ui::KEduVocConjugationWidget * m_ui;
};

KEduVocConjugationWidget::Private::Private()
{
    m_doc = 0;
    m_entry = 0;
    m_identifier = -1;
    m_ui = new Ui::KEduVocConjugationWidget();
}

KEduVocConjugationWidget::KEduVocConjugationWidget( QWidget *parent )
    : QWidget( parent )
    , d( new Private )
{
    d->m_ui->setupUi(this);

    connect( d->m_ui->nextTenseButton, &QPushButton::clicked, this, &KEduVocConjugationWidget::slotNextTense );
    connect( d->m_ui->tenseComboBox, static_cast <void (KComboBox::*)(int)> ( &KComboBox::activated ), this, &KEduVocConjugationWidget::slotTenseSelected );
    connect( d->m_ui->tenseComboBox->lineEdit(), SIGNAL( editingFinished() ), SLOT( tenseEditingFinished() ) );

    d->m_conjugationLineEdits[ KEduVocWordFlag::First | KEduVocWordFlag::Singular ]
        = d->m_ui->singularFirstPersonLineEdit;
    d->m_conjugationLineEdits[ KEduVocWordFlag::Second | KEduVocWordFlag::Singular ]
        = d->m_ui->singularSecondPersonLineEdit;

    d->m_conjugationLineEdits[ KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | KEduVocWordFlag::Singular ]
        = d->m_ui->singularThirdMalePersonLineEdit;
    d->m_conjugationLineEdits[ KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Singular ]
        = d->m_ui->singularThirdFemalePersonLineEdit;
    d->m_conjugationLineEdits[ KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Singular ]
        = d->m_ui->singularThirdNeutralPersonLineEdit;

    d->m_conjugationLineEdits[ KEduVocWordFlag::First | KEduVocWordFlag::Dual ]
        = d->m_ui->dualFirstPersonLineEdit;
    d->m_conjugationLineEdits[ KEduVocWordFlag::Second | KEduVocWordFlag::Dual ]
        = d->m_ui->dualSecondPersonLineEdit;

    d->m_conjugationLineEdits[ KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | KEduVocWordFlag::Dual ]
        = d->m_ui->dualThirdMalePersonLineEdit;
    d->m_conjugationLineEdits[ KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Dual ]
        = d->m_ui->dualThirdFemalePersonLineEdit;
    d->m_conjugationLineEdits[ KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Dual ]
        = d->m_ui->dualThirdNeutralPersonLineEdit;

    d->m_conjugationLineEdits[ KEduVocWordFlag::First | KEduVocWordFlag::Plural ]
        = d->m_ui->pluralFirstPersonLineEdit;
    d->m_conjugationLineEdits[ KEduVocWordFlag::Second | KEduVocWordFlag::Plural ]
        = d->m_ui->pluralSecondPersonLineEdit;

    d->m_conjugationLineEdits[ KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | KEduVocWordFlag::Plural ]
        = d->m_ui->pluralThirdMalePersonLineEdit;
    d->m_conjugationLineEdits[ KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Plural ]
        = d->m_ui->pluralThirdFemalePersonLineEdit;
    d->m_conjugationLineEdits[ KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Plural ]
        = d->m_ui->pluralThirdNeutralPersonLineEdit;

    foreach( const KEduVocWordFlags & index, d->m_conjugationLineEdits.keys() ) {
        connect( d->m_conjugationLineEdits.value( index ), SIGNAL( textChanged( const QString& ) ), SLOT( textChanged( const QString& ) ) );
    }
}

KEduVocConjugationWidget::~KEduVocConjugationWidget()
{
    delete d;
}

void KEduVocConjugationWidget::textChanged( const QString& text )
{
    int valueIndex = d->m_conjugationLineEdits.values().indexOf( qobject_cast <QLineEdit*> ( sender() ) );
    int key = d->m_conjugationLineEdits.keys().value( valueIndex );
    d->m_entry->translation( d->m_identifier )->conjugation( d->m_ui->tenseComboBox->currentText() ).setConjugation( text, (KEduVocWordFlag::Flags)key );
}


void KEduVocConjugationWidget::slotTenseSelected( int sel )
{
    Q_UNUSED(sel);
    updateEntries();
}


void KEduVocConjugationWidget::updateEntries()
{
    d->m_lastTenseSelection = d->m_ui->tenseComboBox->currentText();
    KEduVocConjugation& conjugation = d->m_entry->translation( d->m_identifier )->conjugation( d->m_lastTenseSelection );
    foreach( KEduVocWordFlags flags, d->m_conjugationLineEdits.keys() ) {
        QString text;
        if( conjugation.keys().contains( flags ) ) {
            text = conjugation.conjugation( flags ).text();
        }

        d->m_conjugationLineEdits[ flags ]->setText( text );
    }
}

void KEduVocConjugationWidget::setTranslation( KEduVocExpression * entry, int identifier )
{
    d->m_ui->tenseComboBox->clear();
    d->m_ui->tenseComboBox->completionObject()->clear();

    d->m_entry = entry;
    d->m_identifier = identifier;

    if( !d->m_doc || !entry ) {
        setEnabled(false);
        return;
    }

    // init tenses per language
    d->m_ui->tenseComboBox->addItems( d->m_doc->identifier( identifier ).tenseList() );
    d->m_ui->tenseComboBox->completionObject()->insertItems( d->m_doc->identifier( identifier ).tenseList() );
    if( d->m_ui->tenseComboBox->contains( d->m_lastTenseSelection ) ) {
        d->m_ui->tenseComboBox->setCurrentItem( d->m_lastTenseSelection );
    } else {
        d->m_ui->tenseComboBox->setCurrentIndex( 0 );
    }

    setEnabled(true);
    if( entry->translation( d->m_identifier )->wordType()
            && entry->translation( d->m_identifier )->wordType()->wordType() & KEduVocWordFlag::Verb ) {
        updateVisiblePersons();
        updateEntries();
    }
}

void KEduVocConjugationWidget::setDocument( KEduVocDocument * doc )
{
    d->m_doc = doc;
    d->m_ui->tenseComboBox->clear();
    d->m_ui->tenseComboBox->completionObject()->clear();
}

void KEduVocConjugationWidget::slotNextTense()
{
    if( d->m_ui->tenseComboBox->currentIndex() + 1 < d->m_ui->tenseComboBox->count() ) {
        d->m_ui->tenseComboBox->setCurrentIndex( d->m_ui->tenseComboBox->currentIndex() + 1 );
    } else {
        d->m_ui->tenseComboBox->setCurrentIndex( 0 );
    }
    updateEntries();
}

void KEduVocConjugationWidget::updateVisiblePersons()
{
    if( d->m_identifier < 0 ) {
        showWidgets( false, false, false, false, false, false, falsev);
        return;
    }

    bool dualVisible = d->m_doc->identifier( d->m_identifier ).personalPronouns().dualExists();
    bool maleFemaleDifferent = d->m_doc->identifier( d->m_identifier ).personalPronouns().maleFemaleDifferent();
    bool neutralExists = d->m_doc->identifier( d->m_identifier ).personalPronouns().neutralExists();

    showWidgets( true, true, dualVisible, true, maleFemaleDifferent, maleFemaleDifferent, neutralExists || ( !maleFemaleDifferent ) );

    // set up the personal pronouns
    KEduVocPersonalPronoun pron = d->m_doc->identifier( d->m_identifier ).personalPronouns();

    d->m_ui->singularFirstPersonLabel->setText( pron.personalPronoun( KEduVocWordFlag::First | KEduVocWordFlag::Singular ) );
    d->m_ui->singularSecondPersonLabel->setText( pron.personalPronoun( KEduVocWordFlag::Second | KEduVocWordFlag::Singular ) );
    d->m_ui->singularThirdMalePersonLabel->setText( pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | KEduVocWordFlag::Singular ) );
    d->m_ui->singularThirdFemalePersonLabel->setText( pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Singular ) );
    d->m_ui->singularThirdNeutralPersonLabel->setText( pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Singular ) );

    d->m_ui->dualFirstPersonLabel->setText( pron.personalPronoun(KEduVocWordFlag::First | KEduVocWordFlag::Dual ) );
    d->m_ui->dualSecondPersonLabel->setText( pron.personalPronoun( KEduVocWordFlag::Second | KEduVocWordFlag::Dual ) );
    d->m_ui->dualThirdMalePersonLabel->setText( pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | KEduVocWordFlag::Dual ) );
    d->m_ui->dualThirdFemalePersonLabel->setText( pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Dual ) );
    d->m_ui->dualThirdNeutralPersonLabel->setText( pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Dual ) );

    d->m_ui->pluralFirstPersonLabel->setText( pron.personalPronoun( KEduVocWordFlag::First | KEduVocWordFlag::Plural ) );
    d->m_ui->pluralSecondPersonLabel->setText( pron.personalPronoun( KEduVocWordFlag::Second | KEduVocWordFlag::Plural ) );
    d->m_ui->pluralThirdMalePersonLabel->setText( pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | KEduVocWordFlag::Plural ) );
    d->m_ui->pluralThirdFemalePersonLabel->setText( pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Plural ) );
    d->m_ui->pluralThirdNeutralPersonLabel->setText( pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Plural ) );
}

void KEduVocConjugationWidget::showWidgets( bool tenses, bool singular, bool dual, bool plural, bool maleVisible, bool femaleVisible, bool neuterVisible )
{
    d->m_ui->tenselabel->setVisible( tenses );
    d->m_ui->tenseComboBox->setVisible( tenses );
    d->m_ui->nextTenseButton->setVisible( tenses );

    d->m_ui->singularLabel->setVisible( singular );
    d->m_ui->singularFirstPersonLabel->setVisible( singular );
    d->m_ui->singularFirstPersonLineEdit->setVisible( singular );
    d->m_ui->singularSecondPersonLabel->setVisible( singular );
    d->m_ui->singularSecondPersonLineEdit->setVisible( singular );

    d->m_ui->singularThirdMalePersonLabel->setVisible( singular && maleVisible );
    d->m_ui->singularThirdMalePersonLineEdit->setVisible( singular && maleVisible );
    d->m_ui->singularThirdFemalePersonLabel->setVisible( singular && femaleVisible );
    d->m_ui->singularThirdFemalePersonLineEdit->setVisible( singular && femaleVisible );
    d->m_ui->singularThirdNeutralPersonLabel->setVisible( singular && neuterVisible );
    d->m_ui->singularThirdNeutralPersonLineEdit->setVisible( singular && neuterVisible );

    d->m_ui->dualLabel->setVisible( dual );
    d->m_ui->dualFirstPersonLabel->setVisible( dual );
    d->m_ui->dualFirstPersonLineEdit->setVisible( dual );
    d->m_ui->dualSecondPersonLabel->setVisible( dual );
    d->m_ui->dualSecondPersonLineEdit->setVisible( dual );

    d->m_ui->dualThirdMalePersonLabel->setVisible( dual && maleVisible );
    d->m_ui->dualThirdMalePersonLineEdit->setVisible( dual && maleVisible );
    d->m_ui->dualThirdFemalePersonLabel->setVisible( dual && femaleVisible );
    d->m_ui->dualThirdFemalePersonLineEdit->setVisible( dual && femaleVisible );
    d->m_ui->dualThirdNeutralPersonLabel->setVisible( dual && neuterVisible );
    d->m_ui->dualThirdNeutralPersonLineEdit->setVisible( dual && neuterVisible );

    d->m_ui->pluralLabel->setVisible( plural );
    d->m_ui->pluralFirstPersonLabel->setVisible( plural );
    d->m_ui->pluralFirstPersonLineEdit->setVisible( plural );
    d->m_ui->pluralSecondPersonLabel->setVisible( plural );
    d->m_ui->pluralSecondPersonLineEdit->setVisible( plural );

    d->m_ui->pluralThirdMalePersonLabel->setVisible( plural && maleVisible );
    d->m_ui->pluralThirdMalePersonLineEdit->setVisible( plural && maleVisible );
    d->m_ui->pluralThirdFemalePersonLabel->setVisible( plural && femaleVisible );
    d->m_ui->pluralThirdFemalePersonLineEdit->setVisible( plural && femaleVisible );
    d->m_ui->pluralThirdNeutralPersonLabel->setVisible( plural && neuterVisible );
    d->m_ui->pluralThirdNeutralPersonLineEdit->setVisible( plural && neuterVisible );
}

void KEduVocConjugationWidget::tenseEditingFinished()
{
    const QStringList& oldTenses = d->m_doc->identifier( d->m_identifier ).tenseList();
    if( !oldTenses.contains( d->m_ui->tenseComboBox->currentText() ) ) {
        // add a new tense
        d->m_doc->identifier( d->m_identifier ).setTense( oldTenses.count(), d->m_ui->tenseComboBox->currentText() );
        // put it into the completion
        d->m_ui->tenseComboBox->completionObject()->addItem( d->m_ui->tenseComboBox->currentText() );
    }
}