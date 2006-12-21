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

#include <libkdeedu_extdate_export.h>

/**
	*@class ExtDateEdit
	*@short provides a spinbox widget for entering an extended date.
	*
	*The date is shown using a slight modification of the user's 
	*localized dateFormatShort, in which the day, month and year fields 
	*are shown with some kind of delimiter ("/", "-", etc).  The 
	*modification is that two-digit years are disallowed, so if the 
	*user's dateFormatShort uses a two-digit year, this will be changed 
	*to show all the year's digits (but only for the purposes of this 
	*widget, of course).
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
	*@see ExtDateTimeEdit
	*@author Jason Harris
	*@version 1.0
	*/

#include <QSpinBox>
#include <QFrame>
#include <QLineEdit>
#include <QTime>
#include <QTimeEdit>
#include "extdatetime.h"
#include <kdemacros.h>

class EXTDATE_EXPORT ExtDateEdit : public QSpinBox {
	Q_OBJECT

	public:
/**
	*@short Default constructor.  Creates an ExtDateEdit displaying the 
	*given date.
	*@p d the date to display (default is current system time)
	*@p parent pointer to the parent widget (default: 0)
	*/
		ExtDateEdit( const ExtDate &date = ExtDate::currentDate(), QWidget *parent = 0 );

/**
	*@short Constructor.  Creates an ExtDateEdit displaying the 
	*given date.
	*
	*Differs from the above function only in the data type of its argument.
	*@p jd the date to display (given as an integer representing the Julian Day)
	*@p parent pointer to the parent widget (default: 0)
	*/
		ExtDateEdit( int jd, QWidget *parent = 0 );

/**
	*@short Constructor for UI files (contains only a parent widget argument)
	*/
	ExtDateEdit( QWidget *parent );

/**
	*@short Default destructor.  Empty.
	*/
		~ExtDateEdit();

/**
	*@short Modifies the display date by the given number of steps.
	*
	*Checks to see which date field (Day/Month/Year) is highlighted, 
	*and modifies the highlighted value by the given number of steps.
	*@note Overloaded from QAbstractSpinBox.
	*@see activeField()
	*@p steps the number to add to the current day/month/year
	*/
		void stepBy( int steps );

/**
	*@short Ensures that the text is a valid ExtDate.
	*
	*@note Overloaded from QAbstractSpinBox; called internally by 
	*the baseclass.
	*@p input reference to the text to be validated 
	*(typically the text displayed in the spinbox)
	*@p pos the position in the string at which to begin checking
	*@return QValidator::Acceptable if the string is a valid ExtDate; 
	*otherwise return QValidator::Invalid
	*/
		QValidator::State validate( QString &input, int &pos ) const;

/**
	*@return the internal ExtDate value
	*@see setDate()
	*/
		ExtDate date() const { return m_Date; }
/**
	*@short set the internal ExtDate value, and display the new 
	*date in the spinbox.
	*
	*@p date reference to the new ExtDate
	*@see date()
	*/
		void setDate( const ExtDate &date ) { m_Date = date; setValue( m_Date.jd() ); }

/**
	*@return the currently-active Date field (Day=0; Month=1; Year=2)
	*@see setActiveField()
	*/
		int activeField() const { return ActiveField; }
/**
	*@short set the currently-active Date field 
	*@p i The field to be activated (Day=0; Month=1; Year=2)
	*@note This function does not change highlighting in the GUI; 
	*to sync the highlighting with the value of ActiveField, use
	*highlightActiveField().
	*@see highlightActiveField()
	*/
		void setActiveField( int i ) { ActiveField = i; }

/**
	*@short highlight the currently-active Date field in the spinbox
	*/
		void highlightActiveField();

/**
	*@short create a keyPressEvent, as if the given key had been pressed.
	*@p k the Qt key value for which a keyPressEvent is to be simulated
	*@see edLineEdit::keyPressEvent()
	*@note (why isn't this in edLineEdit?)
	*/
		void invokeKey( Qt::Key k );

	signals:
		void dateChanged( const ExtDate &d );

	protected:
/**
	*@short convert the spinbox internal value (a Julian Day integer) 
	*to a date string to be displayed in the spinbox
	*@p v the internal JD value to be converted.
	*@note Overloaded from QSpinBox.  Called internally.
	*@see valueFromText()
	*/
		QString textFromValue( int v ) const;

/**
	*@short convert the string displayed in the spinbox to its 
	*internal representation (a Julian Day integer) 
	*@p text reference to the date string to be converted to a JD integer
	*@note Overloaded from QSpinBox.  Called internally.
	*@see textFromValue()
	*/
		int valueFromText( const QString &text ) const;

/**
	*@short Highlight the active Date field (Day/Month/Year) when the 
	*spinbox gains input focus.
	*@p e pointer to the QFocusEvent object
	*@note Overloaded from QSpinBox.  Calls QSpinBox::focusInEvent(), and 
	*highlights the active date field.
	*@see highlightActiveField()
	*/
		void focusInEvent( QFocusEvent *e );

/**
	*@short Advance the active date field forward or backward.
	*
	*Changes the "active date field" to the next or previous date field.
	*If already at the first or last field, then 
	*QSpinBox::focusNextPrevChild() is called, which will move input
	*focus to the next/previous widget.
	*@note Overloaded from QWidget.  Called internally when Tab/Shift+Tab
	*or Left/Right arrows are pressed.
	*@p next if true, advance to the next field or widget.  Otherwise, 
	*advance to the previous field or widget.
	*/
		bool focusNextPrevChild(bool next);

