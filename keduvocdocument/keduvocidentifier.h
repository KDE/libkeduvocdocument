/***************************************************************************

    C++ Interface: keduvocidentifier

    -----------------------------------------------------------------------

    begin         : Mi Aug 29 2007

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
#ifndef KEDUVOCIDENTIFIER_H
#define KEDUVOCIDENTIFIER_H

#include "libkeduvocdocument_export.h"

#include "keduvocconjugation.h"
#include "keduvocgrammar.h"

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QSet>


/**
Class to store meta information about a language or any other category in the vocabulary.

 @author
*/
class KEDUVOCDOCUMENT_EXPORT KEduVocIdentifier
{
public:
    KEduVocIdentifier();
    KEduVocIdentifier( const KEduVocIdentifier &other );
    ~KEduVocIdentifier();
    KEduVocIdentifier& operator= ( const KEduVocIdentifier &other );

    QString name() const;
    void setName( const QString& name );
    QString locale() const;
    void setLocale( const QString& name );

private:
    class Private;
    Private * const d;

};

#endif
