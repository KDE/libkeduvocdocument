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

#ifndef KEDUVOCIMAGEWIDGET_H
#define KEDUVOCIMAGEWIDGET_H

#include <QWidget>

#include <keduvocdocument_export.h>

class QTimeLine;

namespace Practice
{

class KEDUVOCDOCUMENT_EXPORT KEduVocImageWidget : public QWidget
{
    Q_OBJECT

public:
    KEduVocImageWidget( QWidget *parent = 0 );
    ~KEduVocImageWidget();

    void setPixmap( const QPixmap& pixmap );
    void setScalingEnabled( bool scaling, bool onlyDownscaling = true );
    void setKeepAspectRatio( Qt::AspectRatioMode mode );
    void setFadingEnabled( bool fading );
    void setAlignment( Qt::Alignment alignment );

protected:
    virtual void paintEvent( QPaintEvent *e );
    virtual void resizeEvent( QResizeEvent *e );

signals:
    void sizeChanged();

private Q_SLOTS:
    void scalePixmap( bool smooth = true );
    void animationFinished();

private:
    class Private;
    Private * const d;
};

}

#endif // PRACTICE_IMAGEWIDGET_H
