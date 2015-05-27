/***************************************************************************

    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright 2015 Rahul Chowdhury <rahul.chowdhury@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


// Own
#include "keduvocwordclassmodel.h"

#include <keduvocwordtype.h>


KEduVocWordClassModel::KEduVocWordClassModel(QObject * parent)
    : KEduVocContainerModel(KEduVocContainer::WordType, parent)
    , d(0)
{
}

KEduVocWordClassModel::~KEduVocWordClassModel()
{
    delete d;
}

KEduVocContainer * KEduVocWordClassModel::rootContainer() const
{
    if (document()) {
        return 0;
    }
    return document()->wordTypeContainer();
}
