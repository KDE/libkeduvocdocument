/***************************************************************************

     extdatetime.h
     -------------
     Copyright (c) 2004 by Michel Guitel <michel.guitel@sap.ap-hop-paris.fr>
     Copyright (c) 2004-2006 by Jason Harris <kstars@30doradus.org>
    
     Note: This code was adapted from the Qt classes QDate and QDateTime,
     copyright Trolltech, Inc. and licensed under the GNU GPL.

     If there is ever a need for an LGPL version of these classes, they
     will need to be reimplemented from scratch to avoid violating 
     Trolltech's license.

 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef EXTDATETIME_H
#define EXTDATETIME_H

#include <limits.h>
#include <qstring.h>
#include <qnamespace.h>
#include <qdatetime.h>
#include <kdemacros.h>

#define INVALID_DAY LONG_MIN

class ExtDateTime;

/**
 *@fn test2_unit
 *@short A function used in the unit tests for this class
 *@note there is no reason to use this function outside the unit tests
 *@param y the year  to be tested
 *@param m the month to be tested
 *@param d the day   to be tested
 */
extern	void test2_unit(int y, int m, int d);

/**
 *@class ExtDate
 *@short Extended-range date class
 *
 *ExtDate encapsulates the calendar date.  It is functionally identical 
 *to the Qt class QDate, except that it allows for dates far outside the 
 *range of valid QDates (years 1752-8000).  In fact, there is no limit 
 *to the range of dates possible, but for practical reasons we limit 
 *the range of allowable years to -50000 to 50000.
 *
 *The date is represented internally as the Julian Day number (stored
 *as a long int).  We also store the year, month and day values as 
 *integers.
 *
 *The class provides functions for converting ExtDates to and from 
 *QDates, Julian Days, and string expressions.  It also provides 
 *operators for comparing ExtDate objects.  ExtDates can be manipulated 
 *by setting the Julian Day, or the year, month or day values.  Years, 
 *months and days may also be incremented.
 *
 *@author Michael Guitel, Jason Harris
 */
class KDE_EXPORT ExtDate
{
	public:
	/**
		*@short Default constructor
		*Create an invalid date
		*/
	ExtDate();
	/**
		*@short Constructor
		*Create an ExtDate for the specified calendar date
		*@p y The year of the date to be created
		*@p m The month of the date to be created
		*@p d The day of the date to be created
		*@note The year must be in the range -50000 to 50000.
		*/
	ExtDate( int y, int m, int d );
	/**
		*@short Constructor
		*Create an ExtDate from the specified QDate
		*@p q The QDate representing the date to be created
		*@note Of course, this constructor cannot be used if you 
		*need a date outside the valid Qt range (years 1752-8000).
		*/
	ExtDate( const QDate &q ) { ExtDate( q.year(), q.month(), q.day() ); }
	/**
		*@short Constructor
		*Create an ExtDate for the specified Julian Day.  
		*@p jd the Julian Day of the date to be created
		*@note The Julian Day is a simple count of the number 
		*of days elapsed since January 1st, in the year -4713.
		*/
	ExtDate( long int jd );
	
	/**
		*@return true if the ExtDate is an Null date 
		*@note a Null date is one that was created with the default 
		*constructor.  Null dates are also invalid dates.
		*@sa isValid()
		*/
	bool isNull() const { return m_jd == INVALID_DAY; }
	
	/**
		*@return true if the ExtDate is a valid date.
		*@note A date may be invalid if it is a Null date, if 
		*its year value exceeds the valid range (-50000 to 50000), 
		*or if its month or day values are out of bounds.
		*/
	bool isValid() const;
	
	/**
		*@return the ExtDate, converted to a QDate.
		*@note If the ExtDate is invalid or falls outside the range 
		*of valid QDates (years 1752 to 8000), the returned QDate 
		*will be invalid.
		*/
	QDate qdate() const;
		
