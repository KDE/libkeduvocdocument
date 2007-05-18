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

#include "extdatetimeedit.h"
#include "extdatetimeedit_p.h"

#include <QDateTimeEdit>
#include <QTimeEdit>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QKeyEvent>
#include <kglobal.h>
#include <klocale.h>
#include <kdebug.h>

#include "extdatetime.h"


class ExtDateEdit::Private
{
public:
    Private(ExtDateEdit* qq)
        : q( qq )
    {
    }

    // Initialize the ExtDate edit.
    void init( const ExtDate &d );

    // slots
    void slotRefreshHighlight();
    void slotEmitDateChanged();

    ExtDateEdit *q;

    uchar ActiveField;  // 0==day; 1==month; 2==year
    ExtDate m_Date;
    QString m_DateFormat;
};

ExtDateEdit::ExtDateEdit( const ExtDate &date, QWidget *parent )
: QSpinBox( parent ), d(new Private(this)) {
	d->init(date);
}

ExtDateEdit::ExtDateEdit( int jd, QWidget *parent )
 : QSpinBox( parent ), d(new Private(this)) {
	ExtDate ed(jd);
	d->init( ed );
}

ExtDateEdit::ExtDateEdit( QWidget *p )
 : QSpinBox( p ), d(new Private(this)) {
	d->init( ExtDate::currentDate() );
}

ExtDateEdit::~ExtDateEdit() {
	delete d;
}

void ExtDateEdit::Private::init( const ExtDate &d ) {
	ActiveField = 0;
	m_Date = d;
	q->setRange( -20000000, 20000000 ); //range of Julian Days

	//Set the date format to be the Locale's short date format, except:
	//always use full years instead of trimming ti two digits
	//and always use two-digit days and months
	m_DateFormat = KGlobal::locale()->dateFormatShort();
	m_DateFormat.replace( "y", "Y" );
	m_DateFormat.replace( "n", "m" );
	m_DateFormat.replace( "e", "d" );

	//Make sure highlight is persistent when value is changed
	connect( q, SIGNAL( valueChanged( int ) ), q, SLOT( slotEmitDateChanged() ) );
	connect( q, SIGNAL( dateChanged( const ExtDate & ) ), q, SLOT( slotRefreshHighlight() ) );

	edLineEdit *edle = new edLineEdit( q );
	q->setLineEdit(edle);

	q->setValue( m_Date.jd() );
	q->highlightActiveField();
}

void ExtDateEdit::Private::slotEmitDateChanged() {
	emit q->dateChanged( q->date() );
}

