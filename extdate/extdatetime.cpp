/*************************************************************************
** Definition of extended range date classe
**
** Created : 031103
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

#include "extdatetime.h"
#include "qregexp.h"

#include <klocale.h>
#include <kdebug.h>
#include <assert.h>
#include <time.h>

static const uint SECS_PER_DAY  = 86400;
static const uint MSECS_PER_DAY = 86400000;
static const uint SECS_PER_HOUR = 3600;
static const uint MSECS_PER_HOUR= 3600000;
static const uint SECS_PER_MIN  = 60;
static const uint MSECS_PER_MIN = 60000;

/*****************************************************************************
  ExtDate class
 *****************************************************************************/

/*****************************************************************************
 *
 * Concepts :
 * a date is represented internally by its Julian Day number, a simple count
 * of the number of days since a remote, arbitrary date (01 January, 4713 BC).
 * This date has Julian Day number zero.
 *
 * ***************************************************************************/

uint ExtDate::m_monthLength[] = {31, 28, 31, 30,  31,  30,  31,  31,  30,  31,  30,  31};
uint ExtDate::m_monthOrigin[] = { 0, 30, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

QString ExtDate::m_shortMonthNames[12] = {
	i18n("Jan"), i18n("Feb"), i18n("Mar"), i18n("Apr"), i18n("May"), i18n("Jun"),
	i18n("Jul"), i18n("Aug"), i18n("Sep"), i18n("Oct"), i18n("Nov"), i18n("Dec")
};
QString ExtDate::m_shortDayNames[7] = {
	i18n("Mon"), i18n("Tue"), i18n("Wed"), i18n("Thu"), i18n("Fri"), i18n("Sat"), i18n("Sun")
};
QString ExtDate::m_longMonthNames[12] = {
	i18n("January"), i18n("February"), i18n("March"), i18n("April"), i18n("May"),
	i18n("June"), i18n("July"), i18n("August"), i18n("September"), i18n("October"),
	i18n("November"), i18n("December")
};
QString ExtDate::m_longDayNames[7] = {
	i18n("Monday"), i18n("Tuesday"), i18n("Wednesday"), i18n("Thursday"),
	i18n("Friday"), i18n("Saturday"), i18n("Sunday")
};


#ifndef QT_NO_DATESTRING
/*****************************************************************************
  Some static function used by ExtDate and ExtDateTime
 *****************************************************************************/

// Replaces tokens by their value. See ExtDateTime::toString() for a list of valid tokens
static QString getFmtString( const QString& f, const QTime* dt = 0, const ExtDate* dd = 0, bool am_pm = FALSE )
{
    if ( f.isEmpty() )
	return QString::null;

    QString buf = f;

    if ( dt ) {
	if ( f == "h" ) {
	    if ( ( am_pm ) && ( dt->hour() > 12 ) )
		buf = QString::number( dt->hour() - 12 );
	    else if ( ( am_pm ) && ( dt->hour() == 0 ) )
		buf = "12";
	    else
		buf = QString::number( dt->hour() );
	} else if ( f == "hh" ) {
	    if ( ( am_pm ) && ( dt->hour() > 12 ) )
		buf = QString::number( dt->hour() - 12 ).rightJustify( 2, '0', TRUE );
	    else if ( ( am_pm ) && ( dt->hour() == 0 ) )
		buf = "12";
	    else
		buf = QString::number( dt->hour() ).rightJustify( 2, '0', TRUE );
	} else if ( f == "m" ) {
	    buf = QString::number( dt->minute() );
	} else if ( f == "mm" ) {
	    buf = QString::number( dt->minute() ).rightJustify( 2, '0', TRUE );
	} else if ( f == "s" ) {
	    buf = QString::number( dt->second() );
	} else if ( f == "ss" ) {
	    buf = QString::number( dt->second() ).rightJustify( 2, '0', TRUE );
	} else if ( f == "z" ) {
	    buf = QString::number( dt->msec() );
	} else if ( f == "zzz" ) {
	    buf = QString::number( dt->msec() ).rightJustify( 3, '0', TRUE );
	} else if ( f == "ap" ) {
	    buf = dt->hour() < 12 ? "am" : "pm";
	} else if ( f == "AP" ) {
	    buf = dt->hour() < 12 ? "AM" : "PM";
	}
    }

    if ( dd ) {
	if ( f == "d" ) {
	    buf = QString::number( dd->day() );
	} else if ( f == "dd" ) {
	    buf = QString::number( dd->day() ).rightJustify( 2, '0', TRUE );
	} else if ( f == "M" ) {
	    buf = QString::number( dd->month() );
	} else if ( f == "MM" ) {
	    buf = QString::number( dd->month() ).rightJustify( 2, '0', TRUE );
#ifndef QT_NO_TEXTDATE
	} else if ( f == "ddd" ) {
	    buf = dd->shortDayName( dd->dayOfWeek() );
	} else if ( f == "dddd" ) {
	    buf = dd->longDayName( dd->dayOfWeek() );
	} else if ( f == "MMM" ) {
	    buf = dd->shortMonthName( dd->month() );
	} else if ( f == "MMMM" ) {
	    buf = dd->longMonthName( dd->month() );
#endif
	} else if ( f == "yy" ) {
	    buf = QString::number( dd->year() ).right( 2 );
	} else if ( f == "yyyy" ) {
	    buf = QString::number( dd->year() );
	}
    }

    return buf;
}

// Parses the format string and uses getFmtString to get the values for the tokens. Ret
static QString fmtDateTime( const QString& f, const QTime* dt = 0, const ExtDate* dd = 0 )
{
    if ( f.isEmpty() ) {
	return QString::null;
    }

    if ( dt && !dt->isValid() )
	return QString::null;
    if ( dd && !dd->isValid() )
	return QString::null;

    bool ap = ( f.contains( "AP" ) || f.contains( "ap" ) );

    QString buf;
    QString frm;
    QChar status = '0';

    for ( int i = 0; i < (int)f.length(); ++i ) {

	if ( f[ i ] == status ) {
	    if ( ( ap ) && ( ( f[ i ] == 'P' ) || ( f[ i ] == 'p' ) ) )
		status = '0';
	    frm += f[ i ];
	} else {
	    buf += getFmtString( frm, dt, dd, ap );
	    frm = QString::null;
	    if ( ( f[ i ] == 'h' ) || ( f[ i ] == 'm' ) || ( f[ i ] == 's' ) || ( f[ i ] == 'z' ) ) {
		status = f[ i ];
		frm += f[ i ];
	    } else if ( ( f[ i ] == 'd' ) || ( f[ i ] == 'M' ) || ( f[ i ] == 'y' ) ) {
		status = f[ i ];
		frm += f[ i ];
	    } else if ( ( ap ) && ( f[ i ] == 'A' ) ) {
		status = 'P';
		frm += f[ i ];
	    } else  if( ( ap ) && ( f[ i ] == 'a' ) ) {
		status = 'p';
		frm += f[ i ];
	    } else {
		buf += f[ i ];
		status = '0';
	    }
	}
    }

    buf += getFmtString( frm, dt, dd, ap );

    return buf;
}
#endif // QT_NO_DATESTRING

ExtDate::ExtDate( int y, int m, int d)
{
	if ( !isValid(y,m,d) ) {
#if defined(QT_CHECK_RANGE)
		qWarning( "ExtDate: Invalid date %04d-%02d-%02d", y, m, d );
#endif
		m_year = 0;
		m_month = 0;
		m_day = 0;
		m_jd = INVALID_DAY;
	} else {
		m_year = y;
		m_month = m;
		m_day = d;
		m_jd = GregorianToJD(y, m, d);
	}
}

ExtDate::ExtDate( long int jd ) {
	m_jd = jd;
	JDToGregorian( jd, m_year, m_month, m_day );
}

long int ExtDate::GregorianToJD( int year, int month, int day )
{
	int m, y, A, B, C, D;

	if (month > 2) {
		m = month;
		y = year;
	} else {
		y = year - 1;
		m = month + 12;
	}

/*  If the date is after 10/15/1582, then take Pope Gregory's modification
	to the Julian calendar into account */

	if ( ( year  >1582 ) ||
		( year ==1582 && month  >9 ) ||
		( year ==1582 && month ==9 && day >15 ))
	{
		A = int(y/100);
		B = 2 - A + int(A/4);
	} else {
		B = 0;
	}

	if (y < 0) {
		C = int((365.25*y) - 0.75);
	} else {
		C = int(365.25*y);
	}

	D = int(30.6001*(m+1));

	long int jd = B + C + D + day + 1720995;

	return jd;
}

void	ExtDate::JDToGregorian( long int jd, int &year, int &month, int &day )
{
	int a, b, c, d, e, alpha;

	if (jd<2299161) {
		a = jd;
	} else {
		alpha = int ((jd-1867216.25)/ 36524.25);
		a = jd + 1 + alpha - int(alpha / 4.0);
	}
	b = a + 1524;
	c = int ((b-122.1)/ 365.25);
	d = int (365.25*c);
	e = int ((b-d)/ 30.6001);

	day = b-d-int(30.6001*e);
	month = (e<14) ? e-1 : e-13;
	year  = (month>2)  ? c-4716 : c-4715;
}

bool ExtDate::isValid() const
{
	return ( isValid( year(), month(), day() ) && jd() != INVALID_DAY );
}

int ExtDate::dayOfWeek() const
{
	//JD 2451545 (01 Jan 2000) was a Saturday, which is dayOfWeek=6.
	int a_day = (( jd() - 2451545 + 6 ) % 7);
	return (a_day == 0) ? 7 : a_day;
}

int ExtDate::dayOfYear() const
{
	return jd() - GregorianToJD( year(), 1, 1) + 1;
}

int ExtDate::daysInMonth() const
{
	if ( isValid() ) {
	  int m = month();
	  int d = m_monthLength[m-1];
	  if (m==2 && leapYear(year())) d++;
	  return d;
	} else {
	  return 31;
	}
}

int ExtDate::daysInYear() const
{
	if ( ! isValid() ) return 365;
	return (leapYear(year()) ? 366 : 365);
}

int ExtDate::weekNumber( int *yearNum ) const
{
	// the year and week number are those of the next Sunday.
	ExtDate a_date( jd() - dayOfWeek() + 7);
	*yearNum = a_date.year();
	return 1 + int( a_date.dayOfYear()/7 );
}

#ifndef QT_NO_TEXTDATE
QString ExtDate::shortMonthName( int month ) {return m_shortMonthNames[month-1];}
QString ExtDate::shortDayName( int weekday ) {return m_shortDayNames[weekday-1];}
QString ExtDate::longMonthName( int month ) {return m_longMonthNames[month-1];}
QString ExtDate::longDayName( int weekday ) {return m_longDayNames[weekday-1];}
#endif //QT_NO_TEXTDATE
#ifndef QT_NO_TEXTSTRING
#if !defined(QT_NO_SPRINTF)
QString ExtDate::toStringSimpleArg( char code, int nb ) const
{
	QString	result;
	switch (code)
	{
		case 'd' :
			{
				switch (nb)
				{
					case 1 :
						result = result.sprintf("%d", day());
						break;
					case 2 :
						result = result.sprintf("%02d", day());
						break;
					case 3 :
						result = shortDayName(dayOfWeek());
						break;
					case 4 :
						result = longDayName(dayOfWeek());
						break;
					default :
						result = "?d?";
						break;
				}
			}
			break;

		case 'M' :
			{
				int	m = month();
				switch (nb)
				{
					case 1 :
						result = result.sprintf("%d", m);
						break;
					case 2 :
						result = result.sprintf("%02d", m);
						break;
					case 3 :
						result = shortMonthName(m);
						break;
					case 4 :
						result = longMonthName(m);
						break;
					default :
						result = "?M?";
						break;
				}
			}
			break;

		case 'y' :
			{
				int	y = year();
				switch (nb)
				{
					case 2 :
						result = result.sprintf("%02d", y%100);
						break;
					case 4 :
						result = result.sprintf("%04d", y);
						break;
					default :
						result = "?y?";
						break;
				}
			}
			break;

		default :
			break;
	}
	return result;
}

QString ExtDate::toString( Qt::DateFormat f) const
{
	QString	a_format;

	if ( ! isValid() ) return QString::null;

	switch (f)
	{
		case Qt::TextDate :	// Sat May 20 1995
			a_format = "ddd MMM d yyyy";
			break;

		case Qt::ISODate :	// YYYY-MM-DD
			a_format = "yyyy-MM-dd";
			break;

		case Qt::LocalDate :	// local settings
			a_format = "toString : Qt::LocalDate not implemented yet";
			break;

		default :
			a_format = "toString : unknown format";
			break;

	}
	return toString(a_format);
}
#endif
QString ExtDate::toString( const QString& format ) const
{
	// d{1-4}, M{1-4}, yy|yyyy
	uint	i, a_nb = 0;
	QString	result;
	char	a_code = '\0';
	char	a_char;

	if ( ! isValid() ) return QString::null;

	for (i = 0 ; i <= format.length() ; i++)
	{
		if (i < format.length())
		{
			a_char = format[i];
		}
		else
		{
			a_char = '\0';
		}
		if (a_code == a_char)
		{
			a_nb++;
		}
		else
		{
			if ((a_code == 'd' || a_code == 'M' || a_code == 'y') && a_nb != 0)
			{
				result += toStringSimpleArg(a_code, a_nb);
			}
			if (a_char == 'd' || a_char == 'M' || a_char == 'y')
			{
				a_code = a_char;
				a_nb = 1;
			}
			else
			{
				a_code = '\0';
				a_nb = 0;
				if (a_char != '\0')
					result += a_char;
			}
		}
	}
	return result;
}
#endif
bool ExtDate::setYMD( int y, int m, int d )
{
	if ( ! isValid(y,m,d) ) {
#if defined(QT_CHECK_RANGE)
		qWarning( "ExtDate: Invalid date %04d-%02d-%02d", y, m, d );
#endif
		m_year = 0;
		m_month = 0;
		m_day = 0;
		m_jd = INVALID_DAY;
		return false;
	} else {
		m_year = y;
		m_month = m;
		m_day = d;
		m_jd = GregorianToJD( y, m, d );
		return true;
	}
}

bool ExtDate::setJD( long int _jd ) {
	if ( _jd == INVALID_DAY ) {
		m_jd = _jd;
		m_year = 0;
		m_month = 0;
		m_day = 0;
		return false;
	} else {
		m_jd = _jd;
		JDToGregorian( _jd, m_year, m_month, m_day );
		return true;
	}
}

ExtDate ExtDate::addDays( int days ) const
{
	ExtDate a_date;
	a_date.setJD( jd() + days );
	return a_date;
}

ExtDate  ExtDate::addMonths( int months ) const
{
	int	a_month = month() + months;
	int	a_year = year() + (a_month - 1)/12;	// month : [1..12]
	a_month  = 1 + (a_month -1) % 12;
	return ExtDate(a_year, a_month, day());
}

ExtDate  ExtDate::addYears( int years ) const
{
	return ExtDate(year() + years, month(), day());
}

int   ExtDate::daysTo( const ExtDate & a_date) const
{
	return a_date.jd() - jd();
}

ExtDate ExtDate::currentDate(Qt::TimeSpec ts)
{
	time_t a_current_time;
	struct tm a_current_time_tm;

	time(&a_current_time);
	switch (ts)
	{
		case Qt::LocalTime :
			localtime_r(&a_current_time, &a_current_time_tm);
			break;

		case Qt::UTC :
			gmtime_r(&a_current_time, &a_current_time_tm);
			break;

		default :
			assert(0);
			break;
	}
	return ExtDate(a_current_time_tm.tm_year + 1900, a_current_time_tm.tm_mon + 1, a_current_time_tm.tm_mday);
}

#ifndef QT_NO_DATESTRING
ExtDate ExtDate::fromString( const QString& s, Qt::DateFormat f )
{
	ExtDate dt = ExtDate();  //initialize invalid date
	if ( s.isEmpty() ) { return dt; }
	if ( f == Qt::LocalDate ) { //can't use LocalFormat
#if defined(QT_CHECK_RANGE)
		qWarning( "QDate::fromString: Parameter out of range" );
#endif
		return dt;
}

	switch( f ) {
		case Qt::ISODate :
		{
			int year( s.mid( 0, 4 ).toInt() );
			int month( s.mid( 5, 2 ).toInt() );
			int day( s.mid( 8, 2 ).toInt() );
			if ( year && month && day )
				return ExtDate( year, month, day );
		}
		break;

		default :
#ifndef QT_NO_TEXTDATE
		case Qt::TextDate :
		{
			/*
				This will fail gracefully if the input string doesn't
				contain any space.
			*/
			int monthPos = s.find( ' ' ) + 1;
			int dayPos = s.find( ' ', monthPos ) + 1;

			QString monthName( s.mid(monthPos, dayPos - monthPos - 1) );
			int month = -1;

			// try English names first
			for ( int i = 0; i < 12; i++ ) {
				if ( monthName == m_shortMonthNames[i] ) {
						month = i + 1;
						break;
				}
			}

			// try the localized names
			if ( month == -1 ) {
				for ( int i = 0; i < 12; i++ ) {
					if ( monthName == shortMonthName( i + 1 ) ) {
						month = i + 1;
						break;
					}
				}
			}

#if defined(QT_CHECK_RANGE)
			if ( month < 1 || month > 12 ) {
				qWarning( "QDate::fromString: Parameter out of range" );
				return dt;
			}
#endif  //if defined(QT_CHECK_RANGE)

			int day = s.mid( dayPos, 2 ).stripWhiteSpace().toInt();
			int year = s.right( 4 ).toInt();
			return ExtDate( year, month, day );
		}
#else
		break;
#endif  //ifndef QT_NO_TEXTDATE
	}

	return dt;
}
#endif  //ifndef QT_NO_DATESTRING

bool ExtDate::isValid( int y, int m, int d )
{
	if (m < 1 || m > 12) return false;
	if (d < 1) return false;
	if (m != 2 && d > (int) m_monthLength[m-1]) return false;
	if (m == 2 && d > ( (int) m_monthLength[1] + (leapYear(y) ? 1 : 0))) return false;
	return true;
}

QDate ExtDate::qdate() const {
	QDate q( year(), month(), day() );

	if ( q.isValid() )
		return q;
	else
		return QDate();
}

bool	ExtDate::leapYear( int year )
{
	// year is the year-number where JC birth is 0
	if ((year % 4) != 0) return false;
	// multiple of 4 : can be a leap year
	// centennial years are NOT leap, but quadri-centennial ARE.
	if ((year % 400) == 0) return true;
	if ((year % 100) == 0) return false;
	// year is multiple of 4 but not centennial so leap year !
	return true;
}

int ExtDate::dayOfYear(int y, int m, int d)
{
	return m_monthOrigin[m-1] + d + ((m > 1) ? (leapYear(y) ? 1 : 0) : 0);
}

/*****************************************************************************
  ExtDateTime member functions
 *****************************************************************************/

/*!
    \class ExtDateTime extdatetime.h
    \reentrant
    \brief The ExtDateTime class provides date and time functions.

    \ingroup time
    \mainclass

    A ExtDateTime object contains a calendar date and a clock time (a
    "datetime"). It is a combination of the ExtDate and QTime classes.
    It can read the current datetime from the system clock. It
    provides functions for comparing datetimes and for manipulating a
    datetime by adding a number of seconds, days, months or years.

    A ExtDateTime object is typically created either by giving a date
    and time explicitly in the constructor, or by using the static
    function currentDateTime(), which returns a ExtDateTime object set
    to the system clock's time. The date and time can be changed with
    setDate() and setTime(). A datetime can also be set using the
    setTime_t() function, which takes a POSIX-standard "number of
    seconds since 00:00:00 on January 1, 1970" value. The fromString()
    function returns a ExtDateTime given a string and a date format
    which is used to interpret the date within the string.

    The date() and time() functions provide access to the date and
    time parts of the datetime. The same information is provided in
    textual format by the toString() function.

    ExtDateTime provides a full set of operators to compare two
    ExtDateTime objects where smaller means earlier and larger means
    later.

    You can increment (or decrement) a datetime by a given number of
    seconds using addSecs() or days using addDays(). Similarly you can
    use addMonths() and addYears(). The daysTo() function returns the
    number of days between two datetimes, and secsTo() returns the
    number of seconds between two datetimes.

    The range of a datetime object is constrained to the ranges of the
    ExtDate and QTime objects which it embodies.

    \sa ExtDate QTime ExtDateTimeEdit
*/


/*!
    \fn ExtDateTime::ExtDateTime()

    Constructs a null datetime (i.e. null date and null time). A null
    datetime is invalid, since the date is invalid.

    \sa isValid()
*/


/*!
    Constructs a datetime with date \a date and null (but valid) time
    (00:00:00.000).
*/

ExtDateTime::ExtDateTime( const ExtDate &date )
    : d(date)
{
}

/*!
    Constructs a datetime with date \a date and time \a time.
*/

ExtDateTime::ExtDateTime( const ExtDate &date, const QTime &time )
    : d(date), t(time)
{
}


/*!
    \fn bool ExtDateTime::isNull() const

    Returns TRUE if both the date and the time are null; otherwise
    returns FALSE. A null datetime is invalid.

    \sa ExtDate::isNull(), QTime::isNull()
*/

/*!
    \fn bool ExtDateTime::isValid() const

    Returns TRUE if both the date and the time are valid; otherwise
    returns FALSE.

    \sa ExtDate::isValid(), QTime::isValid()
*/

/*!
    \fn ExtDate ExtDateTime::date() const

    Returns the date part of the datetime.

    \sa setDate(), time()
*/

/*!
    \fn QTime ExtDateTime::time() const

    Returns the time part of the datetime.

    \sa setTime(), date()
*/

/*!
    \fn void ExtDateTime::setDate( const ExtDate &date )

    Sets the date part of this datetime to \a date.

    \sa date(), setTime()
*/

/*!
    \fn void ExtDateTime::setTime( const QTime &time )

    Sets the time part of this datetime to \a time.

    \sa time(), setDate()
*/


/*!
    Returns the datetime as the number of seconds that have passed
    since 1970-01-01T00:00:00, Coordinated Universal Time (UTC).

    On systems that do not support timezones, this function will
    behave as if local time were UTC.

    \sa setTime_t()
*/

uint ExtDateTime::toTime_t() const
{
    tm brokenDown;
    brokenDown.tm_sec = t.second();
    brokenDown.tm_min = t.minute();
    brokenDown.tm_hour = t.hour();
    brokenDown.tm_mday = d.day();
    brokenDown.tm_mon = d.month() - 1;
    brokenDown.tm_year = d.year() - 1900;
    brokenDown.tm_isdst = -1;
    int secsSince1Jan1970UTC = (int) mktime( &brokenDown );
    if ( secsSince1Jan1970UTC < -1 )
	secsSince1Jan1970UTC = -1;
    return (uint) secsSince1Jan1970UTC;
}

/*!
    \overload

    Convenience function that sets the date and time to local time
    based on the given UTC time.
*/

void ExtDateTime::setTime_t( uint secsSince1Jan1970UTC )
{
    setTime_t( secsSince1Jan1970UTC, Qt::LocalTime );
}

/*!
    Sets the date and time to \a ts time (\c Qt::LocalTime or \c
    Qt::UTC) given the number of seconds that have passed since
    1970-01-01T00:00:00, Coordinated Universal Time (UTC). On systems
    that do not support timezones this function will behave as if
    local time were UTC.

    On Windows, only a subset of \a secsSince1Jan1970UTC values are
    supported, as Windows starts counting from 1980.

    \sa toTime_t()
*/
void ExtDateTime::setTime_t( uint secsSince1Jan1970UTC, Qt::TimeSpec ts )
{
    time_t tmp = (time_t) secsSince1Jan1970UTC;
    tm *brokenDown = 0;

#if defined(Q_OS_UNIX) && defined(QT_THREAD_SUPPORT) && defined(_POSIX_THREAD_SAFE_FUNCTIONS)
    // posix compliant system
    // use the reentrant versions of localtime() and gmtime() where available
    tm res;
    if ( ts == Qt::LocalTime )
	brokenDown = localtime_r( &tmp, &res );
    if ( !brokenDown ) {
	brokenDown = gmtime_r( &tmp, &res );
	if ( !brokenDown ) {
	    d.setJD( ExtDate::GregorianToJD( 1970, 1, 1 ) );
	    t.setHMS(0,0,0);
			//	    t.ds = 0;
	    return;
	}
    }
#else
    if ( ts == Qt::LocalTime )
	brokenDown = localtime( &tmp );
    if ( !brokenDown ) {
	brokenDown = gmtime( &tmp );
	if ( !brokenDown ) {
	    d.setJD( ExtDate::GregorianToJD( 1970, 1, 1 ) );
//	    t.ds = 0;
	    t.setHMS(0,0,0);
			return;
	}
    }
#endif

    d.setJD( ExtDate::GregorianToJD( brokenDown->tm_year + 1900,
				     brokenDown->tm_mon + 1,
				     brokenDown->tm_mday ) );
    t.setHMS( brokenDown->tm_hour, brokenDown->tm_min, brokenDown->tm_sec );
//		t.ds = MSECS_PER_HOUR * brokenDown->tm_hour +
//	   MSECS_PER_MIN * brokenDown->tm_min +
//	   1000 * brokenDown->tm_sec;
}
#ifndef QT_NO_DATESTRING
#ifndef QT_NO_SPRINTF
/*!
    \overload

    Returns the datetime as a string. The \a f parameter determines
    the format of the string.

    If \a f is \c Qt::TextDate, the string format is "Wed May 20
    03:40:13 1998" (using ExtDate::shortDayName(), ExtDate::shortMonthName(),
    and QTime::toString() to generate the string, so the day and month
    names will have localized names).

    If \a f is \c Qt::ISODate, the string format corresponds to the
    ISO 8601 extended specification for representations of dates and
    times, which is YYYY-MM-DDTHH:MM:SS.

    If \a f is \c Qt::LocalDate, the string format depends on the
    locale settings of the system.

    If the format \a f is invalid or the datetime is invalid, toString()
    returns a null string.

    \sa ExtDate::toString() QTime::toString()
*/

QString ExtDateTime::toString( Qt::DateFormat f ) const
{
    if ( !isValid() )
	return QString::null;

    if ( f == Qt::ISODate ) {
	return d.toString( Qt::ISODate ) + "T" + t.toString( Qt::ISODate );
    }
#ifndef QT_NO_TEXTDATE
    else if ( f == Qt::TextDate ) {
#ifndef Q_WS_WIN
	QString buf = d.shortDayName( d.dayOfWeek() );
	buf += ' ';
	buf += d.shortMonthName( d.month() );
	buf += ' ';
	buf += QString().setNum( d.day() );
	buf += ' ';
#else
	QString buf;
	QString winstr;
	QT_WA( {
	    TCHAR out[255];
	    GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_ILDATE, out, 255 );
	    winstr = QString::fromUcs2( (ushort*)out );
	} , {
	    char out[255];
	    GetLocaleInfoA( LOCALE_USER_DEFAULT, LOCALE_ILDATE, (char*)&out, 255 );
	    winstr = QString::fromLocal8Bit( out );
	} );
	switch ( winstr.toInt() ) {
	case 1:
	    buf = d.shortDayName( d.dayOfWeek() ) + " " + QString().setNum( d.day() ) + ". " + d.shortMonthName( d.month() ) + " ";
	    break;
	default:
	    buf = d.shortDayName( d.dayOfWeek() ) + " " + d.shortMonthName( d.month() ) + " " + QString().setNum( d.day() ) + " ";
	    break;
	}
#endif
	buf += t.toString();
	buf += ' ';
	buf += QString().setNum( d.year() );
	return buf;
    }
#endif
    else if ( f == Qt::LocalDate ) {
	return d.toString( Qt::LocalDate ) + " " + t.toString( Qt::LocalDate );
    }
    return QString::null;
}
#endif

