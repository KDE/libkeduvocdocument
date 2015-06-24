/***************************************************************************
    Copyright 2009 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvocimagewidget.h"

#include <QPainter>
#include <QPaintEngine>
#include <QTimer>
#include <QTimeLine>

#include <QDebug>
#include <QtWidgets>

#if defined(Q_WS_X11)
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <QX11Info>
#undef KeyPress
#undef FocusOut
#endif

using namespace Practice;

// The functions centerPixmaps() and transition() are copied from kdelibs/plasma/paintutils.cpp, revision 1133527
// License: LGPLv2+
// Copyright 2005 by Aaron Seigo <aseigo@kde.org>
// Copyright 2008 by Andrew Lake <jamboarder@yahoo.com>
// Don't just modify the code here, if there are issues they should probably also be fixed in libplasma.

void centerPixmaps( QPixmap &from, QPixmap &to )
{
    if( from.size() == to.size() && from.hasAlphaChannel() && to.hasAlphaChannel() ) {
        return;
    }
    QRect fromRect( from.rect() );
    QRect toRect( to.rect() );

    QRect actualRect = QRect( QPoint( 0, 0 ), fromRect.size().expandedTo( toRect.size() ) );
    fromRect.moveCenter( actualRect.center() );
    toRect.moveCenter( actualRect.center() );

    if( from.size() != actualRect.size() || !from.hasAlphaChannel() ) {
        QPixmap result( actualRect.size() );
        result.fill( Qt::transparent );
        QPainter p( &result );
        p.setCompositionMode( QPainter::CompositionMode_Source );
        p.drawPixmap( fromRect.topLeft(), from );
        p.end();
        from = result;
    }

    if( to.size() != actualRect.size() || !to.hasAlphaChannel() ) {
        QPixmap result( actualRect.size() );
        result.fill( Qt::transparent );
        QPainter p( &result );
        p.setCompositionMode( QPainter::CompositionMode_Source );
        p.drawPixmap( toRect.topLeft(), to );
        p.end();
        to = result;
    }
}

QPixmap transition( const QPixmap &from, const QPixmap &to, qreal amount )
{
    if ( from.isNull() && to.isNull() ) {
        return from;
    }

    QPixmap startPixmap( from );
    QPixmap targetPixmap( to );

    if( from.size() != to.size() || !from.hasAlphaChannel() || !to.hasAlphaChannel() ) {
        centerPixmaps( startPixmap, targetPixmap );
    }

    //paint to in the center of from
    QRect toRect = to.rect();

    QColor color;
    color.setAlphaF( amount );


    // If the native paint engine supports Porter/Duff compositing and CompositionMode_Plus
    QPaintEngine *paintEngine = from.paintEngine();
    if( paintEngine &&
            paintEngine->hasFeature( QPaintEngine::PorterDuff ) &&
            paintEngine->hasFeature( QPaintEngine::BlendModes ) ) {

        QPainter p;
        p.begin( &targetPixmap );
        p.setCompositionMode( QPainter::CompositionMode_DestinationIn );
        p.fillRect( targetPixmap.rect(), color );
        p.end();

        p.begin( &startPixmap );
        p.setCompositionMode( QPainter::CompositionMode_DestinationOut );
        p.fillRect( startPixmap.rect(), color );
        p.setCompositionMode( QPainter::CompositionMode_Plus );
        p.drawPixmap( toRect.topLeft(), targetPixmap );
        p.end();

        return startPixmap;
    }
#if defined(Q_WS_X11)
    // We have Xrender support
    else if( paintEngine && paintEngine->hasFeature( QPaintEngine::PorterDuff ) ) {
        // QX11PaintEngine doesn't implement CompositionMode_Plus in Qt 4.3,
        // which we need to be able to do a transition from one pixmap to
        // another.
        //
        // In order to avoid the overhead of converting the pixmaps to images
        // and doing the operation entirely in software, this function has a
        // specialized path for X11 that uses Xrender directly to do the
        // transition. This operation can be fully accelerated in HW.
        //
        // This specialization can be removed when QX11PaintEngine supports
        // CompositionMode_Plus.
        QPixmap source( targetPixmap ), destination( startPixmap );

        source.detach();
        destination.detach();

        Display *dpy = QX11Info::display();

        XRenderPictFormat *format = XRenderFindStandardFormat( dpy, PictStandardA8 );
        XRenderPictureAttributes pa;
        pa.repeat = 1; // RepeatNormal

        // Create a 1x1 8 bit repeating alpha picture
        Pixmap pixmap = XCreatePixmap( dpy, destination.handle(), 1, 1, 8 );
        Picture alpha = XRenderCreatePicture( dpy, pixmap, format, CPRepeat, &pa );
        XFreePixmap( dpy, pixmap );

        // Fill the alpha picture with the opacity value
        XRenderColor xcolor;
        xcolor.alpha = quint16( 0xffff * amount );
        XRenderFillRectangle( dpy, PictOpSrc, alpha, &xcolor, 0, 0, 1, 1 );

        // Reduce the alpha of the destination with 1 - opacity
        XRenderComposite( dpy, PictOpOutReverse, alpha, None, destination.x11PictureHandle(),
                         0, 0, 0, 0, 0, 0, destination.width(), destination.height() );

        // Add source * opacity to the destination
        XRenderComposite( dpy, PictOpAdd, source.x11PictureHandle(), alpha,
                         destination.x11PictureHandle(),
                         toRect.x(), toRect.y(), 0, 0, 0, 0, destination.width(), destination.height() );

        XRenderFreePicture( dpy, alpha );
        return destination;
    }
#endif
    else {
        // Fall back to using QRasterPaintEngine to do the transition.
        QImage under = startPixmap.toImage();
        QImage over  = targetPixmap.toImage();

        QPainter p;
        p.begin( &over );
        p.setCompositionMode( QPainter::CompositionMode_DestinationIn );
        p.fillRect( over.rect(), color );
        p.end();

        p.begin( &under );
        p.setCompositionMode( QPainter::CompositionMode_DestinationOut );
        p.fillRect( under.rect(), color );
        p.setCompositionMode( QPainter::CompositionMode_Plus );
        p.drawImage( toRect.topLeft(), over );
        p.end();

        return QPixmap::fromImage( under );
    }
}

class KEduVocImageWidget::KEduVocImageWidget::Private
{
public:
    Private();

    bool m_fading;
    bool m_scaling;
    bool m_onlyDownscaling;
    Qt::AspectRatioMode m_keepAspectRatio;
    Qt::Alignment m_alignment;
    QPixmap m_originalPixmap;
    QPixmap m_scaledPixmap;
    QPixmap m_scaledBackupPixmap;
    QPixmap m_animationPixmap;
    bool m_scaledPixmapOutOfDate;
    /// Timer used to delay the smooth scaling
    QTimer *m_scaleTimer;
    QTimeLine *m_animation;
};

KEduVocImageWidget::Private::Private()
    : m_fading( true )
    , m_scaling( true )
    , m_onlyDownscaling( true )
    , m_keepAspectRatio( Qt::KeepAspectRatio )
    , m_alignment( Qt::AlignCenter )
{
}

KEduVocImageWidget::KEduVocImageWidget( QWidget *parent )
    : QWidget(parent)
    , d( new Private )
{
    d->m_scaleTimer = new QTimer( this );
    d->m_scaleTimer->setSingleShot( true );
    d->m_scaleTimer->setInterval( 500 );

    d->m_animation = new QTimeLine( 300, this );
    d->m_scaledPixmapOutOfDate = false;

    connect( d->m_scaleTimer, SIGNAL( timeout() ), this, SLOT( scalePixmap() ) );
    connect( d->m_animation, SIGNAL( valueChanged( qreal ) ), this, SLOT( update() ) );
    connect( d->m_animation, SIGNAL( finished() ), this, SLOT( animationFinished() ) );
}

KEduVocImageWidget::~KEduVocImageWidget()
{
    delete d;
}

void KEduVocImageWidget::setPixmap( const QPixmap& pixmap )
{
    //qDebug() << "set new pixmap, size:" << pixmap.size();
    if( d->m_animation->state() == QTimeLine::Running ) {
        d->m_scaledPixmap = transition( d->m_animationPixmap, d->m_scaledPixmap, d->m_animation->currentValue() );
        d->m_animation->stop();
        animationFinished();
    }

    d->m_animationPixmap = d->m_scaledPixmap;
    d->m_originalPixmap = pixmap;
    d->m_scaledPixmap = QPixmap();
    d->m_scaledBackupPixmap = QPixmap();
    d->m_scaledPixmapOutOfDate = true;
    if( !d->m_scaling ) {
        d->m_scaledPixmap = pixmap;
    }
    scalePixmap( true );
    if( d->m_fading ) {
        d->m_animation->start();
    }
    update();
}

void KEduVocImageWidget::setScalingEnabled( bool scaling, bool onlyDownscaling )
{
    d->m_scaling = scaling;
    d->m_onlyDownscaling = onlyDownscaling;
}

void KEduVocImageWidget::setKeepAspectRatio( Qt::AspectRatioMode mode )
{
    d->m_keepAspectRatio = mode;
}

void KEduVocImageWidget::setFadingEnabled( bool fading )
{
    d->m_fading = fading;
}

void KEduVocImageWidget::setAlignment( Qt::Alignment alignment )
{
    d->m_alignment = alignment;
}

void KEduVocImageWidget::paintEvent( QPaintEvent* e )
{
    QWidget::paintEvent( e );
    QPainter painter( this );
    if( d->m_scaling && d->m_scaledPixmapOutOfDate ) {
        d->m_scaleTimer->start();
        scalePixmap( false );
    }
    QPixmap pm = d->m_scaledPixmap;
    if( d->m_animation->state() == QTimeLine::Running ) {
        pm = transition( d->m_animationPixmap, d->m_scaledPixmap, d->m_animation->currentValue() );
    }

    int x =( size().width() - pm.width() ) / 2;
    if( d->m_alignment.testFlag( Qt::AlignLeft ) ) {
        x = 0;
    } else if( d->m_alignment.testFlag( Qt::AlignRight ) ) {
        x = size().width() - pm.width();
    }
    int y = ( size().height() - pm.height() ) / 2;
    if( d->m_alignment.testFlag( Qt::AlignTop ) ) {
        y = 0;
    } else if( d->m_alignment.testFlag( Qt::AlignBottom ) ) {
        y = size().height() - pm.height();
    }
    painter.drawPixmap( x, y, pm );
}

void KEduVocImageWidget::resizeEvent( QResizeEvent* e )
{
    if( !d->m_scaledPixmapOutOfDate ) {
        d->m_scaledBackupPixmap = d->m_scaledPixmap;
    }
    // stop animations when resizing
    if( d->m_animation->state() == QTimeLine::Running ) {
        d->m_animation->stop();
        animationFinished();
    }
    d->m_scaledPixmapOutOfDate = true;
    QWidget::resizeEvent( e );
    emit sizeChanged();
}

void KEduVocImageWidget::scalePixmap( bool smooth )
{
    bool scaleUp = d->m_originalPixmap.width() <= size().width() && d->m_originalPixmap.height() <= size().height();
    if( ( d->m_onlyDownscaling && scaleUp ) || d->m_originalPixmap.size() == size() ) {
        //qDebug() << "no need to scale pixmap";
        d->m_scaledPixmapOutOfDate = false;
        d->m_scaledPixmap = d->m_originalPixmap;
        d->m_scaledBackupPixmap = QPixmap();
    } else if( smooth ) {
        //qDebug() << "smooth scaling to" << size();
        if( d->m_originalPixmap.isNull() || size().isEmpty() ) {
            d->m_scaledPixmapOutOfDate = false;
            d->m_scaledPixmap = QPixmap();
            update();
            return;
        }
        d->m_scaledPixmap = d->m_originalPixmap.scaled( size(), d->m_keepAspectRatio, Qt::SmoothTransformation );
        d->m_scaledBackupPixmap = QPixmap();
        d->m_scaledPixmapOutOfDate = false;
        update();
    } else {
        //qDebug() << "fast scaling to" << size();
        // Try to find out if it makes sense to use the scaled backup pixmap.
        // If the scaled backup gets too small, we use the original image.
        float ratio = 0;
        if( !size().isEmpty() ) {
            ratio = qMin( float( d->m_scaledBackupPixmap.width() ) / size().width(),
                         float( d->m_scaledBackupPixmap.height() ) / size().height());
        }
        if( ratio > 0.4 && !d->m_scaledBackupPixmap.isNull() ) {
            d->m_scaledPixmap = d->m_scaledBackupPixmap.scaled( size(), d->m_keepAspectRatio, Qt::FastTransformation );
        } else {
            if( d->m_originalPixmap.isNull() || size().isEmpty() ) {
                d->m_scaledPixmap = QPixmap();
                return;
            }
            // use the original pixmap
            d->m_scaledPixmap = d->m_originalPixmap.scaled( size(), d->m_keepAspectRatio, Qt::FastTransformation );
            d->m_scaledBackupPixmap = d->m_scaledPixmap;
        }
        d->m_scaledPixmapOutOfDate = true;
    }
}

void KEduVocImageWidget::animationFinished()
{
    d->m_animationPixmap = QPixmap();
}