/***************************************************************************
    Copyright 2008 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvoclanguagepropertiespage.h"
#include "ui_keduvoclanguagepropertiespage.h"

#include <KMessageBox>
#include <KLocalizedString>

#include <QDebug>
#include <QLineEdit>
#include <QInputDialog>
#include <QCheckBox>
#include <QLabel>
#include <QtDBus>
#include <sonnet/speller.h>

#include <keduvocdocument.h>
#include <keduvocexpression.h>
#include <keduvoclesson.h>

#define TENSE_TAG ". "

class KEduVocLanguagePropertiesPage::Private
{
public:
    Private( KEduVocDocument *doc, int identifierIndex );

    int m_currentTense;
    QList<int> m_tenseIndex;
    KEduVocDocument* m_doc;
    int m_identifierIndex;
    QString m_keyboardLayout;
    QString m_spellChecker;
    Ui::KEduVocLanguagePropertiesPage *m_ui;
};

KEduVocLanguagePropertiesPage::Private::Private(KEduVocDocument* doc, int identifierIndex)
{
    m_doc = doc;
    m_identifierIndex = identifierIndex;
    m_ui = new Ui::KEduVocLanguagePropertiesPage();
}

KEduVocLanguagePropertiesPage::KEduVocLanguagePropertiesPage( KEduVocDocument *doc, int identifierIndex, QWidget *parent)
    : QWidget( parent )
    , d( new Private( doc, identifierIndex ) )
{
    d->m_ui->setupUi( this );

    connect( d->m_ui->localeComboBox, static_cast<void (KComboBox::*)( const QString & )>( &KComboBox::currentIndexChanged ), this, &KEduVocLanguagePropertiesPage::localeChanged );
    connect( d->m_ui->downloadGrammarButton, &QPushButton::clicked, this, &KEduVocLanguagePropertiesPage::downloadGrammar );

    QStringList languageCodes =  QLocale().uiLanguages();

    // qmap automatically sorts by keys
    QMap <QString, QString> languageCodeMap;
    foreach( const QString & code, languageCodes ) {
        languageCodeMap[ QLocale( code ).nativeLanguageName() ] = code;
    }
    // add the language, but also it's code as data
    foreach( const QString & language, languageCodeMap.keys() ) {
        d->m_ui->localeComboBox->addItem( language, languageCodeMap.value( language ) );
    }

    if( d->m_identifierIndex < d->m_doc->identifierCount() ) {
        d->m_ui->localeComboBox->setCurrentIndex( d->m_ui->localeComboBox->findData(
                                    d->m_doc->identifier( d->m_identifierIndex ).locale() ) );

        d->m_ui->identifierNameLineEdit->setText( d->m_doc->identifier( d->m_identifierIndex ).name() );
    }

    int index = ( d->m_identifierIndex < d->m_doc->identifierCount() ? d->m_identifierIndex : 0 );
    emit languageSettingsChanged( d->m_doc->identifier( index ).locale() );

    // fonts
    emit editorFontChanged( d->m_doc->identifier( index ).locale() );
    emit practiceFontChanged( d->m_doc->identifier( index ).locale() );

    // keyboard layout
    // try to talk to kxbk - get a list of keyboard layouts
    QDBusInterface kxbk( "org.kde.keyboard", "/Layouts", "org.kde.KeyboardLayouts" );
    QDBusReply <QStringList> reply = kxbk.call( "getLayoutsList" );
    if( reply.isValid() ) {
        QStringList layouts = reply;
        layouts.prepend( QString() );
        d->m_ui->keyboardLayoutComboBox->clear();
        d->m_ui->keyboardLayoutComboBox->addItems( layouts );
        d->m_ui->keyboardLayoutComboBox->setEnabled( true );

        QDBusReply <QString> currentLayout = kxbk.call( "getCurrentLayout" );
        d->m_ui->keyboardLayoutComboBox->setCurrentIndex( d->m_ui->keyboardLayoutComboBox->findText( currentLayout ) );
        if( d->m_identifierIndex < d->m_doc->identifierCount() ) {
            emit keyboardLayoutChanged( d->m_doc->identifier( index ).locale() );
            if( !d->m_keyboardLayout.isEmpty() ) {
                d->m_ui->keyboardLayoutComboBox->setCurrentIndex( d->m_ui->keyboardLayoutComboBox->findText( d->m_keyboardLayout ) );
            }
        }
    } else {
        qDebug() << "kxkb dbus error";
        d->m_ui->keyboardLayoutComboBox->setEnabled( false );
        d->m_ui->keyboardLayoutComboBox->addItem( i18n( "No KDE keyboard selector found." ) );
    }

    Sonnet::Speller speller;
    QMap <QString, QString> dicts = speller.availableDictionaries();
    QMap <QString, QString>::const_iterator iter = dicts.constBegin();
    while( iter != dicts.constEnd() ) {
        d->m_ui->spellcheckerComboBox->addItem( iter.key(), iter.value() );
        ++iter;
    }
    d->m_ui->spellcheckerComboBox->model()->sort( 0 );
    emit spellCheckerChanged( d->m_doc->identifier( index ).locale() );
    d->m_ui->spellcheckerComboBox->setCurrentIndex( d->m_ui->spellcheckerComboBox->findData( d->m_spellChecker ) );
    loadGrammarFromDocument();
}

KEduVocLanguagePropertiesPage::~KEduVocLanguagePropertiesPage()
{
    delete d;
}

int KEduVocLanguagePropertiesPage::languageIdentifierIndex()
{
    return d->m_identifierIndex;
}

void KEduVocLanguagePropertiesPage::setLanguageIdentifierIndex( int newIndex )
{
    d->m_identifierIndex = newIndex;
}

QFont KEduVocLanguagePropertiesPage::editorFont()
{
    return d->m_ui->editorFont->font();
}

void KEduVocLanguagePropertiesPage::setEditorFont( QFont editorFont )
{
    d->m_ui->editorFont->setFont( editorFont );
}

QFont KEduVocLanguagePropertiesPage::practiceFont()
{
    return d->m_ui->practiceFont->font();
}

void KEduVocLanguagePropertiesPage::setPracticeFont( QFont practiceFont )
{
    d->m_ui->practiceFont->setFont( practiceFont );
}

QString KEduVocLanguagePropertiesPage::keyboardLayout()
{
    return d->m_keyboardLayout;
}

void KEduVocLanguagePropertiesPage::setKeyboardLayout( QString keyboardLayout )
{
    d->m_keyboardLayout = keyboardLayout;
}

QString KEduVocLanguagePropertiesPage::spellChecker()
{
    return d->m_spellChecker;
}

void KEduVocLanguagePropertiesPage::setSpellChecker( QString spellChecker )
{
    d->m_spellChecker = spellChecker;
}

/* TODO uncomment after adding ParleyDocument 
namespace DocumentHelper
{
void fetchGrammar( KEduVocDocument* doc, int languageIndex );
}
*/