/*!
    Returns the datetime as a string. The \a format parameter
    determines the format of the result string.

    These expressions may be used for the date:

    \table
    \header \i Expression \i Output
    \row \i d \i the day as number without a leading zero (1-31)
    \row \i dd \i the day as number with a leading zero (01-31)
    \row \i ddd
	    \i the abbreviated localized day name (e.g. 'Mon'..'Sun').
	    Uses ExtDate::shortDayName().
    \row \i dddd
	    \i the long localized day name (e.g. 'Monday'..'Sunday').
	    Uses ExtDate::longDayName().
    \row \i M \i the month as number without a leading zero (1-12)
    \row \i MM \i the month as number with a leading zero (01-12)
    \row \i MMM
	    \i the abbreviated localized month name (e.g. 'Jan'..'Dec').
	    Uses ExtDate::shortMonthName().
    \row \i MMMM
	    \i the long localized month name (e.g. 'January'..'December').
	    Uses ExtDate::longMonthName().
    \row \i yy \i the year as two digit number (00-99)
    \row \i yyyy \i the year as four digit number (1752-8000)
    \endtable

    These expressions may be used for the time:

    \table
    \header \i Expression \i Output
    \row \i h
	    \i the hour without a leading zero (0..23 or 1..12 if AM/PM display)
    \row \i hh
	    \i the hour with a leading zero (00..23 or 01..12 if AM/PM display)
    \row \i m \i the minute without a leading zero (0..59)
    \row \i mm \i the minute with a leading zero (00..59)
    \row \i s \i the second whithout a leading zero (0..59)
    \row \i ss \i the second whith a leading zero (00..59)
    \row \i z \i the milliseconds without leading zeroes (0..999)
    \row \i zzz \i the milliseconds with leading zeroes (000..999)
    \row \i AP
	    \i use AM/PM display. \e AP will be replaced by either "AM" or "PM".
    \row \i ap
	    \i use am/pm display. \e ap will be replaced by either "am" or "pm".
    \endtable

    All other input characters will be ignored.

    Example format strings (assumed that the ExtDateTime is
    21<small><sup>st</sup></small> May 2001 14:13:09)

    \table
    \header \i Format \i Result
    \row \i dd.MM.yyyy	    \i11 21.05.2001
    \row \i ddd MMMM d yy   \i11 Tue May 21 01
    \row \i hh:mm:ss.zzz    \i11 14:13:09.042
    \row \i h:m:s ap	    \i11 2:13:9 pm
    \endtable

    If the datetime is an invalid datetime, then QString::null will be returned.

    \sa ExtDate::toString() QTime::toString()
*/
QString ExtDateTime::toString( const QString& format ) const
{
    return fmtDateTime( format, &t, &d );
}
#endif //QT_NO_DATESTRING

