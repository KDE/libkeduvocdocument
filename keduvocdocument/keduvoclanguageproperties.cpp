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

#include "keduvoclanguageproperties.h"

#include <keduvocdocument.h>
#include <keduvoclanguagepropertiespage.h>

#include <KMessageBox>
#include <QLabel>
#include <KLocalizedString>
#include <QToolTip>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QPushButton>


class KEduVocLanguageProperties::Private
{
public:
    Private( KEduVocDocument* doc );

    KEduVocDocument* m_doc;
    QList<KPageWidgetItem*> m_pages;
    KEduVocLanguagePropertiesPage* m_page;
};

KEduVocLanguageProperties::Private::Private(KEduVocDocument* doc)
{
    m_doc = doc;
    m_page = 0;
}

KEduVocLanguageProperties::KEduVocLanguageProperties( KEduVocDocument* doc, QWidget * parent )
    : KPageDialog( parent )
    , d( new Private( doc ) )
{
    setWindowTitle( i18n( "Edit Languages" ) );
    setFaceType( List );
    setStandardButtons( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );

    QAbstractButton * addLangButton(
        buttonBox()->addButton( i18n( "Add language" ), QDialogButtonBox::ActionRole ) );
    addLangButton->setIcon( QIcon::fromTheme( "list-add" ) );

    QAbstractButton * removeLangButton(
        buttonBox()->addButton( i18n( "Remove language" ), QDialogButtonBox::ActionRole ) );
    removeLangButton->setIcon( QIcon::fromTheme( "list-remove" ) );

    connect( addLangButton, SIGNAL( clicked() ), this, SLOT( slotAppendIdentifier() ) );
    connect( removeLangButton, SIGNAL( clicked() ), this, SLOT( slotDeleteIdentifier() ) );

    for( int i = 0; i < d->m_doc->identifierCount(); i++ ) {
        createPage( i );
    }
}

KEduVocLanguageProperties::~KEduVocLanguageProperties()
{
    delete d;
}


/************** Slots for KEduVocLanguagePropertiesPage signals *****************/

void KEduVocLanguageProperties::loadLanguageSettings( QString locale ) {
    emit languageSettingsChanged( locale );
}

void KEduVocLanguageProperties::loadEditorFont( QString locale ) {
    emit editorFontChanged( locale, d->m_page );
}

void KEduVocLanguageProperties::loadPracticeFont( QString locale ) {
    emit practiceFontChanged( locale, d->m_page );
}

void KEduVocLanguageProperties::loadKeyboardLayout( QString locale ) {
    emit keyboardLayoutChanged( locale, d->m_page );
}

void KEduVocLanguageProperties::loadSpellChecker( QString locale ) {
    emit spellCheckerChanged( locale, d->m_page );
}

void KEduVocLanguageProperties::saveEditorFont( QString locale, QFont font ) {
    emit storeEditorFont( locale, font );
}

void KEduVocLanguageProperties::savePracticeFont( QString locale, QFont font ) {
    emit storePracticeFont( locale, font );
}

void KEduVocLanguageProperties::saveKeyboardLayout( QString locale, QString keyboardLayout ) {
    emit storeKeyboardLayout( locale, keyboardLayout );
}

void KEduVocLanguageProperties::saveSpellChecker( QString locale, QString spellChecker ) {
    emit storeSpellChecker( locale, spellChecker );
}

void KEduVocLanguageProperties::storeSettings( QString locale ) {
    emit saveSettings( locale );
}


