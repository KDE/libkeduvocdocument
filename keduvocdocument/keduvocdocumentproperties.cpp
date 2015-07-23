/***************************************************************************

                   enter document title and author

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>
                     (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvocdocumentproperties.h"

#include "ui_keduvocdocumentproperties.h"

//#include "parleymainwindow.h"
//#include "parleydocument.h"

#include <keduvocdocument.h>
#include <QLineEdit>

#include <QTextEdit>
#include <QLabel>

class KEduVocDocumentProperties::Private
{
public:
    Private( KEduVocDocument * doc, bool languageSetup );

    KEduVocDocument* m_doc;
    // also allow the user to setup two languages - used when first creating a document
    bool m_showLanguages;
    Ui::KEduVocDocumentProperties * m_ui;
};

KEduVocDocumentProperties::Private::Private(KEduVocDocument* doc, bool languageSetup)
{
    m_doc = doc;
    m_showLanguages = languageSetup;
    m_ui = new Ui::KEduVocDocumentProperties();
}

KEduVocDocumentProperties::KEduVocDocumentProperties( KEduVocDocument * doc, bool languageSetup, QWidget* parent )
    : QWidget(parent)
    , d( new Private( doc, languageSetup ) )
{
    d->m_ui->setupUi(this);

    d->m_ui->titleLineEdit->setText( doc->title() );
    d->m_ui->authorLineEdit->setText( doc->author() );
    d->m_ui->contactLineEdit->setText( doc->authorContact() );
    d->m_ui->licenseComboBox->setEditText( doc->license() );
    d->m_ui->commentTextEdit->setText( doc->documentComment() );
    d->m_ui->categoryComboBox->setEditText( doc->category() );

    if (languageSetup) {
        prepareLanguageSelection();
    } else {
        d->m_ui->languageGroupBox->setVisible(false);
    }
}

KEduVocDocumentProperties::~KEduVocDocumentProperties()
{
    delete d;
}

bool KEduVocDocumentProperties::isGrammarCheckBoxChecked()
{
    return d->m_ui->grammarCheckBox->isChecked();
}

bool KEduVocDocumentProperties::isDownloadGrammarCheckBoxChecked()
{
    return d->m_ui->downloadGrammarCheckBox->isChecked();
}

void KEduVocDocumentProperties::setLanguageGroupBoxVisible( bool flag )
{
    d->m_ui->languageGroupBox->setVisible( flag );
}

void KEduVocDocumentProperties::prepareLanguageSelection()
{
    QStringList codes = QLocale().uiLanguages();
    QStringList languageNames;
    foreach( const QString & code, codes ) {
        languageNames.append( QLocale( code ).nativeLanguageName() );
    }
    languageNames.sort();

    d->m_ui->firstLanguageComboBox->addItems( languageNames );
    d->m_ui->firstLanguageComboBox->completionObject()->insertItems( languageNames );
    d->m_ui->firstLanguageComboBox->completionObject()->setIgnoreCase( true );
    d->m_ui->secondLanguageComboBox->addItems( languageNames );
    d->m_ui->secondLanguageComboBox->completionObject()->insertItems( languageNames );
    d->m_ui->secondLanguageComboBox->completionObject()->setIgnoreCase( true );

    d->m_ui->languageGroupBox->setVisible( true );
}

void KEduVocDocumentProperties::accept()
{
    d->m_doc->setTitle( d->m_ui->titleLineEdit->text() );
    d->m_doc->setAuthor( d->m_ui->authorLineEdit->text() );
    d->m_doc->setAuthorContact( d->m_ui->contactLineEdit->text() );
    d->m_doc->setLicense( d->m_ui->licenseComboBox->currentText() );
    d->m_doc->setDocumentComment( d->m_ui->commentTextEdit->toPlainText() );
    d->m_doc->setCategory( d->m_ui->categoryComboBox->currentText() );

    if( d->m_showLanguages ) {
        acceptLanguageConfiguration();
    }
}

void KEduVocDocumentProperties::acceptLanguageConfiguration()
{
    QString firstLanguage = d->m_ui->firstLanguageComboBox->currentText();
    QString firstLocale;
    QString secondLanguage = d->m_ui->secondLanguageComboBox->currentText();
    QString secondLocale;

    // ugly but works for now: iterate over languages to check which code we have
    foreach( const QString & code, QLocale().uiLanguages() ) {
        if( firstLanguage == QLocale( code ).nativeLanguageName() ) {
            firstLocale = code;
        }
        if( secondLanguage == QLocale( code ).nativeLanguageName() ) {
            secondLocale = code;
        }
    }

    d->m_doc->identifier( 0 ).setLocale( firstLocale );
    d->m_doc->identifier( 0 ).setName( firstLanguage );
    d->m_doc->identifier( 1 ).setLocale( secondLocale );
    d->m_doc->identifier( 1 ).setName( secondLanguage );
}