/*!
    Returns a ExtDateTime object containing a datetime \a ndays days
    later than the datetime of this object (or earlier if \a ndays is
    negative).

    \sa daysTo(), addMonths(), addYears(), addSecs()
*/

ExtDateTime ExtDateTime::addDays( int ndays ) const
{
    return ExtDateTime( d.addDays(ndays), t );
}

/*!
    Returns a ExtDateTime object containing a datetime \a nmonths months
    later than the datetime of this object (or earlier if \a nmonths
    is negative).

    \sa daysTo(), addDays(), addYears(), addSecs()
*/

ExtDateTime ExtDateTime::addMonths( int nmonths ) const
{
    return ExtDateTime( d.addMonths(nmonths), t );
}

/*!
    Returns a ExtDateTime object containing a datetime \a nyears years
    later than the datetime of this object (or earlier if \a nyears is
    negative).

    \sa daysTo(), addDays(), addMonths(), addSecs()
*/

ExtDateTime ExtDateTime::addYears( int nyears ) const
{
    return ExtDateTime( d.addYears(nyears), t );
}

/*!
    Returns a ExtDateTime object containing a datetime \a nsecs seconds
    later than the datetime of this object (or earlier if \a nsecs is
    negative).

    \sa secsTo(), addDays(), addMonths(), addYears()
*/

