/***************************************************************************

              manage multiple choice suggestions for queries

    -----------------------------------------------------------------------

    begin          : Mon Oct 29 18:09:29 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>

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


#ifndef KEDUVOCMULTIPLECHOICE_H
#define KEDUVOCMULTIPLECHOICE_H

#include "libkeduvocdocument_export.h"

#include <QtCore/QStringList>

class KEDUVOCDOCUMENT_EXPORT KEduVocMultipleChoice
{
public:
    KEduVocMultipleChoice();
    KEduVocMultipleChoice( const KEduVocMultipleChoice &other );
    KEduVocMultipleChoice( const QStringList &choices );

    ~KEduVocMultipleChoice();

    QStringList choices() const;
    void appendChoice( const QString &s );

    QString choice( int index ) const;

    bool isEmpty() const;
    void clear();
    unsigned size() const;

    bool operator== ( const KEduVocMultipleChoice &choice ) const;

    KEduVocMultipleChoice &operator= ( const KEduVocMultipleChoice &other );

private:
    class KEduVocMultipleChoicePrivate;
    KEduVocMultipleChoicePrivate * const d;
};


#endif // MultipleChoice_included
