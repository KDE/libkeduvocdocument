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

#include "keduvocexportdialog.h"

#include "ui_keduvocexportdialog.h"

#include <keduvoceditordocument.h>
#include <keduvocdocument.h>
#include <keduvockvtml2writer.h>

#include <KLocalizedString>
#include <QStandardPaths>
#include <QUrl>
#include <QDebug>
#include <QFileDialog>
#include <KMessageBox>

#include <QDialogButtonBox>

#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
#include <libxml/xinclude.h>
#include <libxml/catalog.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>


class KEduVocExportDialog::Private
{
public:
    Private( KEduVocDocument *doc, QWidget *parent );

    Ui::KEduVocExportOptions *m_ui;
    KEduVocDocument *m_doc;
    QWidget *m_parent;
};

KEduVocExportDialog::Private::Private( KEduVocDocument *doc, QWidget *parent )
{
    m_doc = doc;
    m_parent = parent;
    m_ui = new Ui::KEduVocExportOptions();
}

KEduVocExportDialog::KEduVocExportDialog( KEduVocDocument *doc, QWidget *parent )
    : QDialog( parent )
    , d( new Private( doc, parent ) )
{
    QWidget *widget = new QWidget();
    d->m_ui->setupUi( widget );

    QDialogButtonBox * buttonDialog = new QDialogButtonBox;
    buttonDialog->setStandardButtons( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget( widget );
    layout->addWidget( buttonDialog );

    setLayout( layout );

    connect( buttonDialog, &QDialogButtonBox::accepted, this, &KEduVocExportDialog::accept );
    connect( buttonDialog, &QDialogButtonBox::rejected, this, &KEduVocExportDialog::reject );

    setWindowTitle( i18n( "Export" ) );
}

KEduVocExportDialog::~KEduVocExportDialog()
{
    delete d;
}

void KEduVocExportDialog::accept()
{
    QDialog::accept();

    if( d->m_ui->csvRadio->isChecked() ) {
        /// Find the CSV filter in the standard filter list
        //!@todo: good and clean solution
        QStringList defaultFilters = KEduVocDocument::pattern( KEduVocDocument::Writing ).split( '\n' );
        QString filter = defaultFilters.filter( "csv" ).join( "\n" );
        QUrl filename = getFileName( filter );
        if( filename != QUrl() ) {
            emit saveEditorDocument( filename );
        }
        return;
    }

    QString xslFile;
    if( d->m_ui->flashCardRadio->isChecked() ) {
        xslFile = QStandardPaths::locate( QStandardPaths::GenericDataLocation, "parley/xslt/flashcards.xsl" );
    } else {
        xslFile = QStandardPaths::locate( QStandardPaths::GenericDataLocation, "parley/xslt/table.xsl" );
    }

    QString filter = "*.html|" + i18n( "HTML document" );
    QUrl filename = getFileName( filter );
    if( filename.isEmpty() ) {
        return;
    }

    qDebug() << "XSLT starting";

    xsltStylesheetPtr cur = NULL;
    xmlDocPtr doc, res;

    xmlSubstituteEntitiesDefault(1);
    xmlLoadExtDtdDefaultValue = 1;
    cur = xsltParseStylesheetFile( (const xmlChar*) xslFile.toLatin1().constData() );

    doc = xmlParseDoc( (const xmlChar*) d->m_doc->toByteArray( d->m_doc->generator() ).constData() );

    res = xsltApplyStylesheet( cur, doc, 0 );
    FILE* result = fopen( QFile::encodeName( filename.toLocalFile() ).constData(), "w" );
    if( result != NULL ) {
        xsltSaveResultToFile( result, res, cur );
        fclose( result );
    } else {
        KMessageBox::error( this, i18n( "Could not write to file \"%1\"", filename.toLocalFile() ) );
    }

    xsltFreeStylesheet( cur );
    xmlFreeDoc( res );
    xmlFreeDoc( doc );

    xsltCleanupGlobals();
    xmlCleanupParser();

    qDebug() << "XSLT finished";
}

QUrl KEduVocExportDialog::getFileName( const QString& filter )
{
    return QUrl::fromLocalFile(QFileDialog::getSaveFileName(
        d->m_parent, i18n( "Export As" )
        , ( d->m_doc->url().fileName() == i18n( "Untitled" ) )
        ? "" : d->m_doc->url().toLocalFile()
        ,  filter ) );

}
