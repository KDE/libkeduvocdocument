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

#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>

class LeitnerSystem;


/**
 * This class displays a given LeitnerSystem on a QWidget
 * It is used to configurate a LeitnerSystem and easily remove
 * and add boxes within a GUI.
 * @author Martin Pfeiffer
 */

class LeitnerSystemView : public QWidget
{
	Q_OBJECT
		
public:
	/**
	 * The public constructor
	 * @param parent the QWidget that is the parent widget
	 */
    	LeitnerSystemView( QWidget* parent = 0 );

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
	
signals:
	void boxClicked(int box);		//is emited if the user clicks on a box 

protected:
	virtual void paintEvent( QPaintEvent* );
//    	virtual void drawContents(QPainter* p, int clipx, int clipy, int clipw, int cliph);
    	virtual void mousePressEvent(QMouseEvent* e);

private:
	LeitnerSystem* m_leitnerSystem;		//the system which is shown

	void drawSystem(QPainter*);		//paints the boxes
	void drawConnections(QPainter*);	//paints the arrows between the boxes
	void calculateSize();

	int m_imageY; 				//high border of the images
	int m_distPixmap;
	int m_highlightedBox;			//the box which is currently highlighted
};

#endif
