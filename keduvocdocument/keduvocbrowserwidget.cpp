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

#include "keduvocbrowserwidget.h"
#include "ui_keduvocbrowserwidget.h"

#include <keduvocexpression.h>
#include <KHTMLPart>

#include <QDragEnterEvent>

using namespace Editor;

class KEduVocBrowserWidget::Private
{
public:
    Private();

    /// Column in the document - corresponds to the language (-KV_COL_TRANS)
    int m_currentTranslation;
    /// Selection in the doc - if more than one row is selected behavior is different
    KEduVocExpression* m_entry;
    KHTMLPart *m_htmlPart;
    QList <DictionaryProvider> m_providers;
    int m_currentProvider;
    Ui::KEduVocBrowserWidget* m_ui;
};

KEduVocBrowserWidget::Private::Private()
{
    m_currentTranslation = -1;
    m_entry = 0;
    m_currentProvider = 0; ///@todo kconfig?
    m_htmlPart = 0;
    m_ui = new Ui::KEduVocBrowserWidget();
}

KEduVocBrowserWidget::KEduVocBrowserWidget( QWidget *parent )
    : QWidget( parent )
    , d( new Private() )
{
    d->m_ui->setupUi( this );

    d->m_htmlPart = new KHTMLPart( d->m_ui->widget );
    QHBoxLayout *layout = new QHBoxLayout( d->m_ui->widget );
    d->m_ui->widget->setLayout( layout );
    layout->addWidget( d->m_htmlPart->widget() );

    connect( d->m_htmlPart->browserExtension(), SIGNAL( openUrlRequest( const QUrl&, const KParts::OpenUrlArguments&, const KParts::BrowserArguments& ) ),
            this, SLOT( openUrl( const QUrl& ) ) );

    connect( d->m_ui->showCurrentButton, &QPushButton::clicked, this, &KEduVocBrowserWidget::showCurrentTranslation );

    setupProviders();
    connect( d->m_ui->providerComboBox, static_cast<void (KComboBox::*)(int)>( &KComboBox::currentIndexChanged ), this, &KEduVocBrowserWidget::providerChanged );

    d->m_htmlPart->setJavaEnabled( false );
    d->m_htmlPart->setPluginsEnabled( false );
    d->m_htmlPart->setJScriptEnabled( true );
    d->m_htmlPart->setMetaRefreshEnabled( true );
    d->m_htmlPart->setDNDEnabled( true );
}

KEduVocBrowserWidget::~KEduVocBrowserWidget()
{
    delete d;
}

void KEduVocBrowserWidget::setupProviders()
{
    d->m_ui->providerComboBox->clear();

    DictionaryProvider provider;
    provider.name = "De-Es Beolingus";
    provider.url = "http://beolingus.org/dings.cgi?service=dees&query=\\{@}";
    provider.languages << "de" << "es";
    d->m_providers.append( provider );
    d->m_ui->providerComboBox->addItem( provider.name );

    provider.name = "De-En Beolingus";
    provider.url = "http://beolingus.org/dings.cgi?query=\\{@}";
    provider.languages << "de" << "en";
    d->m_providers.append( provider );
    d->m_ui->providerComboBox->addItem( provider.name );

    provider.name = "De-En Leo";
    provider.url = "http://dict.leo.org/?search=\\{@}";
    provider.languages << "de" << "en";
    d->m_providers.append( provider );
    d->m_ui->providerComboBox->addItem( provider.name );

    provider.name = "De-Es Leo";
    provider.url = "http://dict.leo.org/?lp=esde&search=\\{@}";
    provider.languages << "de" << "es";
    d->m_providers.append( provider );
    d->m_ui->providerComboBox->addItem( provider.name );

    provider.name = "De-Fr Leo";
    provider.url = "http://dict.leo.org/?lp=frde&search=\\{@}";
    provider.languages << "de" << "fr";
    d->m_providers.append( provider );
    d->m_ui->providerComboBox->addItem( provider.name );

    provider.name = "De-Es Pons";
    provider.url = "http://www.ponsline.de/cgi-bin/wb/w.pl?von=pons.de&Richtung=Spanisch&Begriff=\\{@}";
    provider.languages << "de" << "es";
    d->m_providers.append( provider );
    d->m_ui->providerComboBox->addItem( provider.name );

    provider.name = "Ru-En Multitran";
    provider.url = "http://multitran.ru/c/m.exe?SHL=1&CL=1&l1=1&s=\\{@}";
    provider.languages << "ru" << "en";
    d->m_providers.append( provider );
    d->m_ui->providerComboBox->addItem( provider.name );

    provider.name = "Ru-De Multitran";
    provider.url = "http://multitran.ru/c/m.exe?SHL=1&CL=1&l1=3&s=\\{@}";
    provider.languages << "ru" << "de";
    d->m_providers.append( provider );
    d->m_ui->providerComboBox->addItem( provider.name );

    provider.name = "Ru-Fr Multitran";
    provider.url = "http://multitran.ru/c/m.exe?SHL=1&CL=1&l1=4&s=\\{@}";
    provider.languages << "ru" << "fr";
    d->m_providers.append( provider );
    d->m_ui->providerComboBox->addItem( provider.name );

    provider.name = "Heinzelnisse";
    provider.url = "http://heinzelnisse.info/dict?setOptions=true&searchItem=\\{@}&dictDeNoSearch=on&dictNoDeSearch=on&dictExactSearch=on&dictPhoneticSearch=on&wikiSearch=on&dictNynorskSearch=on&dictBokmaalSearch=checked&forumKeywordSearch=on&suggestion=on";
    provider.languages << "no" << "de";
    d->m_providers.append( provider );
    d->m_ui->providerComboBox->addItem( provider.name );

}

void KEduVocBrowserWidget::setTranslation( KEduVocExpression* entry, int translation )
{
    d->m_currentTranslation = translation;
    d->m_entry = entry;

    if( d->m_ui->followTableCheckBox->isChecked() ) {
        showCurrentTranslation();
    }
}

void KEduVocBrowserWidget::showCurrentTranslation()
{
    if( d->m_entry ) {
        if( d->m_entry->translation( d->m_currentTranslation ) ) {
            QString text = d->m_entry->translation( d->m_currentTranslation )->text();
            if( !text.isEmpty() ) {
                d->m_htmlPart->openUrl( QUrl( QString( d->m_providers.value( d->m_currentProvider ).url.replace( "\\{@}", text ) ) ) );
            }
        }
    }
}

void KEduVocBrowserWidget::openUrl( const QUrl & targetUrl )
{
    d->m_htmlPart->openUrl( targetUrl );
}

void KEduVocBrowserWidget::providerChanged( int provider )
{
    d->m_currentProvider = provider;
    showCurrentTranslation();
}
