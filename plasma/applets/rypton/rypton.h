/***************************************************************************
 *   Copyright (C) 2007 by Carsten Niehaus <cniehaus@kde.org>   >          *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef Rypton_H
#define Rypton_H

#include <QImage>
#include <QPaintDevice>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include <QX11Info>
#include <QWidget>
#include <QGraphicsItem>
#include <QColor>
#include <QTextEdit>

#include <plasma/applet.h>
#include <plasma/dataengine.h>
#include <plasma/widgets/vboxlayout.h>
#include <plasma/widgets/lineedit.h>
#include <plasma/widgets/pushbutton.h>

class QTimer;
class QCheckBox;
class QSpinBox;
class QLineEdit;


class KDialog;
class KTimeZoneWidget;

namespace Plasma
{
    class Svg;
}

class Rypton : public Plasma::Applet
{
    Q_OBJECT
    public:
        Rypton(QObject *parent, const QStringList &args);
        ~Rypton();

        void paintInterface(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget =0);
        void setPath(const QString&);
        QRectF boundingRect() const;
        void constraintsUpdated();

    public slots:
        void updated(const QString &name, const Plasma::DataEngine::Data &data);
        void define();
        void configureDialog();

    protected slots:
        void configAccepted();

    private:
        QRectF m_bounds;
        int m_pixelSize;
        QString m_timezone;
        Plasma::Svg* m_theme;
        QTime m_time;
        QVariant m_thedef;
        KDialog *m_dialog; //should we move this into another class?
        QCheckBox *m_showTimeStringCheckBox;
        QCheckBox *m_showSecondHandCheckBox;
        QCheckBox *m_swapTzs;
        QSpinBox *m_spinWidth;
	QTextEdit *m_defDisplay;
	QLineEdit *m_wordChooser;
	QString m_word;
        QTimer* m_timer;
        int m_width;
        QGraphicsPixmapItem *m_graphicsIcon; 
	Plasma::LineEdit *m_wordEdit;
        Plasma::LineEdit *m_defEdit;
        QStringList m_defList;
        QStringList::iterator m_i;
};

K_EXPORT_PLASMA_APPLET(rypton, Rypton)

#endif
