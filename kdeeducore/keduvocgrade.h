/***************************************************************************
                        Vocabulary Expression Grades for KDE Edu
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

#ifndef KEDUVOCGRADE_H
#define KEDUVOCGRADE_H

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

#include "libkdeedu_core_export.h"

#include <QtCore/QDateTime>

typedef signed char grade_t;
typedef unsigned short count_t;

class KEduVocGradePrivate;

/**
Contains grading information (query date, bad count) for one language with respect to another.

	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class KDEEDUCORE_EXPORT KEduVocGrade
{
public:
    KEduVocGrade();

    /** copy constructor
     * provides safe copy of d pointer
     */
    KEduVocGrade(const KEduVocGrade &other);

      /** returns query count of given translation as int
   *
   * @param index            index of translation
   * @param index       the identifier displayed to the user

   */
  count_t queryCount(   ) const;

  /** set query count of given translation as int
   *
   * @param index            index of translation
   * @param index       the identifier displayed to the user
   * @param count            the new count

   */
  void setQueryCount( count_t count );

  /** returns bad query count of given translation as int
   *
   * @param index            index of translation
   * @param index       the identifier displayed to the user
   * @param reverse          dito, in opposite direction
   */
  count_t badCount( ) const;

  /** set bad query count of given translation as int
   *
   * @param index            index of translation
   * @param index       the identifier displayed to the user
   * @param count            the new count
   */
  void setBadCount( count_t count );

  /** increment bad query count of given translation by 1
   *
   * @param index            index of translation
   * @param index       the identifier displayed to the user
   */
  void incBadCount( );

  /** increment query count of given translation by 1
   *
   * @param index            index of translation
   * @param index       the identifier displayed to the user

   */
  void incQueryCount( );


    /**
     * Clears grading and date information.
     */
    void resetGrades();

  /** sets grade of given translation
   *
   * @param index            index of translation
   * @param index       the identifier displayed to the user
   * @param grade            number of knowlegde: 0=known, x=numbers not knows
   */
  void setGrade( grade_t grade );

  /** returns grade of given translation as int
   *
   * @param index            index of translation
   * @param index         the identifier displayed to the user
   * @return                 number of knowlegde: 0=known, x=numbers not knows
   */
  grade_t grade() const;

  /** increments grade of given translation
   *
   * @param index            index of translation
   * @param index       the identifier displayed to the user

   */
  void incGrade(  );

  /** decrements grade of given translation
   *
   * @param index            index of translation
   * @param index       the identifier displayed to the user

   */
  void decGrade(  );

  /** returns last query date of given translation as int
   *
   * @param index            index of translation
   * @param index       the identifier displayed to the user

   */
  QDateTime queryDate(  ) const;


   /**
   * Set last query date
   * @param index            index of translation
   * @param date             the new date
   */
    void setQueryDate ( const QDateTime & date );

private:

    KEduVocGradePrivate *d;
};

#endif