	/**
		*@return the year of the ExtDate
		*/
	int year() const { return m_year; }
	/**
		*@return the month of the ExtDate, as an integer between 1 and 12
		*@sa monthName()
		*@sa longMonthName()
		*@sa shortMonthName()
		*/
	int month() const { return m_month; }
	/**
		*@return the day of the ExtDate, as an integer between 1 and 31
		*/
	int day() const { return m_day; }
	/**
		*@return the day-of-the week of the ExtDate, as an integer between 1 and 7
		*@sa dayName()
		*@sa longDayName()
		*@sa shortDayName()
		*/
	int dayOfWeek() const;
	/**
		*@return the ExtDate's position within the current year, as an integer 
		*between 1 and 366.
		*/
	int dayOfYear() const;
	/**
		*@return the number of days in the ExtDate's month, as an integer 
		*between 28 and 31.
		*/
	int daysInMonth() const;
	/**
		*@return the number of days in the ExtDate's year.
		*@note The returned value is either 365, or 366 if it is a leap year.
		*/
	int daysInYear() const;
	/**
		*@return the position within the current year of the week containing 
		*the ExtDate.
		*@p yearNum pointer integer, which is set to the year to which the week 
		*belongs
		*@note It is possible for dates near the end of the year to belong to 
		*the first week of the following year.
		*/
	int weekNumber( int *yearNum=0 ) const;
	/**
		*@return the Julian Day representation of the ExtDate.
		*@note The Julian Day is a simple count of the number of days 
		*elapsed since January 1st, in the year -4713.
		*/
	long int jd() const { return m_jd; }

#ifndef QT_NO_TEXTDATE
#ifndef QT_NO_COMPAT
	/**
		*@return the short name of the ExtDate's month (for example, 
		"Aug", short for "August").
		*@note This function is identical to shortMonthName(), and is 
		*provided for convenience.
		*@sa shortMonthName()
		*/
	static QString monthName( int month ) { return shortMonthName( month ); }
	/**
		*@return the short name of the ExtDate's day-of-week (for example,
		*"Thu", short for "Thursday").
		*@note This function is identical to shortDayName(), and is 
		*provided for convenience.
		*@sa shortDayName()
		*/
	static QString dayName( int weekday ) { return shortDayName( weekday ); }
#endif
	/**
		*@return the short name of the ExtDate's month (for example, 
		"Aug", short for "August").
		*/
	static QString shortMonthName( int month );
	/**
		*@return the short name of the ExtDate's day-of-week (for example,
		*"Thu", short for "Thursday").
		*/
	static QString shortDayName( int weekday );
	/**
		*@return the long name of the ExtDate's month (for example, "August").
		*/
	static QString longMonthName( int month );
	/**
		*@return the long name of the ExtDate's day-of-week (for example,
		*"Thursday").
		*/
	static QString longDayName( int weekday );
#endif //QT_NO_TEXTDATE
#ifndef QT_NO_TEXTSTRING
#if !defined(QT_NO_SPRINTF)
	/**
		*@return a string representation of the ExtDate according to 
		*the given date format.
		*@p f the Qt::DateFormat describing the date string
		*@see Qt::DateFormat
		*/
	QString toString( Qt::DateFormat f = Qt::TextDate ) const;
#endif
	/**
	*@return a string representation of the ExtDate according to 
	*the given date format.
	*@p format a QString describing the date format according to 
	*a subset of the KDE date string specification:
	*
	*Any occurence of the following tokens in the format string 
	*are replaced with the corresponding value:
	*
	*@li %Y The year, including centuries prefix (e.g., "1984")
	*@li %y The year, excluding centuries prefix (e.g., "84")
	*@li %n Numerical month value (e.g., "3" for March)
	*@li %m Numerical month value, two digits (e.g., "03" for March)
	*@li %e Numerical day value (e.g., "3" on March 3rd)
	*@li %d Numerical day value, two digits (e.g., "03" on March 3rd)
	*@li %b Month name, short form (e.g., "Mar" for March)
	*@li %B Month name, long form (e.g., "March")
	*@li %a Weekday name, short form (e.g., "Wed" for Wednesday)
	*@li %A Weekday name, long form (e.g., "Wednesday")
	*
	*All other characters in the format string are left as-is.
	*/
	QString toString( const QString& format ) const;
#endif
	/**
		*@short Set the ExtDate according to the given year, month and day.
		*@p y the year of the date to be set
		*@p m the month of the date to be set
		*@p d the day of the date to be set
		*@return true if the ExtDate is valid
		*/
	bool setYMD( int y, int m, int d );
	/**
		*@short Set the ExtDate acording to the given Julian Day.
		*@p _jd the Julian Day of the date to be set
		*@return true if the ExtDate is valid
		*@note The Julian Day is a simple count of the number of days 
		*elapsed since January 1st, in the year -4713.
		*/
	bool setJD( long int _jd );
	
