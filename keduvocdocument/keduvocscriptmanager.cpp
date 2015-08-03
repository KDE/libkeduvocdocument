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
#include "keduvocscriptmanager.h"

#include <KPluginInfo>
#include <KServiceTypeTrader>
#include <KActionCollection>
#include <KNotifications/KPassivePopup>
#include <KMessageBox>
#include <KConfig>
#include <KSharedConfig>
#include <KLocalizedString>

#include <kross/core/action.h>
#include <kross/core/manager.h>

#include <keduvoctranslator.h>
#include <keduvoceditordocument.h>

#include <QFileInfo>
#include <QStandardPaths>
#include <QDebug>
#include <QApplication>

//using namespace Editor;

class KEduVocScriptManager::Private
{
public:
    Private( QWidget *parent, KEduVocEditorDocument* document, KEduVocVocabularyModel* vocabularyModel, QString separator );

    QList<KEduVocScript*> m_scripts;
    QMap<QString, QObject*> m_scriptObjects;
    QList <QAction*> m_scriptActions;
    KEduVocEditorDocument* m_document;
    KEduVocVocabularyModel* m_vocabularyModel;
    QString m_separator;
    QWidget *m_parent;
    ///script objects (objects that will be used from inside the scripts)
    Scripting::KEduVocDocument* m_scriptingDoc;
};

KEduVocScriptManager::Private::Private( QWidget *parent, KEduVocEditorDocument* document, KEduVocVocabularyModel* vocabularyModel, QString separator )
{
    m_parent = parent;
    m_document = document;
    m_vocabularyModel = vocabularyModel;
    m_separator = separator;
}


KEduVocScriptManager::KEduVocScriptManager( QWidget *parent, KEduVocEditorDocument* document, KEduVocVocabularyModel* vocabularyModel, QString separator )
    : d( new Private( parent, document, vocabularyModel, separator ) )
{
    //add Scripting::KEduVocDocument
    d->m_scriptingDoc = new Scripting::KEduVocDocument( d->m_document->document(), d->m_vocabularyModel, d->m_separator );
    addObject( d->m_scriptingDoc, QApplication::applicationName() );
}


KEduVocScriptManager::~KEduVocScriptManager()
{
    delete d;
}


QStringList KEduVocScriptManager::getDesktopFiles()
{
//     QStringList scriptsAvailable;
    QStringList dirs(
    QStandardPaths::locateAll(
            QStandardPaths::DataLocation, "plugins",  QStandardPaths::LocateDirectory ) );
    QStringList filenames;
    foreach( const QString dir,  dirs ) {
        foreach( const QString filename,  QDir( dir ).entryList( QDir::Files ) ) {
            if( filename.endsWith( QLatin1String( ".desktop" ) ) ) {
                filenames << dir + '/' + filename;
            }
        }
    }
    return filenames;
}


QMap<QString, QString> KEduVocScriptManager::categories()
{
    QMap <QString, QString> categories;
    categories["translation"] = "Translation";
    return categories;
}


QString KEduVocScriptManager::getScriptEntry( QString desktopFile )
{
    //open it as a raw configuration file and read the script entry
    KConfig scriptconfig( desktopFile, KConfig::SimpleConfig );
    KConfigGroup group = scriptconfig.group( "Desktop Entry" );
    return group.readEntry( "Script" );
}


QString KEduVocScriptManager::getScriptFileName( QString desktopFile )
{
    QFileInfo desktopFileInfo( desktopFile );
    return desktopFileInfo.absolutePath() + '/' + KEduVocScriptManager::getScriptEntry( desktopFile );
}


QStringList KEduVocScriptManager::enabledScripts()
{
    QStringList enabledScripts;
    // Open parleyrc to read the state of the plugins (enabled/disabled)
    QString fileName = QApplication::applicationName().toLower() + "rc";
    KConfigGroup cfg( KSharedConfig::openConfig( fileName ), "Plugins" );
    // Get list of KPluginInfo for each of the desktop files found
    QList<KPluginInfo> pluginsInfoList = KPluginInfo::fromFiles( getDesktopFiles() );
    // Find which plugins are enabled and add them to enabledScripts list
    KPluginInfo inf;
    foreach( inf, pluginsInfoList ) {
        inf.load( cfg );
        if( inf.isPluginEnabled() )
            enabledScripts.push_back( inf.entryPath() );
//         qDebug() << inf.name() << inf.isPluginEnabled() << inf.pluginName();
    }
    return enabledScripts;
}


void KEduVocScriptManager::disablePlugin( QString desktopFile )
{
    KConfigGroup cfg( KSharedConfig::openConfig( QApplication::applicationName().toLower() + "rc" ), "Plugins");
    KPluginInfo inf( desktopFile );
    //load parleyrc enabled value
    inf.load( cfg );
    inf.setPluginEnabled( false );
    //save enabled=true in parleyrc
    inf.save( cfg );
}


void KEduVocScriptManager::loadScripts()
{
    QStringList scripts = enabledScripts();
    QStringList failed;
    QStringList errorDetails;
    foreach( const QString & script, scripts ) {
        //create a new Script and add it to the m_scripts list
        KEduVocScript * s = new KEduVocScript( getScriptFileName( script ) );
        s->addObjects( d->m_scriptObjects );
        s->activate();
        d->m_scripts.push_back( s );
        if( !s->isActivated() ) {
            failed << getScriptFileName( script );  //TODO: real name?
            errorDetails << s->errorMessage();
            disablePlugin( script );
        }
    }
    //inform with a message box when a script could not be activated
    if( !failed.empty() ) {
        QString errorMessage = "<p>" + i18np( "A script could not be activated and has been disabled.", "%1 scripts could not be activated and have been disabled.", failed.count() ) + ' ' + i18n( "This probably means that there are errors in the script or that the required packages are not installed." ) + "</p>";
        errorMessage += "<ul><li>" + failed.join( "</li><li>" ) + "</li></ul>";
        KMessageBox::detailedError( d->m_parent, errorMessage, errorDetails.join( "<hr/>" ), i18n( "Script Activation" ) );
    }
}

void KEduVocScriptManager::addObject(QObject * obj, const QString & name)
{
    d->m_scriptObjects[name] = obj;
}

KEduVocTranslator * KEduVocScriptManager::translator() {
    return d->m_scriptingDoc->translator();
}

void KEduVocScriptManager::reloadScripts()
{
    //deactivate (delete) all the active scripts
    foreach( KEduVocScript * s, d->m_scripts ) {
        if( s )
            delete s;
    }
    d->m_scripts.clear();

    //reload the scripts menu
    emit editorUnplugActionList( "scripts_actionlist" );
    d->m_scriptActions.clear();
    emit editorPlugActionList( "scripts_actionlist", d->m_scriptActions );

    //load all the enabled scripts
    loadScripts();
}


void KEduVocScriptManager::addScriptAction( const QString & name, QAction * action )
{
    //unplug action list (orelse it will add twice the same entries
    emit editorUnplugActionList( "scripts_actionlist" );

    //add to action collection
    emit editorActionAdded( name, action );

    //add it to actions menu list
    d->m_scriptActions.push_back( action );

    //plug the action list
    emit editorPlugActionList( "scripts_actionlist", d->m_scriptActions );
}