void KEduVocLanguagePropertiesPage::downloadGrammar()
{
    /* TODO uncomment after adding ParleyDocument 
    DocumentHelper::fetchGrammar( d->m_doc, d->m_identifierIndex );*/
    loadGrammarFromDocument();
}

void KEduVocLanguagePropertiesPage::loadGrammarFromDocument()
{
    if( d->m_identifierIndex < d->m_doc->identifierCount() ) {
        // articles
        KEduVocArticle articles = d->m_doc->identifier( d->m_identifierIndex ).article();

        d->m_ui->def_male->setText( articles.article( KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Masculine ) );
        d->m_ui->indef_male->setText( articles.article( KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Masculine ) );

        d->m_ui->def_female->setText( articles.article( KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Feminine ) );
        d->m_ui->indef_female->setText( articles.article( KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Feminine ) );

        d->m_ui->def_natural->setText( articles.article( KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Neuter ) );
        d->m_ui->indef_natural->setText( articles.article( KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Neuter ) );


        d->m_ui->def_male_plural->setText( articles.article( KEduVocWordFlag::Plural | KEduVocWordFlag::Definite | KEduVocWordFlag::Masculine ) );
        d->m_ui->indef_male_plural->setText( articles.article( KEduVocWordFlag::Plural | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Masculine ) );

        d->m_ui->def_female_plural->setText( articles.article( KEduVocWordFlag::Plural | KEduVocWordFlag::Definite | KEduVocWordFlag::Feminine ) );
        d->m_ui->indef_female_plural->setText( articles.article( KEduVocWordFlag::Plural | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Feminine ) );

        d->m_ui->def_natural_plural->setText( articles.article( KEduVocWordFlag::Plural | KEduVocWordFlag::Definite | KEduVocWordFlag::Neuter ) );
        d->m_ui->indef_natural_plural->setText( articles.article( KEduVocWordFlag::Plural | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Neuter ) );

        // personal pronouns
        const KEduVocWordFlags numS = KEduVocWordFlag::Singular;
        const KEduVocWordFlags numD = KEduVocWordFlag::Dual;
        const KEduVocWordFlags numP = KEduVocWordFlag::Plural;

        KEduVocPersonalPronoun pronoun = d->m_doc->identifier( d->m_identifierIndex ).personalPronouns();

        d->m_ui->first_singular->setText( pronoun.personalPronoun( KEduVocWordFlag::First | numS ) );
        d->m_ui->second_singular->setText( pronoun.personalPronoun( KEduVocWordFlag::Second | numS ) );
        d->m_ui->thirdM_singular->setText( pronoun.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | numS ) );
        d->m_ui->thirdF_singular->setText( pronoun.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | numS ) );
        d->m_ui->thirdN_singular->setText( pronoun.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | numS ) );

        d->m_ui->dualFirstLineEdit->setText( pronoun.personalPronoun( KEduVocWordFlag::First | numD ) );
        d->m_ui->dualSecondLineEdit->setText( pronoun.personalPronoun( KEduVocWordFlag::Second | numD ) );
        d->m_ui->dualThirdMaleLineEdit->setText( pronoun.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | numD ) );
        d->m_ui->dualThirdFemaleLineEdit->setText( pronoun.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | numD ) );
        d->m_ui->dualThirdNeutralLineEdit->setText( pronoun.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | numD ) );

        d->m_ui->first_plural->setText( pronoun.personalPronoun( KEduVocWordFlag::First | numP ) );
        d->m_ui->second_plural->setText( pronoun.personalPronoun( KEduVocWordFlag::Second | numP ) );
        d->m_ui->thirdM_plural->setText( pronoun.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | numP ) );
        d->m_ui->thirdF_plural->setText( pronoun.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | numP ) );
        d->m_ui->thirdN_plural->setText( pronoun.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | numP ) );

        d->m_ui->maleFemaleDifferCheckBox->setChecked( pronoun.maleFemaleDifferent() );
        d->m_ui->neutralCheckBox->setChecked( pronoun.neutralExists() );
        d->m_ui->dualCheckBox->setChecked( pronoun.dualExists() );
    }

    // update shown labels etc...
    updateCheckBoxes();

    connect( d->m_ui->maleFemaleDifferCheckBox, &QCheckBox::toggled, this, &KEduVocLanguagePropertiesPage::updateCheckBoxes );
    connect( d->m_ui->neutralCheckBox, &QCheckBox::toggled, this, &KEduVocLanguagePropertiesPage::updateCheckBoxes );
    connect( d->m_ui->dualCheckBox, &QCheckBox::toggled, this, &KEduVocLanguagePropertiesPage::updateCheckBoxes );

    // tenses
    connect( d->m_ui->tenseList, &QListWidget::currentRowChanged, this, &KEduVocLanguagePropertiesPage::slotTenseChosen );
    connect( d->m_ui->deleteButton, &QPushButton::clicked, this, &KEduVocLanguagePropertiesPage::slotDeleteTense );
    connect( d->m_ui->modifyButton, &QPushButton::clicked, this, &KEduVocLanguagePropertiesPage::slotModifyTense );
    connect( d->m_ui->newButton, &QPushButton::clicked, this, &KEduVocLanguagePropertiesPage::slotNewTense );

    if( d->m_identifierIndex < d->m_doc->identifierCount() ) {
        int i = 1;
        foreach( const QString & tenseName, d->m_doc->identifier( d->m_identifierIndex ).tenseList() ) {
            d->m_ui->tenseList->addItem( QString( "%1" ).arg( i++, 2 ).append( TENSE_TAG ).append( tenseName ) );
            d->m_tenseIndex.append(i);
        }
    }

    d->m_currentTense = 0;
    if( d->m_ui->tenseList->count() > 0 ) {
        d->m_ui->tenseList->setCurrentRow( d->m_currentTense );
    }

    d->m_ui->modifyButton->setEnabled( d->m_ui->tenseList->count() > 0 );
    d->m_ui->deleteButton->setEnabled( d->m_ui->tenseList->count() > 0);
    d->m_ui->tenseList->setFocus();
}

