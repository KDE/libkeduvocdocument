/***************************************************************************
                          extdatetimeedit.h  -  K Desktop Planetarium
                             -------------------
    begin                : Tue Aug 30 21:50:00 PST 2005
    copyright            : (C) 2005 by Jason Harris
    email                : kstars@30doradus.org
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef EXTDATETIMEEDIT_H
#define EXTDATETIMEEDIT_H

/**
	*@class ExtDateEdit
	*@short provides a spinbox widget for entering an extended date.
	*
	*The date is shown using a slight modification of the user's 
	*localized dateFormatShort, with day, month and year fields shown 
	*with some kind of delimiter ("/", "-", etc).  The modification is 
	*that two-digit years are disallowed, so if the user's 
	*dateFormatShort uses a two-digit year, this will be changed to 
	*show all year digits (but only for the purposes of this widget, 
	*of course).
	*
	*The user can choose which field will be modified with the up/down 
	*buttons or arrow keys by clicking on the desired field.  The 
	*currently-selected field is highlighted in the widget.  The user 
	*can also change fields using the left/right arrow keys.
	*
	*The user can also enter date strings directly, and the widget 
	*will attempt to guide the user by advancing the cursor between 
	*fields and disallowing out-of-range values.
	* 
	*Internally, the ExtDate is represented as an integer Julian Day.
	*The range of allowable values is (arbitrarily) set at +/- 2e7,
	*corresponding approximately to years +/- 55,000.
	*
	*@author Jason Harris
	*@version 1.0
	*/

#include <QSpinBox>
#include <QWidget>
#include <QLineEdit>
class QTime;
class QTimeEdit;
class ExtDate;
class ExtDateTime;

class ExtDateEdit : public QSpinBox {
	Q_OBJECT

	public:
		ExtDateEdit( const ExtDate &d = ExtDate::currentDate(), QWidget *parent = 0 );
		ExtDateEdit( int jd, QWidget *parent = 0 );
		~ExtDateEdit();

		void stepBy( int steps );
		QValidator::State validate( QString &input, int &pos );

		ExtDate date() const { return m_Date; }
		void setDate( const ExtDate &d ) { m_Date = d; }

		int activeField() const { return ActiveField; }
		void setActiveField( int i ) { ActiveField = i; }

		void highlightActiveField();
		QString simpleDateFormat();
		void invokeKey( Qt::Key k );

	protected:
		QString textFromValue( int v ) const;
		int valueFromText( const QString &text ) const;

		void focusInEvent( QFocusEvent *e );

		bool focusNextPrevChild(bool next);

	private slots:
		void slotRefreshHighlight();

	private:
		uchar ActiveField;  // 0==day; 1==month; 2==year
		ExtDate m_Date;
		QString m_DateFormat;
};

/**
	*@class ExtDateTimeEdit
	*@short provides a pair of spinbox widgets, one to set the date, one to set the time.
	*
	*@author Jason Harris
	*@version 1.0
	*/
class ExtDateTimeEdit : public QWidget {
	Q_OBJECT

	public:
		ExtDateTimeEdit( const ExtDateTime &dt = ExtDateTime::currentDateTime(), QWidget *p=0 );
		ExtDateTimeEdit( const ExtDate &d, const QTime &t, QWidget *p=0 );
		~ExtDateTimeEdit();

		ExtDate date() const { return m_DateEdit->date(); }
		void setDate( const ExtDate &d ) { m_DateEdit->setDate( d ); }
		QTime time() const { return m_TimeEdit->time(); }
		void setTime( const QTime &t ) { m_TimeEdit->setTime( t ); }
		ExtDateTime dateTime() const { return ExtDateTime( date(), time() ); }
		void setDateTime( const ExtDateTime &dt ) { setDate( dt.date() ); setTime( dt.time() ); }

	private:
		QTimeEdit *m_TimeEdit;
		ExtDateEdit *m_DateEdit;
};

class edLineEdit : public QLineEdit {
	public:
		edLineEdit( QWidget *parent=0 );
		~edLineEdit() {}

	protected:
		void mouseReleaseEvent( QMouseEvent * );
		void keyPressEvent( QKeyEvent *e );

	private:
		ExtDateEdit *edParent;
};

#endif  //EXTDATETIMEEDIT_H
