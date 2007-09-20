/***************************************************************************

    C++ Implementation: keduvocdeclination

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
#include "keduvocdeclination.h"

#include <QtCore/QStringList>

class KEduVocDeclination::Private
{
public:
    struct DeclinationCase {
        QStringList test;
    };


    DeclinationGender m_gender;

    Private();
};

KEduVocDeclination::Private::Private()
{
    m_gender = Undefined;
}

KEduVocDeclination::KEduVocDeclination()
    :d (new Private)
{
}

KEduVocDeclination::KEduVocDeclination(const KEduVocDeclination & other)
    :d (new Private)
{
}

KEduVocDeclination & KEduVocDeclination::operator =(const KEduVocDeclination & other)
{
    d->m_gender = other.d->m_gender;
}

KEduVocDeclination::~KEduVocDeclination()
{
    delete d;
}



