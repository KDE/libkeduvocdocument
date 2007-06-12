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

#include <QDateTime>

typedef signed char grade_t;
typedef unsigned short count_t;

/**
Contains grading information (query date, bad count) for one language with respect to another.

	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class KEduVocGrade
{
public:
    void resetGrades();

/// @todo make these private and write the according functions like updateQueryDate, increaseGrade, decreaseGrade etc.
public:
    grade_t m_grade;
    count_t m_queryCount;
    count_t m_badCount;
    QDateTime m_queryDate;


};

#endif
