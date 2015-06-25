/***************************************************************************
    Copyright 2010 Daniel Laidig <laidig@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "keduvoclatexwidget.h"

#include "ui_keduvoclatexwidget.h"

#include <keduvoclatexrenderer.h>
#include <keduvocvocabularymodel.h>
#include <keduvocvocabularyfilter.h>

#include <QDataWidgetMapper>
#include <KLocalizedString>

using namespace Editor;

class KEduVocLatexWidget::Private
{
public:
    Private( KEduVocVocabularyFilter *model, KEduVocDocument *doc );

    KEduVocVocabularyFilter *m_model;
    KEduVocDocument *m_doc;
    QDataWidgetMapper *m_mapper;
    KEduVocTranslation *m_translation;
    QLabel m_hiddenLabel;
    Practice::KEduVocLatexRenderer *m_renderer;
    Ui::KEduVocLatexWidget * m_ui;
};

KEduVocLatexWidget::Private::Private(KEduVocVocabularyFilter* model, KEduVocDocument* doc)
{
    m_translation = 0;
    m_renderer = 0;
    m_doc = doc;
    m_model = model;
    m_ui = new Ui::KEduVocLatexWidget();
}

KEduVocLatexWidget::KEduVocLatexWidget( KEduVocVocabularyFilter *model, KEduVocDocument *doc, QWidget *parent )
    : QWidget(parent)
    , d( new Private( model, doc ) )
{
    d->m_ui->setupUi( this );
    d->m_ui->lineEdit->setPlaceholderText( i18n( "Enter LaTeX code here." ) );
    d->m_mapper = new QDataWidgetMapper( this );
    d->m_mapper->setModel( model );
    KEduVocLatexDelegate *delegate = new KEduVocLatexDelegate( this );
    delegate->setMathModeCheckBox( d->m_ui->mathModeCheckBox );
    d->m_mapper->setItemDelegate( delegate );
    connect( d->m_ui->mathModeCheckBox, &QCheckBox::stateChanged, this, &KEduVocLatexWidget::checkBoxToggled );
    connect( d->m_ui->lineEdit, &QLineEdit::editingFinished, this, &KEduVocLatexWidget::updateLatex, Qt::QueuedConnection);
}

KEduVocLatexWidget::~KEduVocLatexWidget()
{
    delete d;
}

void KEduVocLatexWidget::setTranslation( KEduVocExpression *entry, int translation )
{
    d->m_ui->previewLabel->clear();
    d->m_ui->previewLabel->setMinimumSize( QSize( 1, 1 ) );

    if( entry ) {
        // we need to map the widgets relative to the translation (each translation has 9 columns)
        d->m_mapper->clearMapping();

        d->m_mapper->addMapping( d->m_ui->lineEdit,
                             KEduVocVocabularyModel::EntryColumnsMAX * translation + KEduVocVocabularyModel::Translation );
        d->m_translation = entry->translation( translation );
        updateLatex();
    }
}

void KEduVocLatexWidget::slotDocumentChanged( KEduVocDocument *doc )
{
    d->m_doc = doc;
}

void KEduVocLatexWidget::slotSelectionChanged( const QItemSelection &itemSelected,
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

void KEduVocLatexWidget::checkBoxToggled()
{
    // emulate editing of the line edit
    d->m_ui->lineEdit->setFocus();
    d->m_ui->mathModeCheckBox->setFocus();
}

void KEduVocLatexWidget::updateLatex()
{
    if( !d->m_translation ) {
        return;
    }
    if( Practice::KEduVocLatexRenderer::isLatex( d->m_translation->text() ) ) {
        if( !d->m_renderer) {
            d->m_renderer = new Practice::KEduVocLatexRenderer( this );
            d->m_renderer->setResultLabel( d->m_ui->previewLabel);
        }
        d->m_renderer->renderLatex( d->m_translation->text() );
    }
}


class KEduVocLatexDelegate::Private
{
public:
    Private();

    QCheckBox *m_checkBox;
};

KEduVocLatexDelegate::Private::Private()
{
    m_checkBox = 0;
}

KEduVocLatexDelegate::KEduVocLatexDelegate( QObject *parent )
    : QItemDelegate( parent )
    , d( new Private )
{
}

KEduVocLatexDelegate::~KEduVocLatexDelegate()
{
    delete d;
}

void KEduVocLatexDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
    if( !index.isValid() ) {
        return;
    }

    if( editor ) {
        QLineEdit *entry = static_cast <QLineEdit *> ( editor );
        if( entry ) {
            QString text = index.model()->data( index ).toString();
            if( text.startsWith( QLatin1String( "$$" ) ) && text.endsWith( QLatin1String( "$$" ) ) ) {
                entry->setText( text.mid( 2, text.count() - 4 ) );
                d->m_checkBox->setChecked( true );
            } else if( text.startsWith( QString::fromUtf8( "§§" ) ) && text.endsWith( QString::fromUtf8( "§§" ) ) ) {
                entry->setText( text.mid( 2, text.count() - 4 ) );
                d->m_checkBox->setChecked( false );
            } else {
                entry->setText( QString() );
                return;
            }
        }
    }
}

void KEduVocLatexDelegate::setMathModeCheckBox( QCheckBox *checkBox ) {
    d->m_checkBox = checkBox;
}

void KEduVocLatexDelegate::setModelData( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
    if( !index.isValid() ) {
        return;
    }

    if( editor ) {
        QLineEdit *entry = static_cast <QLineEdit *> ( editor );
        if( entry ) {
            QString text = entry->text();
            if( d->m_checkBox->isChecked() ) {
                model->setData( index, QString( "$$" + text + "$$" ) );
            } else {
                model->setData( index, QString( QString::fromUtf8( "§§" ) + text + QString::fromUtf8( "§§" ) ) );
            }
        }
    }
}
