/***************************************************************************

                   create a KEduVocDocument from a Vokabeln file

    -----------------------------------------------------------------------

    begin        : Wed Jun 15 19:32:00 PDT 2005

    copyright    : (C) 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include <klocale.h>
#include <kapplication.h>
#include <kmessagebox.h>
#include <kdebug.h>

#include "keduvocdocument.h"

#include "keduvocvokabelnreader.h"

KEduVocVokabelnReader::KEduVocVokabelnReader(QIODevice *file)
{
  // the file must be already open
  m_inputFile = file;
  m_errorMessage = "";
}


bool KEduVocVokabelnReader::readDoc(KEduVocDocument *doc)
{
  m_doc = doc;

  m_doc->setAuthor("http://www.vokabeln.de");

  QTextStream inputStream(m_inputFile);
  inputStream.setCodec("ISO-8851-1");
  inputStream.setAutoDetectUnicode(false);
  inputStream.seek(0);

  QString title,
          lang1,
          lang2,
          expression,
          original,
          translation,
          lessonDescr,
          temp;

  int     i,
          wordCount,
          lesson;

  int lines = 10000;

  QStringList titles,
              languages,
              words,
              lessonDescriptions;

  bool keepGoing = true;

  while (keepGoing)
  {
    temp = inputStream.readLine();
    keepGoing = !(temp.indexOf("\",") > 0);
    title.append(temp);
    if (keepGoing)
      title.append(" ");
  }

  titles = title.split("\",");
  m_doc->setTitle(titles[0].mid(1));
  wordCount = titles[1].section(',', 0, 0).toInt();

  inputStream.readLine();
  inputStream.readLine();

  lang1 = inputStream.readLine();
  languages = lang1.split("\",");

  m_doc->appendIdentifier(languages[0].mid(1));
  m_doc->appendIdentifier(languages[1].mid(1));

  keepGoing = true;
  while (keepGoing)
    keepGoing = !(inputStream.readLine().indexOf("8. Lernhilfe") > 0); //DO NOT translate

  for (i = 0; i <= 14; i++)
    inputStream.readLine();

  for (i = 0; i < wordCount - 1; i++)
  {
    int c = 0;
    expression.resize(0);

    while (c < 2)
    {
      temp = inputStream.readLine();
      c+= temp.count("\",", Qt::CaseSensitive);
      expression.append(temp);
      if (c < 2)
        expression.append(" ");
    }

    words = expression.split("\",");
    original = words[0].mid(1);
    translation = words[1].mid(1);
    lesson = words[2].toInt();

    KEduVocExpression kve;
    kve.setOriginal(original);
    kve.setGrade(1, 0, false);
    kve.setGrade(1, 0, true);
    kve.setLesson(lesson);
    kve.addTranslation(translation);

    m_doc->appendEntry(&kve);

    inputStream.readLine();
    inputStream.readLine();
  }

  inputStream.readLine();
  inputStream.readLine();
  inputStream.readLine();

  for (int i = 0; !inputStream.atEnd() && i < lines; i++)
  {
    lessonDescr = inputStream.readLine();
    lessonDescr = lessonDescr.mid(1, lessonDescr.length() - 2);
    if (!lessonDescr.isEmpty())
      lessonDescriptions.append(lessonDescr);
    else
      break;
    inputStream.readLine();
  }

  if (lessonDescriptions.count() > 0)
    m_doc->setLessonDescriptions(lessonDescriptions);

  return true;
}
