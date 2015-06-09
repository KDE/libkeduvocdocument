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
#include "keduvocvocabularyheaderview.h"

using namespace Editor;

class KEduVocVocabularyHeaderView::Private
{
public:
    Private();

    int m_sortSection;
};

KEduVocVocabularyHeaderView::Private::Private()
{
    m_sortSection = -1;
}

KEduVocVocabularyHeaderView::KEduVocVocabularyHeaderView( Qt::Orientation orientation, QWidget * parent )
    : QHeaderView( orientation, parent ),
      d( new Private )
{
    setSectionsClickable(true);
    connect(this, &KEduVocVocabularyHeaderView::sectionClicked, this, &KEduVocVocabularyHeaderView::updateSorting);
}

KEduVocVocabularyHeaderView::~KEduVocVocabularyHeaderView()
{
    delete d;
}

void KEduVocVocabularyHeaderView::setSortSelection( int sortSelection )
{
    d->m_sortSection = sortSelection;
}

int KEduVocVocabularyHeaderView::sortSelection()
{
    return d->m_sortSection;
}

void KEduVocVocabularyHeaderView::updateSorting( int index )
{
    if( d->m_sortSection != index ) {
        setSortIndicatorShown( true );
        setSortIndicator( index, Qt::AscendingOrder );
        d->m_sortSection = index;
        return;
    }

    if( sortIndicatorOrder() == Qt::DescendingOrder ) {
        setSortIndicatorShown( true );
        setSortIndicator( index, Qt::DescendingOrder );
        return;
    }

    if( !isSortIndicatorShown() ) {
        setSortIndicatorShown( true );
        setSortIndicator( index, Qt::AscendingOrder );
        return;
    }

    setSortIndicatorShown( false );
    d->m_sortSection = -1;
    model()->sort( -1 );
}
