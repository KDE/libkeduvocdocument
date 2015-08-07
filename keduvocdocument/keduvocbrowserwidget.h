/***************************************************************************

    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCBROWSERWIDGET_H
#define KEDUVOCBROWSERWIDGET_H

#include <QWidget>

#include <keduvocdocument_export.h>

class KEduVocExpression;
class KHTMLPart;


namespace Editor
{

struct DictionaryProvider {
    QString name;
    QString url;
    QStringList languages;
};

class KEDUVOCDOCUMENT_EXPORT KEduVocBrowserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KEduVocBrowserWidget( QWidget* parent = 0 );
    ~KEduVocBrowserWidget();

public slots:
    /**
     * Update the word
     * @param entry
     * @param translation
     */
    void setTranslation( KEduVocExpression* entry, int translation );


private slots:
    void showCurrentTranslation();
    void openUrl( const QUrl& targetUrl );
    void providerChanged( int );
private:
    class Private;
    Private * const d;

    /**
     * Load a list of providers of online dictionaries
     */
    void setupProviders();
};

}

#endif
