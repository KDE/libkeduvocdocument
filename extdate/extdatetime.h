/*************************************************************************
**
** Definition of extended range date classe
**
** Created : 031102
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
**********************************************************************/

#ifndef EXTDATETIME_H
#define EXTDATETIME_H

#ifndef QT_H
#include "qstring.h"
#include "qnamespace.h"
#include "qdatetime.h"
#endif // QT_H


/*****************************************************************************
  ExtDate class
 *****************************************************************************/

extern	void test2_unit(int y, int m, int d);

class Q_EXPORT ExtDate
{
public:
    ExtDate() { jd = my_invalid_day; }
    ExtDate( int y, int m, int d );
    ExtDate( const QDate &q ) { ExtDate( q.year(), q.month(), q.day() ); }
    bool isNull() const { return jd == 0; }
    bool isValid() const;

    QDate qdate() const;

    int year() const;
    int month() const;
    int day() const;
    int dayOfWeek() const;
    int dayOfYear() const;
    int daysInMonth() const;
    int daysInYear() const;
    int weekNumber( int *yearNum = 0 ) const;

#ifndef QT_NO_TEXTDATE
#ifndef QT_NO_COMPAT
    static QString monthName( int month ) { return shortMonthName( month ); }
    static QString dayName( int weekday ) { return shortDayName( weekday ); }
#endif
    static QString shortMonthName( int month );
    static QString shortDayName( int weekday );
    static QString longMonthName( int month );
    static QString longDayName( int weekday );
#endif //QT_NO_TEXTDATE
#ifndef QT_NO_TEXTSTRING
#if !defined(QT_NO_SPRINTF)
    QString toString( Qt::DateFormat f = Qt::TextDate ) const;
#endif
    QString toString( const QString& format ) const;
#endif
    bool   setYMD( int y, int m, int d );

    ExtDate  addDays( int days )		const;
    ExtDate  addMonths( int months )      const;
    ExtDate  addYears( int years )        const;
    int	   daysTo( const ExtDate & )	const;

    bool   operator==( const ExtDate &d ) const { return jd == d.jd; };
    bool   operator!=( const ExtDate &d ) const { return jd != d.jd; };
    bool   operator<( const ExtDate &d )	const { return jd < d.jd; };
    bool   operator<=( const ExtDate &d ) const { return jd <= d.jd; };
    bool   operator>( const ExtDate &d )	const { return jd > d.jd; };
    bool   operator>=( const ExtDate &d ) const { return jd >= d.jd; };

    static ExtDate currentDate( Qt::TimeSpec ts = Qt::LocalTime );
#ifndef QT_NO_DATESTRING
    static ExtDate fromString( const QString& s, Qt::DateFormat f = Qt::TextDate );
#endif
    static bool	 isValid( int y, int m, int d );
    static bool	 leapYear( int year );

    static uint	 gregorianToJulian( int y, int m, int d );
    static void	 julianToGregorian( uint jd, int &y, int &m, int &d );
private:
    int	 jd;
    void   set_jd(int the_jd) {jd = the_jd;};
    static int	month_length[12];
    static int	month_origin[12];
    static QString my_shortMonthNames[12];
    static QString my_shortDayNames[7];
    static QString my_longMonthNames[12];
    static QString my_longDayNames[7];
    QString toStringSimpleArg( char code, int nb ) const;

    static int my_invalid_day;
    friend class ExtDateTime;
    static int dayOfYear(int y, int m, int d);
    static int dayRank(int y, int m, int d);
#ifndef QT_NO_DATASTREAM
    friend Q_EXPORT QDataStream &operator<<( QDataStream &, const ExtDate & );
    friend Q_EXPORT QDataStream &operator>>( QDataStream &, ExtDate & );
    friend Q_EXPORT QDataStream &operator<<( QDataStream &, const ExtDateTime & );
    friend Q_EXPORT QDataStream &operator>>( QDataStream &, ExtDateTime & );
#endif
};

/*****************************************************************************
  ExtDateTime class
 *****************************************************************************/

class Q_EXPORT ExtDateTime
{
public:
    ExtDateTime() {}				// set null date and null time
    ExtDateTime( const ExtDate & );
    ExtDateTime( const ExtDate &, const QTime & );

    bool   isNull()	const		{ return d.isNull() && t.isNull(); }
    bool   isValid()	const		{ return d.isValid() && t.isValid(); }

    ExtDate  date()	const		{ return d; }
    QTime  time()	const		{ return t; }
    uint   toTime_t()	const;
    void   setDate( const ExtDate &date ) { d = date; }
    void   setTime( const QTime &time ) { t = time; }
    void   setTime_t( uint secsSince1Jan1970UTC );
    void   setTime_t( uint secsSince1Jan1970UTC, Qt::TimeSpec );
#ifndef QT_NO_DATESTRING
#ifndef QT_NO_SPRINTF
    QString toString( Qt::DateFormat f = Qt::TextDate )	const;
#endif
    QString toString( const QString& format ) const;
#endif
    ExtDateTime addDays( int days )	const;
    ExtDateTime addMonths( int months )   const;
    ExtDateTime addYears( int years )     const;
    ExtDateTime addSecs( int secs )	const;
    int	   daysTo( const ExtDateTime & )	const;
    int	   secsTo( const ExtDateTime & )	const;

    bool   operator==( const ExtDateTime &dt ) const;
    bool   operator!=( const ExtDateTime &dt ) const;
    bool   operator<( const ExtDateTime &dt )  const;
    bool   operator<=( const ExtDateTime &dt ) const;
    bool   operator>( const ExtDateTime &dt )  const;
    bool   operator>=( const ExtDateTime &dt ) const;

    static ExtDateTime currentDateTime();
    static ExtDateTime currentDateTime( Qt::TimeSpec );
#ifndef QT_NO_DATESTRING
    static ExtDateTime fromString( const QString& s, Qt::DateFormat f = Qt::TextDate );
#endif
private:
    ExtDate  d;
    QTime  t;
#ifndef QT_NO_DATASTREAM
    friend Q_EXPORT QDataStream &operator<<( QDataStream &, const ExtDateTime &);
    friend Q_EXPORT QDataStream &operator>>( QDataStream &, ExtDateTime & );
#endif
};

/*****************************************************************************
  Date and time stream functions
 *****************************************************************************/

#ifndef QT_NO_DATASTREAM
Q_EXPORT QDataStream &operator<<( QDataStream &, const ExtDate & );
Q_EXPORT QDataStream &operator>>( QDataStream &, ExtDate & );
#endif // QT_NO_DATASTREAM

#endif // EXTDATE_H

