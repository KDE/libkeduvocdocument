//
// C++ Implementation: leitnersystemview
//
// Description:
//
//
// Author: Martin Pfeiffer <martin-pfeiffer-bensheim@web.de>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "leitnersystemview.h"

#include "leitnersystem.h"

#include <QPainter>
#include <QMouseEvent>

#include <kdebug.h>

class LeitnerSystemView::Private
{
public:
	Private( LeitnerSystemView* qq )
	  : q( qq ), m_highlightedBox( -1 )
	{
	}

	LeitnerSystemView *q;

	LeitnerSystem* m_leitnerSystem;		//the system which is shown

	void drawSystem(QPainter*);		//paints the boxes
	void drawConnections(QPainter*);	//paints the arrows between the boxes
	void calculateSize();

	int m_imageY; 				//high border of the images
	int m_distPixmap;
	int m_highlightedBox;			//the box which is currently highlighted
};


LeitnerSystemView::LeitnerSystemView( QWidget* parent )
  : QWidget( parent ), d( new Private( this ) )
{
}


LeitnerSystemView::~LeitnerSystemView()
{
	delete d;
}

void LeitnerSystemView::Private::drawSystem( QPainter* p )
{
	m_imageY = q->height() / 2 - 32;

	//draw the boxes' icons
	for(int i = 0; i < m_leitnerSystem->getNumberOfBoxes(); i++)
	{
		if(i == m_highlightedBox)
		{
			//p->drawPixmap(12 + i * 64 + i*10, m_imageY, KGlobal::iconLoader()->loadIcon("leitnerbox", K3Icon::Panel));
			p->drawRect(12 + i * 64 + i*10, m_imageY,64,64);
			p->fillRect(12 + i * 64 + i*10, m_imageY,64,64, QBrush(Qt::red));
		}
		else
		{	//for each box 74 = 64(pixmap) + 10(distance between two boxes)
			//p->drawPixmap(12 + i * 74, m_imageY, KGlobal::iconLoader()->loadIcon("leitnerbox", K3Icon::Panel));
			p->drawRect(12 + i * 64 + i*10, m_imageY,64,64);
		}
	}
}

void LeitnerSystemView::Private::drawConnections(QPainter* p)
{
	//dist = number of boxes that are in between the two boxes
	//width = width of the rect for the arc
	int dist, width = 0;
	int numberOfBoxes = m_leitnerSystem->getNumberOfBoxes();

	p->setPen( QPen(Qt::green, 2) );

	//paint the connections for the correct word boxes, above the boxes
	for(int i = 0; i < numberOfBoxes; i++)
	{
		if( m_leitnerSystem->correctBoxNumber( i ) != -1 )
		{
			dist = m_leitnerSystem->correctBoxNumber( i ) - i;

			if(dist <= 0)
			{
		        	// (dist*(-1) -1)*64 == for each box in between take 64
		        	// dist*(-1)*10 == the gaps in between
		        	// 2*22; 2*21 == the distances of the endings on the picture
				width = (dist*(-1) -1)*64 + dist*(-1)*10 + 2*22 + 2*21;

				p->drawArc( 12 + (dist+i)*74 + 21, m_imageY-(width/3), width, /*(height()/2 - 12-32) *2*/ width/3*2, 0, 180*16);
			}
			else
			{
				width = (dist-1)*64 + dist*10 + 2*21;
				p->drawArc(12 + i*74 + 21+22 ,m_imageY-(width/3) , width, /*(height()/2 - 12-32) *2*/width/3*2, 0, 180*16);
			}
		}
	}

	//paint the connections for the wrong word boxes, below the boxes
	p->setPen(QPen(Qt::red, 2));

	for(int i = 0; i < numberOfBoxes; i++)
	{
		if( m_leitnerSystem->wrongBoxNumber( i ) != -1 )
		{
			dist = m_leitnerSystem->wrongBoxNumber( i ) - i;

			if(dist <= 0)
			{
				width = (dist*(-1) -1)*64 + dist*(-1)*10 + 2*22 + 2*21;
				p->drawArc(12+ (dist+i)*74 + 21 ,m_imageY+64-width/3 , width, width/3*2 , 180*16, 180*16);
			}
			else
			{
				width = (dist-1)*64 + dist*10 + 2*21;
				p->drawArc(12 + i*74 + 21+22 ,m_imageY+64-width/3 , width, width/3*2, 180*16, 180*16);
			}
		}
	}
}

void LeitnerSystemView::setSystem( LeitnerSystem* leitnersystem )
{
	d->m_leitnerSystem = leitnersystem;

	//calculate the new sizes
	d->calculateSize();
	update();
}

void LeitnerSystemView::highlightBox(int box)
{
	d->m_highlightedBox = box;
	update();
}

void LeitnerSystemView::paintEvent( QPaintEvent* )
{
	QPainter p( this );
	p.eraseRect( 0, 0, width(), height() );

	d->drawSystem( &p );

	d->drawConnections( &p );
}

void LeitnerSystemView::Private::calculateSize()
{
	//margin = 12
	//distance between boxes = 10
	//boxes = 64*64

	int numberOfBoxes = m_leitnerSystem->getNumberOfBoxes();
	QString x;
	int height, dist, tmpMaxC, tmpMaxW;
	tmpMaxC = 0;
	tmpMaxW = 0;
	height = 0;

	for(int i = 0; i < numberOfBoxes; i++)
	{
		if( m_leitnerSystem->correctBoxNumber( i ) != -1 )
		{
			dist = m_leitnerSystem->correctBoxNumber( i ) - i;

			if( abs(dist) >= abs(tmpMaxC) )
				tmpMaxC = dist;
		}

		if( m_leitnerSystem->wrongBoxNumber( i ) != -1 )
		{
			dist = m_leitnerSystem->wrongBoxNumber( i ) - i;

			if( abs(dist) >= abs(tmpMaxW) )
				tmpMaxW = dist;
		}
	}

	if( tmpMaxC <= 0 )
		height += (( abs(tmpMaxC) -1)*64 + abs(tmpMaxC)*10 + 2*22 + 2*21)/3;
	else
		height += ((tmpMaxC-1)*64 + tmpMaxC*10 + 2*21)/3;

	if( tmpMaxW <= 0 )
		height += (( abs(tmpMaxW) -1)*64 + abs(tmpMaxW)*10 + 2*22 + 2*21)/3;
	else
		height += (( tmpMaxW-1)*64 + tmpMaxW*10 + 2*21)/3;

	height += 24+64;

//	resizeContents( numberOfBoxes * 64 + (numberOfBoxes - 1)*10 + 2 * 12, height );
	q->setMinimumSize( numberOfBoxes * 64 + (numberOfBoxes - 1)*10 + 2 * 12, height );
}

void LeitnerSystemView::mousePressEvent(QMouseEvent* e)
{
	kDebug() << "mouseClick" << endl;
	//if the user has clicked into a box
	if( e->y() > d->m_imageY && e->y() < d->m_imageY + 64 && e->x() < width() )
	{
		int dd = (e->x()-12)/74;

		if((e->x()-12-74*dd) <= 64)
		{
			//signal for prefLeitner to set the comboboxes to the clicked box
			emit boxClicked( dd );
			d->m_highlightedBox = dd;

			update();
		}
	}
}

#include "leitnersystemview.moc"

