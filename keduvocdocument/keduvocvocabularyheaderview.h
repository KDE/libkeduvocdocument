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

#ifndef KEDUVOCVOCABULARYHEADERVIEW_H
#define KEDUVOCVOCABULARYHEADERVIEW_H

#include <QHeaderView>

#include <keduvocdocument_export.h>

namespace Editor
{

class KEDUVOCDOCUMENT_EXPORT KEduVocVocabularyHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit KEduVocVocabularyHeaderView( Qt::Orientation orientation, QWidget * parent = 0 );
    ~KEduVocVocabularyHeaderView();

    // Set m_sortSelection
    void setSortSelection( int sortSelection );

    // Return m_sortSelection
    int sortSelection();

public slots:
    void updateSorting( int );

private:
    class Private;
    Private * const d;
};
}

#endif