ExtDateTime ExtDateTime::addSecs( int nsecs ) const
{
	long int dd = d.jd();
	int tt = MSECS_PER_HOUR*t.hour() + MSECS_PER_MIN*t.minute() + 1000*t.second() + t.msec();
	tt += nsecs*1000;

	while ( tt < 0 ) {
		tt += MSECS_PER_DAY;
		--dd;
	}

	while ( tt > MSECS_PER_DAY ) {
		tt -= MSECS_PER_DAY;
		++dd;
	}

	ExtDateTime ret;
	ret.setTime( QTime().addMSecs( tt ) );
	ret.setDate( ExtDate( dd ) );

	return ret;
}

/*!
    Returns the number of days from this datetime to \a dt (which is
    negative if \a dt is earlier than this datetime).

    \sa addDays(), secsTo()
*/

int ExtDateTime::daysTo( const ExtDateTime &dt ) const
{
    return d.daysTo( dt.d );
}

/*!
    Returns the number of seconds from this datetime to \a dt (which
    is negative if \a dt is earlier than this datetime).

    Example:
    \code
    ExtDateTime dt = ExtDateTime::currentDateTime();
    ExtDateTime xmas( ExtDate(dt.date().year(),12,24), QTime(17,00) );
    qDebug( "There are %d seconds to Christmas", dt.secsTo(xmas) );
    \endcode

    \sa addSecs(), daysTo(), QTime::secsTo()
*/

