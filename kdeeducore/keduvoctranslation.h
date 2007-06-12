/***************************************************************************
                        Vocabulary Expression Translation for KDE Edu
    -----------------------------------------------------------------------
    copyright      :(C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef KEDUVOCEXPRESSIONTRANSLATION_H
#define KEDUVOCEXPRESSIONTRANSLATION_H


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
#include <QMap>
#include <QString>

#include "keduvocgrade.h"
#include "keduvocgrammar.h"
#include "keduvocmultiplechoice.h"
// @todo: move conjugation to it's proper file #include "keduvocconjugation.h"


/**
	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/

class KDEEDUCORE_EXPORT KEduVocTranslation
{
public:
    KEduVocTranslation( );
    KEduVocTranslation( const QString &translation );

    ~KEduVocTranslation();

    QString translation () const;
    void setTranslation ( const QString & expr );


    void resetGrades();


  /** sets the pronunciation of this expression
   *
   * @param index            index of expression
   * @param expression       pronunciation of this index
   */
  void setPronunciation( const QString & expression);

  /** returns the pronunciation of this expression
   *
   * @param index            index of expression
   * @return                 pronunciation or "" if none available
   */
  QString pronunciation() const;

  /** returns comments of this expression
   *
   * @param index            index of expression
   * @return                 comment or "" if no comment available
   */
  QString comment() const;

  /** sets comment of this expression
   *
   * @param index            index of expression
   * @param expr             comment of this index
   */
  void setComment( const QString & expr);

  /** sets false friend of this expression
   *
   * @param index            index of expression
   * @param indexint indexFrom        index of original translation
   * @param expression       false friend of this index
   */
  void setFalseFriend( int indexFrom, const QString & expression );

  /** returns false friend of this expression
   *
   * @param index            index of expression

   * @return                 false friend or "" if no string available
   */
  QString falseFriend( int indexFrom) const;

  /** sets synonym this expression
   *
   * @param index            index of expression
   * @param expression       synonym of this index
   */
  void setSynonym( const QString & expression);

  /** returns synonym of this expression
   *
   * @param index            index of expression
   * @return                 synonym or "" if no string available
   */
  QString synonym() const;

  /** sets example this expression
   *
   * @param index            index of expression
   * @param expression       example of this index
   */
  void setExample( const QString & expression);

  /** returns example of this expression
   *
   * @param index            index of expression
   * @return                 example or "" if no string available
   */
  QString example() const;

  /** sets usage label this expression
   *
   * @param index            index of expression
   * @param usage            usage label of this index
   */
  void setUsageLabel( const QString & usage);

  /** returns usage label of this expression
   *
   * @param index            index of expression
   * @return                 usage or "" if no string available
   */
  QString usageLabel() const;

  /** sets paraphrase of this expression
   *
   * @param index            index of expression
   * @param expression       paraphrase of this index
   */
  void setParaphrase( const QString & expression);

  /** returns paraphrase of this expression
   *
   * @param index            index of expression
   * @return                 paraphrase or "" if no string available
   */
  QString paraphrase() const;

  /** sets antonym this expression
   *
   * @param index            index of expression
   * @param expression       antonym of this index
   */
  void setAntonym( const QString & expression);

  /** returns antonym of this expression
   *
   * @param index            index of expression
   * @return                 antonym or "" if no string available
   */
  QString antonym() const;

  /** returns type of this expression
   *
   * @return                 type or "" if no type available
   */
  QString type() const;

