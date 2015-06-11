/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCCONTAINERVIEW_H
#define KEDUVOCCONTAINERVIEW_H

#include <QTreeView>

#include <keduvoccontainermodel.h>
#include <keduvocdocument_export.h>

class KEduVocContainer;
class KEduVocExpression;

class KEduVocContainerModel;


/**
 * View for containers (KEduVocContainer).
 * @author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 */
class KEDUVOCDOCUMENT_EXPORT KEduVocContainerView : public QTreeView
{
    Q_OBJECT
public:
    /** Set up the lesson list widget.
     * Also creates the menu to manipulate the lessons.
     * @param parent parent widget */
    KEduVocContainerView( QWidget *parent = 0 );

    ~KEduVocContainerView();

    /** Set the model for the view.
     * @param model the model */
    virtual void setModel( KEduVocContainerModel *model );

    /**
     * Return the ContainerModel for this View
     */
    KEduVocContainerModel* model();


public slots:
    /** let the user type a new name for the container */
    void slotRename();


private:
    class Private;
    Private * const d;
};


#endif