	/**
		*@return an ExtDate created by adding the given number of days 
		*to this ExtDate object.
		*@p days the number of days to add to the current ExtDate.  The 
		*numbers of days may be negative.
		*@note The current ExtDate object is not affected by this function;
		*the modified ExtDate is provided in the return value.
		*/
	ExtDate addDays( int days ) const;
	/**
		*@return an ExtDate created by adding the given number of months 
		*to this ExtDate object.
		*@p months the number of months to add to the current ExtDate.  The 
		*numbers of months may be negative.
		*@note The current ExtDate object is not affected by this function;
		*the modified ExtDate is provided in the return value.
		*/
	ExtDate addMonths( int months ) const;
	/**
		*@return an ExtDate created by adding the given number of years 
		*to this ExtDate object.
		*@p years the number of years to add to the current ExtDate.  The 
		*numbers of years may be negative.
		*@note The current ExtDate object is not affected by this function;
		*the modified ExtDate is provided in the return value.
		*/
	ExtDate addYears( int years ) const;
	/**
		*@return The number of days between the current ExtDate and the
		*ExtDate provided as an argument.  For example, if the current 
		*ExtDate is March 15, 2006 and the ExtDate given as an argument
		*represents March 18, 2006, then this function wuld return 3.
		*@p d the target date to which the number of days will be counted
		*/
	int daysTo( const ExtDate &d ) const;
	
	/**
		*@return true if the two ExtDates are equal
		*/
	bool operator==( const ExtDate &d ) const { return m_jd == d.jd(); };
	/**
		*@return true if the two ExtDates are not equal
		*/
	bool operator!=( const ExtDate &d ) const { return m_jd != d.jd(); };
	/**
		*@return true if the left-hand ExtDate is earlier than the 
		*right-hand ExtDate
		*/
	bool operator<( const ExtDate &d )  const { return m_jd  < d.jd(); };
	/**
		*@return true if the left-hand ExtDate is earlier than or equal to the 
		*right-hand ExtDate
		*/
	bool operator<=( const ExtDate &d ) const { return m_jd <= d.jd(); };
	/**
		*@return true if the left-hand ExtDate is later than the 
		*right-hand ExtDate
		*/
	bool operator>( const ExtDate &d )  const { return m_jd  > d.jd(); };
	/**
		*@return true if the left-hand ExtDate is later than or equal to the 
		*right-hand ExtDate
		*/
	bool operator>=( const ExtDate &d ) const { return m_jd >= d.jd(); };
	
