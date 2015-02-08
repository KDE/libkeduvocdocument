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

#ifndef KEDUVOCWORDCLASSMODEL_H
#define KEDUVOCWORDCLASSMODEL_H

#include "keduvoccontainermodel.h"

/**
  * Model for the tree of word types.
  */
class KEduVocWordClassModel : public KEduVocContainerModel
{
    Q_OBJECT

public:
    explicit KEduVocWordClassModel(QObject *parent = 0);

    // TODO implement this function
    QModelIndex appendWordClass(const QModelIndex& parent, const QString & wordTypeName = QString());

    // TODO implement this function
    void deleteWordClass(const QModelIndex& wordTypeIndex);

protected:
    KEduVocContainer * rootContainer() const;
};


#endif