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
#include "keduvocscript.h"

#include <QDebug>
#include <KLocalizedString>

#include <kross/core/action.h>


class KEduVocScript::Private
{
public:
    Private( QString file );

    bool m_activated;
    QString m_file;
    QPointer<QObject> m_object;
    QMap<QString, QObject*> m_scriptObjects;
    QString m_errorMessage;
};

KEduVocScript::Private::Private( QString file )
{
    m_file = file;
    m_activated = false;
}

KEduVocScript::KEduVocScript( QString file )
    : d( new Private( file ) )
{
}

KEduVocScript::~KEduVocScript()
{
    delete  d;
}


bool KEduVocScript::isActivated()
{
    return d->m_activated;
}


void KEduVocScript::activate()
{
    qDebug() << "Activating Script" << fileName();
    if( isActivated() ) {
        qDebug() << "Script already activated";
        return;
    }
    if( !exists() ) {
        qDebug() << "Script file given does not exist";
        d->m_errorMessage = i18n( "The script file does not exist." );
        return;
    }

    // Create the script container. m_object is the parent QObject,
    // so that our action instance will be destroyed once the m_object
    // is destroyed.
    d->m_object = new QObject();
    Kross::Action* action = new Kross::Action( d->m_object, d->m_file);
    // Publish our myobject instance and connect signals with
    // scripting functions.
    QMapIterator <QString, QObject*> i( d->m_scriptObjects );
    while( i.hasNext() ) {
        i.next();
        qDebug() << i.key();
        action->addObject( i.value() , i.key(), Kross::ChildrenInterface::AutoConnectSignals );
    }

    // Set the file to be execute
    action->setFile( d->m_file );

    // Execute the script.
    action->trigger();

    d->m_activated = !action->isFinalized();
    if( !d->m_activated ) {
        qDebug() << "Script not activated";
        QString msg = action->errorMessage();
        QString trace = action->errorTrace();
        msg.replace( '<', "&lt;" ).replace( '\n', "<br/>" );
        trace.replace( '<', "&lt;" ).replace( '\n', "<br/>" );
        d->m_errorMessage = "<p><strong>" + i18n( "Error in file %1 at line %2:", fileName(),
                                              action->errorLineNo()) + "</strong><br/>" + msg + "<br/><strong>"
                         + i18nc( "debug information in error message", "Backtrace:" ) +
                         "</strong><br/>" + trace + "</p>";
    }
}

void KEduVocScript::deactivate()
{
    delete d->m_object;
    d->m_activated = false;
}

bool KEduVocScript::exists()
{
    QFileInfo fileInfo( d->m_file );
    return fileInfo.exists();
}


QString KEduVocScript::fileName()
{
    return d->m_file;
}


void KEduVocScript::addObject( QString name, QObject * object )
{
    d->m_scriptObjects.insert( name, object );
}

void KEduVocScript::addObjects( QMap<QString, QObject*> objects )
{
    d->m_scriptObjects.unite( objects );
}

QString KEduVocScript::errorMessage()
{
    return d->m_errorMessage;
}
