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

  QString m_original;

  // all these vectors must be deleted in removeTranslation()
  QStringList m_expressionTypes;
  QStringList m_translations;
  QStringList m_remarks;
  QStringList m_usageLabels;
  QStringList m_paraphrases;
  QStringList m_fauxAmi;
  QStringList m_reverseFauxAmi;
  QStringList m_synonym;
  QStringList m_example;
  QStringList m_antonym;
  QStringList m_pronunciations;
  QList<grade_t> m_grades;
  QList<grade_t> m_reverseGrades;
  QList<count_t> m_queryCounts;
  QList<count_t> m_reverseQueryCounts;
  QList<count_t> m_badCounts;
  QList<count_t> m_reverseBadCounts;
  QList<QDateTime> m_queryDates;
  QList<QDateTime> m_reverseQueryDates;
  QList<KEduVocConjugation> m_conjugations;
  QList<KEduVocComparison> m_comparisons;
  QList<KEduVocMultipleChoice> m_multipleChoices;

  QString m_leitnerBox;
  int m_sortIndex;
  int m_lesson;
  bool m_inQuery;
  bool m_active;
};


void KEduVocExpression::KEduVocExpressionPrivate::init()
{
  m_grades.append(KV_NORM_GRADE);
  m_reverseGrades.append(KV_NORM_GRADE);
  m_inQuery = false;
  m_active = true;
  m_queryCounts.append(0);
  m_reverseQueryCounts.append(0);
  m_badCounts.append(0);
  m_reverseBadCounts.append(0);
  QDateTime dt;
  m_queryDates.append(dt);
  m_reverseQueryDates.append(dt);
  m_lesson = 0;
  m_sortIndex = 0;
}