void KEduVocLanguagePropertiesPage::accept()
{
    if( !isEnabled() ) {
        // disabled pages are deleted languages
        return;
    }

    // general
    QString locale = d->m_ui->localeComboBox->itemData( d->m_ui->localeComboBox->currentIndex() ).toString();

    // if no locales are found (kdebase not installed?) at least fall back to using the language name as locale.
    if( locale.isEmpty() ) {
        locale = d->m_ui->identifierNameLineEdit->text();
    }
    d->m_doc->identifier( d->m_identifierIndex ).setLocale( locale );
    d->m_doc->identifier( d->m_identifierIndex ).setName( d->m_ui->identifierNameLineEdit->text() );

    emit storeEditorFont( locale, d->m_ui->editorFont->font() );
    emit storePracticeFont( locale, d->m_ui->practiceFont->font() );

    if( d->m_ui->keyboardLayoutComboBox->isEnabled() ) {
        emit storeKeyboardLayout( locale, d->m_ui->keyboardLayoutComboBox->currentText() );
    }
    int index = d->m_ui->spellcheckerComboBox->currentIndex();
    emit storeSpellChecker( locale, d->m_ui->spellcheckerComboBox->itemData(index).toString() );
    emit saveSettings( locale );

    // articles
    const KEduVocWordFlag::Flags artSing = KEduVocWordFlag::Singular;
//    const KEduVocWordFlag::Flags artDual = KEduVocWordFlag::Dual;
    const KEduVocWordFlag::Flags artPlur = KEduVocWordFlag::Plural;

    const KEduVocWordFlag::Flags artDef = KEduVocWordFlag::Definite;
    const KEduVocWordFlag::Flags artIndef = KEduVocWordFlag::Indefinite;

    KEduVocArticle article;

    article.setArticle( d->m_ui->def_male->text(),  artSing | artDef | KEduVocWordFlag::Masculine );
    article.setArticle( d->m_ui->indef_male->text(),  artSing | artIndef | KEduVocWordFlag::Masculine );
    article.setArticle( d->m_ui->def_female->text(),  artSing | artDef | KEduVocWordFlag::Feminine );
    article.setArticle( d->m_ui->indef_female->text(),  artSing | artIndef | KEduVocWordFlag::Feminine );
    article.setArticle( d->m_ui->def_natural->text(),  artSing | artDef | KEduVocWordFlag::Neuter );
    article.setArticle( d->m_ui->indef_natural->text(),  artSing | artIndef | KEduVocWordFlag::Neuter );


    article.setArticle( d->m_ui->def_male_plural->text(),  artPlur | artDef | KEduVocWordFlag::Masculine );
    article.setArticle( d->m_ui->indef_male_plural->text(),  artPlur | artIndef | KEduVocWordFlag::Masculine );
    article.setArticle( d->m_ui->def_female_plural->text(),  artPlur | artDef | KEduVocWordFlag::Feminine );
    article.setArticle( d->m_ui->indef_female_plural->text(),  artPlur | artIndef | KEduVocWordFlag::Feminine );
    article.setArticle( d->m_ui->def_natural_plural->text(),  artPlur | artDef | KEduVocWordFlag::Neuter );
    article.setArticle( d->m_ui->indef_natural_plural->text(),  artPlur | artIndef | KEduVocWordFlag::Neuter );

    d->m_doc->identifier( d->m_identifierIndex ).setArticle( article );

    // personal pronouns
    KEduVocPersonalPronoun pronoun;
    const KEduVocWordFlags numS = KEduVocWordFlag::Singular;
    const KEduVocWordFlags numD = KEduVocWordFlag::Dual;
    const KEduVocWordFlags numP = KEduVocWordFlag::Plural;

    pronoun.setPersonalPronoun( d->m_ui->first_singular->text(),  KEduVocWordFlag::First | numS );
    pronoun.setPersonalPronoun( d->m_ui->second_singular->text(), KEduVocWordFlag::Second | numS );
    pronoun.setPersonalPronoun( d->m_ui->thirdM_singular->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | numS );
    pronoun.setPersonalPronoun( d->m_ui->thirdF_singular->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | numS );
    pronoun.setPersonalPronoun( d->m_ui->thirdN_singular->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | numS) ;

    pronoun.setPersonalPronoun( d->m_ui->dualFirstLineEdit->text(), KEduVocWordFlag::First | numD );
    pronoun.setPersonalPronoun( d->m_ui->dualSecondLineEdit->text(), KEduVocWordFlag::Second | numD );
    pronoun.setPersonalPronoun( d->m_ui->dualThirdMaleLineEdit->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | numD );
    pronoun.setPersonalPronoun( d->m_ui->dualThirdFemaleLineEdit->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | numD );
    pronoun.setPersonalPronoun( d->m_ui->dualThirdNeutralLineEdit->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | numD );

    pronoun.setPersonalPronoun( d->m_ui->first_plural->text(), KEduVocWordFlag::First | numP );
    pronoun.setPersonalPronoun( d->m_ui->second_plural->text(), KEduVocWordFlag::Second |  numP );
    pronoun.setPersonalPronoun( d->m_ui->thirdM_plural->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | numP );
    pronoun.setPersonalPronoun( d->m_ui->thirdF_plural->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | numP );
    pronoun.setPersonalPronoun( d->m_ui->thirdN_plural->text(), KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | numP );

    pronoun.setMaleFemaleDifferent( d->m_ui->maleFemaleDifferCheckBox->isChecked() );
    pronoun.setNeutralExists( d->m_ui->neutralCheckBox->isChecked() );
    pronoun.setDualExists( d->m_ui->dualCheckBox->isChecked() );

    d->m_doc->identifier( d->m_identifierIndex ).setPersonalPronouns( pronoun );

    // tenses
    QList<int> tenseIndex;
    QStringList tenses;

    QString str;
    for( int i = 0; i < (int) d->m_ui->tenseList->count(); i++ ) {
        str = d->m_ui->tenseList->item( i )->text();
        tenses.append( str.mid( str.indexOf( TENSE_TAG ) + QString( TENSE_TAG ).length() ) );
    }

    d->m_doc->identifier( d->m_identifierIndex ).setTenseList( tenses );
}

