/***************************************************************************

                   enter document title and author

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>
                     (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCDOCUMENTPROPERTIES_H
#define KEDUVOCDOCUMENTPROPERTIES_H

#include <keduvocdocument_export.h>

#include <QWidget>

class KEduVocDocument;

class KEDUVOCDOCUMENT_EXPORT KEduVocDocumentProperties : public QWidget
{
    Q_OBJECT

public:
    KEduVocDocumentProperties( KEduVocDocument * doc, bool languageSetup, QWidget *parent );
    ~KEduVocDocumentProperties();

    bool isGrammarCheckBoxChecked();

    bool isDownloadGrammarCheckBoxChecked();

    void setLanguageGroupBoxVisible( bool flag );

public slots:
    void accept();

private:
    class Private;
    Private * const d;

    void prepareLanguageSelection();
    void acceptLanguageConfiguration();
};

#endif
