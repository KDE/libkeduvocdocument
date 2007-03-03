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

#ifndef KEDUVOCEXPRESSION_H
#define KEDUVOCEXPRESSION_H

#include <libkdeedu_core_export.h>

#define KV_MAX_GRADE       7
#define KV_MIN_GRADE       0

#define KV_NORM_GRADE      0       // not queried yet
#define KV_NORM_COLOR      Qt::black
#define KV_NORM_TEXT       I18N_NOOP("Not Queried Yet")

#define KV_LEV1_GRADE      1
#define KV_LEV1_TEXT       I18N_NOOP("Level 1")

#define KV_LEV2_GRADE      2
#define KV_LEV2_TEXT       I18N_NOOP("Level 2")

#define KV_LEV3_GRADE      3
#define KV_LEV3_TEXT       I18N_NOOP("Level 3")

#define KV_LEV4_GRADE      4
#define KV_LEV4_TEXT       I18N_NOOP("Level 4")

#define KV_LEV5_GRADE      5
#define KV_LEV5_TEXT       I18N_NOOP("Level 5")

#define KV_LEV6_GRADE      6
#define KV_LEV6_TEXT       I18N_NOOP("Level 6")

#define KV_LEV7_GRADE      7
#define KV_LEV7_TEXT       I18N_NOOP("Level 7")

#include <QtCore/QDateTime>

#include "keduvocgrammar.h"
#include "keduvocmultiplechoice.h"

typedef signed char grade_t;
typedef unsigned short count_t;

/**
  This class contains one vocabulary expression as an original with one or more
  translations
  */
class KDEEDUCORE_EXPORT KEduVocExpression
{
public:

  /** default constructor for an empty vocabulary expression
   */
  explicit KEduVocExpression();

  /** Constructor for a vocabulary expression with an original
   *
   * @param expression       original
   * @param lesson           lesson number, 0 for none
   */
  explicit KEduVocExpression(const QString & expression, int lesson = 0);

  /** Constructor for a vocabulary expression with an original and one or more translations
   *
   * @param expression       expression
   * @param separator        expression will be split into an original and one or more translations using separator
   * @param lesson           lesson number, 0 for none
   */
  KEduVocExpression(const QString & expression, const QString & separator, int lesson = 0);

  KEduVocExpression(const KEduVocExpression &expression);

  ~KEduVocExpression();

  /** adds a new translation of this entry
   * @param expression       translation
   * @param grade            grade of knowledge of this translation
   * @param reverseGrade     dito, in opposite direction
   */
  void addTranslation(const QString & expression, grade_t grade = KV_NORM_GRADE, grade_t reverseGrade = KV_NORM_GRADE);

  /** removes translation
   *
   * @param index            number of translation 1..x
   */
  void removeTranslation(int index);

  /** returns index of lesson (0 = none)
   */
  int lesson() const;

  /** sets index of lesson (0 = none)
   */
  void setLesson(int l);

  /** returns original expression of this entry
   */
  QString original() const;

  /** sets original expression of this entry
   */
  void setOriginal(const QString & expression);

  /** returns number of max. translations of all expressions
   */
  int translationCount() const;

  /** returns flag if entry is "selected" for queries
   */
  bool isInQuery() const;

  /** set entry "selected"
   */
  void setInQuery(bool flag = true);

  /** returns flag if entry is activated for queries
   */
  bool isActive() const;

  /** set entry active (enabled for queries)
   */
  void setActive(bool flag = true);

  /** returns translation of this expression
   *
   * @param index            number of translation
   * @return                 expression or "" if no translation available
   */
  QString translation(int index) const;

  /** sets translation of this expression
   *
   * @param index            number of translation
   * @param expression       expression of this index
   */
  void setTranslation(int index, const QString & expression);

  /** sets pronunciation of this expression
   *
   * @param index            index of expression
   * @param expression       pronunciation of this index
   */
  void setPronunciation(int index, const QString & expression);

  /** returns pronunciation of this expression
   *
   * @param index            index of expression
   * @return                 pronunciation or "" if none available
   */
  QString pronunciation(int index) const;

  /** returns remarks of this expression
   *
   * @param index            index of expression
   * @return                 remark or "" if no remark available
   */
  QString remark(int index) const;

  /** sets remark of this expression
   *
   * @param index            index of expression
   * @param expr             remark of this index
   */
  void setRemark(int index, const QString & expr);

  /** sets false friend of this expression
   *
   * @param index            index of expression
   * @param expression       false friend of this index
   * @param reverse          if true, represents the direction translation to original
   */
  void setFauxAmi(int index, const QString & expression, bool reverse = false);

  /** returns false friend of this expression
   *
   * @param index            index of expression
   * @param reverse          if true, represents the direction translation to original
   * @return                 false friend or "" if no string available
   */
  QString fauxAmi(int index, bool reverse = false) const;

  /** sets synonym this expression
   *
   * @param index            index of expression
   * @param expression       synonym of this index
   */
  void setSynonym(int index, const QString & expression);

  /** returns synonym of this expression
   *
   * @param index            index of expression
   * @return                 synonym or "" if no string available
   */
  QString synonym(int index) const;

  /** sets example this expression
   *
   * @param index            index of expression
   * @param expression       example of this index
   */
  void setExample(int index, const QString & expression);