void KEduVocLanguagePropertiesPage::updateCheckBoxes()
{
    bool maleFemale = d->m_ui->maleFemaleDifferCheckBox->isChecked();
    bool neutral = d->m_ui->neutralCheckBox->isChecked();
    bool dual = d->m_ui->dualCheckBox->isChecked();

    d->m_ui->neutralCheckBox->setVisible( maleFemale );
    d->m_ui->male_c_label->setVisible( maleFemale );
    d->m_ui->female_c_label->setVisible( maleFemale );
    d->m_ui->thirdM_singular->setVisible( maleFemale );
    d->m_ui->thirdF_singular->setVisible( maleFemale );
    d->m_ui->thirdM_plural->setVisible( maleFemale );
    d->m_ui->thirdF_plural->setVisible( maleFemale );

    d->m_ui->dualLabel->setVisible( dual );
    d->m_ui->dualFirstLineEdit->setVisible( dual );
    d->m_ui->dualSecondLineEdit->setVisible( dual );

    d->m_ui->dualThirdMaleLineEdit->setVisible( dual && maleFemale );
    d->m_ui->dualThirdFemaleLineEdit->setVisible( dual && maleFemale );

    if( !maleFemale ) {
        d->m_ui->natural_c_label->setVisible( false );
        d->m_ui->thirdN_singular->setVisible( true );
        d->m_ui->thirdN_plural->setVisible( true );
        d->m_ui->dualThirdNeutralLineEdit->setVisible( dual );
    } else {
        d->m_ui->natural_c_label->setVisible( neutral );
        d->m_ui->thirdN_singular->setVisible( neutral );
        d->m_ui->thirdN_plural->setVisible( neutral );
        d->m_ui->dualThirdNeutralLineEdit->setVisible( dual && neutral );
    }
}

