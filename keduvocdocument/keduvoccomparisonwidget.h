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

#ifndef KEDUVOCCOMPARISONWIDGET_H
#define KEDUVOCCOMPARISONWIDGET_H

#include <keduvocdocument_export.h>

#include <QWidget>

class KEduVocExpression;
class KEduVocTranslation;
class KEduVocDocument;

namespace Editor
{

class KEDUVOCDOCUMENT_EXPORT KEduVocComparisonWidget : public QWidget
{
    Q_OBJECT

public:
    KEduVocComparisonWidget( QWidget *parent = 0 );
    ~KEduVocComparisonWidget();

public slots:
    /**
     * To know about word types that are currently defined.
     * @param doc
     */
    void setDocument( KEduVocDocument* doc );
    /**
     * Update the word
     * @param entry
     * @param translation
     */
    void setTranslation( KEduVocExpression* entry, int translation );

private slots:
    void slotMakeAdjectiveButton();
    void slotMakeAdverbButton();

    void slotComparativeChanged();
    void slotSuperlativeChanged();

private:
    class Private;
    Private * const d;
};

}

#endif
