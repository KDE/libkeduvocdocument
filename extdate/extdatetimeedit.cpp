/***************************************************************************
                          extdatetimeedit.cpp  -  K Desktop Planetarium
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

#include <QTime>
#include <QDateTimeEdit>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QKeyEvent>
#include <kglobal.h>
#include <klocale.h>
#include <kdebug.h>

#include "extdatetime.h"

#include "extdatetimeedit.h"

ExtDateEdit::ExtDateEdit( const ExtDate &d, QWidget *parent )
: QSpinBox( parent ) {
	init (d);
}

ExtDateEdit::ExtDateEdit( int jd, QWidget *parent )
 : QSpinBox( parent ) {
	ExtDate ed(jd);
	init( ed );
}

ExtDateEdit::ExtDateEdit( QWidget *p )
 : QSpinBox( p ) {
	init( ExtDate::currentDate() );
}

ExtDateEdit::~ExtDateEdit() {
}

void ExtDateEdit::init( const ExtDate &d ) {
	ActiveField = 0;
	m_Date = d;
	setRange( -20000000, 20000000 ); //range of Julian Days

	//Set the date format to be the Locale's short date format, except:
	//always use full years instead of trimming ti two digits
	//and always use two-digit days and months
	m_DateFormat = KGlobal::locale()->dateFormatShort();
	m_DateFormat.replace( "y", "Y" );
	m_DateFormat.replace( "n", "m" );
	m_DateFormat.replace( "e", "d" );

	//Make sure highlight is persistent when value is changed
	connect( this, SIGNAL( valueChanged( int ) ), this, SLOT( slotEmitDateChanged() ) );
	connect( this, SIGNAL( dateChanged( const ExtDate & ) ), this, SLOT( slotRefreshHighlight() ) );

	edLineEdit *edle = new edLineEdit( this );
	setLineEdit(edle);

	setValue( m_Date.jd() );
	highlightActiveField();
}

void ExtDateEdit::slotEmitDateChanged() {
	emit dateChanged( date() );
}

QString ExtDateEdit::simpleDateFormat() {
	//Convert the KDE date format string (e.g., "%Y-%m-%d") to one 
	//that accurately represents the number of digits in each date 
	//field (e.g., "YYYY-MM-DD").  Note that while the Months and 
	//Days fields will always have two digits, the number of digits 
	//in the Years field depends on the displayed year.
	QString result = m_DateFormat;
	result.replace( "%Y", "YYYY" );
	result.replace( "%m", "MM" );
	result.replace( "%d", "DD" );

	int i=result.indexOf( "Y" );
	int dLength = result.length() - cleanText().length();
	if ( dLength > 0 ) { //the years field must have more than 4 digits
		for ( int j=0; j<dLength; j++ ) result.insert( i, "Y" );
	} else if ( dLength < 0 ) { //the years field has less than 4 digits
		result.remove( i, -1*dLength );
	}

	return result;
}

void ExtDateEdit::highlightActiveField() {
	int iStart(0), iLength(0);
	QString sdf = simpleDateFormat();

	//Pick out the position and length of the currently-active field
	if ( ActiveField == 0 ) { //Days field
		iStart = sdf.indexOf( "D" );
		iLength = 2; //The Days field should always be two digits
	} else if ( ActiveField == 1 ) { //Months field
		iStart = sdf.indexOf( "M" );
		iLength = 2; //The Months field should always be two digits
	} else { //Years field
		iStart = sdf.indexOf( "Y" );
		iLength = sdf.lastIndexOf( "Y" ) - iStart + 1;
	}

	lineEdit()->setCursorPosition( iStart + iLength );
	lineEdit()->setSelection( iStart, iLength );

// 	//DEBUG
// 	kDebug() << "selected text: " << lineEdit()->selectedText() << endl;

}

void ExtDateEdit::slotRefreshHighlight() {
	highlightActiveField();
}

void ExtDateEdit::stepBy( int steps ) {
	switch ( ActiveField ) {
		case 0: //days field
			m_Date = m_Date.addDays( steps );
			break;
		
		case 1: //months field
			m_Date = m_Date.addMonths( steps );
			break;
		
		case 2: //years field
			m_Date = m_Date.addYears( steps );
			break;
	}

	int v = m_Date.jd();
	if      ( v > maximum() ) setValue( maximum() );
	else if ( v < minimum() ) setValue( minimum() );
	else    setValue( v );
}

QValidator::State ExtDateEdit::validate( QString &input, int & ) const {
	if ( ExtDate::fromString( input, m_DateFormat ).isValid() )
		return QValidator::Acceptable;
	else
		return QValidator::Invalid;
}

QString ExtDateEdit::textFromValue( int v ) const {
	return ExtDate( v ).toString( m_DateFormat );
}

int ExtDateEdit::valueFromText( const QString &text ) const {
	ExtDate d = ExtDate::fromString( text, m_DateFormat );
	
	if ( d.isValid() ) 
		return d.jd();
	else 
		return INVALID_DAY;
}

//Warning: this function assumes that the lineEdit() contains 
//no prefix or suffix.  I believe this assumption is always valid, 
//but want to be explicit about this.
bool ExtDateEdit::focusNextPrevChild( bool next ) {
	if ( !focusWidget() ) return false;

	int NewField = ActiveField;
	int pos = lineEdit()->cursorPosition(); //assumes no prefix/suffix!
	int step = ( next ? 1 : -1 );
	
	QString sdf = simpleDateFormat();
	while ( NewField == ActiveField ) {
		pos += step;

		if ( pos >= sdf.length() || pos < 0 )
			return QSpinBox::focusNextPrevChild( next );

		QChar c = sdf.at(pos);

		if ( c == 'D' ) NewField = 0;
		if ( c == 'M' ) NewField = 1;
		if ( c == 'Y' ) NewField = 2;

// 		//DEBUG
// 		kDebug() << pos << "  " << c << "  " << NewField << endl;

	}

	ActiveField = NewField;
	highlightActiveField();
	return true;
}

void ExtDateEdit::invokeKey( Qt::Key k ) {
	QKeyEvent *e = new QKeyEvent( QEvent::KeyPress, k, 0, 0 );
	keyPressEvent( e );
	delete e;
}

void ExtDateEdit::focusInEvent( QFocusEvent *e ) {
// 	//DEBUG
// 	kDebug() << "focusInEvent()" << endl;

	QSpinBox::focusInEvent(e);
	highlightActiveField();
}

ExtDateTimeEdit::ExtDateTimeEdit( const ExtDateTime &dt, QWidget *parent )
: QFrame( parent ) {
	init( dt );
}

ExtDateTimeEdit::ExtDateTimeEdit( const ExtDate &date, const QTime &time, QWidget *parent )
: QFrame( parent ) {
	init( ExtDateTime( date, time ) );
}

ExtDateTimeEdit::ExtDateTimeEdit( QWidget *p )
: QFrame( p ) {
	init( ExtDateTime::currentDateTime() );
}

ExtDateTimeEdit::~ExtDateTimeEdit() {
}

void ExtDateTimeEdit::init( const ExtDateTime &dt ) {
	QHBoxLayout *hlay = new QHBoxLayout( this );
	hlay->setMargin( 0 );
	m_DateEdit = new ExtDateEdit( dt.date(), this );
	m_TimeEdit = new QTimeEdit( dt.time(), this );

	hlay->addWidget( m_DateEdit );
	hlay->addWidget( m_TimeEdit );

	connect( m_DateEdit, SIGNAL( dateChanged( const ExtDate & ) ), this, SLOT( slotEmitDateTimeChanged() ) );
	connect( m_TimeEdit, SIGNAL( timeChanged( const QTime & ) ), this, SLOT( slotEmitDateTimeChanged() ) );
}

void ExtDateTimeEdit::slotEmitDateTimeChanged() {
	emit dateTimeChanged( dateTime() );
}

edLineEdit::edLineEdit( QWidget *parent ) : QLineEdit( parent ) {
	edParent = (ExtDateEdit*)parent;
}

void edLineEdit::mouseReleaseEvent( QMouseEvent * ) {
// 	//DEBUG
// 	kDebug() << "mousePressEvent()" << endl;

	//assumes no prefix/suffix!
	QString sdf = edParent->simpleDateFormat();
	int p = cursorPosition();
	if ( p >= sdf.length() ) p = sdf.length() - 1;
	QChar c = sdf.at( p );
	while ( c != 0 && c != 'D' && c != 'M' && c != 'Y' )
		c = edParent->simpleDateFormat().at( --p );

	if ( c == 'D' ) edParent->setActiveField( 0 );
	else if ( c == 'M' ) edParent->setActiveField( 1 );
	else if ( c == 'Y' ) edParent->setActiveField( 2 );

// 	//DEBUG
// 	kDebug() << "ActiveField = " << edParent->activeField() << endl;

	edParent->highlightActiveField();
}

void edLineEdit::keyPressEvent( QKeyEvent *e ) {
// 	//DEBUG
// 	kDebug() << "keyPressEvent()" << endl;

	switch ( e->key() ) {
		case Qt::Key_Up:
			edParent->stepBy( 1 );
			break;
		case Qt::Key_Down:
			edParent->stepBy( -1 );
			break;
		case Qt::Key_Left:
			edParent->invokeKey( Qt::Key_Backtab );
			break;
		case Qt::Key_Right:
			edParent->invokeKey( Qt::Key_Tab );
			break;
		default:
			e->ignore();
			break;
	}
}

#include "extdatetimeedit.moc"