	/**
		*@return an ExtDate representing the current date, according to the 
		*computer's clock.
		*@p ts a Qt::TimeSpec value that determines whether the date is
		*computed from the Local Time or the Universal Time.
		*@sa Qt::TimeSpec
		*/
	static ExtDate currentDate( Qt::TimeSpec ts = Qt::LocalTime );
#ifndef QT_NO_DATESTRING
	/**
		*@return an ExtDate constructed from the string representation 
		*given as an argument.
		*@p s The string representation of the date
		*@note No sring format is specified, so this function will attempt 
		*both Qt::TextDate and Qt::ISODate formats.
		*/
	static ExtDate fromString( const QString& s );
	/**
		*@return an ExtDate constructed from the string representation 
		*given as an argument.
		*@p s The string representation of the date
		*@p f The Qt::DateFormat with which to interpret the date string
		*@note This function only understands Qt::TextDate and Qt::ISODate
		*formats.  It does not handle Qt::LocalDate.
		*/
	static ExtDate fromString( const QString& s, Qt::DateFormat f);
	/**
		*@return an ExtDate constructed from the string representation 
		*given as an argument.
		*@p s The string representation of the date
		*@p format A string representing the date format, acording to 
		*the KDE date string specification.
		*@see ExtDate::toString( const QString &format )
		*/
	static ExtDate fromString( const QString& s, const QString &format );
#endif
	/**
		*@return true if the date described by the arguments is valid
		*@p y the year of the date to be tested
		*@p m the month of the date to be tested
		*@p d the day of the date to be tested
		*/
	static bool isValid( int y, int m, int d );
	/**
		*@return true if the given year is a leap year.
		*@p year the year to be tested
		*/
	static bool leapYear( int year );
	
	/**
		*@return the Julian Day corresponding to the date described by 
		*the arguments
		*@p y The year of the date to be converted
		*@p m The month of the date to be converted
		*@p d The day of the date to be converted
		*/
	static long int GregorianToJD( int y, int m, int d );
	/**
		*@return the year, month and day corresponding to the given Julian Day
		*@p jd The Julian Day to be converted
		*@p y The date's year is returned as a reference through this argument
		*@p m The date's month is returned as a reference through this argument
		*@p d The date's day is returned as a reference through this argument
		*/
	static void JDToGregorian( long int jd, int &y, int &m, int &d );

private:
	/**
		*@return the position within the year of the date described by the arguments
		*@p y the year of the date
		*@p m the month of the date
		*@p d the day of the date
		*/
	static int dayOfYear(int y, int m, int d);

	long int m_jd;
	int m_year, m_month, m_day;
	static uint m_monthLength[12];
	static uint m_monthOrigin[12];
	static QString m_shortMonthNames[12];
	static QString m_shortDayNames[7];
	static QString m_longMonthNames[12];
	static QString m_longDayNames[7];

	friend class ExtDateTime;

#ifndef QT_NO_DATASTREAM
	friend KDE_EXPORT QDataStream &operator<<( QDataStream &, const ExtDate & );
	friend KDE_EXPORT QDataStream &operator>>( QDataStream &, ExtDate & );
	friend KDE_EXPORT QDataStream &operator<<( QDataStream &, const ExtDateTime & );
	friend KDE_EXPORT QDataStream &operator>>( QDataStream &, ExtDateTime & );
#endif
};

/**
	*@class ExtDateTime
	*@short Extended-range date class
	*
	*ExtDateTime encapsulates the calendar date and time.  It is functionally 
	*identical to the Qt class QDateTime, except that it allows for dates 
	*far outside the range of valid QDates (years 1752-8000).  In fact, there 
	*is no limit to the range of dates posiible, but for practical reasons we 
	*limit the range of allowable years to -50000 to 50000.
	*
	*Internally, an ExtDateTime is simply an ExtDate paired with a QTime, with 
	*very little interaction between these two pieces.
	*
	*@author Michael Guitel, Jason Harris
	*/
class KDE_EXPORT ExtDateTime
{
public:
	/**
		*@short Default constructor.  Creates a null ExtDateTime object.
		*/
	ExtDateTime();
	/**
		*@short Constructor.  Sets the date according to the argument; 
		*the time is set to midnight.
		*@p d The date to be set
		*/
	ExtDateTime( const ExtDate &d );
	/**
		*@short Constructor.  Sets date and time according to the arguments 
		*@p d The date to be set
		*@p t The time to be set
		*/
	ExtDateTime( const ExtDate &d, const QTime &t );
	
