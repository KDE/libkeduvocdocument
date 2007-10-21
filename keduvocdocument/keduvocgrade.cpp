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
    count_t m_totalPracticeCount;
    count_t m_badCount;
    QDateTime m_practiceDate;
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
    setPracticeCount( other.practiceCount() );
    setBadCount( other.badCount() );
    setPracticeDate( other.practiceDate() );
}

KEduVocGrade::~KEduVocGrade()
{
    delete d;
}

void KEduVocGrade::resetGrades()
{
    d->m_grade = KV_NORM_GRADE;
    d->m_totalPracticeCount = 0;
    d->m_badCount = 0;

    QDateTime dt;
    dt.setTime_t( 0 );
    d->m_practiceDate = dt;
}


grade_t KEduVocGrade::grade() const
{
    return d->m_grade;
}


void KEduVocGrade::setGrade( grade_t grade )
{
    if ( grade > KV_MAX_GRADE ) {
        grade = KV_MAX_GRADE;
    }
    d->m_grade = grade;
}


void KEduVocGrade::incGrade()
{
    setGrade( grade() + 1 );
}


void KEduVocGrade::decGrade()
{
    if ( grade() == KV_MIN_GRADE ) {
        return;
    }
    setGrade( grade() - 1 );
}


count_t KEduVocGrade::practiceCount()  const
{
    return d->m_totalPracticeCount;
}


void KEduVocGrade::incPracticeCount()
{
    setPracticeCount( practiceCount() + 1 );
}


void KEduVocGrade::incBadCount()
{
    setBadCount( badCount() + 1 );
}


void KEduVocGrade::setPracticeCount( count_t count )
{
    d->m_totalPracticeCount = count;
}


count_t KEduVocGrade::badCount() const
{
    return d->m_badCount;
}


void KEduVocGrade::setBadCount( count_t count )
{
    d->m_badCount = count;
}


QDateTime KEduVocGrade::practiceDate() const
{
    return d->m_practiceDate;
}


void KEduVocGrade::setPracticeDate( const QDateTime & date )
{
    d->m_practiceDate = date;
}

KEduVocGrade & KEduVocGrade::operator =(const KEduVocGrade & other)
{
    d->m_grade = other.d->m_grade;
    d->m_totalPracticeCount = other.d->m_totalPracticeCount;
    d->m_badCount = other.d->m_badCount;
    d->m_practiceDate = other.d->m_practiceDate;

    return *this;
}

bool KEduVocGrade::operator ==(const KEduVocGrade & other) const
{
    return d->m_grade == other.d->m_grade &&
        d->m_totalPracticeCount == other.d->m_totalPracticeCount &&
        d->m_badCount == other.d->m_badCount &&
        d->m_practiceDate == other.d->m_practiceDate;
}