void KEduVocLanguagePropertiesPage::localeChanged( const QString & locale )
{
    d->m_ui->identifierNameLineEdit->setText( locale );
}


// ************** TENSES *********************

void KEduVocLanguagePropertiesPage::slotTenseChosen( int index )
{
    d->m_currentTense = index;
}

void KEduVocLanguagePropertiesPage::slotNewTense()
{
    bool ok;
    QString getTense = QInputDialog::getText( this, i18n( "Tense Name" ), i18n( "Enter name of tense:" ), QLineEdit::Normal, QString(), &ok );
    if( !ok )
        return;

    QString str;
    int i = d->m_ui->tenseList->count() + 1;
    d->m_ui->tenseList->addItem( QString( "%1" ).arg( i, 2 ).append( TENSE_TAG ).append( getTense.simplified() ) );
    d->m_tenseIndex.append( -( i - 1 ) );

    d->m_currentTense = d->m_ui->tenseList->count();
    d->m_ui->tenseList->setCurrentRow( i - 1 );
    d->m_ui->modifyButton->setEnabled( true );
    d->m_ui->deleteButton->setEnabled( true );
}

void KEduVocLanguagePropertiesPage::slotModifyTense()
{
    if( d->m_ui->tenseList->count() != 0 && (int) d->m_ui->tenseList->count() > d->m_currentTense ) {
        QString str = d->m_ui->tenseList->item( d->m_currentTense )->text();
        str = str.mid( str.indexOf( TENSE_TAG ) + QString( TENSE_TAG ).length() );

        bool ok;
        QString getTense = QInputDialog::getText( this, i18n( "Tense Name" ), i18n( "Enter name of tense:" ), QLineEdit::Normal, str, &ok );
        if( !ok )
            return;

        int i = d->m_currentTense + 1;
        d->m_ui->tenseList->item( d->m_currentTense )->setText( QString( "%1" ).arg( i, 2 ).append( TENSE_TAG ).append( getTense.simplified() ) );
    }
}