int ExtDateTime::secsTo( const ExtDateTime &dt ) const
{
    return t.secsTo(dt.t) + d.daysTo(dt.d)*SECS_PER_DAY;
}


/*!
    Returns TRUE if this datetime is equal to \a dt; otherwise returns FALSE.

    \sa operator!=()
*/

bool ExtDateTime::operator==( const ExtDateTime &dt ) const
{
    return  t == dt.t && d == dt.d;
}

/*!
    Returns TRUE if this datetime is different from \a dt; otherwise
    returns FALSE.

    \sa operator==()
*/

bool ExtDateTime::operator!=( const ExtDateTime &dt ) const
{
    return  t != dt.t || d != dt.d;
}

/*!
    Returns TRUE if this datetime is earlier than \a dt; otherwise
    returns FALSE.
*/

bool ExtDateTime::operator<( const ExtDateTime &dt ) const
{
    if ( d < dt.d )
	return TRUE;
    return d == dt.d ? t < dt.t : FALSE;
}

/*!
    Returns TRUE if this datetime is earlier than or equal to \a dt;
    otherwise returns FALSE.
*/

bool ExtDateTime::operator<=( const ExtDateTime &dt ) const
{
    if ( d < dt.d )
	return TRUE;
    return d == dt.d ? t <= dt.t : FALSE;
}

