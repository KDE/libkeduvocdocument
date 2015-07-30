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

#ifndef KEDUVOCEXPORTDIALOG_H
#define KEDUVOCEXPORTDIALOG_H

#include <keduvocdocument_export.h>

#include <QDialog>

class KEduVocDocument;
class QUrl;

class KEDUVOCDOCUMENT_EXPORT KEduVocExportDialog : public QDialog
{
    Q_OBJECT
public:
    KEduVocExportDialog( KEduVocDocument *doc, QWidget *parent = 0 );
    ~KEduVocExportDialog();

signals:
    /**
     * Signal connected in KEduVocEditorDocument like this
     * 
     * connect( exportDialog, &KEduVocExportDialog::saveEditorDocument, this, &KEduVocEditorDocument::saveAs );
     */
    void saveEditorDocument( QUrl filename );

private slots:
    void accept();

private:
    class Private;
    Private * const d;

    QUrl getFileName(const QString& filter);
};

#endif
