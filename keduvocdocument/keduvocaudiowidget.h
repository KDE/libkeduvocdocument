/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCAUDIOWIDGET_H
#define KEDUVOCAUDIOWIDGET_H

#include <QWidget>

//#include "ui_keduvocaudiowidget.h"
#include <keduvocdocument_export.h>

class QMediaPlayer;

class KEduVocExpression;

namespace Editor
{

class KEDUVOCDOCUMENT_EXPORT KEduVocAudioWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KEduVocAudioWidget( QWidget* parent = 0 );
    ~KEduVocAudioWidget();

public slots:
    /**
     * Update the word
     * @param entry
     * @param translation
     */
    void setTranslation( KEduVocExpression* entry, int translation );

private slots:
    void slotAudioFileChanged( const QString & url );
    void playAudio();
//     void recordAudio();
    void slotPlaybackFinished();

private:
    class Private;
    Private * const d;
};

}

#endif
