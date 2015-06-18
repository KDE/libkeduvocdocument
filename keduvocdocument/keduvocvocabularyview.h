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

#ifndef KEDUVOCVOCABULARYVIEW_H
#define KEDUVOCVOCABULARYVIEW_H

#include <sonnet/dialog.h>

// Qt
#include <QTableView>
#include <QMap>

#include <keduvocdocument_export.h>

// KDE
class KActionCollection;
class KActionMenu;
class QAction;
class QUrl;

// KEduVocDocument
class KEduVocExpression;
class KEduVocDocument;
class KEduVocVocabularyFilter;
class KEduVocTranslator;


namespace Editor
{
/* FIXME Reimplement this after editor.h is added to keduvoc, and change the constructor accordingly, removing KActionCollection
 * class EditorWindow;
 */
class KEduVocVocabularyDelegate;

class KEDUVOCDOCUMENT_EXPORT KEduVocVocabularyView : public QTableView
{
    Q_OBJECT
public:
    /**
     * @param automaticTranslation corresponding to Prefs::automaticTranslation()
     */

    KEduVocVocabularyView( QWidget *parent, KActionCollection* actionCollection, bool automaticTranslation );
    ~KEduVocVocabularyView();

    KActionMenu* columnsActionMenu();

    void setFilter( KEduVocVocabularyFilter * model );

    /** Save column visibility settings 
     * Example Usage:
     * foo.saveColumnVisibility();
     * DocumentSettings ds( foo.document()->url().url() );
     * ds.setVisibleColumns( foo.visibleColumns() );
     * ds.save();
     * 
     * Where foo is a KEduVocVocabularyView object
     */
    void saveColumnVisibility() const;

    /** Return Column Visibility for model */
    QList <int> visibleColumns();

    void setVisibleColumns( QList <int> visibleColumns );

    QModelIndexList getSelectedIndexes() const;

    /** Set m_automaticTranslation to @param automaticTranslation */
    void setAutomaticTranslation( bool automaticTranslation );

    /** Return current m_automaticTranslation */
    bool automaticTranslation();

    QByteArray state();

    void setState( QByteArray state );

    KEduVocDocument* document();

    void setSpellCheck( QString spellCheck );

    QString spellCheck();

public slots:
    /**
     * @param visibleColumns corresponding to DocumentSettings::visibleColumns()
     * Example Usage:
     * DocumentSettings ds( foo.document()->url().url() );
     * d s.load();
     * visibleColumns = ds.visibleColumns();
     * Where foo is a KEduVocVocabularyView object
     * 
     * @param state
     * Example Usage (in Parley):
     * 
     * KConfig parleyConfig("parleyrc");
     * KConfigGroup documentGroup(&parleyConfig, "Document " + foo.document()->url().url()); 
     * QByteArray state = documentGroup.readEntry("VocabularyColumns", QByteArray());
     * 
     * QByteArray saveState = foo.horizontalHeader()->saveState();
     * documentGroup.writeEntry("VocabularyColumns", foo.horizontalHeader()->saveState());
     * 
     * Where foo is a KEduVocVocabularyView object
     */
    void reset( QList <int> visibleColumns, QByteArray state );
    void appendEntry();

    /** Delete the currently selected entries */
    void deleteSelectedEntries( bool askConfirmation = true );

    /** put the marked text/object into the clipboard*/
    void slotEditCopy();

    /** Cut an entry */
    void slotCutEntry();

    /** paste the clipboard into the document*/
    void slotEditPaste();

    /**
     * Append a char to the last edited entry (used to add the phonetic stuff).
     * @param c the char
     */
    void appendChar( const QChar & c );

    void setDocument( KEduVocDocument * doc );

    void setTranslator( KEduVocTranslator* translator );

    /**
     * @param spellCheck corresponding to LanguageSettings::spellChecker
     * Example Usage:
     * QString locale = foo.document()->identifier( language ).locale();
     * LanguageSettings settings(locale);
     * QString spellCheck = settings.spellChecker();
     */
    void checkSpelling( int language, QString spellCheck );

    /**
     * Show the vocabulary columns dialog to enable or disable the columns in the view
     */
    void slotShowVocabularyColumnsDialog();

signals:
    void translationChanged( KEduVocExpression*, int );

private slots:
    void slotCurrentChanged( const QModelIndex& current, const QModelIndex& previous );
    void slotSelectionChanged( const QItemSelection&, const QItemSelection& );

    void continueSpelling();
    void misspelling( const QString &word, int start );
    void spellingReplace( const QString& oldWord, int start, const QString &newWord );

private:
    class Private;
    Private * const d;

    void selectIndex( const QModelIndex &index );
    // trap enter presses at the end of the document to add a new entry instead of moving to the first cell
    
    /**
     * @param smartAppend corresponds to Prefs::smartAppend()
     */
    bool eventFilter( QObject *obj, QEvent *event, bool smartAppend );

    // Make this private to avoid inadvertent use. Instead use setFilter() which is public.
    virtual void setModel( QAbstractItemModel *model ) {
        Q_UNUSED( model )
    }
};

}

#endif
