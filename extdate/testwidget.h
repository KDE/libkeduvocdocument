/***************************************************************************
                          testwidget.cpp  -  description
                             -------------------
    begin                : Sun Apr 11 2004
    copyright            : (C) 2004 by Jason Harris
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

#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <kmainwindow.h>

class KDatePicker;
class KLineEdit;
class KDateWidget;
class ExtDatePicker;
class ExtDateWidget;
class QGridLayout;
class QDate;
class ExtDate;

class TestWidget : public KMainWindow {
	Q_OBJECT
	public:
		TestWidget( QWidget *parent, const char *name );
		~TestWidget() {}

	public slots:
		void slotKDateChanged(QDate);
		void slotExtDateChanged(const ExtDate&);

	private:
		QGridLayout *glay;
		KDatePicker *kdp;
		ExtDatePicker *edp;
		ExtDateWidget *edw;
		KDateWidget *kdw;
		KLineEdit *kdpEdit, *edpEdit;
};

#endif //ifndef TESTWIDGET_H