QString ExtDateEdit::simpleDateFormat() const {
	//Convert the KDE date format string (e.g., "%Y-%m-%d") to one 
	//that accurately represents the number of digits in each date 
	//field (e.g., "YYYY-MM-DD").  Note that while the Months and 
	//Days fields will always have two digits, the number of digits 
	//in the Years field depends on the displayed year.
	QString result = d->m_DateFormat;
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
	if ( d->ActiveField == 0 ) { //Days field
		iStart = sdf.indexOf( "D" );
		iLength = 2; //The Days field should always be two digits
	} else if ( d->ActiveField == 1 ) { //Months field
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

void ExtDateEdit::Private::slotRefreshHighlight() {
	q->highlightActiveField();
}

void ExtDateEdit::stepBy( int steps ) {
	switch ( d->ActiveField ) {
		case 0: //days field
			d->m_Date = d->m_Date.addDays( steps );
			break;
		
		case 1: //months field
			d->m_Date = d->m_Date.addMonths( steps );
			break;
		
		case 2: //years field
			d->m_Date = d->m_Date.addYears( steps );
			break;
	}

	int v = d->m_Date.jd();
	if      ( v > maximum() ) setValue( maximum() );
	else if ( v < minimum() ) setValue( minimum() );
	else    setValue( v );
}

QValidator::State ExtDateEdit::validate( QString &input, int & ) const {
	if ( ExtDate::fromString( input, d->m_DateFormat ).isValid() )
		return QValidator::Acceptable;
	else
		return QValidator::Invalid;
}

ExtDate ExtDateEdit::date() const {
	return d->m_Date;
}

void ExtDateEdit::setDate( const ExtDate &date ) {
	d->m_Date = date;
	setValue( d->m_Date.jd() );
}

int ExtDateEdit::activeField() const {
	return d->ActiveField;
}

void ExtDateEdit::setActiveField( int i ) {
	d->ActiveField = i;
}

QString ExtDateEdit::textFromValue( int v ) const {
	return ExtDate( v ).toString( d->m_DateFormat );
}

int ExtDateEdit::valueFromText( const QString &text ) const {
	ExtDate date = ExtDate::fromString( text, d->m_DateFormat );
	
	if ( date.isValid() ) 
		return date.jd();
	else 
		return INVALID_DAY;
}

//Warning: this function assumes that the lineEdit() contains 
//no prefix or suffix.  I believe this assumption is always valid, 
//but want to be explicit about this.
bool ExtDateEdit::focusNextPrevChild( bool next ) {
	if ( !focusWidget() ) return false;

	int NewField = d->ActiveField;
	int pos = lineEdit()->cursorPosition(); //assumes no prefix/suffix!
	int step = ( next ? 1 : -1 );
	
	QString sdf = simpleDateFormat();
	while ( NewField == d->ActiveField ) {
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

	d->ActiveField = NewField;
	highlightActiveField();
	return true;
}

void ExtDateEdit::invokeKey( Qt::Key k ) {
	QKeyEvent e( QEvent::KeyPress, k, Qt::NoModifier );
	keyPressEvent( &e );
}

void ExtDateEdit::focusInEvent( QFocusEvent *e ) {
// 	//DEBUG
// 	kDebug() << "focusInEvent()" << endl;

	QSpinBox::focusInEvent(e);
	highlightActiveField();
}

class ExtDateTimeEdit::Private
{
public:
    Private(ExtDateTimeEdit *qq, const ExtDateTime& dt);

    // slots
    void emitDateTimeChanged();

    ExtDateTimeEdit *q;
    QTimeEdit *m_TimeEdit;
    ExtDateEdit *m_DateEdit;
};

ExtDateTimeEdit::ExtDateTimeEdit( const ExtDateTime &dt, QWidget *parent )
   : QFrame( parent ), d( new Private( this, dt ) )
{
}

ExtDateTimeEdit::ExtDateTimeEdit( const ExtDate &date, const QTime &time, QWidget *parent )
   : QFrame( parent ), d( new Private( this, ExtDateTime( date, time ) ) )
{
}

ExtDateTimeEdit::ExtDateTimeEdit( QWidget *parent )
   : QFrame( parent ), d( new Private( this, ExtDateTime::currentDateTime() ) )
{
}

ExtDateTimeEdit::~ExtDateTimeEdit() {
	delete d;
}

ExtDateTimeEdit::Private::Private( ExtDateTimeEdit *qq, const ExtDateTime &dt )
   : q( qq )
{
	QHBoxLayout *hlay = new QHBoxLayout( q );
	hlay->setMargin( 0 );
	m_DateEdit = new ExtDateEdit( dt.date(), q );
	m_TimeEdit = new QTimeEdit( dt.time(), q );

	hlay->addWidget( m_DateEdit );
	hlay->addWidget( m_TimeEdit );

	connect( m_DateEdit, SIGNAL( dateChanged( const ExtDate & ) ), q, SLOT( emitDateTimeChanged() ) );
	connect( m_TimeEdit, SIGNAL( timeChanged( const QTime & ) ), q, SLOT( emitDateTimeChanged() ) );
}

ExtDate ExtDateTimeEdit::date() const
{
	return d->m_DateEdit->date();
}

void ExtDateTimeEdit::setDate( const ExtDate &date )
{
	d->m_DateEdit->setDate( date );
}

QTime ExtDateTimeEdit::time() const
{
	return d->m_TimeEdit->time();
}

void ExtDateTimeEdit::setTime( const QTime &t )
{
	d->m_TimeEdit->setTime( t );
}

ExtDateTime ExtDateTimeEdit::dateTime() const
{
	return ExtDateTime( date(), time() );
}

void ExtDateTimeEdit::setDateTime( const ExtDateTime &dt )
{
	setDate( dt.date() );
	setTime( dt.time() );
}

void ExtDateTimeEdit::Private::emitDateTimeChanged()
{
	emit q->dateTimeChanged( q->dateTime() );
}

edLineEdit::edLineEdit( ExtDateEdit *parent ) : QLineEdit( parent ) {
	edParent = parent;
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
