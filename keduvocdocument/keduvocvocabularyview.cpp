/***************************************************************************

    Copyright 2006, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

///@file keduvocvocabularyview.cpp

#include "keduvocvocabularyview.h"

#include <keduvoctranslation.h>
#include <keduvocexpression.h>
#include <keduvocvocabularymodel.h>
#include <keduvocvocabularymimedata.h>
#include <keduvocvocabularyfilter.h>
#include <keduvocvocabularydelegate.h>
#include <keduvocvocabularyheaderview.h>
#include <keduvocvocabularycolumnsdialog.h>


#include <QHeaderView>
#include <QPainter>
#include <QResizeEvent>
#include <QClipboard>
#include <QTimer>
#include <QApplication>
#include <QDebug>
#include <QString>

#include <KNotification>
#include <QAction>
#include <KActionCollection>
#include <KToggleAction>
#include <KLocalizedString>
#include <KMessageBox>
#include <QUrl>
#include <sonnet/backgroundchecker.h>

using namespace Editor;

class KEduVocVocabularyView::Private
{
public:
    Private( KActionCollection* actionCollection, bool automaticTranslation );

    KActionCollection* m_actionCollection;
    QAction* m_appendEntryAction;
    QAction* m_deleteEntriesAction;
    QAction* m_copyAction;
    QAction* m_cutAction;
    QAction* m_pasteAction;
    QAction* m_selectAllAction;
    QAction* m_clearSelectionAction;

    KEduVocVocabularyFilter* m_model;
    KEduVocVocabularyDelegate* m_vocabularyDelegate;
    KEduVocDocument *m_doc;

    int m_spellColumn;
    int m_spellRow;
    Sonnet::BackgroundChecker *m_spellChecker;
    Sonnet::Dialog *m_spellDialog;

    bool m_automaticTranslation; // Prefs::automaticTranslation()
    QList <int> m_visibleColumns;
    QByteArray m_state;
    QString m_spellCheck; // LanguageSettings::spellChecker()
};

KEduVocVocabularyView::Private::Private( KActionCollection* actionCollection, bool automaticTranslation )
    : m_model( 0 ), 
      m_doc( 0 ),
      m_spellChecker( 0 ), 
      m_spellDialog( 0 ),
      m_actionCollection( actionCollection )
{
    m_automaticTranslation = automaticTranslation;
}


/** FIXME Replace QWidget in constructor with EditorWindow and d->m_actionCollection with parent->actionColletion() after adding editor.h to keduvoc */
KEduVocVocabularyView::KEduVocVocabularyView( QWidget * parent, KActionCollection* actionCollection, bool automaticTranslation )
    : QTableView(parent), 
      d( new Private( actionCollection, automaticTranslation ) )
{
    installEventFilter( this );

    setHorizontalHeader( new KEduVocVocabularyHeaderView( Qt::Horizontal, this ) );

    horizontalHeader()->setSectionResizeMode( QHeaderView::Interactive );
    horizontalHeader()->setSectionsMovable( true );
    setEditTriggers( QAbstractItemView::AnyKeyPressed | QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked );

    setSortingEnabled( true );
    setTabKeyNavigation( true );

    d->m_vocabularyDelegate = new KEduVocVocabularyDelegate( d->m_automaticTranslation, this );
    setItemDelegate( d->m_vocabularyDelegate );

    setFrameStyle( QFrame::NoFrame );
    setAlternatingRowColors( true );

    // Enable context menus
    setContextMenuPolicy( Qt::ActionsContextMenu );
    horizontalHeader()->setContextMenuPolicy( Qt::ActionsContextMenu );

    setWordWrap( true );
    setDragEnabled( true );

    // smooth scrolling horizontally, otherwise it tries to jump from item to item.
    setHorizontalScrollMode( QAbstractItemView::ScrollPerPixel );

    d->m_appendEntryAction = new QAction( this );
    d->m_actionCollection->addAction( "edit_append", d->m_appendEntryAction );
    d->m_actionCollection->setDefaultShortcut(
        d->m_appendEntryAction, QKeySequence( Qt::Key_Insert ) );
    d->m_appendEntryAction->setIcon( QIcon::fromTheme( "list-add-card" ) );
    d->m_appendEntryAction->setText( i18n( "&Add New Entry" ) );
    connect( d->m_appendEntryAction, SIGNAL( triggered( bool ) ), SLOT( appendEntry() ) );
    d->m_appendEntryAction->setShortcut( QKeySequence( Qt::Key_Insert ) );
    d->m_appendEntryAction->setWhatsThis( i18n( "Append a new row to the vocabulary" ) );
    d->m_appendEntryAction->setToolTip( d->m_appendEntryAction->whatsThis() );
    d->m_appendEntryAction->setStatusTip( d->m_appendEntryAction->whatsThis() );
    addAction( d->m_appendEntryAction );

    d->m_deleteEntriesAction = new QAction( this );
    d->m_actionCollection->addAction( "edit_remove_selected_area", d->m_deleteEntriesAction );
    d->m_actionCollection->setDefaultShortcut(
        d->m_deleteEntriesAction, QKeySequence::Delete );
    d->m_deleteEntriesAction->setIcon( QIcon::fromTheme( "list-remove-card" ) );
    d->m_deleteEntriesAction->setText( i18n( "&Delete Entry" ) );
    connect( d->m_deleteEntriesAction, SIGNAL( triggered( bool ) ), this, SLOT( deleteSelectedEntries() ) );
    d->m_deleteEntriesAction->setShortcut( QKeySequence::Delete );
    d->m_deleteEntriesAction->setWhatsThis( i18n( "Delete the selected rows" ) );
    d->m_deleteEntriesAction->setToolTip( d->m_deleteEntriesAction->whatsThis() );
    d->m_deleteEntriesAction->setStatusTip( d->m_deleteEntriesAction->whatsThis() );
    addAction( d->m_deleteEntriesAction );

    QAction* separator = new QAction( this );
    separator->setSeparator( true );
    addAction( separator );

    d->m_copyAction = KStandardAction::copy( this, SLOT( slotEditCopy() ), d->m_actionCollection );
    d->m_actionCollection->setDefaultShortcut( d->m_copyAction, QKeySequence::Copy );
    d->m_copyAction->setWhatsThis( i18n( "Copy" ) );
    d->m_copyAction->setToolTip( d->m_copyAction->whatsThis() );
    d->m_copyAction->setStatusTip( d->m_copyAction->whatsThis() );
    addAction( d->m_copyAction );

    d->m_cutAction = KStandardAction::cut( this, SLOT( slotCutEntry() ), d->m_actionCollection );
    d->m_actionCollection->setDefaultShortcut( d->m_cutAction, QKeySequence::Cut );
    d->m_cutAction->setWhatsThis( i18n( "Cut" ) );
    d->m_cutAction->setToolTip( d->m_cutAction->whatsThis() );
    d->m_cutAction->setStatusTip( d->m_cutAction->whatsThis() );
    addAction( d->m_cutAction );

    d->m_pasteAction = KStandardAction::paste( this, SLOT( slotEditPaste() ), d->m_actionCollection );
    d->m_actionCollection->setDefaultShortcut( d->m_pasteAction, QKeySequence::Paste );
    d->m_pasteAction->setWhatsThis( i18n( "Paste" ) );
    d->m_pasteAction->setToolTip( d->m_pasteAction->whatsThis() );
    d->m_pasteAction->setStatusTip( d->m_pasteAction->whatsThis() );
    addAction( d->m_pasteAction );

    d->m_selectAllAction = KStandardAction::selectAll( this, SLOT( selectAll() ), d->m_actionCollection );
    d->m_actionCollection->setDefaultShortcut( d->m_selectAllAction, QKeySequence::SelectAll );
    d->m_selectAllAction->setWhatsThis( i18n( "Select all rows" ) );
    d->m_selectAllAction->setToolTip( d->m_selectAllAction->whatsThis() );
    d->m_selectAllAction->setStatusTip( d->m_selectAllAction->whatsThis() );

    d->m_clearSelectionAction = KStandardAction::deselect( this, SLOT( clearSelection() ), d->m_actionCollection );
    d->m_actionCollection->setDefaultShortcut( d->m_clearSelectionAction, QKeySequence::Deselect );
    d->m_clearSelectionAction->setWhatsThis( i18n( "Deselect all rows" ) );
    d->m_clearSelectionAction->setToolTip( d->m_clearSelectionAction->whatsThis() );
    d->m_clearSelectionAction->setStatusTip( d->m_clearSelectionAction->whatsThis() );

    // vocabulary columns dialog
    QAction *vocabularyColumnsDialogAction = new QAction( this );
    d->m_actionCollection->addAction( "show_vocabulary_columns_dialog", vocabularyColumnsDialogAction );
    vocabularyColumnsDialogAction->setIcon( QIcon::fromTheme( "view-file-columns" ) );
    vocabularyColumnsDialogAction->setText( i18n( "Vocabulary Columns..." ) );
    vocabularyColumnsDialogAction->setWhatsThis( i18n( "Toggle display of individual vocabulary columns" ) );
    vocabularyColumnsDialogAction->setToolTip( vocabularyColumnsDialogAction->whatsThis() );
    vocabularyColumnsDialogAction->setStatusTip( vocabularyColumnsDialogAction->whatsThis() );
    horizontalHeader()->addAction( vocabularyColumnsDialogAction );
    addAction( vocabularyColumnsDialogAction );
    connect( vocabularyColumnsDialogAction, SIGNAL( triggered( bool ) ), this, SLOT( slotShowVocabularyColumnsDialog() ) );
}