KPageWidgetItem*  KEduVocLanguageProperties::createPage( int i )
{
    d->m_page = new KEduVocLanguagePropertiesPage( d->m_doc, i, this );

    // signal-slot connections between KEduVocLanguagePropertiesPage and KEduVocLanguageProperties
    connect( d->m_page, &KEduVocLanguagePropertiesPage::languageSettingsChanged, this, &KEduVocLanguageProperties::loadLanguageSettings );
    connect( d->m_page, &KEduVocLanguagePropertiesPage::editorFontChanged, this, &KEduVocLanguageProperties::loadEditorFont);
    connect( d->m_page, &KEduVocLanguagePropertiesPage::practiceFontChanged, this, &KEduVocLanguageProperties::loadPracticeFont );
    connect( d->m_page, &KEduVocLanguagePropertiesPage::keyboardLayoutChanged, this, &KEduVocLanguageProperties::loadKeyboardLayout );
    connect( d->m_page, &KEduVocLanguagePropertiesPage::spellCheckerChanged, this, &KEduVocLanguageProperties::loadSpellChecker );
    connect( d->m_page, &KEduVocLanguagePropertiesPage::storeEditorFont, this, &KEduVocLanguageProperties::saveEditorFont );
    connect( d->m_page, &KEduVocLanguagePropertiesPage::storePracticeFont, this, &KEduVocLanguageProperties::savePracticeFont );
    connect( d->m_page, &KEduVocLanguagePropertiesPage::storeKeyboardLayout, this, &KEduVocLanguageProperties::saveKeyboardLayout );
    connect( d->m_page, &KEduVocLanguagePropertiesPage::storeSpellChecker, this, &KEduVocLanguageProperties::saveSpellChecker );;
    connect( d->m_page, &KEduVocLanguagePropertiesPage::saveSettings, this, &KEduVocLanguageProperties::storeSettings );

    QString name = i18n( "New Language" );

    // check if this language already exists in the doc
    if( d->m_doc->identifierCount() <= i ) {
      d->m_doc->appendIdentifier();
    }
    if( d->m_doc->identifierCount() > i ) {
        name = d->m_doc->identifier( i ).name();
        emit languageSettingsChanged( d->m_doc->identifier(i).locale() );
    }

    KPageWidgetItem* editPage = new KPageWidgetItem( d->m_page, name );
    editPage->setHeader( i18nc( "Edit language properties", "Properties for %1", name ) );

    d->m_pages.append( editPage );
    addPage( editPage );

    editPage->setIcon( QIcon::fromTheme( "set-language" ) );

    connect( d->m_page->identifierNameLineEdit(), &QLineEdit::textChanged, this, &KEduVocLanguageProperties::pageNameChanged );
    connect( this, &KEduVocLanguageProperties::accepted, d->m_page, &KEduVocLanguagePropertiesPage::accept );

    return editPage;
}

void KEduVocLanguageProperties::accept()
{
    KEduVocDocument *doc = d->m_doc;
    int deleted = 0;

    for( int index = 0; index < d->m_pages.count(); index++ ) {
        if( d->m_pages.value( index )->isEnabled() ) {
            if( index - deleted >= doc->identifierCount() ) {
                // add a language
                doc->appendIdentifier();
            }
            d->m_page = static_cast<KEduVocLanguagePropertiesPage *>( d->m_pages.value( index )->widget() );
            connect( d->m_page, &KEduVocLanguagePropertiesPage::languageSettingsChanged, this, &KEduVocLanguageProperties::loadLanguageSettings );
            connect( d->m_page, &KEduVocLanguagePropertiesPage::editorFontChanged, this, &KEduVocLanguageProperties::loadEditorFont );
            connect( d->m_page, &KEduVocLanguagePropertiesPage::practiceFontChanged, this, &KEduVocLanguageProperties::loadPracticeFont );
            connect( d->m_page, &KEduVocLanguagePropertiesPage::keyboardLayoutChanged, this, &KEduVocLanguageProperties::loadKeyboardLayout );
            connect( d->m_page, &KEduVocLanguagePropertiesPage::spellCheckerChanged, this, &KEduVocLanguageProperties::loadSpellChecker );
            connect( d->m_page, &KEduVocLanguagePropertiesPage::storeEditorFont, this, &KEduVocLanguageProperties::saveEditorFont );
            connect( d->m_page, &KEduVocLanguagePropertiesPage::storePracticeFont, this, &KEduVocLanguageProperties::savePracticeFont );
            connect( d->m_page, &KEduVocLanguagePropertiesPage::storeKeyboardLayout, this, &KEduVocLanguageProperties::saveKeyboardLayout );
            connect( d->m_page, &KEduVocLanguagePropertiesPage::storeSpellChecker, this, &KEduVocLanguageProperties::saveSpellChecker );;
            connect( d->m_page, &KEduVocLanguagePropertiesPage::saveSettings, this, &KEduVocLanguageProperties::storeSettings );
            d->m_page->setLanguageIdentifierIndex( index - deleted );
        } else {
            // page is disabled, delete the language
            if( index < doc->identifierCount() ) {
                if( KMessageBox::warningYesNo( this, i18n( "Really delete language: %1?", doc->identifier( index - deleted ).name() ), i18n( "Remove Language" ) ) == KMessageBox::Yes ) {
                    doc->removeIdentifier( index - deleted );
                    deleted++;
                }
            }
        }
    }

    KPageDialog::accept();
}


void KEduVocLanguageProperties::slotAppendIdentifier()
{
    // if a page that was "removed" is selected, simply enable it again to not delete it.
    if( currentPage() && !currentPage()->isEnabled() ) {
        currentPage()->setEnabled( true );
        return;
    }

    int i = d->m_pages.count();
    KPageWidgetItem* newPage = createPage( i );
    setCurrentPage( newPage );
}

void KEduVocLanguageProperties::slotDeleteIdentifier()
{
    currentPage()->setEnabled( false );
}

void KEduVocLanguageProperties::pageNameChanged( const QString & newName )
{
    currentPage()->setName( newName );
}
