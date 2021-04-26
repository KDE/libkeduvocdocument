/*
 * SPDX-FileCopyrightText: 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "keduvoctext.h"

#include "kvtml2defs.h"
#include "keduvockvtml2writer.h"

#include <QDomDocument>

class KEduVocText::KEduVocTextPrivate
{
public:
    /// This is the word itself. The vocabulary. This is what it is all about.
    QString m_text;

    grade_t m_preGrade;         // Before it gets to grade 1.
    grade_t m_grade;
    count_t m_totalPracticeCount;
    count_t m_badCount;
    QDateTime m_practiceDate;
    quint32 m_interval;         // Interval in seconds until next training is due.
};

KEduVocText::KEduVocText(const QString& text)
        :d( new KEduVocTextPrivate )
{
    d->m_text = text;
    resetGrades();
}

KEduVocText::KEduVocText( const KEduVocText &other )
    : d( new KEduVocTextPrivate )
{
    d->m_text = other.d->m_text;
    setPreGrade( other.preGrade() );
    setGrade( other.grade() );
    setPracticeCount( other.practiceCount() );
    setBadCount( other.badCount() );
    setPracticeDate( other.practiceDate() );
    setInterval( other.interval() );
}

KEduVocText::~KEduVocText()
{
    delete d;
}

QString KEduVocText::text() const
{
    return d->m_text;
}

void KEduVocText::setText( const QString & expr )
{
    d->m_text = expr.simplified();
}

void KEduVocText::resetGrades()
{
    d->m_preGrade = KV_NORM_GRADE;
    d->m_grade = KV_NORM_GRADE;
    d->m_totalPracticeCount = 0;
    d->m_badCount = 0;

    QDateTime dt;
    dt.setTime_t( 0 );
    d->m_practiceDate = dt;
    d->m_interval = 0;
}


grade_t KEduVocText::preGrade() const
{
    return d->m_preGrade;
}


void KEduVocText::setPreGrade( grade_t grade )
{
    if ( grade > KV_MAX_GRADE ) {
        grade = KV_MAX_GRADE;
    }
    d->m_preGrade = grade;
}


grade_t KEduVocText::grade() const
{
    return d->m_grade;
}


void KEduVocText::setGrade( grade_t grade )
{
    if ( grade > KV_MAX_GRADE ) {
        grade = KV_MAX_GRADE;
    }
    d->m_grade = grade;
}


void KEduVocText::incGrade()
{
    setGrade( qMax<grade_t>(grade() + 1, KV_LEV1_GRADE ) );
}


void KEduVocText::decGrade()
{
    if ( grade() == KV_MIN_GRADE ) {
        return;
    }
    setGrade( grade() - 1 );
}


count_t KEduVocText::practiceCount()  const
{
    return d->m_totalPracticeCount;
}


void KEduVocText::incPracticeCount()
{
    setPracticeCount( practiceCount() + 1 );
}


void KEduVocText::incBadCount()
{
    setBadCount( badCount() + 1 );
}


void KEduVocText::setPracticeCount( count_t count )
{
    d->m_totalPracticeCount = count;
}


count_t KEduVocText::badCount() const
{
    return d->m_badCount;
}


void KEduVocText::setBadCount( count_t count )
{
    d->m_badCount = count;
}


QDateTime KEduVocText::practiceDate() const
{
    return d->m_practiceDate;
}


void KEduVocText::setPracticeDate( const QDateTime & date )
{
    d->m_practiceDate = date;
}

quint32 KEduVocText::interval() const
{
    return d->m_interval;
}


void KEduVocText::setInterval( quint32 interval )
{
    d->m_interval = interval;
}


KEduVocText &KEduVocText::operator=(const KEduVocText &other)
{
    d->m_text = other.d->m_text;
    d->m_preGrade = other.d->m_preGrade;
    d->m_grade = other.d->m_grade;
    d->m_totalPracticeCount = other.d->m_totalPracticeCount;
    d->m_badCount = other.d->m_badCount;
    d->m_practiceDate = other.d->m_practiceDate;
    d->m_interval = other.d->m_interval;

    return *this;
}

bool KEduVocText::operator==(const KEduVocText &other) const
{
    return
        d->m_text == other.d->m_text &&
        d->m_preGrade == other.d->m_preGrade &&
        d->m_grade == other.d->m_grade &&
        d->m_totalPracticeCount == other.d->m_totalPracticeCount &&
        d->m_badCount == other.d->m_badCount &&
        d->m_practiceDate == other.d->m_practiceDate &&
        d->m_interval == other.d->m_interval;
}

void KEduVocText::toKVTML2(QDomElement& parent)
{
    QDomDocument domDoc = parent.ownerDocument();
    if (d->m_text.isEmpty() && d->m_totalPracticeCount == 0) {
        return;
    }

    // the text
    KEduVocKvtml2Writer::appendTextElement( parent, KVTML_TEXT, text() );

    // grades
    if ( d->m_totalPracticeCount > 0 ) {
        QDomElement gradeElement = domDoc.createElement( KVTML_GRADE );

            //<pregrade>2</pregrade>
        KEduVocKvtml2Writer::appendTextElement( gradeElement, KVTML_PREGRADE, QString::number( preGrade() ) );

            //<currentgrade>2</currentgrade>
        KEduVocKvtml2Writer::appendTextElement( gradeElement, KVTML_CURRENTGRADE, QString::number( grade() ) );

            //<count>6</count>
        KEduVocKvtml2Writer::appendTextElement( gradeElement, KVTML_COUNT, QString::number( practiceCount() ) );

            //<errorcount>1</errorcount>
        KEduVocKvtml2Writer::appendTextElement( gradeElement, KVTML_ERRORCOUNT, QString::number( badCount() ) );

            //<date>949757271</date>
        KEduVocKvtml2Writer::appendTextElement( gradeElement, KVTML_DATE,  practiceDate().toString( Qt::ISODate ) );

            //<interval>300</interval>
        KEduVocKvtml2Writer::appendTextElement( gradeElement, KVTML_INTERVAL,  QString::number(interval()) );


        parent.appendChild( gradeElement );
    }
}

void KEduVocText::fromKVTML2(QDomElement & parent)
{
    setText( parent.firstChildElement( KVTML_TEXT ).text() );

    // grade element
    const QDomElement& gradeElement = parent.firstChildElement( KVTML_GRADE );
    if ( !gradeElement.isNull() ) {

        setPreGrade( gradeElement.firstChildElement(KVTML_PREGRADE).text().toInt() );
        setGrade( gradeElement.firstChildElement(KVTML_CURRENTGRADE).text().toInt() );

        setPracticeCount( gradeElement.firstChildElement(KVTML_COUNT).text().toInt() );

        setBadCount( gradeElement.firstChildElement(KVTML_ERRORCOUNT).text().toInt() );

        QString dateString = gradeElement.firstChildElement(KVTML_DATE).text();
        if ( !dateString.isEmpty() ) {
            QDateTime value = QDateTime::fromString( dateString, Qt::ISODate );
            setPracticeDate( value );
        }
        setInterval( gradeElement.firstChildElement(KVTML_INTERVAL).text().toInt() );
    }
}

bool KEduVocText::isEmpty()
{
    return d->m_text.isEmpty();
}
