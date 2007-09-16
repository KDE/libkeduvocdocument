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

class KEduVocGrade::KEduVocGradePrivate
{
public:
    grade_t m_grade;
    count_t m_queryCount;
    count_t m_badCount;
    QDateTime m_queryDate;
};

KEduVocGrade::KEduVocGrade()
        :d( new KEduVocGradePrivate )
{
    resetGrades();
}

KEduVocGrade::KEduVocGrade( const KEduVocGrade &other )
        :d( new KEduVocGradePrivate )
{
    setGrade( other.grade() );
    setQueryCount( other.queryCount() );
    setBadCount( other.badCount() );
    setQueryDate( other.queryDate() );
}

KEduVocGrade::~KEduVocGrade()
{
    delete d;
}

void KEduVocGrade::resetGrades()
{
    d->m_grade = KV_NORM_GRADE;
    d->m_queryCount = 0;
    d->m_badCount = 0;

    QDateTime dt;
    dt.setTime_t( 0 );
    d->m_queryDate = dt;
}


grade_t KEduVocGrade::grade() const
{
    return d->m_grade;
}


void KEduVocGrade::setGrade( grade_t grade )
{
    if ( grade > KV_MAX_GRADE )
        grade = KV_MAX_GRADE;
    d->m_grade = grade;
}


void KEduVocGrade::incGrade()
{
    setGrade( grade() + 1 );
}


void KEduVocGrade::decGrade()
{
    setGrade( grade() - 1 );
}


count_t KEduVocGrade::queryCount()  const
{
    return d->m_queryCount;
}


void KEduVocGrade::incQueryCount()
{
    setQueryCount( queryCount() + 1 );
}


void KEduVocGrade::incBadCount()
{
    setBadCount( badCount() + 1 );
}


void KEduVocGrade::setQueryCount( count_t count )
{
    d->m_queryCount = count;
}


count_t KEduVocGrade::badCount() const
{
    return d->m_badCount;
}


void KEduVocGrade::setBadCount( count_t count )
{
    d->m_badCount = count;
}


QDateTime KEduVocGrade::queryDate() const
{
    return d->m_queryDate;
}


void KEduVocGrade::setQueryDate( const QDateTime & date )
{
    d->m_queryDate = date;
}

KEduVocGrade & KEduVocGrade::operator =(const KEduVocGrade & other)
{
    d->m_grade = other.d->m_grade;
    d->m_queryCount = other.d->m_queryCount;
    d->m_badCount = other.d->m_badCount;
    d->m_queryDate = other.d->m_queryDate;

    return *this;
}

bool KEduVocGrade::operator ==(const KEduVocGrade & other) const
{
    return d->m_grade == other.d->m_grade &&
        d->m_queryCount == other.d->m_queryCount &&
        d->m_badCount == other.d->m_badCount &&
        d->m_queryDate == other.d->m_queryDate;
}
