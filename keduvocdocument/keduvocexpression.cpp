/***************************************************************************
                        Vocabulary Expression for KDE Edu
    -----------------------------------------------------------------------
    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "keduvocexpression.h"

#include <KDebug>


class KEduVocExpression::KEduVocExpressionPrivate
{
public:
  KEduVocExpressionPrivate(KEduVocExpression* qq)
    : q(qq)
  {
    init();
  }

  void init();

  bool operator==(const KEduVocExpressionPrivate &p) const;

  KEduVocExpression* q;

  QString m_leitnerBox;
  int m_sortIndex;
  int m_lesson;
  bool m_inQuery;
  bool m_active;
  int m_sizeHint;

  QMap <int, KEduVocTranslation> m_translations;
};


void KEduVocExpression::KEduVocExpressionPrivate::init()
{
  m_translations.clear();

  m_inQuery = false;
  m_active = true;
  m_lesson = 0;
  m_sortIndex = 0;
  m_sizeHint = 0;
}


bool KEduVocExpression::KEduVocExpressionPrivate::operator==(const KEduVocExpression::KEduVocExpressionPrivate &p) const
{
  return
    m_translations == p.m_translations &&
    m_leitnerBox == p.m_leitnerBox &&
    m_lesson == p.m_lesson &&
    m_sortIndex == p.m_sortIndex &&
    m_inQuery == p.m_inQuery &&
    m_active == p.m_active;
}


KEduVocExpression::KEduVocExpression()
  : d(new KEduVocExpressionPrivate(this))
{
}

KEduVocExpression::KEduVocExpression(const QString & expression, int lesson)
  : d(new KEduVocExpressionPrivate(this))
{
  setTranslation(0, expression.simplified());
  d->m_lesson = lesson;
}

KEduVocExpression::KEduVocExpression(const QString & expression, const QString & separator, int lesson)
  : d(new KEduVocExpressionPrivate(this))
{
  QString se;
  QString expr = expression;
  d->m_lesson = lesson;

  if (separator.length()) {
    int pos = expr.indexOf(separator);
    int translationIndex = 0;
      se = expr.left(pos).simplified();
      setTranslation(translationIndex, se);
      expr.remove(0, pos + separator.length());

      // gather all translations
      while ((pos = expr.indexOf(separator)) != -1) {
        translationIndex++;
        se = expr.left(pos).simplified();
        setTranslation(translationIndex, se);
        expr.remove(0, pos + separator.length());
      }
      translationIndex++;
      setTranslation(translationIndex, expr.simplified());
  }
}


KEduVocExpression::KEduVocExpression(const KEduVocExpression &expression)
  : d(new KEduVocExpressionPrivate(*expression.d))
{
}


KEduVocExpression::~KEduVocExpression()
{
  delete d;
}


void KEduVocExpression::removeTranslation(int index)
{
    d->m_translations.remove(index);
}


void KEduVocExpression::setTranslation(int index, const QString & expr)
{
  if (index < 0)
    return;

///@todo get rid of this nonsense:
/* not needed with qmap but for now all translations <= index are expected to exist
  // extend translations with empty strings if necessary
  for (int i = d->m_translations.count(); i < index + 1; i++) {
      d->m_translations.append("");
  }
*/
  for (int i = d->m_translations.count(); i <= index; i++) {
      d->m_translations[i] = KEduVocTranslation("");
  }
//  if (index <= translations.count())

  d->m_translations[index] = expr.simplified();
}


int KEduVocExpression::lesson() const
{
  return d->m_lesson;
}


void KEduVocExpression::setLesson(int l)
{
  d->m_lesson = l;
}

QString KEduVocExpression::leitnerBox() const
{
  return d->m_leitnerBox;
}


void KEduVocExpression::setLeitnerBox(const QString& box)
{
  d->m_leitnerBox = box;
}


bool KEduVocExpression::isInQuery() const
{
  return d->m_inQuery;
}


void KEduVocExpression::setInQuery(bool flag)
{
  d->m_inQuery = flag;
}


bool KEduVocExpression::isActive() const
{
  return d->m_active;
}


void KEduVocExpression::setActive(bool flag)
{
  d->m_active = flag;
}

int KEduVocExpression::sizeHint() const
{
  return d->m_sizeHint;
}

void KEduVocExpression::setSizeHint(int sizeHint)
{
  d->m_sizeHint = sizeHint;
}

void KEduVocExpression::resetGrades(int index)
{
    if ( index == -1 ) { // clear grades for all languages
        foreach (KEduVocTranslation trans, d->m_translations) {
            trans.resetGrades();
        }
        return;
    }

    // only language index
    if ( d->m_translations.contains(index) ) {
        d->m_translations[index].resetGrades();
    }
}


KEduVocExpression& KEduVocExpression::operator=(const KEduVocExpression &expression)
{
  *d = *expression.d;
  return *this;
}


bool KEduVocExpression::operator==(const KEduVocExpression &expression) const
{
  return (*d == *expression.d);
}

KEduVocTranslation & KEduVocExpression::translation(int index) const
{
    return d->m_translations[index];
}

QList< int > KEduVocExpression::translationIndices() const
{
    return d->m_translations.keys();
}

