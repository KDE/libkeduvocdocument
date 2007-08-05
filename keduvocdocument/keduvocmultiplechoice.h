/***************************************************************************

              manage multiple choice suggestions for queries

    -----------------------------------------------------------------------

    begin          : Mon Oct 29 18:09:29 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
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

#include "libkdeedu_core_export.h"

#include <QtCore/QString>
#include <QtCore/QStringList>

#define MAX_MULTIPLE_CHOICE  5  // select one out of x

class KEduVocMultipleChoicePrivate;

class KEDUVOCDOCUMENT_EXPORT KEduVocMultipleChoice
{

public:

   KEduVocMultipleChoice();
   KEduVocMultipleChoice(const KEduVocMultipleChoice &other);
   KEduVocMultipleChoice (const QStringList &choices);
   
   ~KEduVocMultipleChoice();
   
   void setChoices (const QStringList &choices);
   QStringList choices() const;

   void appendChoice (const QString &s);

   void setChoice(int index, const QString &s);
   QString choice (int index) const;

   bool isEmpty() const;
   void clear();
   unsigned size();

   bool operator==(const KEduVocMultipleChoice &choice) const;
   
   KEduVocMultipleChoice &operator=(const KEduVocMultipleChoice &other);

private:

   KEduVocMultipleChoicePrivate * const d;
};


#endif // MultipleChoice_included
