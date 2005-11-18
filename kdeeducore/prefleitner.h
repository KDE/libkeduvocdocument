#ifndef PREFLEITNER_H
#define PREFLEITNER_H

/***************************************************************************
 *   Copyright (C) 2005 by Martin Pfeiffer                                 *
 *   hubipete@gmx.net                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 **************************************************************************/

#include "prefleitnerbase.h"

class LeitnerSystemView;
class LeitnerSystem;
class LeitnerBox;

/**
 * This class is a dialogue for configuring a LeitnerSystem
 * @author Martin Pfeiffer <hubipete@gmx.net>
 */
class PrefLeitner : public QDialog, public Ui::PrefLeitnerBase
{
	Q_OBJECT

public:
	/**
	 * The public contructor
	 * @param parent is the parent widget
	 */
	PrefLeitner( QWidget* parent = 0 );

	/**
	 * The public contructur which sets also the LeitnerSystem
	 * @param parent is the parent widget
	 * @param system a pointer to the LeitnerSystem to configure
	 */
	PrefLeitner( QWidget* parent = 0 , LeitnerSystem* system = 0 );

	/**
	 * Sets the LeitnerSystem for the dialogue
	 * @param system a pointer to the LeitnerSystem to configure
	 */
	void setLeitnerSystem( LeitnerSystem* system );

	/**
	 * Get the LeitnerSystem that is set to the dialogue
	 * @return a pointer to the actual set LeitnerSystem
	 */
	LeitnerSystem* system();

public slots:
	void slotCorrectWord( const QString& newBox );
	void slotWrongWord( const QString& newBox );
	void slotBoxName( const QString& newName );
	void slotAddBox();
	void slotDeleteBox();
	void slotDiscard();
	void slotApply();

private slots:
	/**
	 * catches the signal from the view if user clicks on a box
	 */
	void slotBoxClicked( int );
private:
	LeitnerSystemView* m_leitnerSystemView;	//the LeitnerSystemView which shows the selected system
	LeitnerSystem* m_selectedSystem;	//the currently selected system to be changed
	LeitnerBox* m_selectedBox;		//the currently selected box

	void refreshSystemView();		//refresh the LeitnerSystemView
	void newSystem();
};

#endif //PREFLEITNER_H
