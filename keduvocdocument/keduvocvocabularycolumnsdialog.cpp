/***************************************************************************

                      options to enable/disable columns for the languages

    -----------------------------------------------------------------------
    copyright     :(C) 2008 Javier goday <jgoday@gmail.com>
    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvocvocabularycolumnsdialog.h"

// KDE imports
#include <keduvocdocument.h>
#include <keduvocvocabularymodel.h>
#include <KLocalizedString>
#include <KTitleWidget>

// Qt imports
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>
#include <QVBoxLayout>
#include <QDialogButtonBox>

const int COLUMNS_LIMIT = 1; // columns for row

using namespace Editor;

class KEduVocVocabularyColumnsDialog::Private
{
public:
    Private( KEduVocDocument *doc, QList <int> visibleColumns );

    QGridLayout *m_box;
    KEduVocDocument *m_doc;
    QMap <int, QStandardItemModel *> m_models;
    QList <int> m_visibleColumns;
};

KEduVocVocabularyColumnsDialog::Private::Private( KEduVocDocument *doc, QList <int> visibleColumns )
    : m_models()
{
    m_box = new QGridLayout();
    m_doc = doc;
    m_visibleColumns = visibleColumns;
}

void KEduVocVocabularyColumnsDialog::setBox( QGridLayout * box )
{
    d->m_box = box;
}

QGridLayout* KEduVocVocabularyColumnsDialog::box()
{
    return d->m_box;
}

void KEduVocVocabularyColumnsDialog::setDoc( KEduVocDocument * doc )
{
    d->m_doc = doc;
}

KEduVocDocument* KEduVocVocabularyColumnsDialog::doc()
{
    return d->m_doc;
}

void KEduVocVocabularyColumnsDialog::setModels( QMap <int, QStandardItemModel *> models )
{
    d->m_models = models;
}

QMap <int, QStandardItemModel *> KEduVocVocabularyColumnsDialog::models()
{
    return d->m_models;
}

void KEduVocVocabularyColumnsDialog::setVisibleColumns(QList< int > visibleColumns)
{
    d->m_visibleColumns = visibleColumns;
}

QList< int > KEduVocVocabularyColumnsDialog::visibleColumns()
{
    return d->m_visibleColumns;
}

KEduVocVocabularyColumnsDialog::KEduVocVocabularyColumnsDialog(KEduVocDocument *doc, QList <int> visibleColumns, QWidget *parent)
    : QDialog(parent),
    d( new Private( doc, visibleColumns ) )
{
    QDialogButtonBox * button_dialog = new QDialogButtonBox;
    button_dialog->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel );

    setWindowTitle( i18n("Vocabulary Columns") );

    QWidget *main_widget = new QWidget( this );
    main_widget->setLayout( d->m_box );

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget( main_widget );
    layout->addWidget( button_dialog );

    setLayout( layout );

    connect( button_dialog, &QDialogButtonBox::accepted, this, &KEduVocVocabularyColumnsDialog::accept );
    connect( button_dialog, &QDialogButtonBox::rejected, this, &KEduVocVocabularyColumnsDialog::reject );

    KTitleWidget *titleWidget = new KTitleWidget(this);
    titleWidget->setText( i18n( "Enable/Disable the columns for each language" ) );
    titleWidget->setPixmap( QIcon::fromTheme( "view-file-columns" ).pixmap( 22, 22 ), KTitleWidget::ImageRight );
    d->m_box->addWidget( titleWidget, 0, 0, 1, 2 );

    createLanguagesLayout();
}

KEduVocVocabularyColumnsDialog::~KEduVocVocabularyColumnsDialog()
{
    delete d;
}

void KEduVocVocabularyColumnsDialog::accept()
{
    saveVisibleColumns();
    QDialog::accept();
}

void KEduVocVocabularyColumnsDialog::createLanguagesLayout()
{
    int row = 1;
    int column = 0;
    for( int i = 0; i < d->m_doc->identifierCount(); i++ ) {
        d->m_models[i] = new QStandardItemModel( this );

        QGroupBox *box = new QGroupBox( d->m_doc->identifier( i ).name(), 0 );
        QVBoxLayout *box_layout = new QVBoxLayout( 0 );

        d->m_box->addWidget( box, row, column );

        if( column < COLUMNS_LIMIT ) {
            column ++;
        } else {
            row ++;
            column = 0;
        }

        box->setLayout( box_layout );

        addLanguage( i, box_layout );
    }
}

void KEduVocVocabularyColumnsDialog::addLanguage( int i, QVBoxLayout *parent )
{
    for( int c = 1; c < KEduVocVocabularyModel::EntryColumnsMAX; c++ ) {
        QStandardItem *item_data = new QStandardItem( KEduVocVocabularyModel::columnTitle( d->m_doc, i, c ) );
        item_data->setData( QVariant( c - 1 ), KEduVocVocabularyColumnsDialog::ColumnRole );
        item_data->setData( QVariant( i ), KEduVocVocabularyColumnsDialog::LanguageRole );
        item_data->setCheckable( true );
        item_data->setEditable( false );

        // check if the column is active
        if( d->m_visibleColumns.size() >= ( i * 8 + c ) ) {
            item_data->setCheckState( d->m_visibleColumns.at( i * 8 + c ) == 1 ? Qt::Checked : Qt::Unchecked );
        }

        d->m_models[i]->insertRow( c - 1, item_data );
    }

    QTreeView *treeView = new QTreeView(this);
    treeView->header()->hide();
    treeView->setModel( d->m_models [i] );

    parent->addWidget( treeView );
}

void KEduVocVocabularyColumnsDialog::saveVisibleColumns()
{
    QList <int> columns;

    for( int i = 0; i < d->m_doc->identifierCount(); i++ ) {  // each model for every language
        columns << 1;
        for( int row = 0; row < d->m_models[i]->rowCount(); row++ ) { // the columns for one language
            int enabled = ( d->m_models [i]->item( row )->checkState() == Qt::Checked ) ? 1 : 0;
            columns << enabled;
        }
    }

    // i think that this is not the place for this
    setVisibleColumns(columns);
}
