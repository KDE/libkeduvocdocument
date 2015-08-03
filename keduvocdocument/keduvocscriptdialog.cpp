/***************************************************************************

    Copyright 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "keduvocscriptdialog.h"

#include <keduvoctranslator.h>
#include <keduvocscriptmanager.h>

#include <KLocalizedString>
#include <KPluginInfo>
#include <KSharedConfig>
#include <KConfigGroup>

#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QApplication>

#include <kross/core/action.h>
#include <kross/core/manager.h>

class KEduVocScriptDialog::Private
{
public:
    Private( KEduVocScriptManager * scriptManager );

    KPluginSelector* m_kps;
    QList<KPluginInfo> pluginsInfoList;
    KConfigGroup* scriptsConfigGroup;
    KEduVocScriptManager* m_scriptManager;
};

KEduVocScriptDialog::Private::Private( KEduVocScriptManager* scriptManager )
{
    m_scriptManager = scriptManager;

    //Add KPluginSelector as the main widget of the dialog
    m_kps = new KPluginSelector( 0 );
    m_kps->setMinimumSize( 500, 500 );

    //Load available plugins
    pluginsInfoList = KPluginInfo::fromFiles( KEduVocScriptManager::getDesktopFiles() );

    //     m_kps->addPlugins ( pluginsInfoList,KPluginSelector::ReadConfigFile,i18n ( "Playlist" ),QString ( "playlist" ),KSharedConfig::openConfig ( "parleyrc" ) );
    ///@todo frameworks scripts dialog is not finding any scripts
    m_kps->addPlugins( pluginsInfoList,
                       KPluginSelector::ReadConfigFile,
                       QString(),
                       QString(),
                       KSharedConfig::openConfig( QString( QApplication::applicationName().toLower() + "rc" ) ) );
}

KEduVocScriptDialog::KEduVocScriptDialog( KEduVocScriptManager * scriptManager )
    : QDialog()
    , d( new Private( scriptManager ) )
{
    //Configure window
    setWindowTitle( i18n( "Script Dialog" ) );

    QDialogButtonBox * button_dialog = new QDialogButtonBox;
    button_dialog->setStandardButtons( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget( d->m_kps );
    layout->addWidget( button_dialog );

    setLayout( layout );

    connect( button_dialog, &QDialogButtonBox::accepted, this, &KEduVocScriptDialog::accept );
    connect( button_dialog, &QDialogButtonBox::rejected, this, &KEduVocScriptDialog::reject );
}

KEduVocScriptDialog::~KEduVocScriptDialog()
{
    delete d;
}

void KEduVocScriptDialog::accept()
{
    //Update KPluginInfo object changes
    d->m_kps->updatePluginsState();   //necessary for KPluginInfo::isPluginEnabled to work

    //Save changes in config file (parleyrc)
    d->m_kps->save();

    //Reload scripts
    d->m_scriptManager->reloadScripts();

    //Close dialog
    done( 0 );
}
