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
#include <QHBoxLayout>
#include <QKeyEvent>
#include <kglobal.h>
#include <klocale.h>
#include "extdatetime.h"

#include "extdatetimeedit.h"

ExtDateEdit::ExtDateEdit( const ExtDate &d, QWidget *parent ) 
: QSpinBox( parent ), ActiveField(0), m_Date(d) {
	setRange( -20000000, 20000000 ); //range of Julian Days

	//Set the date format to be the Locale's short date format, except 
	//always use full years instead of two-digit years
	m_DateFormat = KGlobal::locale()->dateFormatShort();
	m_DateFormat.replace( "y", "Y" );
}

ExtDateEdit::ExtDateEdit( int jd, QWidget *parent ) {
	ExtDateEdit( ExtDate(jd), parent );
}

ExtDateEdit::~ExtDateEdit() {
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

QValidator::State ExtDateEdit::validate( QString &input, int &pos ) {
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

void ExtDateEdit::paintEvent( QPaintEvent *e ) {
	QSpinBox::paintEvent( e );
}

void ExtDateEdit::keyPressEvent( QKeyEvent *e ) {
	e->ignore();
}

ExtDateTimeEdit::ExtDateTimeEdit( const ExtDateTime &dt, QWidget *parent )
: QWidget( parent ) {
	QHBoxLayout *hlay = new QHBoxLayout( this );
	m_DateEdit = new ExtDateEdit( dt.date(), parent );
	m_TimeEdit = new QTimeEdit( dt.time(), parent );

	hlay->addWidget( m_DateEdit );
	hlay->addWidget( m_TimeEdit );
}

ExtDateTimeEdit::ExtDateTimeEdit( const ExtDate &d, const QTime &t, QWidget *parent ) {
	ExtDateTimeEdit( ExtDateTime( d, t ), parent );
}

ExtDateTimeEdit::~ExtDateTimeEdit() {
}

#include "extdatetimeedit.moc"
