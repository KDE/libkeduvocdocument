/***************************************************************************
 *   Copyright (C) 2005      by Martin Pfeiffer <hubipete@gmx.net>         *
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
 ***************************************************************************/

#ifndef LEITNERSYSTEMVIEW_H
#define LEITNERSYSTEMVIEW_H

#include <libkdeedu_core_export.h>

#include <QtGui/QWidget>

class LeitnerSystem;

/**
 * This class displays a given LeitnerSystem on a QWidget
 * It is used to configurate a LeitnerSystem and easily remove
 * and add boxes within a GUI.
 * @author Martin Pfeiffer
 */
class KDEEDUCORE_EXPORT LeitnerSystemView : public QWidget
{
	Q_OBJECT

public:
	/**
	 * The public constructor
	 * @param parent the QWidget that is the parent widget
	 */
	explicit LeitnerSystemView( QWidget* parent = 0 );

    	~LeitnerSystemView();

	/**
	 * Sets the LeitnerSystem to be displayed in the LeitnerSystemView
	 * @param system the LeitnerSystem to be displayed
	 */
    	void setSystem( LeitnerSystem* system );

	/**
	 * Highlight a special box to indicate that this one is selected
	 * @param box the number of the box to be highlighted
	 */
	void highlightBox( int box );

Q_SIGNALS:
	void boxClicked(int box);  //is emitted if the user clicks on a box

protected:
	virtual void paintEvent( QPaintEvent* );
//    	virtual void drawContents(QPainter* p, int clipx, int clipy, int clipw, int cliph);
    	virtual void mousePressEvent(QMouseEvent* e);

private:
	class LeitnerSystemViewPrivate;
	LeitnerSystemViewPrivate* const d;

	Q_DISABLE_COPY( LeitnerSystemView )
};

#endif
