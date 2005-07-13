#ifndef SPECTRUM_H
#define SPECTRUM_H
/***************************************************************************
 *   Copyright (C) 2005 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
 *   
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <qwidget.h>
#include <qcolor.h>
#include <qpainter.h>
#include <qimage.h>
#include <qstring.h>
#include <qvaluelist.h>

#include <kimageeffect.h>
#include <kdebug.h>
#include <kpixmapeffect.h>

#define MAXCOLOR = 750
#define MINCOLOR = 450

class SpectrumWidget : public QWidget
{
	Q_OBJECT

	public:
		SpectrumWidget( QWidget *parent, const char* name = 0 );
		~SpectrumWidget();

		void setSpectra( QValueList<double> l ){
			m_spectra = l;
		}
	
	private:
		QValueList<double> m_spectra;

		/**
		 * @return the adjusted value of the @p color. The
		 * correction depends on @p factor which has been
		 * figured out emperically
		 */
		int Adjust( double color, double factor );

		/**
		 * This method changes the three values @p r @p g and @p b to the 
		 * correct values
		 * param wavelength the wavelength for which the color is searched
		 * param r red
		 * param g green 
		 * param b blue
		 */
		void wavelengthToRGB( double wavelength, int& r, int& g, int& b );

		double Gamma;
		int IntensityMax;

		/**
		 * draws the spectra-lines
		 */
		void drawLines( QPainter *p );

		int xPos( double value );

		/**
		 * @returns the color of a line
		 * @param spectrum the value of the spectrum
		 */
		QColor linecolor( double spectrum );
	
		double startValue;
		double endValue;

		int m_realWidth;
		int m_realHeight;

		double m_stretch;
	
	protected:
		virtual void paintEvent( QPaintEvent *e );
};
#endif // SPECTRUM_H