bool KEduVocExpression::KEduVocExpressionPrivate::operator==(const KEduVocExpression::KEduVocExpressionPrivate &p) const
{
  return m_original == p.m_original &&
    m_expressionTypes == p.m_expressionTypes &&
    m_translations == p.m_translations &&
    m_remarks == p.m_remarks &&
    m_usageLabels == p.m_usageLabels &&
    m_paraphrases == p.m_paraphrases &&
    m_fauxAmi == p.m_fauxAmi &&
    m_reverseFauxAmi == p.m_reverseFauxAmi &&
    m_synonym == p.m_synonym &&
    m_example == p.m_example &&
    m_antonym == p.m_antonym &&
    m_pronunciations == p.m_pronunciations &&
    m_grades == p.m_grades &&
    m_reverseGrades == p.m_reverseGrades &&
    m_queryCounts == p.m_queryCounts &&
    m_reverseQueryCounts == p.m_reverseQueryCounts &&
    m_badCounts == p.m_badCounts &&
    m_reverseBadCounts == p.m_reverseBadCounts &&
    m_queryDates == p.m_queryDates &&
    m_reverseQueryDates == p.m_reverseQueryDates &&
    m_conjugations == p.m_conjugations &&
    m_comparisons == p.m_comparisons &&
    m_multipleChoices == p.m_multipleChoices &&
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
  setOriginal(expression.simplified());
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

    if (pos == -1) {
      setOriginal(expr.simplified());
    }
    else {
      se = expr.left(pos).simplified();
      setOriginal(se);
      expr.remove(0, pos + separator.length());

      // gather all translations
      while ((pos = expr.indexOf(separator)) != -1) {
        se = expr.left(pos).simplified();
        addTranslation(se, KV_NORM_GRADE, KV_NORM_GRADE);
        expr.remove(0, pos + separator.length());
      }
      addTranslation(expr.simplified(), KV_NORM_GRADE, KV_NORM_GRADE);
    }
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


int KEduVocExpression::translationCount() const
{
  return d->m_translations.count();
}


QString KEduVocExpression::remark(int index) const
{
  if (index >= d->m_remarks.count() || index < 0) {
    return "";
  }
  else {
    return d->m_remarks[index];
  }
}


void KEduVocExpression::setRemark(int index, const QString & expr)
{
  if (index < 0)
    return;

  // extend remarks with empty strings if necessary
  if (d->m_remarks.count() <= index)
    for (int i = d->m_remarks.count(); i < index + 1; i++)
      d->m_remarks.append("");

  d->m_remarks[index] = expr.simplified();
}


void KEduVocExpression::setFauxAmi(int index, const QString & expr, bool rev_ami)
{
  if (index < 1)
    return;

  if (rev_ami) {
    // extend friend with empty strings if necessary
    if (d->m_reverseFauxAmi.count() <= index)
      for (int i = d->m_reverseFauxAmi.count(); i < index + 1; i++)
        d->m_reverseFauxAmi.append("");

    d->m_reverseFauxAmi[index] = expr.simplified();
  }
  else {
    // extend friend with empty strings if necessary
    if (d->m_fauxAmi.count() <= index)
      for (int i = d->m_fauxAmi.count(); i < index + 1; i++)
        d->m_fauxAmi.append("");

    d->m_fauxAmi[index] = expr.simplified();
  }
}


QString KEduVocExpression::fauxAmi(int index, bool rev_ami) const
{
  if (rev_ami) {
    if (index >= d->m_reverseFauxAmi.count() || index < 1) {
      return "";
    }

    return d->m_reverseFauxAmi[index];
  }

  if (index >= d->m_fauxAmi.count() || index < 1) {
    return "";
  }

  return d->m_fauxAmi[index];
}


void KEduVocExpression::setSynonym(int index, const QString & expr)
{
  if (index < 0)
    return;

  // extend synonym with empty strings if necessary
  if (d->m_synonym.count() <= index)
    for (int i = d->m_synonym.count(); i < index + 1; i++)
      d->m_synonym.append("-");

  d->m_synonym[index] = expr.simplified();
}


QString KEduVocExpression::synonym(int index) const
{
  if (index >= d->m_synonym.count() || index < 0) {
    return "";
  }
  else {
    return d->m_synonym[index];
  }
}


void KEduVocExpression::setExample(int index, const QString & expr)
{
  if (index < 0)
    return;

  // extend exampls with empty strings if necessary
  if (d->m_example.count() <= index)
    for (int i = d->m_example.count(); i < index + 1; i++)
      d->m_example.append("");

  d->m_example[index] = expr.simplified();
}


QString KEduVocExpression::example(int index) const
{
  if (index >= d->m_example.count() || index < 0) {
    return "";
  }
  else {
    return d->m_example[index];
  }
}


void KEduVocExpression::setUsageLabel(int index, const QString & expr)
{
  if (index < 0)
    return;

  // extend labels with empty strings if necessary
  if (d->m_usageLabels.count() <= index)
    for (int i = d->m_usageLabels.count(); i < index + 1; i++)
      d->m_usageLabels.append("");

  d->m_usageLabels[index] = expr.simplified();
}


QString KEduVocExpression::usageLabel(int index) const
{
  if (index >= d->m_usageLabels.count() || index < 0) {
    return "";
  }
  else {
    return d->m_usageLabels[index];
  }
}


void KEduVocExpression::setParaphrase(int index, const QString & expr)
{
  if (index < 0)
    return;

  // extend phrase with empty strings if necessary
  if (d->m_paraphrases.count() <= index)
    for (int i = d->m_paraphrases.count(); i < index + 1; i++)
      d->m_paraphrases.append("");

  d->m_paraphrases[index] = expr.simplified();
}


QString KEduVocExpression::paraphrase(int index) const
{
  if (index >= d->m_paraphrases.count() || index < 0) {
    return "";
  }
  else {
    return d->m_paraphrases[index];
  }
}


void KEduVocExpression::setAntonym(int index, const QString & expr)
{
  if (index < 0)
    return;

  // extend antonym with empty strings if necessary
  if (d->m_antonym.count() <= index)
    for (int i = d->m_antonym.count(); i < index + 1; i++)
      d->m_antonym.append("");

  d->m_antonym[index] = expr.simplified();
}


QString KEduVocExpression::antonym(int index) const
{
  if (index >= d->m_antonym.count() || index < 0) {
    return "";
  }
  else {
    return d->m_antonym[index];
  }
}


void KEduVocExpression::setConjugation(int index, const KEduVocConjugation &con)
{
  if (index < 0)
    return;

  // extend conjugation with empty elements
  if (d->m_conjugations.count() <= index)
    for (int i = d->m_conjugations.count(); i < index + 1; i++)
      d->m_conjugations.append(KEduVocConjugation());

  d->m_conjugations[index] = con;
}


KEduVocConjugation KEduVocExpression::conjugation(int index) const
{
  if (index >= d->m_conjugations.count() || index < 0) {
    return KEduVocConjugation();
  }
  else {
    return d->m_conjugations[index];
  }
}


void KEduVocExpression::setComparison(int index, const KEduVocComparison &con)
{
  if (index < 0)
    return;

  // extend comparison with empty elements
  if (d->m_comparisons.count() <= index)
    for (int i = d->m_comparisons.count(); i < index + 1; i++)
      d->m_comparisons.append(KEduVocComparison());

  d->m_comparisons[index] = con;
}


KEduVocComparison KEduVocExpression::comparison (int index) const
{
  if (index >= d->m_comparisons.count() || index < 0) {
    return KEduVocComparison();
  }
  else {
    return d->m_comparisons[index];
  }
}


void KEduVocExpression::setMultipleChoice(int index, const KEduVocMultipleChoice &mc)
{
  if (index < 0)
    return;

  // extend comparison with empty elements
  if (d->m_multipleChoices.count() <= index)
    for (int i = d->m_multipleChoices.count(); i < index + 1; i++)
      d->m_multipleChoices.append(KEduVocMultipleChoice());

  d->m_multipleChoices[index] = mc;
}


KEduVocMultipleChoice KEduVocExpression::multipleChoice(int index) const
{
  if (index >= d->m_multipleChoices.count() || index < 0) {
    return KEduVocMultipleChoice();
  }
  else {
    return d->m_multipleChoices[index];
  }
}


QString KEduVocExpression::pronunciation(int index) const
{
  if (index >= d->m_pronunciations.count() || index < 0) {
    return "";
  }
  else {
    return d->m_pronunciations[index];
  }
}


void KEduVocExpression::setPronunciation(int index, const QString & expr)
{
  if (index < 0) return;

  // extend with empty strings if necessary
  if (d->m_pronunciations.count() <= index)
    for (int i = d->m_pronunciations.count(); i < index + 1; i++)
      d->m_pronunciations.append("");

  d->m_pronunciations[index] = expr.simplified();
}


void KEduVocExpression::addTranslation(const QString & expr, grade_t grade, grade_t rev_grade)
{
  if (grade > KV_MAX_GRADE)
    grade = KV_MAX_GRADE;

  if (rev_grade > KV_MAX_GRADE)
    rev_grade = KV_MAX_GRADE;

  d->m_grades.append(grade);
  d->m_reverseGrades.append(rev_grade);
  d->m_translations.append(expr.simplified());
}


QString KEduVocExpression::translation(int index) const
{
  if (index > d->m_translations.count() || index < 1)
    return "";
  else
    return d->m_translations[index-1];
}


void KEduVocExpression::removeTranslation(int index)
{
  if (index <= 0)
    return;

  if (index <= translationCount())
    d->m_translations.removeAt(index - 1);

  if (index < d->m_remarks.count())
    d->m_remarks.removeAt(index - 1);

  if (index < d->m_conjugations.count())
    d->m_conjugations.removeAt(index -1);

  if (index < d->m_comparisons.count())
    d->m_comparisons.removeAt(index - 1);

  if (index < d->m_fauxAmi.count())
    d->m_fauxAmi.removeAt(index -  1);

  if (index < d->m_reverseFauxAmi.count())
    d->m_reverseFauxAmi.removeAt(index - 1);

  if (index < d->m_synonym.count())
    d->m_synonym.removeAt(index - 1);

  if (index < d->m_example.count())
    d->m_example.removeAt(index - 1);

  if (index < d->m_usageLabels.count())
    d->m_usageLabels.removeAt(index - 1);

  if (index < d->m_paraphrases.count())
    d->m_paraphrases.removeAt(index - 1);

  if (index < d->m_antonym.count())
    d->m_antonym.removeAt(index - 1);

  if (index < d->m_expressionTypes.count())
    d->m_expressionTypes.removeAt(index - 1);

  if (index < d->m_pronunciations.count())
    d->m_pronunciations.removeAt(index - 1);

  if (index < d->m_grades.count())
    d->m_grades.removeAt(index - 1);

  if (index < d->m_reverseGrades.count())
    d->m_reverseGrades.removeAt(index - 1);

  if (index < d->m_queryCounts.count())
    d->m_queryCounts.removeAt(index - 1);

  if (index < d->m_reverseQueryCounts.count())
    d->m_reverseQueryCounts.removeAt(index - 1);

  if (index < d->m_badCounts.count())
    d->m_badCounts.removeAt(index - 1);

  if (index < d->m_reverseBadCounts.count())
    d->m_reverseBadCounts.removeAt(index - 1);

  if (index < d->m_queryDates.count())
    d->m_queryDates.removeAt(index - 1);

  if (index < d->m_reverseQueryDates.count())
    d->m_reverseQueryDates.removeAt(index - 1);
}


void KEduVocExpression::setTranslation(int index, const QString & expr)
{
  if (index <= 0)
    return;

  // extend translations with empty strings if necessary
  if (d->m_translations.count() < index)
    for (int i = d->m_translations.count(); i < index; i++)
      d->m_translations.append("");

//  if (index <= translations.count())
  d->m_translations[index-1] = expr.simplified();
}


grade_t KEduVocExpression::grade(int index, bool rev_grade) const
{
  if (rev_grade) {
    if (index >= d->m_reverseGrades.count() || index < 1) {
      return KV_NORM_GRADE;
    }
    else if (d->m_reverseGrades[index] > KV_MAX_GRADE) {
      return KV_MAX_GRADE;
    }

    return d->m_reverseGrades[index];

  }
  else {
    if (index >= d->m_grades.count() || index < 1) {
      return KV_NORM_GRADE;
    }
    else if (d->m_grades[index] > KV_MAX_GRADE) {
      return KV_MAX_GRADE;
    }

    return d->m_grades[index];
  }
}


void KEduVocExpression::setGrade(int index, grade_t grade, bool rev_grade)
{
  if (index < 1)
    return;

  if (grade > KV_MAX_GRADE)
    grade = KV_MAX_GRADE;
  if (grade < KV_MIN_GRADE)
    grade = KV_MIN_GRADE;

  if (rev_grade) {
    // extend rev grades with standard grade if necessary
    if (d->m_reverseGrades.count() <= index)
      for (int i = d->m_reverseGrades.count(); i <= index; i++) {
        d->m_reverseGrades.append(KV_NORM_GRADE);
    }
    d->m_reverseGrades[index] = grade;
  }
  else {
    // extend grades with standard grade if necessary
    if (d->m_grades.count() <= index)
      for (int i = d->m_grades.count(); i <= index; i++) {
        d->m_grades.append(KV_NORM_GRADE);
      }
    d->m_grades[index] = grade;
  }
}


void KEduVocExpression::incGrade(int index, bool rev_grade)
{
  if (index < 1)
    return;

  if (rev_grade) {
    // extend rev grades with standard grade if necessary
    if (d->m_reverseGrades.count() <= index)
      for (int i = d->m_reverseGrades.count(); i <= index; i++) {
        d->m_reverseGrades.append(KV_NORM_GRADE);
    }
    if (d->m_reverseGrades[index] < KV_MAX_GRADE)
      d->m_reverseGrades[index]++;
  }
  else {
    // extend grades with standard grade if necessary
    if (d->m_grades.count() <= index)
      for (int i = d->m_grades.count(); i <= index; i++) {
        d->m_grades.append(KV_NORM_GRADE);
      }
    if (d->m_grades[index] < KV_MAX_GRADE)
      d->m_grades[index]++;
  }
}


void KEduVocExpression::decGrade(int index, bool rev_grade)
{
  if (index < 1)
    return;

  if (rev_grade) {
    // extend rev grades with standard grade if necessary
    if (d->m_reverseGrades.count() <= index)
      for (int i = d->m_reverseGrades.count(); i <= index; i++) {
        d->m_reverseGrades.append(KV_NORM_GRADE);
    }
    if (d->m_reverseGrades[index] > KV_MIN_GRADE)
      d->m_reverseGrades[index]--;
  }
  else {
    // extend grades with standard grade if necessary
    if (d->m_grades.count() <= index)
      for (int i = d->m_grades.count(); i <= index; i++) {
        d->m_grades.append(KV_NORM_GRADE);
      }
    if (d->m_grades[index] > KV_MIN_GRADE)
      d->m_grades[index]--;
  }
}


count_t KEduVocExpression::queryCount(int index, bool rev_count)  const
{
  if (rev_count) {
    if (index >= d->m_reverseQueryCounts.count() || index < 1) {
      return 0;
    }

    return d->m_reverseQueryCounts[index];
  }

  if (index >= d->m_queryCounts.count() || index < 1) {
    return 0;
  }

  return d->m_queryCounts[index];
}


void KEduVocExpression::setQueryCount(int index, count_t count, bool rev_count)
{
  if (index < 1)
    return;

  if (rev_count) {
    // extend rev counts with 0 if necessary
    if (d->m_reverseQueryCounts.count() <= index)
      for (int i = d->m_reverseQueryCounts.count(); i <= index; i++) {
        d->m_reverseQueryCounts.append(0);
    }

    d->m_reverseQueryCounts[index] = count;

  }
  else {
    // extend counts with 0 if necessary
    if (d->m_queryCounts.count() <= index)
      for (int i = d->m_queryCounts.count(); i <= index; i++) {
        d->m_queryCounts.append(0);
      }
    d->m_queryCounts[index] = count;
  }
}


count_t KEduVocExpression::badCount(int index, bool rev_count) const
{
  if (rev_count) {
    if (index >= d->m_reverseBadCounts.count() || index < 1) {
      return 0;
    }

    return d->m_reverseBadCounts[index];
  }

  if (index >= d->m_badCounts.count() || index < 1) {
    return 0;
  }

  return d->m_badCounts[index];
}


void KEduVocExpression::setBadCount(int index, count_t count, bool rev_count)
{
  if (index < 1)
    return;

  if (rev_count) {
    // extend rev counts with 0 if necessary
    if (d->m_reverseBadCounts.count() <= index)
      for (int i = d->m_reverseBadCounts.count(); i <= index; i++) {
        d->m_reverseBadCounts.append(0);
    }

    d->m_reverseBadCounts[index] = count;

  }
  else {
    // extend counts with 0 if necessary
    if (d->m_badCounts.count() <= index)
      for (int i = d->m_badCounts.count(); i <= index; i++) {
        d->m_badCounts.append(0);
      }
    d->m_badCounts[index] = count;
  }
}


QDateTime KEduVocExpression::queryDate(int index, bool rev_date) const
{
  if (rev_date) {
    if (index >= d->m_reverseQueryDates.count() || index < 1)
    {
      QDateTime dt;
      dt.setTime_t(0);
      return dt;
    }

    return d->m_reverseQueryDates[index];
  }

  if (index >= d->m_queryDates.count() || index < 1)
  {
    QDateTime dt;
    dt.setTime_t(0);
    return dt;
  }

  return d->m_queryDates[index];
}


void KEduVocExpression::setQueryDate(int index, const QDateTime & date, bool rev_date)
{
  if (index < 1)
    return;

  if (rev_date) {
    // extend rev dates with 0 if necessary
    if (d->m_reverseQueryDates.count() <= index)
      for (int i = d->m_reverseQueryDates.count(); i <= index; i++)
      {
        QDateTime dt;
        dt.setTime_t(0);
        d->m_reverseQueryDates.append(dt);
      }

    d->m_reverseQueryDates[index] = date;

  }
  else {
    // extend dates with 0 if necessary
    if (d->m_queryDates.count() <= index)
      for (int i = d->m_queryDates.count(); i <= index; i++)
      {
        QDateTime dt;
        dt.setTime_t(0);
        d->m_queryDates.append(dt);
      }
    d->m_queryDates[index] = date;
  }
}


bool KEduVocExpression::uniqueType() const
{
  bool unique = true;
  QString type0 = type(0);
  for (int i = 1; i < translationCount(); i++)
    if (type0 != type(i))
      unique = false;
  return unique;
}


QString KEduVocExpression::type(int index) const
{
  if (index >= d->m_expressionTypes.count() || index < 0) {
    return "";
  }
  else {
    return d->m_expressionTypes[index];
  }
}


void KEduVocExpression::setType(int index, const QString &type)
{
  if (index < 0)
    return;

  // extend types with empty strings if necessary
  if (d->m_expressionTypes.count() <= index)
    for (int i = d->m_expressionTypes.count(); i < index + 1; i++)
      d->m_expressionTypes.append("");

  d->m_expressionTypes[index] = type.simplified();
}


int KEduVocExpression::lesson() const
{
  return d->m_lesson;
}


void KEduVocExpression::setLesson(int l)
{
  d->m_lesson = l;
}


void KEduVocExpression::incQueryCount(int index, bool rev_count)
{
  setQueryCount(index, queryCount(index, rev_count) + 1, rev_count);
}


void KEduVocExpression::incBadCount(int index, bool rev_count)
{
  setBadCount(index, badCount(index, rev_count) + 1, rev_count);
}


QString KEduVocExpression::original() const
{
  return d->m_original;
}


void KEduVocExpression::setOriginal(const QString & expr)
{
  d->m_original = expr;
}


QString KEduVocExpression::leitnerBox()
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


KEduVocExpression& KEduVocExpression::operator=(const KEduVocExpression &expression)
{
  *d = *expression.d;
  return *this;
}


bool KEduVocExpression::operator==(const KEduVocExpression &expression) const
{
  return (*d == *expression.d);
}

