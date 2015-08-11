/***************************************************************************
    Copyright 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    Copyright 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2007-2009 Frederik Gladhorn <gladhorn@kde.org>
    Copyright 2008 Daniel Laidig <d.laidig@gmx.de>
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCEDITOR_H
#define KEDUVOCEDITOR_H

//#include "scripts/scripting/parley.h"

#include <keduvocvocabularymodel.h>
#include <keduvocvocabularyfilter.h>
#include <keduvoceditordocument.h>
#include <keduvoctranslator.h>

#include <keduvocdocument_export.h>

#include <KXmlGuiWindow>

class QSignalMapper;
class QLineEdit;
class KActionMenu;
class KRecentFilesAction;
class KEduVocScriptManager;
class QAction;

class KEduVocEditorDocument;

///@brief contains objects for the editor main window

// Models around the Collection / KEduVocDocument
class KEduVocLessonModel;
class KEduVocVocabularyModel;
class KEduVocVocabularyFilter;
class KEduVocWordClassModel;

namespace Editor
{
class KEduVocVocabularyView;
class KEduVocLessonView;
class KEduVocWordTypeView;
class KEduVocInflectionWidget;
class KEduVocComparisonWidget;
class KEduVocSummaryWordWidget;
class KEduVocLatexWidget;
class KEduVocSynonymWidget;

class KEDUVOCDOCUMENT_EXPORT KEduVocEditorWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:

    enum Action {
        FileNew,
        FileOpen, FileOpenDownloaded,
        FileSave, FileSaveAs,
        FileExport,
        FileProperties,
        FileClose, FileQuit,
        Preferences,
        LanguagesProperties,
        RemoveGrades,
        CheckSpelling,
        ToggleShowSublessons,
        AutomaticTranslation,
        StartPractice, ConfigurePractice, ExportPracticeResults,
        EnterEditMode,
        ToggleSearchBar, SearchVocabulary,
        ShowScriptManager
    };

    /**
     * @param parent The application mainwindow
     * @param editorDocument The KEduVocEditorDocument object of the application mainwindow
     */
    KEduVocEditorWindow( QWidget* parent, KEduVocEditorDocument* editorDocument );
    ~KEduVocEditorWindow();

    /**
     * setup the action (menus etc)
     */
    void initActions();

    /** setup the main model*/
    void initModel();

    /** setup the main view*/
    void initView();

    void initDockWidgets();

    /** This will look at the lesson list and also the combo box to determine what should be displayed in the table. */
    void updateTableFilter();

    /**
     * Return m_visibleColumns
     */
    QList <int> visibleColumns();

    /**
     * Return m_entriesPerLesson
     */
    int entriesPerLesson();

    /**
     * Return m_subLessonEntries
     */
    bool subLessonEntries();

    /**
     * Return m_automaticTranslation
     */
    bool automaticTranslation();

    /**
     * Return m_showSearch
     */
    bool showSearch();

    /**
     * Return m_spellChecker
     */
    QString spellChecker();

    /**
     * Return m_separator
     */
    QString separator();

public slots:
    /**
     * Load enabled scripts (from configuration parleyrc)
     */
    void initScripts();

    /**
     * Edit languages contained in the document.
     * This includes adding/removing languages,
     * editing articles, personal pronouns and tenses.
     */
    void slotLanguagesChanged();

    void slotConfigShowSearch();

    /**
     *  Display script manager (open a new window / or Settings window)
     */
    void slotShowScriptManager();

    void applyPrefs();

    /**
     * Removes all grading information from the current document
     */
    void removeGrades();

    /** when closing the application, save the editor's state */
    void saveState();

    /**
     * Set the current doc (after creating a new one or opening a file)
     */
    void updateDocument( KEduVocDocument *doc );

    /**
     * DBus method for adding words by external apps
     */
    void addWordWithTranslation( const QStringList &w );

    /**
      * Set/Unset Automatic Translation in Prefs
      */
    void setAutomaticTranslation( bool v );

    /**
     * Set m_vocabularyView->m_spellCheck
     */
    void setSpellCheck( int language );

    // KEduVocScriptManager slots

    /**
     * Invoke unplugActionList( list )
     */
    void unplugAction( QString list );

    /**
     * Invoke plugActionList( list, scriptActions )
     */
    void plugAction( QString list, QList <QAction*> scriptActions );

    /**
     * Add a new action to actionCollection()
     */
    void editorAddActionCollection( const QString& name, QAction* action );

    /**
     * Set m_visibleColumns 
     */
    void setVisibleColumns( QList <int> visibleColumns );

    /**
     * Set m_entriesPerLesson
     */
    void setEntriesPerLesson( int entriesPerLesson );

    /**
     * Set m_subLessonEntries
     */
    void setSubLessonEntries( bool subLessonEntries );

    /**
     * Set m_automaticTranslation
     */
    void storeAutomaticTranslation( bool automaticTranslation );

    /**
     * Set m_spellChecker
     */
    void setSpellChecker( QString spellChecker );

    /**
     * Set m_separator
     */
    void setSeparator( QString separator );

    void setShowSearch( bool showSearch );

    QAction* createAction( Action id, const QObject *recvr, const char *slot, KActionCollection *parent );

    KRecentFilesAction* createRecentFilesAction( const QObject *recvr, const char *slot, QObject *parent );

    QAction* createDownloadAction( const QObject *recvr, const char *slot, KActionCollection *collection );

    QAction* createUploadAction( const QObject *recvr, const char *slot, KActionCollection *collection );


