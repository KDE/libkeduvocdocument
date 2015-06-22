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

#ifndef KEDUVOCCONJUGATIONWIDGET_H
#define KEDUVOCCONJUGATIONWIDGET_H

#include <keduvocdocument_export.h>

#include <QWidget>

class KEduVocExpression;
class KEduVocDocument;

namespace Editor
{

class KEDUVOCDOCUMENT_EXPORT KEduVocConjugationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KEduVocConjugationWidget( QWidget *parent = 0 );
    ~KEduVocConjugationWidget();

public slots:
    void setDocument( KEduVocDocument* doc );
    void setTranslation( KEduVocExpression* entry, int translation );

signals:
    void sigModified();

private slots:
    void textChanged( const QString& );
    void slotTenseSelected( int );
    void slotNextTense();
    void tenseEditingFinished();

private:
    class Private;
    Private * const d;

    void updateVisiblePersons();

    void showWidgets( bool tenses, bool singular, bool dual, bool plural, bool maleVisible, bool femaleVisible, bool neuterVisible );

    /**
     * Fill the line edits
     */
    void updateEntries();
};

}

#endif