	/**
		*@return true if the object specifies a null Date/Time.
		*@note Null Date/Times are also invalid Date/Times
		*@sa isValid()
		*/	
	bool   isNull()	const		{ return d.isNull() && t.isNull(); }
	/**
		*@return true if the both the Date and Time specified in the 
		*object are valid
		*/
	bool   isValid()	const		{ return d.isValid() && t.isValid(); }
	
	/**
		*@return The ExtDate component of the Date/Time
		*/
	ExtDate  date()	const		{ return d; }
	/**
		*@return The QTime component of the Date/Time
		*/
	QTime  time()	const		{ return t; }
	/**
		*@return unsigned int corresponding to the UNIX representation 
		*of the Date/Time.
		*/
	uint   toTime_t()	const;
	/**
		*@short set the ExtDate component to the given argument, leaving the 
		*QTime component at its current value
		*@p date the ExtDate to be set
		*/
	void   setDate( const ExtDate &date ) { d = date; }
	/**
		*@short set the QTime component to the given argument, leaving the 
		*ExtDate component at its current value
		*@p time the QTimee to be set
		*/
	void   setTime( const QTime &time ) { t = time; }
	/**
		*@short set the Date/Time by converting from the given 
		*UNIX time.  
		*@p secs the UNIX time to convert
		*@note UNIX time is the number of seconds since 1970 Jan 1 00:00
		*/
	void   setTime_t( uint secs );
	/**
		*@short set the Date/Time by converting from the given 
		*UNIX time.  
		*@p secs the UNIX time to convert
		*@p ts Specifies whether Universal Time or Local Time should be set.
		*@note UNIX time is the number of seconds since 1970 Jan 1 00:00
		*/
	void   setTime_t( uint secs, Qt::TimeSpec ts);
#ifndef QT_NO_DATESTRING
#ifndef QT_NO_SPRINTF
	/**
		*@return a string representation of the Date/Time according to 
		*the given date format.
		*@p f the Qt::DateFormat describing the date string
		*@sa ExtDate::toString( Qt::DateFormat )
		*/
	QString toString( Qt::DateFormat f = Qt::TextDate )	const;
#endif
	/**
		*@return a string representation of the Date/Time according to 
		*the given date format.
		*@p format string representation of the date/time format, using 
		*the KDE date/time string specification:
		*
		*Any occurence of the following tokens in the format string 
		*are replaced with the corresponding value:
		*
		*@li %Y The year, including centuries prefix (e.g., "1984")
		*@li %y The year, excluding centuries prefix (e.g., "84")
		*@li %n Numerical month value (e.g., "3" for March)
		*@li %m Numerical month value, two digits (e.g., "03" for March)
		*@li %e Numerical day value (e.g., "3" on March 3rd)
		*@li %d Numerical day value, two digits (e.g., "03" on March 3rd)
		*@li %b Month name, short form (e.g., "Mar" for March)
		*@li %B Month name, long form (e.g., "March")
		*@li %a Weekday name, short form (e.g., "Wed" for Wednesday)
		*@li %A Weekday name, long form (e.g., "Wednesday")
		*@li %H Hour in 24h format, 2 digits
		*@li %k Hour in 24h format, 1-2 digits
		*@li %I Hour in 12h format, 2 digits
		*@li %l Hour in 12h format, 1-2 digits
		*@li %M Minute, 2 digits
		*@li %S Seconds, 2 digits
		*@li %p pm/am
		*
		*All other characters in the format string are left as-is.
		*
		*@sa ExtDate::toString( Qt::DateFormat )
		*/
	QString toString( const QString& format ) const;
#endif
	/**
		*@return an ExtDateTime constructed by adding the given number 
		*of days to the current Date/Time.
		*@p days the number of days to add to the current Date/Time.
		*@note the number of days can be negative
		*/
	ExtDateTime addDays( int days )	const;
	/**
		*@return an ExtDateTime constructed by adding the given number 
		*of months to the current Date/Time.
		*@p months the number of months to add to the current Date/Time.
		*@note the number of months can be negative
		*/
	ExtDateTime addMonths( int months )   const;
	/**
		*@return an ExtDateTime constructed by adding the given number 
		*of years to the current Date/Time.
		*@p years the number of years to add to the current Date/Time.
		*@note the number of years can be negative
		*/
	ExtDateTime addYears( int years )     const;
	/**
		*@return an ExtDateTime constructed by adding the given number 
		*of seconds to the current Date/Time.
		*@p seconds the number of days to add to the current Date/Time.
		*@note the number of seconds can be negative
		*/
	ExtDateTime addSecs( int secs )	const;
	/**
		*@return the number of days between the current Date/Time and the 
		*target Date/Time given as the argument.
		*@p dt the target Date/Time 
		*/
	int	   daysTo( const ExtDateTime &dt )	const;
	/**
		*@return the number of seconds between the current Date/Time and the 
		*target Date/Time given as the argument.
		*@p dt the target Date/Time 
		*/
	int	   secsTo( const ExtDateTime &dt )	const;
	
