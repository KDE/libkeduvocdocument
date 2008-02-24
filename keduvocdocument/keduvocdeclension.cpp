/***************************************************************************

    C++ Implementation: keduvocdeclension

    -----------------------------------------------------------------------

    begin         : Do Sep 20 2007

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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
#include "keduvocdeclension.h"

#include <QtCore/QMap>

class KEduVocDeclension::Private
{
public:
    QMap<int, KEduVocText> m_declensions;
};

KEduVocDeclension::KEduVocDeclension()
    :d (new Private)
{
}

KEduVocDeclension::KEduVocDeclension(const KEduVocDeclension & other)
    :d (new Private)
{
    d->m_declensions = other.d->m_declensions;
}

KEduVocDeclension & KEduVocDeclension::operator =(const KEduVocDeclension & other)
{
    d->m_declensions = other.d->m_declensions;
    return *this;
}

KEduVocDeclension::~KEduVocDeclension()
{
    delete d;
}

KEduVocText& KEduVocDeclension::declension(DeclensionNumber number, DeclensionCase decCase)
{
    return d->m_declensions[indexOf(number, decCase)];
}

void KEduVocDeclension::setDeclension(const KEduVocText & declension, DeclensionNumber number, DeclensionCase decCase)
{
    d->m_declensions[indexOf(number, decCase)] = declension;
}

int KEduVocDeclension::indexOf(DeclensionNumber number, DeclensionCase decCase)
{
    return number * DeclensionCaseMAX + decCase;
}

bool KEduVocDeclension::isEmpty()
{
    return d->m_declensions.isEmpty();
}

