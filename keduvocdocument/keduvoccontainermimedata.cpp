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

#include "keduvoccontainermimedata.h"

class KEduVocContainerMimeData::Private
{
public:
    QList<KEduVocContainer*> m_containers;
};

KEduVocContainerMimeData::KEduVocContainerMimeData()
    : d( new Private)
{
}

KEduVocContainerMimeData::~KEduVocContainerMimeData()
{
    delete d;
}


void KEduVocContainerMimeData::addContainer(KEduVocContainer * container)
{
    d->m_containers.append(container);
}

QList<KEduVocContainer *> KEduVocContainerMimeData::containerList() const
{
    return d->m_containers;
}