/// @todo is this still needed? if so rather in KEduVocExpression
//   /** all langs have same type ?
//    *
//    * @return                 true if all have same type
//    */
//   bool uniqueType () const;

 /** sets type of this expression
   *
   * @param index            index of type
   * @param type             type of this expression ("" = none)
   */
  void setType( const QString &type);

  /** sets grade of given translation
   *
   * @param index            index of translation
   * @param indexint indexFrom        the identifier displayed to the user
   * @param grade            number of knowlegde: 0=known, x=numbers not knows
   */
  void setGrade( int indexFrom, grade_t grade );

  /** returns grade of given translation as int
   *
   * @param index            index of translation
   * @param indexint indexFrom          the identifier displayed to the user
   * @return                 number of knowlegde: 0=known, x=numbers not knows
   */
  grade_t grade( int indexFrom ) const;

  /** increments grade of given translation
   *
   * @param index            index of translation
   * @param indexint indexFrom        the identifier displayed to the user

   */
  void incGrade(  int indexFrom );

  /** decrements grade of given translation
   *
   * @param index            index of translation
   * @param indexint indexFrom        the identifier displayed to the user

   */
  void decGrade(  int indexFrom );

  /** returns last query date of given translation as int
   *
   * @param index            index of translation
   * @param indexint indexFrom        the identifier displayed to the user

   */
  QDateTime queryDate(  int indexFrom ) const;

  /** set last query date of given translation as int
   *
   * @param index            index of translation
   * @param indexint indexFrom        the identifier displayed to the user
   * @param date             the new date

   */
  void setQueryDate(  int indexFrom, const QDateTime & date );

  /** returns conjugations if available
   *
   * @param index            index of translation
   */
  KEduVocConjugation conjugation() const;

  /** sets conjugations
   *
   * @param index            index of translation
   * @param conjugation      conjugation block
   */
  void setConjugation(const KEduVocConjugation & conjugation);

  /** returns comparison if available
   *
   * @param index            index of translation
   */
  KEduVocComparison comparison() const;

  /** sets comparison
   *
   * @param index            index of translation
   * @param comparison       comparison block
   */
  void setComparison( const KEduVocComparison & comparison);

  /** returns multiple choice if available
   *
   * @param index            index of multiple choice
   */
  KEduVocMultipleChoice multipleChoice() const;

  /** sets multiple choice
   *
   * @param index            index of translation
   * @param mc               multiple choice block
   */
  void setMultipleChoice( const KEduVocMultipleChoice &mc);


  /** returns query count of given translation as int
   *
   * @param index            index of translation
   * @param indexint indexFrom        the identifier displayed to the user

   */
  count_t queryCount(  int indexFrom  ) const;

  /** set query count of given translation as int
   *
   * @param index            index of translation
   * @param indexint indexFrom        the identifier displayed to the user
   * @param count            the new count

   */
  void setQueryCount( int indexFrom, count_t count );

  /** returns bad query count of given translation as int
   *
   * @param index            index of translation
   * @param indexint indexFrom        the identifier displayed to the user
   * @param reverse          dito, in opposite direction
   */
  count_t badCount( int indexFrom ) const;

  /** set bad query count of given translation as int
   *
   * @param index            index of translation
   * @param indexint indexFrom        the identifier displayed to the user
   * @param count            the new count
   */
  void setBadCount( int indexFrom, count_t count );

  /** increment bad query count of given translation by 1
   *
   * @param index            index of translation
   * @param indexint indexFrom        the identifier displayed to the user
   */
  void incBadCount( int indexFrom );

  /** increment query count of given translation by 1
   *
   * @param index            index of translation
   * @param indexint indexFrom        the identifier displayed to the user

   */
  void incQueryCount( int indexFrom );

    KEduVocTranslation& operator=(const KEduVocTranslation &translation);
    bool operator==(const KEduVocTranslation &translation) const;

private:
    //KEduVocTranslationPrivate* const d;

    /// This is the word itself. The vocabulary. This is what it is all about.
    QString m_translation;
    /// noun:male etc (language dependend)
    QString m_types;
    QString m_usageLabel;
    QString m_comment;
    QString m_paraphrase;
    QString m_synonym;
    QString m_example;
    QString m_antonym;
    QString m_pronunciation;

    KEduVocConjugation m_conjugation;

    KEduVocComparison m_comparison;

    KEduVocMultipleChoice m_multipleChoice;

    // Here come all int indexFrom grades. (If you want, imagine the TO grades as int indexFrom of the other translation. That is where they belong. )
    // User is asked to give THIS here as answer, than the grades go here.
    // User answers, this is the source, grades go to the other translation.
    // Grades go to the translation the user has to type/choose/whatever.
    // not all have to be supplied
    QMap<int, KEduVocGrade> m_grades;

    /// One false friend string per other language
    QMap<int, QString> m_falseFriends;

};

#endif
