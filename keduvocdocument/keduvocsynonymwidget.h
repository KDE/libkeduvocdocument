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

#ifndef KEDUVOCSYNONYMWIDGET_H
#define KEDUVOCSYNONYMWIDGET_H

#include <QWidget>
#include <QModelIndex>

#include <keduvocdocument_export.h>

class KEduVocExpression;
class KEduVocTranslation;
class KEduVocDocument;
class QStringListModel;

namespace Editor
{

class KEDUVOCDOCUMENT_EXPORT KEduVocSynonymWidget : public QWidget
{
    Q_OBJECT
public:
    enum SynonymWidgetType {
        Synonym,
        Antonym,
        FalseFriend
    };

    explicit KEduVocSynonymWidget( SynonymWidgetType type, QWidget *parent = 0 );
    ~KEduVocSynonymWidget();

public slots:
    /**
     * Register the document inside the widget so that it can be marked as modified.
     * @param doc
     */
    void setDocument( KEduVocDocument *doc );

    void setTranslation( KEduVocExpression* entry, int translation );

private slots:
    void togglePair();
    void updateList();

private:
    class Private;
    Private * const d;
};
}
#endif