KEduVocVocabularyView::~KEduVocVocabularyView()
{
    delete d;
}

void KEduVocVocabularyView::setAutomaticTranslation( bool automaticTranslation )
{
    d->m_automaticTranslation = automaticTranslation;
}

bool KEduVocVocabularyView::automaticTranslation()
{
    return d->m_automaticTranslation;
}

QList <int> KEduVocVocabularyView::visibleColumns()
{
    return d->m_visibleColumns;
}

QByteArray KEduVocVocabularyView::state()
{
    return d->m_state;
}

void KEduVocVocabularyView::setState( QByteArray state )
{
    d->m_state = state;
}

void KEduVocVocabularyView::setSpellCheck( QString spellCheck )
{
    d->m_spellCheck = spellCheck;
}

QString KEduVocVocabularyView::spellCheck()
{
    return d->m_spellCheck;
}

void KEduVocVocabularyView::setFilter( KEduVocVocabularyFilter * model )
{
    QTableView::setModel( model );

    d->m_model = model;
    connect( selectionModel(), SIGNAL( currentChanged( const QModelIndex &, const QModelIndex & ) ),
            SLOT( slotCurrentChanged( const QModelIndex &, const QModelIndex & ) ) );
    connect( selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ),
            SLOT( slotSelectionChanged( const QItemSelection&, const QItemSelection& ) ) );
    slotSelectionChanged( QItemSelection(), QItemSelection() );
}