	/**
		*@return true if the two ExtDateTime objects are equal
		*/
	bool   operator==( const ExtDateTime &dt ) const;
	/**
		*@return true if the two ExtDateTime objects are not equal
		*/
	bool   operator!=( const ExtDateTime &dt ) const;
	/**
		*@return true if the left-hand ExtDateTime object is less than 
		*the right-hand ExtDateTime object.
		*/
	bool   operator<( const ExtDateTime &dt )  const;
	/**
		*@return true if the left-hand ExtDateTime object is less than 
		*or equal to the right-hand ExtDateTime object.
		*/
	bool   operator<=( const ExtDateTime &dt ) const;
	/**
		*@return true if the left-hand ExtDateTime object is greater than 
		*the right-hand ExtDateTime object.
		*/
	bool   operator>( const ExtDateTime &dt )  const;
	/**
		*@return true if the left-hand ExtDateTime object is greater than 
		*or equal to the right-hand ExtDateTime object.
		*/
	bool   operator>=( const ExtDateTime &dt ) const;
	
	/**
		*@return Date/Time constructed from the current date and time
		*according to the compter's clock.
		*/
	static ExtDateTime currentDateTime();
	/**
		*@return Date/Time constructed from the current date and time
		*according to the compter's clock.
		*@p ts Specifies whether to interpret the computer time as 
		*Universal Time or Local Time.
		*/
	static ExtDateTime currentDateTime( Qt::TimeSpec ts );
#ifndef QT_NO_DATESTRING
	/**
		*@return ExtDateTime constructed from the given strng representation
		*@p s String representation of the Date/Time
		*/
	static ExtDateTime fromString( const QString& s );
	/**
		*@return ExtDateTime constructed from the given strng representation
		*@p s String representation of the Date/Time
		*@p f Specifies the Date format to use in interpretng the string.
		*/
	static ExtDateTime fromString( const QString& s, Qt::DateFormat f );
#endif
	private:
		ExtDate  d;
		QTime  t;
#ifndef QT_NO_DATASTREAM
	friend KDE_EXPORT QDataStream &operator<<( QDataStream &, const ExtDateTime &);
	friend KDE_EXPORT QDataStream &operator>>( QDataStream &, ExtDateTime & );
#endif
};

/*****************************************************************************
  Date and time stream functions
 *****************************************************************************/

#ifndef QT_NO_DATASTREAM
KDE_EXPORT QDataStream &operator<<( QDataStream &, const ExtDate & );
KDE_EXPORT QDataStream &operator>>( QDataStream &, ExtDate & );
#endif // QT_NO_DATASTREAM

#endif // EXTDATE_H

