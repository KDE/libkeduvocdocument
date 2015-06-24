/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright 2008 Javier Goday <jgoday@gmail.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvocsummarywordwidget.h"

#include "ui_keduvocsummarywordwidget.h"

// Qt headers
#include <QWidget>
#include <QAbstractItemModel>
#include <QDataWidgetMapper>
#include <QHeaderView>
#include <QItemSelection>
#include <QLabel>
#include <QTreeView>
#include <QDebug>

// KEduVocDocument
#include <keduvoccontainer.h>
#include <keduvocdocument.h>
#include <keduvocexpression.h>
#include <keduvocwordtype.h>
#include <keduvocvocabularymodel.h>
#include <keduvoclessonmodel.h>
#include <keduvocwordclassmodel.h>
#include <keduvocvocabularyfilter.h>

using namespace Editor;

class KEduVocSummaryWordWidget::Private
{
public:
    Private( KEduVocVocabularyFilter *model, KEduVocDocument *doc );

    KEduVocVocabularyFilter *m_model;
    KEduVocDocument *m_doc;
    QDataWidgetMapper *m_mapper;
    KEduVocWordClassModel *m_wordTypeModel;
    QTreeView *m_wordTypeView;
    KEduVocExpression *m_entry;
    int m_translationId;
    Ui::KEduVocSummaryWordWidget * m_ui;
};

KEduVocSummaryWordWidget::Private::Private( KEduVocVocabularyFilter *model, KEduVocDocument *doc )
{
    m_doc = doc;
    m_wordTypeModel = 0;
    m_wordTypeView = 0;
    m_entry = 0;
    m_translationId = 0;
    m_ui = new Ui::KEduVocSummaryWordWidget();
    Q_ASSERT( model );
    Q_ASSERT( m_doc );
    m_model = model;
}

KEduVocSummaryWordWidget::KEduVocSummaryWordWidget( KEduVocVocabularyFilter *model, KEduVocDocument *doc, QWidget *parent )
    : QWidget(parent)
    , d( new Private( model, doc ) )
{
    d->m_ui->setupUi(this);
    slotDocumentChanged( d->m_doc );

    d->m_mapper = new QDataWidgetMapper( this );
    d->m_mapper->setModel( model );
    d->m_mapper->setItemDelegate( new KEduVocSummaryWordDelegate( this ) );

    connect( d->m_ui->wordTypeComboBox, static_cast<void ( KComboBox::* )( const QString & )>( &KComboBox::currentIndexChanged ), this, &KEduVocSummaryWordWidget::wordTypeSelected );
}

KEduVocSummaryWordWidget::~KEduVocSummaryWordWidget()
{
    delete d;
}

void KEduVocSummaryWordWidget::setTranslation( KEduVocExpression *entry, int translation )
{
    if( entry ) {
        // we need to map the widgets relative to the translation (each translation has 9 columns)
        d->m_mapper->clearMapping();

        d->m_mapper->addMapping( d->m_ui->wordEntry,
                             KEduVocVocabularyModel::EntryColumnsMAX * translation + KEduVocVocabularyModel::Translation );
        //m_mapper->addMapping(wordTypeComboBox,
        //                    VocabularyModel::EntryColumnsMAX * translation + VocabularyModel::WordType);
        d->m_mapper->addMapping( d->m_ui->pronunciationEntry,
                             KEduVocVocabularyModel::EntryColumnsMAX * translation + KEduVocVocabularyModel::Pronunciation );
        d->m_mapper->addMapping( d->m_ui->exampleEntry,
                             KEduVocVocabularyModel::EntryColumnsMAX * translation + KEduVocVocabularyModel::Example );
        d->m_mapper->addMapping( d->m_ui->paraphraseEntry,
                             KEduVocVocabularyModel::EntryColumnsMAX * translation + KEduVocVocabularyModel::Paraphrase );
        d->m_mapper->addMapping( d->m_ui->commentEntry,
                             KEduVocVocabularyModel::EntryColumnsMAX * translation + KEduVocVocabularyModel::Comment );

        d->m_ui->languageLabel->setText( "<b>" + d->m_doc->identifier( translation ).name() + "</b>" );
        d->m_ui->lessonLabel->setText( entry->lesson()->name() );

        setCurrentWordType( entry, translation );
    } else {
        clear();
    }

    d->m_entry = entry;
    d->m_translationId = translation;
}

