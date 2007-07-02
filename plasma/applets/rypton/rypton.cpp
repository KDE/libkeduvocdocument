/***************************************************************************
 *   Copyright (C) 2007 by Thomas Georgiou <TAGeorgiou@gmail.com>          *
 *   Copyright (C) 2007 by Jeff Cooper <weirdsox11@gmail.com>              *
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

#include "rypton.h"

#include <math.h>

#include <QApplication>
#include <QBitmap>
#include <QGraphicsScene>
#include <QMatrix>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QStyleOptionGraphicsItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>
//#include <QTextArea>
#include <QLineEdit>
#include <iostream>


#include <KDebug>
#include <KLocale>
#include <KIcon>
#include <KSharedConfig>
#include <KTimeZoneWidget>
#include <KDialog>
#include <QTime>
#include <plasma/svg.h>
#include <plasma/phase.h>

using namespace Plasma;

Rypton::Rypton(QObject *parent, const QStringList &args)
    : Plasma::Applet(parent, args),
      m_defDisplay(0),
      m_dialog(0)
{
    //setHasConfigurationInterface(true);

    dataEngine("chemicaldata")->connectSource("11", this);

    //  Icon in upper-left corner
    QIcon *icon = new KIcon("dictionary");
    m_graphicsIcon = new QGraphicsPixmapItem(icon->pixmap(32,32), this);

    //  Position lineedits
    const int wordEditOffset = 40;
    m_graphicsIcon->setPos(-40 + wordEditOffset,-7);


    m_wordEdit = new Plasma::LineEdit(this);
    m_wordEdit->setTextInteractionFlags(Qt::TextEditorInteraction);
    m_wordEdit->setDefaultText(i18n("Enter word to define here"));

    connect(m_wordEdit, SIGNAL(editingFinished()), this, SLOT(define()));


    Phase::self()->animateItem(m_wordEdit, Phase::Appear);

    m_theme = new Plasma::Svg("widgets/toolbox-button", this);
    m_theme->resize();
    constraintsUpdated();
}

void Rypton::define()
{
    kDebug() << "Rypton::define()" << endl;
    m_word = m_wordEdit->toPlainText();
    dataEngine("chemicaldata")->connectSource(m_word, this);
}

QRectF Rypton::boundingRect() const
{
    if (m_wordEdit->isVisible()) 
        return m_wordEdit->geometry().adjusted(-15,-12,15,12+40);
    else 
        return m_wordEdit->geometry().adjusted(-15,-12,15,12);
}

void Rypton::constraintsUpdated()
{
    kDebug() << "Rypton::constraintsUpdated()" << endl;
    prepareGeometryChange();
}

void Rypton::updated(const QString& source, const Plasma::DataEngine::Data &data)
{
    kDebug() << "Rypton::updated()" << endl;
    Q_UNUSED(source);
//X 
//X    kDebug() << data["Mass"].toString() << endl;
//X     update();
}

void Rypton::showConfigurationInterface()
{
}

void Rypton::configAccepted()
{
}

Rypton::~Rypton()
{
}

void Rypton::paintInterface(QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}
#include "rypton.moc"