/*!
    Returns TRUE if this datetime is later than \a dt; otherwise
    returns FALSE.
*/

bool ExtDateTime::operator>( const ExtDateTime &dt ) const
{
    if ( d > dt.d )
	return TRUE;
    return d == dt.d ? t > dt.t : FALSE;
}

/*!
    Returns TRUE if this datetime is later than or equal to \a dt;
    otherwise returns FALSE.
*/

bool ExtDateTime::operator>=( const ExtDateTime &dt ) const
{
    if ( d > dt.d )
	return TRUE;
    return d == dt.d ? t >= dt.t : FALSE;
}

/*!
    \overload

    Returns the current datetime, as reported by the system clock.

    \sa ExtDate::currentDate(), QTime::currentTime()
*/

ExtDateTime ExtDateTime::currentDateTime()
{
    return currentDateTime( Qt::LocalTime );
}

/*!
  Returns the current datetime, as reported by the system clock, for the
  TimeSpec \a ts. The default TimeSpec is LocalTime.

  \sa ExtDate::currentDate(), QTime::currentTime(), Qt::TimeSpec
*/

ExtDateTime ExtDateTime::currentDateTime( Qt::TimeSpec ts )
{
    ExtDateTime dt;
    dt.setDate( ExtDate::currentDate(ts) );
    QTime t = t.currentTime(ts);
    if ( t.hour()==0 && t.minute()==0 )         // midnight or right after?
        dt.setDate( ExtDate::currentDate(ts) ); // fetch date again
    dt.setTime( t );
    return dt;
}