void KEduVocSummaryWordWidget::slotDocumentChanged( KEduVocDocument *doc )
{
    d->m_doc = doc;
    if( !d->m_doc ) {
        qDebug() << "Set invalid document";
        delete d->m_wordTypeModel;
        d->m_wordTypeModel = 0;
    } else {
        delete d->m_wordTypeView;
        if (!d->m_wordTypeModel) {
            qDebug() << "Create word type model for summary view";
            d->m_wordTypeModel = new KEduVocWordClassModel( this );
        }
        d->m_wordTypeModel->setDocument( d->m_doc );
        d->m_wordTypeView = new QTreeView( this );
        d->m_wordTypeView->setModel( d->m_wordTypeModel );
        d->m_ui->wordTypeComboBox->setModel( d->m_wordTypeModel );
        d->m_ui->wordTypeComboBox->setView( d->m_wordTypeView );

        d->m_wordTypeView->setColumnHidden( 1, true );
        d->m_wordTypeView->header()->setVisible( false );
        d->m_wordTypeView->setRootIsDecorated( true );
        d->m_wordTypeView->expandAll();
    }
}


void KEduVocSummaryWordWidget::slotSelectionChanged( const QItemSelection &itemSelected,
        const QItemSelection &itemDeselected )
{
    Q_UNUSED( itemDeselected )

    if( itemSelected.indexes().size() >= 1 ) {
        // the selected index belongs to KEduVocVocabularyFilter, when we need it from the vocabulary model
        QModelIndex index = d->m_model->index( itemSelected.indexes().at( 0 ).row(),
                                           itemSelected.indexes().at( 0 ).column() );
        d->m_mapper->setCurrentModelIndex( index );
    }
}

/*
void SummaryWordWidget::populateLessonList(KEduVocExpression *entry)
{
    lessonComboBox->clear();

    LessonModel *basicLessonModel = new LessonModel(this);
    lessonComboBox->setModel(basicLessonModel);
    QTreeView *view = new QTreeView(this);

    view->setModel(basicLessonModel);
    lessonComboBox->setView(view);

    basicLessonModel->setDocument(m_doc);

    view->header()->setVisible(false);
    view->setRootIsDecorated(true);
    view->expandAll();

    view->setCurrentIndex(basicLessonModel->index(entry->lesson()));
}
*/

void KEduVocSummaryWordWidget::setCurrentWordType( KEduVocExpression *entry, int translation )
{
    if( entry && entry->translation( translation )->wordType() ) {
        qDebug() << "Set current word type: " << entry->translation( translation )->wordType()->name();
        // select the right word type
        d->m_wordTypeView->setCurrentIndex( d->m_wordTypeModel->index( entry->translation( translation )->wordType() ) );
    } else {
        d->m_ui->wordTypeComboBox->setCurrentIndex( -1 );
    }
}

void KEduVocSummaryWordWidget::clear()
{
    qDebug() << "Clear summary widget";

    d->m_ui->languageLabel->setText( QString() );
    d->m_ui->wordEntry->setText( QString() );

    // lessonComboBox->clear();
    d->m_ui->lessonLabel->setText( QString() );

    d->m_ui->pronunciationEntry->setText( QString() );
    d->m_ui->exampleEntry->setText( QString() );
    d->m_ui->paraphraseEntry->setText( QString() );
    d->m_ui->commentEntry->setText( QString() );
}

KEduVocSummaryWordDelegate::KEduVocSummaryWordDelegate( QObject *parent )
    : QItemDelegate( parent )
{
}

void KEduVocSummaryWordDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
    if( !index.isValid() ) {
        return;
    }

    if( editor ) {
        switch( KEduVocVocabularyModel::columnType( index.column() ) ) {
        case KEduVocVocabularyModel::WordClass:
            break;

        case KEduVocVocabularyModel::Comment:
        case KEduVocVocabularyModel::Pronunciation:
        case KEduVocVocabularyModel::Translation:
        case KEduVocVocabularyModel::Example:
        case KEduVocVocabularyModel::Paraphrase:

            QLineEdit *entry = static_cast <QLineEdit *> ( editor );
            if( entry ) {
                entry->setText( index.model()->data( index ).toString() );
            }
            break;
        }
    }
}

void KEduVocSummaryWordWidget::wordTypeSelected( const QString& wordTypeName )
{
    if( !d->m_doc || !d->m_entry ) {
        return;
    }

    KEduVocContainer* container = d->m_doc->wordTypeContainer()->childContainer( wordTypeName );
    if( container ) {
        KEduVocWordType *wordType = static_cast <KEduVocWordType*> ( container );
        if( wordType ) {
            d->m_entry->translation( d->m_translationId )->setWordType( wordType );
        }
    }
}