void KEduVocVocabularyView::slotCurrentChanged( const QModelIndex & current, const QModelIndex & previous )
{
    Q_UNUSED( previous );
    KEduVocExpression* entry = 0;
    if( current.isValid() ) {
        entry =  model()->data( current, KEduVocVocabularyModel::EntryRole ).value<KEduVocExpression*>();
    }
    emit translationChanged( entry, KEduVocVocabularyModel::translation( current.column() ) );
}

void KEduVocVocabularyView::reset( QList <int> visibleColumns, QByteArray state )
{
    QTableView::reset();
    emit translationChanged( 0, 0 );

    if ( d->m_doc ) {
        d->m_visibleColumns = visibleColumns;
        d->m_state = state;

        if( !horizontalHeader()->restoreState( state ) ) {
            resizeColumnsToContents();
        }
    }

    horizontalHeader()->setSectionsMovable( true );

    for( int i = 0; i < model()->columnCount( QModelIndex() ); i++ ) {
        if( i < visibleColumns.size() ) {
            setColumnHidden( i, !visibleColumns.value( i ) );
        } else {
            if( KEduVocVocabularyModel::columnType( i ) != KEduVocVocabularyModel::Translation ) {
                setColumnHidden( i, true );
            }
        }
    }
}

void KEduVocVocabularyView::saveColumnVisibility() const
{
    if( !d->m_doc ) {
        return;
    }

    // Generate a QList<int> for saving
    QList <int> visibleList;
    for( int i = 0; i < d->m_model->columnCount(); ++i ) {
        visibleList.append( static_cast<int>( !isColumnHidden( i ) ) );
    }
    d->m_visibleColumns = visibleList;
}

void KEduVocVocabularyView::appendEntry()
{
    QModelIndex newIndex = d->m_model->appendEntry();
    scrollTo( newIndex );
    selectionModel()->clear(); // otherwise proxy mapping gets screwed for some reason
    selectionModel()->select( newIndex, QItemSelectionModel::ClearAndSelect );
    selectionModel()->setCurrentIndex( newIndex, QItemSelectionModel::ClearAndSelect );
    edit( newIndex );
}

