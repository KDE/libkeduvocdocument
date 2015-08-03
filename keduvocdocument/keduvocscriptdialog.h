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
#ifndef KEDUVOCSCRIPTDIALOG_H
#define KEDUVOCSCRIPTDIALOG_H

#include <keduvocscriptmanager.h>
#include <keduvocdocument_export.h>

#include <QDialog>
#include <KPluginSelector>

/**
    @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
*/
class KEDUVOCDOCUMENT_EXPORT KEduVocScriptDialog : public QDialog
{
public:
    KEduVocScriptDialog( KEduVocScriptManager* scriptManager );

    ~KEduVocScriptDialog();

    /**
     * Executed when user clicks OK button.
     * Saves the state of the plugins (which ones are loaded) in the config file
     * and makes the necessary loads/unloads of plugins
     */
    void accept();

private:
    class Private;
    Private * const d;
};

#endif