  /** returns example of this expression
   *
   * @param index            index of expression
   * @return                 example or "" if no string available
   */
  QString example(int index) const;

  /** sets usage label this expression
   *
   * @param index            index of expression
   * @param usage            usage label of this index
   */
  void setUsageLabel(int index, const QString & usage);

  /** returns usage label of this expression
   *
   * @param index            index of expression
   * @return                 usage or "" if no string available
   */
  QString usageLabel(int index) const;

  /** sets paraphrase of this expression
   *
   * @param index            index of expression
   * @param expression       paraphrase of this index
   */
  void setParaphrase(int index, const QString & expression);

  /** returns paraphrase of this expression
   *
   * @param index            index of expression
   * @return                 paraphrase or "" if no string available
   */
  QString paraphrase(int index) const;

  /** sets antonym this expression
   *
   * @param index            index of expression
   * @param expression       antonym of this index
   */
  void setAntonym(int index, const QString & expression);

  /** returns antonym of this expression
   *
   * @param index            index of expression
   * @return                 antonym or "" if no string available
   */
  QString antonym(int index) const;

  /** returns type of this expression
   *
   * @return                 type or "" if no type available
   */
  QString type(int index) const;

  /** all langs have same type ?
   *
   * @return                 true if all have same type
   */
  bool uniqueType () const;

  /** sets type of this expression
   *
   * @param index            index of type
   * @param type             type of this expression ("" = none)
   */
  void setType(int index, const QString & type);

  /** sets grade of given translation
   *
   * @param index            index of translation
   * @param grade            number of knowlegde: 0=known, x=numbers not knows
   * @param reverse          if true, represents the direction translation to original
   */
  void setGrade(int index, grade_t grade, bool reverse = false);

  /** returns grade of given translation as int
   *
   * @param index            index of translation
   * @param reverse          if true, represents the direction translation to original
   * @return                 number of knowlegde: 0=known, x=numbers not knows
   */
  grade_t grade(int index, bool reverse = false) const;

  /** increments grade of given translation
   *
   * @param index            index of translation
   * @param reverse          if true, represents the direction translation to original
   */
  void incGrade(int index, bool reverse = false);

  /** decrements grade of given translation
   *
   * @param index            index of translation
   * @param reverse          if true, represents the direction translation to original
   */
  void decGrade(int index, bool reverse = false);

  /** returns last query date of given translation as int
   *
   * @param index            index of translation
   * @param reverse          if true, represents the direction translation to original
   */
  QDateTime queryDate(int index, bool reverse = false) const;

  /** set last query date of given translation as int
   *
   * @param index            index of translation
   * @param date             the new date
   * @param reverse          if true, represents the direction translation to original
   */
  void setQueryDate(int index, const QDateTime & date, bool reverse = false);

  /** returns conjugations if available
   *
   * @param index            index of translation
   */
  KEduVocConjugation conjugation(int index) const;

  /** sets conjugations
   *
   * @param index            index of translation
   * @param conjugation      conjugation block
   */
  void setConjugation(int index, const KEduVocConjugation & conjugation);

  /** returns comparison if available
   *
   * @param index            index of translation
   */
  KEduVocComparison comparison(int index) const;

  /** sets comparison
   *
   * @param index            index of translation
   * @param comparison       comparison block
   */
  void setComparison(int index, const KEduVocComparison & comparison);

  /** returns multiple choice if available
   *
   * @param index            index of multiple choice
   */
  KEduVocMultipleChoice multipleChoice(int index) const;

  /** sets multiple choice
   *
   * @param index            index of translation
   * @param mc               multiple choice block
   */
  void setMultipleChoice(int index, const KEduVocMultipleChoice & mc);

  /** returns query count of given translation as int
   *
   * @param index            index of translation
   * @param reverse          if true, represents the direction translation to original
   */
  count_t queryCount(int index, bool reverse = false) const;

  /** set query count of given translation as int
   *
   * @param index            index of translation
   * @param count            the new count
   * @param reverse          if true, represents the direction translation to original
   */
  void setQueryCount(int index, count_t count, bool reverse = false);

  /** returns bad query count of given translation as int
   *
   * @param index            index of translation
   * @param reverse          dito, in opposite direction
   */
  count_t badCount(int index, bool reverse = false) const;

  /** set bad query count of given translation as int
   *
   * @param index            index of translation
   * @param count            the new count
   * @param reverse          if true, represents the direction translation to original
   */
  void setBadCount(int index, count_t count, bool reverse = false);

  /** increment bad query count of given translation by 1
   *
   * @param index            index of translation
   * @param reverse          if true, represents the direction translation to original
   */
  void incBadCount(int index, bool reverse = false);

  /** increment query count of given translation by 1
   *
   * @param index            index of translation
   * @param reverse          if true, represents the direction translation to original
   */
  void incQueryCount(int index, bool reverse = false);

  /** sets the box of the Leitner system which actually contains the expression
   *
   * @param box              the box's name
   */
  void setLeitnerBox(const QString & box);

  /** returns the name of the Leitner system's box actually containing the expression
   *
   * @return                 the box's name
   */
  QString leitnerBox();

  KEduVocExpression& operator=(const KEduVocExpression &expression);

  bool operator==(const KEduVocExpression &expression) const;

private:
  class Private;
  Private* const d;
};

#endif // KEduVocExpression_H
