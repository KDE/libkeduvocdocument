/***************************************************************************

    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvocvocabularyfilter.h"
#include <keduvocvocabularymodel.h>

class KEduVocVocabularyFilter::Private
{
public:
    Private();

    KEduVocVocabularyModel * m_model;
    QString m_filterString;
};

KEduVocVocabularyFilter::Private::Private() {
    m_model = 0;
    m_filterString = "";
}

KEduVocVocabularyFilter::KEduVocVocabularyFilter(QObject *parent)
    : QSortFilterProxyModel(parent),
      d( new Private )
{
    // do not use capitalization for searches
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    // sort locale aware: at least puts umlauts and accents in the right position.
    // Not sure about languages that are more different.
    // Also depends on the current locale.
    setSortLocaleAware(true);

    // eventually accept only one language if so desired...
    setFilterKeyColumn(-1);
}

KEduVocVocabularyFilter::~KEduVocVocabularyFilter()
{
    delete d;
}

QModelIndex KEduVocVocabularyFilter::appendEntry(KEduVocExpression *expression)
{
    if (!d->m_model) {
        return QModelIndex();
    }
    return mapFromSource(d->m_model->appendEntry(expression));
}

void KEduVocVocabularyFilter::setSourceModel(KEduVocVocabularyModel * model)
{
    QSortFilterProxyModel::setSourceModel(model);
    d->m_model = model;
}

void KEduVocVocabularyFilter::setSearchString(const QString & expression)
{
    d->m_filterString = expression;
    invalidateFilter();
}

bool KEduVocVocabularyFilter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (d->m_filterString.isEmpty()) {
        return true;
    }

    int columns = d->m_model->columnCount(QModelIndex());
    for (int i = 0; i < columns; i += KEduVocVocabularyModel::EntryColumnsMAX) {
        QModelIndex index = sourceModel()->index(sourceRow, i, sourceParent);
        if (sourceModel()->data(index).toString().contains(d->m_filterString, Qt::CaseInsensitive)) {
            return true;
        }
    }
    return false;
}

KEduVocLesson * KEduVocVocabularyFilter::lesson()
{
    if (d->m_model) {
        return d->m_model->lesson();
    }
    return 0;
}