	private slots:
/**
	*@short Simply calls highlightActiveField()
	*@note connected to the valueChanged() Signal, to make sure 
	*that highlighting is persistent.
	*/
		void slotRefreshHighlight();

		void slotEmitDateChanged();

	public:
/**
	*@short Convert the KDE date format for internal use.
	*
	*The KDE date format string (e.g., "%Y-%m-%d" does not reflect the 
	*number of characters in the date string.  Specifically, while 
	*we use a restricted subset of KDE formats that guarantees two-digit
	*Day and Month fields, the number of digits in the Year field depends 
	*on the date being displayed.  This function simply converts the date 
	*format string to one that reflects the correct number of digits
	*(e.g., "YYYY-MM-DD").  
	*
	*This function is used internally to properly highlight the active 
	*field, and to process mouse and key events correctly.
	*/
		QString simpleDateFormat();

	private:
	/**
	 *@short Initialize the ExtDate edit.
	 *@internal
	 *@note This function is called in each constructor.
	 */
		void init( const ExtDate &d );

		uchar ActiveField;  // 0==day; 1==month; 2==year
		ExtDate m_Date;
		QString m_DateFormat;
};

/**
	*@class ExtDateTimeEdit
	*@short provides a pair of spinbox widgets, one to set the date, 
	*one to set the time.
	*
	*@author Jason Harris
	*@version 1.0
	*/
class EXTDATE_EXPORT ExtDateTimeEdit : public QFrame {
	Q_OBJECT

	public:
/**
	*@short Default Constructor. Displays the given Date and Time.
	*@p dt reference to an ExtDateTime whose date and time values are 
	*to be displayed (defaults to the system's current date and time)
	*@p p pointer to the parent widget (default: 0)
	*/
	ExtDateTimeEdit( const ExtDateTime &dt = ExtDateTime::currentDateTime(), QWidget *p=0 );

/**
	*@short Constructor. Displays the given Date and Time.
	*
	*This is essentially identical to the above function, differing only in 
	*the type of its arguments.
	*@p d reference to an ExtDate which is to be displayed in the date box
	*@p t reference to a QTime which is to be displayed in the time box
	*@p p pointer to the parent widget (default: 0)
	*/
	ExtDateTimeEdit( const ExtDate &date, const QTime &time, QWidget *p=0 );

/**
	*@short Constructor for UI files (contains only a parent widget argument)
	*/
	ExtDateTimeEdit( QWidget *p );

/**
	*@short Default Destructor. Empty.
	*/
		~ExtDateTimeEdit();

/**
	*@return the internal ExtDate value
	*@see setDate()
	*/
		ExtDate date() const { return m_DateEdit->date(); }
/**
	*@short set the internal ExtDate value, and display the new 
	*date in the date spinbox.
	*
	*@p d reference to the new ExtDate
	*@see date()
	*/
		void setDate( const ExtDate &date ) { m_DateEdit->setDate( date ); }

/**
	*@return the internal QTime value
	*@see setTime()
	*/
		QTime time() const { return m_TimeEdit->time(); }
/**
	*@short set the internal QTime value, and display the new 
	*time in the time spinbox.
	*
	*@p t reference to the new QTime
	*@see time()
	*/
		void setTime( const QTime &t ) { m_TimeEdit->setTime( t ); }

/**
	*@return the ExtDateTime value displayed in the date and time boxes
	*@see setDateTime()
	*/
		ExtDateTime dateTime() const { return ExtDateTime( date(), time() ); }
/**
	*@short set the internal ExtDate and QTime values, and display the new 
	*values in their respective spinboxes.
	*
	*@p dt reference to the new ExtDateTime
	*@see dateTime()
	*/
		void setDateTime( const ExtDateTime &dt ) { setDate( dt.date() ); setTime(
dt.time() ); }

	signals:
		void dateTimeChanged( const ExtDateTime &dt );

	private slots:
		void slotEmitDateTimeChanged();

	private:
	/**
	 *@short Initialize the ExtDate edit.
	 *@internal
	 *@note This function is called in each constructor.
	 */
		void init( const ExtDateTime &dt );
		
		QTimeEdit *m_TimeEdit;
		ExtDateEdit *m_DateEdit;
};

/**
	*@class edLineEdit
	*@short A QLineEdit adapted for use by ExtDateEdit.
	*
	*This class simply provides custom keyboard and mouse event handlers
	*needed for the ExtDateEdit widget.
	*@author Jason Harris
	*@version 1.0
	*/
class edLineEdit : public QLineEdit {
	public:
/**
	*@short Default constructor.
	*@p parent pointer to the parent widget; defaults to 0
	*/
		edLineEdit( QWidget *parent=0 );
/**
	*@short Default destructor. Empty.
	*/
		~edLineEdit() {}

	protected:
/**
	*@short Set the active date field based on where in the date 
	*string the mouse was clicked.
	*/
		void mouseReleaseEvent( QMouseEvent * );
/**
	*@short Handle arrow-key press events.
	*
	*Up/Down arrows call ExtDateEdit::stepBy(); 
	*Left/Right arrows invoke a key press of Shift+Tab/Tab, 
	*which automatically calls
	*ExtDateEdit::focusNextPrevChild()
	*/
		void keyPressEvent( QKeyEvent *e );

	private:
		ExtDateEdit *edParent;
};

#endif  //EXTDATETIMEEDIT_H
