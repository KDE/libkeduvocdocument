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
    resetGrades();
}


void KEduVocGrade::resetGrades()
{
    m_grade = KV_NORM_GRADE;
    m_queryCount = 0;
    m_badCount = 0;

    QDateTime dt;
    dt.setTime_t(0);
    m_queryDate = dt;
}


grade_t KEduVocGrade::grade () const
{
    return m_grade;
}


void KEduVocGrade::setGrade ( grade_t grade )
{
    if ( grade > KV_MAX_GRADE )
        grade = KV_MAX_GRADE;
    if ( grade < KV_MIN_GRADE )
        grade = KV_MIN_GRADE;
    m_grade = grade;
}


void KEduVocGrade::incGrade ()
{
    setGrade ( grade() + 1 );
}


void KEduVocGrade::decGrade ()
{
    setGrade ( grade() - 1 );
}


count_t KEduVocGrade::queryCount ( )  const
{
    return m_queryCount;
}


void KEduVocGrade::incQueryCount ( )
{
    setQueryCount ( queryCount() + 1 );
}


void KEduVocGrade::incBadCount ( )
{
    setBadCount ( badCount() + 1 );
}


void KEduVocGrade::setQueryCount ( count_t count )
{
    m_queryCount = count;
}


count_t KEduVocGrade::badCount ( ) const
{
    return m_badCount;
}


void KEduVocGrade::setBadCount ( count_t count )
{
    m_badCount = count;
}


QDateTime KEduVocGrade::queryDate () const
{
    return m_queryDate;
}


void KEduVocGrade::setQueryDate ( const QDateTime & date )
{
    m_queryDate = date;
}
