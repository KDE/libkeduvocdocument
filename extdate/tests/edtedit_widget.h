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

#ifndef EDTEDIT_WIDGET_H
#define EDTEDIT_WIDGET_H

#include <kmainwindow.h>

class QVBoxLayout;
class ExtDate;
class ExtDateEdit;
class ExtDateTimeEdit;

class EDTEdit : public KMainWindow {
	Q_OBJECT
	public:
		EDTEdit( QWidget *parent );
		~EDTEdit();

//	public slots:

	private:
		QVBoxLayout *vlay;
		ExtDateEdit *ed;
		ExtDateTimeEdit *edt;
};

#endif //ifndef EDTEDIT_WIDGET_H
