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
      m_dialog(0),
      m_showTimeStringCheckBox(0),
      m_spinWidth(0)
{
    setFlags(QGraphicsItem::ItemIsMovable);
    
    KConfigGroup cg = appletConfig();
    m_width = cg.readEntry("width", 500);
    m_wordEdit = new Plasma::LineEdit(this);
    m_wordEdit->setTextInteractionFlags(Qt::TextEditorInteraction);
    m_wordEdit->setDefaultText(i18n("Enter word to define here"));
    Phase::self()->animateItem(m_wordEdit, Phase::Appear);
    m_defEdit = new Plasma::LineEdit(this);
    m_defEdit->setTextInteractionFlags(Qt::TextBrowserInteraction);

    m_defEdit->hide();
    m_wordEdit->setZValue(m_defEdit->zValue()+1);

    //  Icon in upper-left corner
    QIcon *m_icon = new KIcon("dictionary");
    m_graphicsIcon = new QGraphicsPixmapItem(m_icon->pixmap(32,32), this);

    //  Position lineedits
    const int wordEditOffset = 40;
    m_graphicsIcon->setPos(-40 + wordEditOffset,-7);
    m_wordEdit->setPos(wordEditOffset,0);
    m_wordEdit->setTextWidth(m_width-wordEditOffset);
    m_defEdit->setTextWidth(m_width);
    m_defEdit->setPos(0,40);

    m_word = QString("22");
    Plasma::DataEngine* m_dataEngine = dataEngine("chemicaldata");
    m_dataEngine->connectSource(m_word, this);
    connect(m_wordEdit, SIGNAL(editingFinished()), this, SLOT(define()));
}

void Rypton::define()
{
    kDebug() << "Rypton::define()" << endl;

    m_word = m_wordEdit->toPlainText();
    Plasma::DataEngine* m_dataEngine = dataEngine("chemicaldata");
    m_dataEngine->connectSource(m_word, this);
}

QRectF Rypton::boundingRect() const
{
    if (m_defEdit->isVisible()) 
        return m_defEdit->geometry().adjusted(-15,-12,15,12+40);
    else 
        return m_defEdit->geometry().adjusted(-15,-12,15,12);
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

void Rypton::configureDialog()
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
