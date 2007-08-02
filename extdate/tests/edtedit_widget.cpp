/***************************************************************************
                          edtedit_widget.cpp  -  description
                             -------------------
    begin                : Sun Sept 4 2005
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

#include <QWidget>
#include <QTimeEdit>
#include <QVBoxLayout>
#include <kdebug.h>

#include "../extdatetime.h"
#include "../extdatetimeedit.h"
#include "edtedit_widget.h"

EDTEdit::EDTEdit( QWidget *p=0 ) : KXmlGuiWindow( p ) {
	QWidget *w = new QWidget(this);

	vlay = new QVBoxLayout( w );

	ed = new ExtDateEdit( ExtDate::currentDate(), w );
	edt = new ExtDateTimeEdit( ExtDateTime::currentDateTime(), w );

	vlay->addWidget( ed );
	vlay->addWidget( edt );

	setCentralWidget(w);
}

EDTEdit::~EDTEdit() {
	//Output current date setting on exit
	kDebug() << "ExDateEdit:     " << ed->date().toString();
	kDebug() << "ExDateTimeEdit: " << edt->date().toString();
}

#include "edtedit_widget.moc"
