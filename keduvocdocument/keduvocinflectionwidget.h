/***************************************************************************
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright 2011 Jan Gerrit Marker <jangerrit@weiler-marker.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCINFLECTIONWIDGET_H
#define KEDUVOCINFLECTIONWIDGET_H

#include <QStackedWidget>

#include <keduvocdeclension.h>
#include <keduvocdocument_export.h>

class KEduVocExpression;
class KEduVocDocument;

namespace Editor
{
class KEduVocDeclensionWidget;
class KEduVocConjugationWidget;

class KEDUVOCDOCUMENT_EXPORT KEduVocInflectionWidget : public QStackedWidget
{
    Q_OBJECT

public:
    explicit KEduVocInflectionWidget( QWidget *parent = 0 );
    ~KEduVocInflectionWidget();

public slots:
    void setDocument( KEduVocDocument* doc );
    void setTranslation( KEduVocExpression* entry, int translation );

private slots:
    void setWordType();

private:
    class Private;
    Private * const d;
};

}

#endif
