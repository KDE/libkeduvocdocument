/***************************************************************************
                        Vocabulary Expression for KDE Edu
    -----------------------------------------------------------------------
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                           (C) 2005-2007 Peter Hedlund
    email                : peter.hedlund@kdemail.net
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

void KEduVocExpression::Init()
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

KEduVocExpression::KEduVocExpression()
{
  Init();
}

KEduVocExpression::KEduVocExpression(const QString & expression, int lesson)
{
  Init();
  setOriginal(expression.simplified());
  m_lesson = lesson;
}

KEduVocExpression::KEduVocExpression(const QString & expression, const QString & separator, int lesson)
{
  Init();
  QString se;
  QString expr = expression;
  m_lesson = lesson;

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

int KEduVocExpression::translationCount() const
{
  return m_translations.count();
}


QString KEduVocExpression::remark(int index) const
{
  if (index >= m_remarks.count() || index < 0) {
    return "";
  }
  else {
    return m_remarks[index];
  }
}


void KEduVocExpression::setRemark(int index, const QString & expr)
{
  if (index < 0)
    return;

  // extend remarks with empty strings if necessary
  if (m_remarks.count() <= index)
    for (int i = m_remarks.count(); i < index + 1; i++)
      m_remarks.append("");

  m_remarks[index] = expr.simplified();
}


void KEduVocExpression::setFauxAmi(int index, const QString & expr, bool rev_ami)
{
  if (index < 1)
    return;

  if (rev_ami) {
    // extend friend with empty strings if necessary
    if (m_reverseFauxAmi.count() <= index)
      for (int i = m_reverseFauxAmi.count(); i < index + 1; i++)
        m_reverseFauxAmi.append("");

    m_reverseFauxAmi[index] = expr.simplified();
  }
  else {
    // extend friend with empty strings if necessary
    if (m_fauxAmi.count() <= index)
      for (int i = m_fauxAmi.count(); i < index + 1; i++)
        m_fauxAmi.append("");

    m_fauxAmi[index] = expr.simplified();
  }
}


QString KEduVocExpression::fauxAmi(int index, bool rev_ami) const
{
  if (rev_ami) {
    if (index >= m_reverseFauxAmi.count() || index < 1) {
      return "";
    }

    return m_reverseFauxAmi[index];
  }

  if (index >= m_fauxAmi.count() || index < 1) {
    return "";
  }

  return m_fauxAmi[index];
}


void KEduVocExpression::setSynonym(int index, const QString & expr)
{
  if (index < 0)
    return;

  // extend synonym with empty strings if necessary
  if (m_synonym.count() <= index)
    for (int i = m_synonym.count(); i < index + 1; i++)
      m_synonym.append("-");

  m_synonym[index] = expr.simplified();
}


QString KEduVocExpression::synonym(int index) const
{
  if (index >= m_synonym.count() || index < 0) {
    return "";
  }
  else {
    return m_synonym[index];
  }
}


void KEduVocExpression::setExample(int index, const QString & expr)
{
  if (index < 0)
    return;

  // extend exampls with empty strings if necessary
  if (m_example.count() <= index)
    for (int i = m_example.count(); i < index + 1; i++)
      m_example.append("");

  m_example[index] = expr.simplified();
}


QString KEduVocExpression::example(int index) const
{
  if (index >= m_example.count() || index < 0) {
    return "";
  }
  else {
    return m_example[index];
  }
}


void KEduVocExpression::setUsageLabel(int index, const QString & expr)
{
  if (index < 0)
    return;

  // extend labels with empty strings if necessary
  if (m_usageLabels.count() <= index)
    for (int i = m_usageLabels.count(); i < index + 1; i++)
      m_usageLabels.append("");

  m_usageLabels[index] = expr.simplified();
}


QString KEduVocExpression::usageLabel(int index) const
{
  if (index >= m_usageLabels.count() || index < 0) {
    return "";
  }
  else {
    return m_usageLabels[index];
  }
}


void KEduVocExpression::setParaphrase(int index, const QString & expr)
{
  if (index < 0)
    return;

  // extend phrase with empty strings if necessary
  if (m_paraphrases.count() <= index)
    for (int i = m_paraphrases.count(); i < index + 1; i++)
      m_paraphrases.append("");

  m_paraphrases[index] = expr.simplified();
}


QString KEduVocExpression::paraphrase(int index) const
{
  if (index >= m_paraphrases.count() || index < 0) {
    return "";
  }
  else {
    return m_paraphrases[index];
  }
}


void KEduVocExpression::setAntonym(int index, const QString & expr)
{
  if (index < 0)
    return;

  // extend antonym with empty strings if necessary
  if (m_antonym.count() <= index)
    for (int i = m_antonym.count(); i < index + 1; i++)
      m_antonym.append("");

  m_antonym[index] = expr.simplified();
}


QString KEduVocExpression::antonym(int index) const
{
  if (index >= m_antonym.count() || index < 0) {
    return "";
  }
  else {
    return m_antonym[index];
  }
}


void KEduVocExpression::setConjugation(int index, const KEduVocConjugation &con)
{
  if (index < 0)
    return;

  // extend conjugation with empty elements
  if (m_conjugations.count() <= index)
    for (int i = m_conjugations.count(); i < index + 1; i++)
      m_conjugations.append(KEduVocConjugation());

  m_conjugations[index] = con;
}


KEduVocConjugation KEduVocExpression::conjugation(int index) const
{
  if (index >= m_conjugations.count() || index < 0) {
    return KEduVocConjugation();
  }
  else {
    return m_conjugations[index];
  }
}


void KEduVocExpression::setComparison(int index, const KEduVocComparison &con)
{
  if (index < 0)
    return;

  // extend comparison with empty elements
  if (m_comparisons.count() <= index)
    for (int i = m_comparisons.count(); i < index + 1; i++)
      m_comparisons.append(KEduVocComparison());

  m_comparisons[index] = con;
}


KEduVocComparison KEduVocExpression::comparison (int index) const
{
  if (index >= m_comparisons.count() || index < 0) {
    return KEduVocComparison();
  }
  else {
    return m_comparisons[index];
  }
}


void KEduVocExpression::setMultipleChoice(int index, const KEduVocMultipleChoice &mc)
{
  if (index < 0)
    return;

  // extend comparison with empty elements
  if (m_multipleChoices.count() <= index)
    for (int i = m_multipleChoices.count(); i < index + 1; i++)
      m_multipleChoices.append(KEduVocMultipleChoice());

  m_multipleChoices[index] = mc;
}


KEduVocMultipleChoice KEduVocExpression::multipleChoice(int index) const
{
  if (index >= m_multipleChoices.count() || index < 0) {
    return KEduVocMultipleChoice();
  }
  else {
    return m_multipleChoices[index];
  }
}


QString KEduVocExpression::pronunciation(int index) const
{
  if (index >= m_pronunciations.count() || index < 0) {
    return "";
  }
  else {
    return m_pronunciations[index];
  }
}


void KEduVocExpression::setPronunciation(int index, const QString & expr)
{
  if (index < 0) return;

  // extend with empty strings if necessary
  if (m_pronunciations.count() <= index)
    for (int i = m_pronunciations.count(); i < index + 1; i++)
      m_pronunciations.append("");

  m_pronunciations[index] = expr.simplified();
}


void KEduVocExpression::addTranslation(const QString & expr, grade_t grade, grade_t rev_grade)
{
  if (grade > KV_MAX_GRADE)
    grade = KV_MAX_GRADE;

  if (rev_grade > KV_MAX_GRADE)
    rev_grade = KV_MAX_GRADE;

  m_grades.append(grade);
  m_reverseGrades.append(rev_grade);
  m_translations.append(expr.simplified());
}


QString KEduVocExpression::translation(int index) const
{
  if (index > m_translations.count() || index < 1)
    return "";
  else
    return m_translations[index-1];
}


void KEduVocExpression::removeTranslation(int index)
{
  if (index <= 0)
    return;

  if (index <= translationCount())
    m_translations.removeAt(index - 1);

  if (index < m_remarks.count())
    m_remarks.removeAt(index - 1);

  if (index < m_conjugations.count())
    m_conjugations.removeAt(index -1);

  if (index < m_comparisons.count())
    m_comparisons.removeAt(index - 1);

  if (index < m_fauxAmi.count())
    m_fauxAmi.removeAt(index -  1);

  if (index < m_reverseFauxAmi.count())
    m_reverseFauxAmi.removeAt(index - 1);

  if (index < m_synonym.count())
    m_synonym.removeAt(index - 1);

  if (index < m_example.count())
    m_example.removeAt(index - 1);

  if (index < m_usageLabels.count())
    m_usageLabels.removeAt(index - 1);

  if (index < m_paraphrases.count())
    m_paraphrases.removeAt(index - 1);

  if (index < m_antonym.count())
    m_antonym.removeAt(index - 1);

  if (index < m_expressionTypes.count())
    m_expressionTypes.removeAt(index - 1);

  if (index < m_pronunciations.count())
    m_pronunciations.removeAt(index - 1);

  if (index < m_grades.count())
    m_grades.removeAt(index - 1);

  if (index < m_reverseGrades.count())
    m_reverseGrades.removeAt(index - 1);

  if (index < m_queryCounts.count())
    m_queryCounts.removeAt(index - 1);

  if (index < m_reverseQueryCounts.count())
    m_reverseQueryCounts.removeAt(index - 1);

  if (index < m_badCounts.count())
    m_badCounts.removeAt(index - 1);

  if (index < m_reverseBadCounts.count())
    m_reverseBadCounts.removeAt(index - 1);

  if (index < m_queryDates.count())
    m_queryDates.removeAt(index - 1);

  if (index < m_reverseQueryDates.count())
    m_reverseQueryDates.removeAt(index - 1);
}


void KEduVocExpression::setTranslation(int index, const QString & expr)
{
  if (index <= 0)
    return;

  // extend translations with empty strings if necessary
  if (m_translations.count() < index)
    for (int i = m_translations.count(); i < index; i++)
      m_translations.append("");

//  if (index <= translations.count())
  m_translations[index-1] = expr.simplified();
}


grade_t KEduVocExpression::grade(int index, bool rev_grade) const
{
  if (rev_grade) {
    if (index >= m_reverseGrades.count() || index < 1) {
      return KV_NORM_GRADE;
    }
    else if (m_reverseGrades[index] > KV_MAX_GRADE) {
      return KV_MAX_GRADE;
    }

    return m_reverseGrades[index];

  }
  else {
    if (index >= m_grades.count() || index < 1) {
      return KV_NORM_GRADE;
    }
    else if (m_grades[index] > KV_MAX_GRADE) {
      return KV_MAX_GRADE;
    }

    return m_grades[index];
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
    if (m_reverseGrades.count() <= index)
      for (int i = m_reverseGrades.count(); i <= index; i++) {
        m_reverseGrades.append(KV_NORM_GRADE);
    }
    m_reverseGrades[index] = grade;
  }
  else {
    // extend grades with standard grade if necessary
    if (m_grades.count() <= index)
      for (int i = m_grades.count(); i <= index; i++) {
        m_grades.append(KV_NORM_GRADE);
      }
    m_grades[index] = grade;
  }
}


void KEduVocExpression::incGrade(int index, bool rev_grade)
{
  if (index < 1)
    return;

  if (rev_grade) {
    // extend rev grades with standard grade if necessary
    if (m_reverseGrades.count() <= index)
      for (int i = m_reverseGrades.count(); i <= index; i++) {
        m_reverseGrades.append(KV_NORM_GRADE);
    }
    if (m_reverseGrades[index] < KV_MAX_GRADE)
      m_reverseGrades[index]++;
  }
  else {
    // extend grades with standard grade if necessary
    if (m_grades.count() <= index)
      for (int i = m_grades.count(); i <= index; i++) {
        m_grades.append(KV_NORM_GRADE);
      }
    if (m_grades[index] < KV_MAX_GRADE)
      m_grades[index]++;
  }
}


void KEduVocExpression::decGrade(int index, bool rev_grade)
{
  if (index < 1)
    return;

  if (rev_grade) {
    // extend rev grades with standard grade if necessary
    if (m_reverseGrades.count() <= index)
      for (int i = m_reverseGrades.count(); i <= index; i++) {
        m_reverseGrades.append(KV_NORM_GRADE);
    }
    if (m_reverseGrades[index] > KV_MIN_GRADE)
      m_reverseGrades[index]--;
  }
  else {
    // extend grades with standard grade if necessary
    if (m_grades.count() <= index)
      for (int i = m_grades.count(); i <= index; i++) {
        m_grades.append(KV_NORM_GRADE);
      }
    if (m_grades[index] > KV_MIN_GRADE)
      m_grades[index]--;
  }
}


count_t KEduVocExpression::queryCount(int index, bool rev_count)  const
{
  if (rev_count) {
    if (index >= m_reverseQueryCounts.count() || index < 1) {
      return 0;
    }

    return m_reverseQueryCounts[index];
  }

  if (index >= m_queryCounts.count() || index < 1) {
    return 0;
  }

  return m_queryCounts[index];
}


void KEduVocExpression::setQueryCount(int index, count_t count, bool rev_count)
{
  if (index < 1)
    return;

  if (rev_count) {
    // extend rev counts with 0 if necessary
    if (m_reverseQueryCounts.count() <= index)
      for (int i = m_reverseQueryCounts.count(); i <= index; i++) {
        m_reverseQueryCounts.append(0);
    }

    m_reverseQueryCounts[index] = count;

  }
  else {
    // extend counts with 0 if necessary
    if (m_queryCounts.count() <= index)
      for (int i = m_queryCounts.count(); i <= index; i++) {
        m_queryCounts.append(0);
      }
    m_queryCounts[index] = count;
  }
}


count_t KEduVocExpression::badCount(int index, bool rev_count) const
{
  if (rev_count) {
    if (index >= m_reverseBadCounts.count() || index < 1) {
      return 0;
    }

    return m_reverseBadCounts[index];
  }

  if (index >= m_badCounts.count() || index < 1) {
    return 0;
  }

  return m_badCounts[index];
}


void KEduVocExpression::setBadCount(int index, count_t count, bool rev_count)
{
  if (index < 1)
    return;

  if (rev_count) {
    // extend rev counts with 0 if necessary
    if (m_reverseBadCounts.count() <= index)
      for (int i = m_reverseBadCounts.count(); i <= index; i++) {
        m_reverseBadCounts.append(0);
    }

    m_reverseBadCounts[index] = count;

  }
  else {
    // extend counts with 0 if necessary
    if (m_badCounts.count() <= index)
      for (int i = m_badCounts.count(); i <= index; i++) {
        m_badCounts.append(0);
      }
    m_badCounts[index] = count;
  }
}


QDateTime KEduVocExpression::queryDate(int index, bool rev_date) const
{
  if (rev_date) {
    if (index >= m_reverseQueryDates.count() || index < 1)
    {
      QDateTime dt;
      dt.setTime_t(0);
      return dt;
    }

    return m_reverseQueryDates[index];
  }

  if (index >= m_queryDates.count() || index < 1)
  {
    QDateTime dt;
    dt.setTime_t(0);
    return dt;
  }

  return m_queryDates[index];
}


void KEduVocExpression::setQueryDate(int index, const QDateTime & date, bool rev_date)
{
  if (index < 1)
    return;

  if (rev_date) {
    // extend rev dates with 0 if necessary
    if (m_reverseQueryDates.count() <= index)
      for (int i = m_reverseQueryDates.count(); i <= index; i++)
      {
        QDateTime dt;
        dt.setTime_t(0);
        m_reverseQueryDates.append(dt);
      }

    m_reverseQueryDates[index] = date;

  }
  else {
    // extend dates with 0 if necessary
    if (m_queryDates.count() <= index)
      for (int i = m_queryDates.count(); i <= index; i++)
      {
        QDateTime dt;
        dt.setTime_t(0);
        m_queryDates.append(dt);
      }
    m_queryDates[index] = date;
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
  if (index >= m_expressionTypes.count() || index < 0) {
    return "";
  }
  else {
    return m_expressionTypes[index];
  }
}


void KEduVocExpression::setType(int index, const QString &type)
{
  if (index < 0)
    return;

  // extend types with empty strings if necessary
  if (m_expressionTypes.count() <= index)
    for (int i = m_expressionTypes.count(); i < index + 1; i++)
      m_expressionTypes.append("");

  m_expressionTypes[index] = type.simplified();
}


int KEduVocExpression::lesson() const
{
  return m_lesson;
}


void KEduVocExpression::setLesson(int l)
{
  m_lesson = l;
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
  return m_original;
}


void KEduVocExpression::setOriginal(const QString & expr)
{
  m_original = expr;
}


QString& KEduVocExpression::leitnerBox()
{
  return m_leitnerBox;
}


void KEduVocExpression::setLeitnerBox(const QString& box)
{
  m_leitnerBox = box;
}

