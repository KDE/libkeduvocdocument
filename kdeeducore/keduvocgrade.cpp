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

#include "keduvocgrade.h"

KEduVocGrade::KEduVocGrade()
{
    m_grade = KV_NORM_GRADE;
    m_queryCount = 0;
    m_badCount = 0;

    QDateTime dt;
    dt.setTime_t(0);
    m_queryDate = dt;
}


void KEduVocGrade::resetGrades()
{

}

