/***************************************************************************
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

#ifndef EXTDATETIMEEDIT_P_H
#define EXTDATETIMEEDIT_P_H

#include <QLineEdit>

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
		explicit edLineEdit( ExtDateEdit *parent=0 );
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
