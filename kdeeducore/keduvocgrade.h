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


#include <QtCore/QDateTime>

typedef signed char grade_t;
typedef unsigned short count_t;

/**
Contains grading information (query date, bad count) for one language with respect to another.

	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class KEduVocGrade
{
public:
    KEduVocGrade();
    void resetGrades();

/// @todo make these private and write the according functions like updateQueryDate, increaseGrade, decreaseGrade etc.
public:
    grade_t m_grade;
    count_t m_queryCount;
    count_t m_badCount;
    QDateTime m_queryDate;


};

#endif
