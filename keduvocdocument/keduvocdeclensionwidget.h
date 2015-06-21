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

#ifndef KEDUVOCDECLENSIONWIDGET_H
#define KEDUVOCDECLENSIONWIDGET_H

#include <QWidget>

#include <keduvocdeclension.h>
#include <keduvocdocument_export.h>

class KEduVocExpression;
class KEduVocDocument;

namespace Editor
{

class KEDUVOCDOCUMENT_EXPORT KEduVocDeclensionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KEduVocDeclensionWidget( QWidget *parent = 0 );
    ~KEduVocDeclensionWidget();

public slots:
    void setDocument( KEduVocDocument* doc );
    void setTranslation( KEduVocExpression* entry, int translation );

    /**
     * Fill the line edits
     */
    void updateEntries();

    void nextNumber();

signals:
    void sigModified();

private slots:
    void textChanged( const QString& );

private:
    class Private;
    Private * const d;

    int currentAdditionalWordFlag();
    void setupLineEdits();
};

}

#endif