void KEduVocLanguagePropertiesPage::updateListBox( int start )
{
    QString str;
    for( int i = start; i < (int) d->m_ui->tenseList->count(); i++ ) {
        str = d->m_ui->tenseList->item( i )->text();
        str = str.mid( str.indexOf( TENSE_TAG ) + QString( TENSE_TAG ).length() );
        d->m_ui->tenseList->item( i )->setText( QString( "%1" ).arg( i + 1, 2 ).append( TENSE_TAG ).append( str ) );
    }
}

void KEduVocLanguagePropertiesPage::slotDeleteTense()
{
    int act = d->m_currentTense;
    if( d->m_ui->tenseList->count() > 0 && (int) d->m_ui->tenseList->count() > act ) {

        QString t = d->m_ui->tenseList->item( act )->text();

        foreach( KEduVocExpression * exp, d->m_doc->lesson()->entries( KEduVocLesson::Recursive ) ) {
            for( int lang = 0; lang < d->m_doc->identifierCount(); lang++ ) {
                if( exp->translation( lang )->conjugationTenses().contains( t ) ) {
                    KMessageBox::information( this, i18n( "The selected user defined tense could not be deleted\nbecause it is in use." ),    i18n( "Deleting Tense Description" ) );
                    return;
                }
            }
        }

        delete d->m_ui->tenseList->takeItem( act );
        d->m_tenseIndex.erase( d->m_tenseIndex.begin() + act );

        if( (int) d->m_ui->tenseList->count() <= act )
            act = d->m_ui->tenseList->count() - 1;
        else
            updateListBox( act ); // update items after current

        if( act >= 0 )
            d->m_ui->tenseList->setCurrentRow( act );
    }
    d->m_ui->modifyButton->setEnabled( d->m_ui->tenseList->count() > 0 );
    d->m_ui->deleteButton->setEnabled( d->m_ui->tenseList->count() > 0 );
}
