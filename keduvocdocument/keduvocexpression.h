/***************************************************************************
                        Vocabulary Expression for KDE Edu
    -----------------------------------------------------------------------
    copyright            : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
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

#ifndef KEDUVOCEXPRESSION_H
#define KEDUVOCEXPRESSION_H

#include "libkdeedu_core_export.h"

#include <QtCore/QDateTime>

#include "keduvocgrammar.h"
#include "keduvocmultiplechoice.h"
#include "keduvoctranslation.h"

/**
  This class contains one vocabulary expression as an original with one or more
  translations
  */
class KEDUVOCDOCUMENT_EXPORT KEduVocExpression
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

// ***********
//
//   /** adds a new translation of this entry
//    * @param expression       translation
//    * @param grade            grade of knowledge of this translation
//    * @param reverseGrade     dito, in opposite direction
//    */
//   void addTranslation(const QString & expression, grade_t grade = KV_NORM_GRADE, grade_t reverseGrade = KV_NORM_GRADE);
//
//   /** removes translation
//    *
//    * @param index            number of translation 1..x
//    */
//   void removeTranslation(int index);
//
  /** returns index of lesson (0 = none)
   */
  int lesson() const;

  /** sets index of lesson (0 = none)
   */
  void setLesson(int l);
//
//   /** returns original expression of this entry
//    */
//   QString original() const;
//
//   /** sets original expression of this entry
//    */
//   void setOriginal(const QString & expression);
//
//   /** returns number of max. translations of all expressions
//    */
//   int translationCount() const;


    /** reset all grades of the entry
     * @param index     identifier (language)
     */
    void resetGrades(int index);




  /** all langs have same type ?
   *
   * @return                 true if all have same type
   */
  bool uniqueType () const;
//
//   /** sets type of this expression
//    *
//    * @param index            index of type
//    * @param type             type of this expression ("" = none)
//    */
//   void setType(int index, const QString & type);
//
//   /** sets grade of given translation
//    *
//    * @param index            index of translation
//    * @param grade            number of knowlegde: 0=known, x=numbers not knows
//    * @param reverse          if true, represents the direction translation to original
//    */
//   void setGrade(int index, grade_t grade, bool reverse = false);
//
//   /** returns grade of given translation as int
//    *
//    * @param index            index of translation
//    * @param reverse          if true, represents the direction translation to original
//    * @return                 number of knowlegde: 0=known, x=numbers not knows
//    */
//   grade_t grade(int index, bool reverse = false) const;
//
//   /** increments grade of given translation
//    *
//    * @param index            index of translation
//    * @param reverse          if true, represents the direction translation to original
//    */
//   void incGrade(int index, bool reverse = false);
//
//   /** decrements grade of given translation
//    *
//    * @param index            index of translation
//    * @param reverse          if true, represents the direction translation to original
//    */
//   void decGrade(int index, bool reverse = false);
//
//   /** returns last query date of given translation as int
//    *
//    * @param index            index of translation
//    * @param reverse          if true, represents the direction translation to original
//    */
//   QDateTime queryDate(int index, bool reverse = false) const;
//
//   /** set last query date of given translation as int
//    *
//    * @param index            index of translation
//    * @param date             the new date
//    * @param reverse          if true, represents the direction translation to original
//    */
//   void setQueryDate(int index, const QDateTime & date, bool reverse = false);
//
//   /** returns conjugations if available
//    *
//    * @param index            index of translation
//    */
//   KEduVocConjugation conjugation(int index) const;
//
//   /** sets conjugations
//    *
//    * @param index            index of translation
//    * @param conjugation      conjugation block
//    */
//   void setConjugation(int index, const KEduVocConjugation & conjugation);
//
//   /** returns comparison if available
//    *
//    * @param index            index of translation
//    */
//   KEduVocComparison comparison(int index) const;
//
//   /** sets comparison
//    *
//    * @param index            index of translation
//    * @param comparison       comparison block
//    */
//   void setComparison(int index, const KEduVocComparison & comparison);
//
//   /** returns multiple choice if available
//    *
//    * @param index            index of multiple choice
//    */
//   KEduVocMultipleChoice multipleChoice(int index) const;
//
//   /** sets multiple choice
//    *
//    * @param index            index of translation
//    * @param mc               multiple choice block
//    */
//   void setMultipleChoice(int index, const KEduVocMultipleChoice & mc);
//
//   /** returns query count of given translation as int
//    *
//    * @param index            index of translation
//    * @param reverse          if true, represents the direction translation to original
//    */
//   count_t queryCount(int index, bool reverse = false) const;
//
//   /** set query count of given translation as int
//    *
//    * @param index            index of translation
//    * @param count            the new count
//    * @param reverse          if true, represents the direction translation to original
//    */
//   void setQueryCount(int index, count_t count, bool reverse = false);
//
//   /** returns bad query count of given translation as int
//    *
//    * @param index            index of translation
//    * @param reverse          dito, in opposite direction
//    */
//   count_t badCount(int index, bool reverse = false) const;
//
//   /** set bad query count of given translation as int
//    *
//    * @param index            index of translation
//    * @param count            the new count
//    * @param reverse          if true, represents the direction translation to original
//    */
//   void setBadCount(int index, count_t count, bool reverse = false);
//
//   /** increment bad query count of given translation by 1
//    *
//    * @param index            index of translation
//    * @param reverse          if true, represents the direction translation to original
//    */
//   void incBadCount(int index, bool reverse = false);
//
//   /** increment query count of given translation by 1
//    *
//    * @param index            index of translation
//    * @param reverse          if true, represents the direction translation to original
//    */
//   void incQueryCount(int index, bool reverse = false);
//
//   /** sets the box of the Leitner system which actually contains the expression
//    *
//    * @param box              the box's name
//    */
//   void setLeitnerBox(const QString & box);
//
//   /** returns the name of the Leitner system's box actually containing the expression
//    *
//    * @return                 the box's name
//    */
//   QString leitnerBox();
// **************


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

    int sizeHint() const;
    void setSizeHint(int sizeHint);

  /** returns this translation
   *
   * @return                 expression or "" if no translation available
   */
    QString translationString(int index) const;

// bad
//    QMap<int, KEduVocExpressionTranslation> allTranslations() const;


  void setTranslation(int index, const KEduVocTranslation & translation);
  /**
   * Add a translation to this expression
   * @param index            number of translation = the identifier
   * @param expression       the translation
   */
  void setTranslation(int index, const QString &expression);

  void setTranslation(const QString &identifier, const QString & expression);



  /** removes a translation
   *
   * @param index            number of translation 1..x
   */
  void removeTranslation(int index);


  /** returns number of max. translations of all expressions
    @todo kill this - will be wrong. We now should have a Identifiers class to handle this kind of stuff
   */
  int translationCount() const;


    /**
     * Get a mutable reference to the translation
     * @param index of the language identifier
     * @return the translation
     */
    KEduVocTranslation & translation (int index) const;

    QList<int> translationIndices ( ) const;



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
  class KEduVocExpressionPrivate;
  KEduVocExpressionPrivate* const d;
};

#endif // KEduVocExpression_H
