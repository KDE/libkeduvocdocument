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

#include "keduvocaudiowidget.h"

#include "ui_keduvocaudiowidget.h"

#include <keduvocexpression.h>

#include <QtMultimedia/QMediaPlayer>
#include <QDebug>

using namespace Editor;

class KEduVocAudioWidget::Private
{
public:
    Private();

    /// Column in the document - corresponds to the language (-KV_COL_TRANS)
    int m_currentTranslation;
    /// Selection in the doc - if more than one row is selected behavior is different
    KEduVocExpression* m_entry;

    QMediaPlayer* m_player;        ///< media object for the files

    Ui::KEduVocAudioWidget * m_ui;
};

KEduVocAudioWidget::Private::Private()
{
    m_currentTranslation = -1;
    m_player = 0;
    m_entry = 0;
    m_ui = new Ui::KEduVocAudioWidget();
}

KEduVocAudioWidget::KEduVocAudioWidget( QWidget *parent )
    : QWidget( parent )
    , d( new Private )
{
    d->m_ui->setupUi(this);

    connect( d->m_ui->audioUrlRequester, &KUrlRequester::textChanged, this, &KEduVocAudioWidget::slotAudioFileChanged );
    connect( d->m_ui->playButton, &QPushButton::clicked, this, &KEduVocAudioWidget::playAudio );
//     connect(recordButton, SIGNAL(clicked()), SLOT(recordAudio()));

    d->m_ui->playButton->setEnabled( false );
    d->m_ui->playButton->setIcon( QIcon::fromTheme( "media-playback-start" ) );
    d->m_ui->recordButton->setVisible( false );
//     recordButton->setEnabled(false);
//     recordButton->setIcon(QIcon::fromTheme("media-record"));
    d->m_ui->audioUrlRequester->setEnabled( false );
}

KEduVocAudioWidget::~KEduVocAudioWidget()
{
    delete d;
}

void KEduVocAudioWidget::setTranslation( KEduVocExpression* entry, int translation )
{
    d->m_currentTranslation = translation;
    d->m_entry = entry;

    if( d->m_entry ) {
        d->m_ui->recordButton->setEnabled( true );
        d->m_ui->audioUrlRequester->setEnabled( true );
        d->m_ui->audioUrlRequester->setUrl( QUrl::fromLocalFile( d->m_entry->translation( d->m_currentTranslation )->soundUrl().toLocalFile() ) );
    } else {
        d->m_ui->recordButton->setEnabled( false );
        d->m_ui->audioUrlRequester->setEnabled( false );
        if( d->m_player ) {
            if( d->m_player->state() == QMediaPlayer::PlayingState ) {
                d->m_ui->playButton->setEnabled( true );
            } else {
                d->m_ui->playButton->setEnabled( false );
            }
        }
        d->m_ui->audioUrlRequester->clear();
    }
}

void KEduVocAudioWidget::slotAudioFileChanged( const QString & url )
{
    if( d->m_entry ) {
        d->m_entry->translation( d->m_currentTranslation )->setSoundUrl( QUrl::fromLocalFile( url ) );
    }
    d->m_ui->playButton->setEnabled( !url.isEmpty() );
}

void KEduVocAudioWidget::playAudio()
{
    QUrl soundFile = d->m_entry->translation( d->m_currentTranslation )->soundUrl();

    if( !d->m_player ) {
        d->m_player = new QMediaPlayer( this );
        connect( d->m_player, SIGNAL( finished() ), SLOT( slotPlaybackFinished() ) );
    } else {
        if( d->m_player->state() == QMediaPlayer::PlayingState ) {
            d->m_player->stop();
            slotPlaybackFinished();
        }
    }
    d->m_player->setMedia( soundFile );
    d->m_player->setVolume(50);

    d->m_ui->playButton->setIcon( QIcon::fromTheme( "media-playback-stop" ) );
    d->m_player->play();
}

/*
void AudioWidget::recordAudio()
{
    /// FIXME: When Phonon gains the ability to record sound, implement me :)
}
*/

void KEduVocAudioWidget::slotPlaybackFinished()
{
    d->m_ui->playButton->setIcon( QIcon::fromTheme( "media-playback-start" ) );
    d->m_ui->playButton->setEnabled( !d->m_ui->audioUrlRequester->url().isEmpty() );
}