void KEduVocVocabularyView::appendChar( const QChar &c )
{
    const QModelIndex &index = selectionModel()->currentIndex();
    d->m_model->setData( index, QString( d->m_model->data( index ).toString() + c ) );
}

void KEduVocVocabularyView::deleteSelectedEntries( bool askConfirmation )
{
    QSet <int> rows;
    foreach( const QModelIndex & index, selectionModel()->selectedIndexes() ) {
        rows.insert( index.row() );
    }

    bool del = true;
    if( askConfirmation ) {
        del = KMessageBox::Continue == KMessageBox::warningContinueCancel( this, i18np( "Do you really want to delete the selected entry?", "Do you really want to delete the selected %1 entries?", rows.count() ), i18n( "Delete" ), KStandardGuiItem::del() );
    }

    if( del ) {
        emit translationChanged( 0, 0 );
        while( !selectionModel()->selectedIndexes().isEmpty() ) {
            d->m_model->removeRows( selectionModel()->selectedIndexes()[0].row(), 1, QModelIndex() );
        }
    }
}

void KEduVocVocabularyView::slotEditCopy()
{
    QModelIndexList sortedIndexes = selectionModel()->selectedIndexes();
    qSort( sortedIndexes );
    QMimeData *mimeData = d->m_model->mimeData( sortedIndexes );

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setMimeData( mimeData );
}

void KEduVocVocabularyView::slotEditPaste()
{
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    const KEduVocVocabularyMimeData *vocMimeData = qobject_cast<const KEduVocVocabularyMimeData *>( mimeData );
    if( vocMimeData ) {
        qDebug() << "Clipboard contains vocabulary mime data.";
        foreach( const KEduVocVocabularyMimeData::MimeExpression & mimeEntry, vocMimeData->expressionList() ) {
            KEduVocExpression *pasteExpression = new KEduVocExpression( mimeEntry.expression );
            d->m_model->appendEntry( pasteExpression );

            // find word type (create if not found)
            KEduVocWordType *type = d->m_doc->wordTypeContainer();
            foreach( int translation, mimeEntry.wordTypes.keys() ) {
                // append if needed
                foreach( const QString & typeName, mimeEntry.wordTypes.value( translation ).wordType ) {
                    qDebug() << mimeEntry.wordTypes.value( translation ).wordType;
                    KEduVocContainer *childType = type->childContainer( typeName );
                    if( !childType ) {
                        // the doc does not contain the right word type - create it
                        childType = new KEduVocWordType( typeName );
                        type->appendChildContainer( childType );
                    }
                    type = static_cast<KEduVocWordType*>( childType );
                }
                pasteExpression->translation( translation )->setWordType( type );
                // check for special type stuff
                if( type->wordType() != mimeEntry.wordTypes.value( translation ).grammarType ) {
                    if( type->wordType() == KEduVocWordFlag::NoInformation ) {
                        type->setWordType( mimeEntry.wordTypes.value( translation ).grammarType );
                    }
                }
            }
        }
    } else if( mimeData->hasText() ) {
        qDebug() << "Clipboard contains text data.";
        // split at newline
        QStringList lines = clipboard->text().split( '\n' );
        foreach( QString line, lines ) {
            // split at tabs or semicolon:
            d->m_model->appendEntry( new KEduVocExpression( line.split( QRegExp("[\t;]"), QString::KeepEmptyParts ) ) );
        }
    }
}

void KEduVocVocabularyView::slotCutEntry()
{
    slotEditCopy();
    deleteSelectedEntries( false );
}

void KEduVocVocabularyView::slotSelectionChanged( const QItemSelection &, const QItemSelection & )
{
    bool hasSelection = selectionModel()->hasSelection();
    d->m_deleteEntriesAction->setEnabled( hasSelection );
    d->m_clearSelectionAction->setEnabled( hasSelection );
    d->m_copyAction->setEnabled( hasSelection );
    d->m_cutAction->setEnabled( hasSelection );
}

void KEduVocVocabularyView::setDocument( KEduVocDocument * doc )
{
    d->m_doc = doc;
    d->m_vocabularyDelegate->setDocument( doc );
    QTimer::singleShot( 0, this, SLOT( reset() ) );
}

KEduVocDocument* KEduVocVocabularyView::document()
{
    return d->m_doc;
}

/**
 * Set the translator to be used by the delegate
 * @param translator
 */
void KEduVocVocabularyView::setTranslator( KEduVocTranslator* translator )
{
    d->m_vocabularyDelegate->setTranslator( translator );
}

