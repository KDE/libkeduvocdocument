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

#include "keduvocmultiplechoice.h"

KEduVocMultipleChoice::KEduVocMultipleChoice
  (const QString &mc1, const QString &mc2, const QString &mc3, const QString &mc4,const QString &mc5)
{
   setMC1 (mc1);
   setMC2 (mc2);
   setMC3 (mc3);
   setMC4 (mc4);
   setMC5 (mc5);
}


bool KEduVocMultipleChoice::isEmpty() const
{
  return muc1.simplified().isEmpty() &&
         muc2.simplified().isEmpty() &&
         muc3.simplified().isEmpty() &&
         muc4.simplified().isEmpty() &&
         muc5.simplified().isEmpty();
}


void KEduVocMultipleChoice::clear()
{
   muc1 = "";
   muc2 = "";
   muc3 = "";
   muc4 = "";
   muc5 = "";
}


QString KEduVocMultipleChoice::mc (unsigned idx) const
{
   switch (idx) {
     case 0: return muc1;
     case 1: return muc2;
     case 2: return muc3;
     case 3: return muc4;
     case 4: return muc5;
   }
   return "";
}


unsigned KEduVocMultipleChoice::size()
{
   normalize();
   unsigned num = 0;
   if (!muc1.isEmpty())
     ++num;
   if (!muc2.isEmpty())
     ++num;
   if (!muc3.isEmpty())
     ++num;
   if (!muc4.isEmpty())
     ++num;
   if (!muc5.isEmpty())
     ++num;
   return num;
}


void KEduVocMultipleChoice::normalize()
{
  // fill from first to last

  if (muc1.isEmpty()) {
    muc1 = muc2;
    muc2 = "";
  }

  if (muc2.isEmpty()) {
    muc2 = muc3;
    muc3 = "";
  }

  if (muc3.isEmpty()) {
    muc3 = muc4;
    muc4 = "";
  }

  if (muc4.isEmpty()) {
    muc4 = muc5;
    muc5 = "";
  }

}


bool KEduVocMultipleChoice::operator==(const KEduVocMultipleChoice &choice) const
{
  return muc1 == choice.muc1 &&
    muc2 == choice.muc2 &&
    muc3 == choice.muc3 &&
    muc4 == choice.muc4 &&
    muc5 == choice.muc5;
}

void KEduVocMultipleChoice::setMC1(const QString &s) 
{ 
	muc1 = s; 
}

void KEduVocMultipleChoice::setMC2(const QString &s) 
{ 
	muc2 = s; 
}

void KEduVocMultipleChoice::setMC3(const QString &s) 
{ 
	muc3 = s; 
}

void KEduVocMultipleChoice::setMC4(const QString &s) 
{ 
    muc4 = s; 
}

void KEduVocMultipleChoice::setMC5(const QString &s) 
{ 
    muc5 = s; 
}

QString KEduVocMultipleChoice::mc1() const 
{ 
    return muc1; 
}

QString KEduVocMultipleChoice::mc2() const 
{ 
    return muc2; 
}

QString KEduVocMultipleChoice::mc3() const 
{ 
    return muc3; 
}

QString KEduVocMultipleChoice::mc4() const 
{ 
    return muc4; 
}

QString KEduVocMultipleChoice::mc5() const 
{ 
	return muc5; 
}