#ifndef QT_NO_DATESTRING
/*!
    Returns the ExtDateTime represented by the string \a s, using the
    format \a f, or an invalid datetime if this is not possible.

    Note for \c Qt::TextDate: It is recommended that you use the
    English short month names (e.g. "Jan"). Although localized month
    names can also be used, they depend on the user's locale settings.

    \warning Note that \c Qt::LocalDate cannot be used here.
*/
ExtDateTime ExtDateTime::fromString( const QString& s, Qt::DateFormat f )
{
    if ( ( s.isEmpty() ) || ( f == Qt::LocalDate ) ) {
#if defined(QT_CHECK_RANGE)
	qWarning( "ExtDateTime::fromString: Parameter out of range" );
#endif
	ExtDateTime dt;
	dt.d.setJD( INVALID_DAY );
	return dt;
    }
    if ( f == Qt::ISODate ) {
	return ExtDateTime( ExtDate::fromString( s.mid(0,10), Qt::ISODate ),
			  QTime::fromString( s.mid(11), Qt::ISODate ) );
    }
#if !defined(QT_NO_REGEXP) && !defined(QT_NO_TEXTDATE)
    else if ( f == Qt::TextDate ) {
	QString monthName( s.mid( 4, 3 ) );
	int month = -1;
	// Assume that English monthnames are the default
	for ( int i = 0; i < 12; ++i ) {
	    if ( monthName == ExtDate::m_shortMonthNames[i] ) {
		month = i + 1;
		break;
	    }
	}
	// If English names can't be found, search the localized ones
	if ( month == -1 ) {
	    for ( int i = 1; i <= 12; ++i ) {
		if ( monthName == ExtDate::shortMonthName( i ) ) {
		    month = i;
		    break;
		}
	    }
	}
#if defined(QT_CHECK_RANGE)
	if ( month < 1 || month > 12 ) {
	    qWarning( "ExtDateTime::fromString: Parameter out of range" );
	    ExtDateTime dt;
	    dt.d.setJD( INVALID_DAY );
	    return dt;
	}
#endif
	int day = s.mid( 8, 2 ).simplifyWhiteSpace().toInt();
	int year = s.right( 4 ).toInt();
	ExtDate date( year, month, day );
	QTime time;
	int hour, minute, second;
	int pivot = s.find( QRegExp(QString::fromLatin1("[0-9][0-9]:[0-9][0-9]:[0-9][0-9]")) );
	if ( pivot != -1 ) {
	    hour = s.mid( pivot, 2 ).toInt();
	    minute = s.mid( pivot+3, 2 ).toInt();
	    second = s.mid( pivot+6, 2 ).toInt();
	    time.setHMS( hour, minute, second );
	}
	return ExtDateTime( date, time );
    }
#endif //QT_NO_REGEXP
    return ExtDateTime();
}
#endif //QT_NO_DATESTRING






#ifndef QT_NO_DATASTREAM
Q_EXPORT QDataStream &operator<<( QDataStream & ostream, const ExtDate & date)
{
	return ostream << (Q_UINT32)(date.jd());
}

Q_EXPORT QDataStream &operator>>( QDataStream & ostream, ExtDate & date)
{
	Q_UINT32 julday;
	ostream >> julday;
	date.setJD( julday );
	return ostream;
}

Q_EXPORT QDataStream &operator<<( QDataStream & ostream, const ExtDateTime & dt)
{
	return ostream << dt.d << dt.t;
}

Q_EXPORT QDataStream &operator>>( QDataStream & ostream, ExtDateTime & dt)
{
	ostream >> dt.d >> dt.t;
	return ostream;
}

#endif // QT_NO_DATASTREAM