void KEduVocVocabularyView::slotShowVocabularyColumnsDialog( QList <int> visibleColumns, QByteArray state )
{
    KEduVocVocabularyColumnsDialog *dialog = new KEduVocVocabularyColumnsDialog( d->m_doc, d->m_visibleColumns, this );

    if( dialog->exec() == QDialog::Accepted ) {
        reset( visibleColumns, state );
    }
}

void KEduVocVocabularyView::checkSpelling( int language, QString spellCheck )
{
    if( !d->m_model->rowCount() ) {
        KMessageBox::information( this, i18n( "Nothing to spell check." ) );
        return;
    }

    if( !d->m_spellChecker ) {
        d->m_spellChecker = new Sonnet::BackgroundChecker( this );
        d->m_spellDialog = new Sonnet::Dialog( d->m_spellChecker, this );
        connect( d->m_spellDialog, SIGNAL( done( const QString& ) ), this, SLOT( continueSpelling() ) );
        connect( d->m_spellDialog, SIGNAL( misspelling( const QString&, int ) ), this, SLOT( misspelling( const QString&, int ) ) );
        connect( d->m_spellDialog, SIGNAL( replace( const QString&, int, const QString& ) ), this, SLOT( spellingReplace( const QString&, int, const QString& ) ) );
    }

    d->m_spellColumn = language * KEduVocVocabularyModel::EntryColumnsMAX;
    d->m_spellRow = -1;
    if( d->m_spellColumn < 0 ) {
        return;
    }

    d->m_spellCheck = spellCheck;
    QString locale = d->m_doc->identifier( language ).locale();
    QString spellCode = d->m_spellCheck.isEmpty() ? locale : d->m_spellCheck;
    d->m_spellChecker->changeLanguage( spellCode );
    if( !d->m_spellChecker->speller().isValid() ) {
        qDebug() << "Invalid Language, popup here!";
        KNotification::event(KNotification::Warning, i18nc( "@title of a popup", "No Spell Checker Available" ), i18nc( "@popupmessage", "Either the language set up is incorrect or no spellchecker was installed for this locale: %1.", locale ) );
    }
    d->m_spellDialog->show();
    continueSpelling();
}

void KEduVocVocabularyView::continueSpelling()
{
    qDebug() << "Check spelling: " << d->m_spellRow << d->m_spellColumn;
    ++d->m_spellRow;
    while( d->m_spellRow < d->m_model->rowCount() ) {
        QModelIndex index = d->m_model->index( d->m_spellRow, d->m_spellColumn );
        qDebug() << "  " << d->m_model->data( index ).toString();
        if( !d->m_model->data( index ).toString().isEmpty() ) {
            d->m_spellDialog->setBuffer( d->m_model->data( index ).toString() );
            break;
        } else {
            ++d->m_spellRow;
        }
    }
}

void KEduVocVocabularyView::selectIndex( const QModelIndex &newIndex )
{
    selectionModel()->select( newIndex, QItemSelectionModel::ClearAndSelect );
    selectionModel()->setCurrentIndex( newIndex, QItemSelectionModel::ClearAndSelect );
    scrollTo( newIndex );
}

bool KEduVocVocabularyView::eventFilter( QObject* obj, QEvent* event, bool smartAppend )
{
    if( event->type() == QEvent::KeyPress && smartAppend ) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>( event );
        if( keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
            if( selectionModel()->currentIndex().row() == d->m_model->rowCount() - 1 ) {
                appendEntry();
            }
        }
    }
    // standard event processing
    return QObject::eventFilter( obj, event );
}

void KEduVocVocabularyView::misspelling( const QString & word, int start )
{
    Q_UNUSED( word )
    Q_UNUSED( start )
    QModelIndex index = d->m_model->index( d->m_spellRow, d->m_spellColumn );
    selectIndex( index );
}

void KEduVocVocabularyView::spellingReplace( const QString & oldWord, int start, const QString & newWord )
{
    qDebug() << oldWord << start << newWord;
    QModelIndex index = d->m_model->index( d->m_spellRow, d->m_spellColumn);
    QString data = index.data().toString();
    QString newData = data.replace( start, oldWord.length(), newWord );
    qDebug() << "Changing " << data << " to " << newData;
    d->m_model->setData( index, newData );
}

QModelIndexList KEduVocVocabularyView::getSelectedIndexes() const
{
    return selectionModel()->selectedIndexes();
}
