/*
 * manage multiple choice suggestions for queries
 * SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
 * SPDX-FileCopyrightText: 2005 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KEDUVOCMULTIPLECHOICE_H
#define KEDUVOCMULTIPLECHOICE_H

#include "keduvocdocument_export.h"

#include <QStringList>

class KEDUVOCDOCUMENT_EXPORT KEduVocMultipleChoice
{
public:
    KEduVocMultipleChoice();
    KEduVocMultipleChoice( const KEduVocMultipleChoice &other );
    explicit KEduVocMultipleChoice( const QStringList &choices );

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
