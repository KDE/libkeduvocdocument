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

#ifndef KEDUVOCIMAGECHOOSERWIDGET_H
#define KEDUVOCIMAGECHOOSERWIDGET_H

#include <keduvocdocument_export.h>
#include <QWidget>

class KEduVocExpression;

namespace Editor
{

class KEDUVOCDOCUMENT_EXPORT KEduVocImageChooserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KEduVocImageChooserWidget( QWidget* parent = 0 );
    ~KEduVocImageChooserWidget();

public slots:
    /**
     * Update the word
     * @param entry
     * @param translation
     */
    void setTranslation( KEduVocExpression* entry, int translation );

// signals:
//     /**
//      * Important to let other widgets know that a certain word type is edited (might make conjugations/comparison/declension appear)
//      * @param
//      */
//     void imageChanged(const QString&);

private slots:
    void slotImageChanged( const QString & url );

protected:
    bool eventFilter( QObject * obj, QEvent * event );

private:
    class Private;
    Private * const d;
};

}
#endif
