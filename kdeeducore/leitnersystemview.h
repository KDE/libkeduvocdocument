//
// C++ Interface: leitnersystemview
//
// Description: 
//
//
// Author: Martin Pfeiffer <martin-pfeiffer-bensheim@web.de>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
	void boxClicked(int box);		//is emited if the user clicks on a box 

protected:
	virtual void paintEvent( QPaintEvent* );
//    	virtual void drawContents(QPainter* p, int clipx, int clipy, int clipw, int cliph);
    	virtual void mousePressEvent(QMouseEvent* e);

private:
	class Private;
	Private* const d;

	Q_DISABLE_COPY( LeitnerSystemView )
};

#endif