private slots:

    /** Make the search bar visible and focus it */
    void startSearch();

signals:
    void signalSetData( const QList <int>& entries, int currentTranslation );

    /**
     * Sample corresponding slot in application mainwindow class
     * 
     * void changeVisibleColumns( KEduVocDocument *doc )
     * {
     *      DocumentSettings ds( doc->url().url() );
     *      ds.load();
     *      QList <int> vc = ds.visibleColumns();
     *      m _editor->setVisibleColumns( vc );
     *      editorWindow.setVisibleColumns( ds.visibleColumns() );
     * }
     * 
     * Where editorWindow is a EditorWindow object
     */
    void visibleColumnsChanged( KEduVocDocument *doc );

    /**
     * Sample corresponding slot in application mainwindow class
     * 
     * void storeAutomaticTranslation( bool v )
     * {
     *      Prefs::setAutomaticTranslation( v );
     * }
     */
    void saveAutomaticTranslation( bool v );

    /**
     * Sample corresponding slot in application mainwindow class
     * 
     * void changeEntriesPerLesson()
     * {
     *      editorWindow.setEntriesPerLesson( Prefs::entriesPerLesson() );
     * }
     * 
     * Where editorWindow is a EditorWindow object
     */
    void entriesPerLessonChanged();

    /**
     * Sample corresponding slot in application mainwindow class
     * 
     * void changeSubLessonEntries()
     * {
     *      editorWindow.setSubLessonEntries( Prefs::showSublessonentries() );
     * }
     * 
     * Where editorWindow is a EditorWindow object
     */
    void subLessonEntriesChanged();

    /**
     * Sample corresponding slot in application mainwindow class
     * 
     * void setAutomaticTranslation()
     * {
     *      editorWindow.saveAutomaticTranslation( Prefs::automaticTranslation() );
     * }
     * 
     * Where editorWindow is a EditorWindow object
     */
    void automaticTranslationChanged();

    /**
     * Sample corresponding slot in application mainwindow class
     * 
     * void setShowSearch()
     * {
     *      editorWindow.setShowSearch( Prefs::showSearch() );
     * }
     * 
     * Where editorWindow is a EditorWindow object
     */
    void showSearchChanged();

    /**
     * Sample corresponding slot in application mainwindow class
     * 
     * void setSpellChecker( QString locale )
     * {
     *      LanguageSettings settings(locale);
     *      editorWindow.setSpellChecker( settings.spellChecker() );
     * }
     * 
     * Where editorWindow is a EditorWindow object
     */
    void spellCheckerChanged( QString locale );

    /**
     * Sample corresponding slot in application mainwindow class
     * 
     * void storeShowSearch( bool v )
     * {
     *      Prefs::setShowSearch( v );
     * }
     */
    void saveShowSearch( bool v );

    /**
     * Sample corresponding slot in application mainwindow class
     * 
     * void setSeparator()
     * {
     *      editorWindow.setSeparator( Prefs::separator() );
     * }
     * 
     * Where editorWindow is a EditorWindow object
     */
    void separatorChanged();

private:
    class Private;
    Private * const d;

    friend class ::KEduVocEditorDocument;
    friend class Scripting::KEduVocDocument;
    friend class ::KEduVocScriptManager;
};

}

#endif // EDITOR_H
