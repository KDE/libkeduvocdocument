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
  (const QStringList &choices)
  : m_choices(choices)
{
}


bool KEduVocMultipleChoice::isEmpty() const
{
  return m_choices.isEmpty();
}


void KEduVocMultipleChoice::clear()
{
  m_choices.clear();
}


QString KEduVocMultipleChoice::choice (int index) const
{
  QString choice;
  if (m_choices.size() >= index)
  {
    choice = m_choices[index];
  }
  return choice;
}

void KEduVocMultipleChoice::setChoice(int index, const QString &s)
{
  while (m_choices.size() < index)
  {
    m_choices.append(QString());
  }
  m_choices[index] = s;
}

unsigned KEduVocMultipleChoice::size()
{
  return m_choices.size();
}

bool KEduVocMultipleChoice::operator==(const KEduVocMultipleChoice &choice) const
{
  return m_choices == choice.m_choices;
}

void KEduVocMultipleChoice::appendChoice(const QString &s) 
{ 
  m_choices.append(s);
}
