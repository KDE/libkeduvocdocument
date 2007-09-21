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
    QMap<int, QString> m_declinations;
};

KEduVocDeclination::KEduVocDeclination()
    :d (new Private)
{
}

KEduVocDeclination::KEduVocDeclination(const KEduVocDeclination & other)
    :d (new Private)
{
    d->m_declinations = other.d->m_declinations;
}

KEduVocDeclination & KEduVocDeclination::operator =(const KEduVocDeclination & other)
{
    d->m_declinations = other.d->m_declinations;
    return *this;
}

KEduVocDeclination::~KEduVocDeclination()
{
    delete d;
}

QString KEduVocDeclination::declination(DeclinationNumber number, DeclinationCase decCase)
{
    if ( m_declinations.contains(indexOf(number, decCase)) ) {
        return m_declinations.value(indexOf(number, decCase));
    } else {
        return QString();
    }
}

void KEduVocDeclination::setDeclination(const QString & declination, DeclinationNumber number, DeclinationCase decCase)
{
    m_declinations[indexOf(number, decCase)] = declination;
}

int KEduVocDeclination::indexOf(DeclinationNumber number, DeclinationCase decCase)
{
    return number * 10 + decCase;
}

bool KEduVocDeclination::isEmpty()
{
    return m_declinations.isEmpty();
}



