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


#ifndef MultipleChoice_included
#define MultipleChoice_included

#include "libkdeedu_core_export.h"

#include <QtCore/QString>

#define MAX_MULTIPLE_CHOICE  5  // select one out of x

class KDEEDUCORE_EXPORT KEduVocMultipleChoice
{

public:

   KEduVocMultipleChoice() {}

   KEduVocMultipleChoice (const QString &mc1, const QString &mc2, const QString &mc3, const QString &mc4, const QString &mc5);

   void setMC1 (const QString &s);
   void setMC2 (const QString &s);
   void setMC3 (const QString &s);
   void setMC4 (const QString &s);
   void setMC5 (const QString &s);

   QString mc1 () const;
   QString mc2 () const;
   QString mc3 () const;
   QString mc4 () const;
   QString mc5 () const;

   QString mc (unsigned idx) const;

   bool isEmpty() const;
   void normalize();
   void clear();
   unsigned size();

   bool operator==(const KEduVocMultipleChoice &choice) const;

protected:

   QString  muc1, muc2, muc3, muc4, muc5;
};


#endif // MultipleChoice_included
