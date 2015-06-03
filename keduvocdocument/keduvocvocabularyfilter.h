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

#ifndef KEDUVOCVOCABULARYFILTER_H
#define KEDUVOCVOCABULARYFILTER_H

#include "keduvocdocument_export.h"
#include <QSortFilterProxyModel>

class KEduVocLesson;
class KEduVocExpression;

class KEduVocVocabularyModel;

class KEDUVOCDOCUMENT_EXPORT KEduVocVocabularyFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    KEduVocVocabularyFilter(QObject *parent = 0);
    ~KEduVocVocabularyFilter();

    void setSourceModel(KEduVocVocabularyModel* model);

    QModelIndex appendEntry(KEduVocExpression *expression = 0);
    KEduVocLesson * lesson();

public slots:
    void setSearchString(const QString& expression);

protected:
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    class Private;
    Private * const d;
};


#endif
