/***************************************************************************

              dialog page for multiple choice suggestions

    -----------------------------------------------------------------------

    begin         : Mon Oct 29 18:09:29 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2006 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCMULTIPLECHOICEWIDGET_H
#define KEDUVOCMULTIPLECHOICEWIDGET_H

#include <QModelIndex>
#include <QWidget>

#include <keduvocdocument_export.h>

class KEduVocExpression;
class KEduVocTranslation;
class QStringListModel;

namespace Editor
{

class KEDUVOCDOCUMENT_EXPORT KEduVocMultipleChoiceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KEduVocMultipleChoiceWidget( QWidget *parent = 0 );
    ~KEduVocMultipleChoiceWidget();

public slots:
    void setTranslation( KEduVocExpression* entry, int translation );

protected:
    bool eventFilter( QObject *obj, QEvent *event );

private slots:
    void slotDataChanged( const QModelIndex & topLeft, const QModelIndex & bottomRight );
    void slotAddChoiceButton();
    void slotRemoveChoiceButton();

private:
    class Private;
    Private * const d;
};
}

#endif
