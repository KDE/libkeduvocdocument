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
#ifndef KEDUVOCSCRIPT_H
#define KEDUVOCSCRIPT_H

#include <keduvoctranslator.h>
#include <keduvocdocument_export.h>

#include <QString>
#include <QObject>
#include <QPointer>
#include <QMap>

/**
 * This class represents the activated script and is used by the ScriptManager
 * to activate/deactivate scripts.
 *
 * @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
 */
class KEDUVOCDOCUMENT_EXPORT KEduVocScript: public QObject
{
    Q_OBJECT
public:
    /**
     * Script class constructor
     *
     * @param file The path to the script file to be activated (executed)
     */
    KEduVocScript( QString file );
    ~KEduVocScript();
    /**
     * Returns true if the script was successfully activated; false otherwise
     */
    bool isActivated();
    /**
     * Activates the script
     */
    void activate();
    /**
     * Deactivate the script
     */
    void deactivate();
    /**
     * Checks if the script file assigned to the Script object exists as a file on the
     * given path.
     *
     * @return True if the script file exists
     *         False if it does not exist
    */
    bool exists();
    /**
     * Returns the file that was given as parameter to the constructor
     */
    QString fileName();
    /**
     * Add an object to be accessible by the script
     * @param name Name to appear in the script
     * @param object Object to be accessible by the script
     */
    void addObject( QString name, QObject * object );
    /**
     * Adds more than one scripting Objects to the script
     * @param objects Map of the objects to add
     */
    void addObjects( QMap<QString, QObject*> objects );
    /**
     * Returns an html error message if there have been errors in the script.
     */
    QString errorMessage();
private:
    class Private;
    Private * const d;
};

#endif
