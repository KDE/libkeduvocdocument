/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef KEDUVOCEDITORDOCUMENT_H
#define KEDUVOCEDITORDOCUMENT_H

#include <keduvocdocument_export.h>
#include <keduvocdocument.h>

#include <QObject>

class QTimer;
class QCheckBox;
class KEduVocLanguagePropertiesPage;

/**
    @author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class KEDUVOCDOCUMENT_EXPORT KEduVocEditorDocument : public QObject
{
    Q_OBJECT
public:
    KEduVocEditorDocument( QWidget* parent );
    ~KEduVocEditorDocument();


    KEduVocDocument *document();

    void setTitle( const QString& title );

    /** Enable/disable the timed auto backup
     @todo Move autobackup functionality into libkdeedu which is responsible for
    file opening/closing and locking already.*/
    void enableAutoBackup( bool enable );
    //void fetchGrammar(int languageIndex);

public slots:
    /** open a new application window */
    void slotFileNew();

    /** open a document */
    void slotFileOpen();

    /** opens a file from the recent files menu */
    void slotFileOpenRecent( const QUrl& url );
    /** open a downloaded (knewstuff/get hot new stuff) document */
    void openGHNS();
    /** download new vocabularies */
    void slotGHNS();
    /** upload the current file */
    void uploadFile();

    /** Opens the given url, displays an error message and returns false on failure */
    bool open(const QUrl &);

    /** close the document*/
    void close();
    /** When quitting, ask for confirmation if the doc has not been saved */
    bool queryClose();


    /** save a document */
    void save();
    void saveAs( QUrl file = QUrl() );

    void exportDialog();

    void newDocument(bool wizard);

    /** General doc properties like title, author etc */
    void documentProperties();
    void languageProperties();

    /** Slots for KEduVocLanguageProperties signals */
    void loadLanguageSettings( QString locale );
    void loadEditorFont( QString locale, KEduVocLanguagePropertiesPage* page );
    void loadPracticeFont( QString locale, KEduVocLanguagePropertiesPage* page );
    void loadKeyboardLayout( QString locale, KEduVocLanguagePropertiesPage* page );
    void loadSpellChecker( QString locale, KEduVocLanguagePropertiesPage* page );
    void saveEditorFont( QString locale, QFont font );
    void savePracticeFont( QString locale, QFont font );
    void saveKeyboardLayout( QString locale, QString keyboardLayout );
    void saveSpellChecker( QString locale, QString spellChecker );
    void storeSettings( QString locale );

    bool autoBackup();
    void setAutoBackup( bool autoBackup );

    QString separator();
    void setSeparator( QString separator );

    bool autoSave();
    void setAutoSave( bool autoSave );

    void setQueryClose( bool queryClose );

    int backupTime();
    void setBackupTime( int backupTime );

signals:
    /** Emitted when the document pointer is changed.
     @todo Stop using documentChanged(0) as a replacement for destoyed in editor classes.**/
    void documentChanged( KEduVocDocument *newDocument );
    void languagesChanged();
    void statesNeedSaving();

    /**
     * Sample corresponding slot for calling class :
     * 
     * void loadLanguageSettings( QString locale ) {
     *     LanguageSettings settings( locale );
     *     settings.load();
     * }
     */
    void languageSettingsChanged( QString );

    /**
     * Sample corresponding slot for calling class :
     * 
     * void loadEditorFont( QString locale, KEduVocLanguagePropertiesPage* page ) {
     *     LanguageSettings settings( locale );
     *     settings.load();
     *     page->setEditorFont( settings.editorFont() );
     * }
     */
    void editorFontChanged( QString, KEduVocLanguagePropertiesPage* );

    /**
     * Sample corresponding slot for calling class :
     * 
     * void loadPracticeFont( QString locale, KEduVocLanguagePropertiesPage* page ) {
     *     LanguageSettings settings( locale );
     *     settings.load();
     *     page->setPracticeFont( settings.practiceFont() );
     * }
     */
    void practiceFontChanged( QString, KEduVocLanguagePropertiesPage* );

    /**
     * Sample corresponding slot for calling class :
     * 
     * void loadKeyboardLayout( QString locale, KEduVocLanguagePropertiesPage* page ) {
     *     LanguageSettings settings( locale );
     *     settings.load();
     *     page->setKeyboardLayout( settings.keyboardLayout() );
     * }
     */
    void keyboardLayoutChanged( QString, KEduVocLanguagePropertiesPage* );

    /**
     * Sample corresponding slot for calling class :
     * 
     * void loadSpellChecker( QString locale, KEduVocLanguagePropertiesPage* page ) {
     *     LanguageSettings settings( locale );
     *     settings.load();
     *     page->setSpellChecker( settings.spellChecker() );
     * }
     */
    void spellCheckerChanged( QString, KEduVocLanguagePropertiesPage* );

    /**
     * Sample corresponding slot for calling class :
     * 
     * void saveEditorFont( QString locale, QFont font ) {
     *     LanguageSettings settings( locale );
     *     settings.load();
     *     settings.setEditorFont( font );
     * }
     */
    void storeEditorFont( QString, QFont );

    /**
     * Sample corresponding slot for calling class :
     * 
     * void savePracticeFont( QString locale, QFont font ) {
     *     LanguageSettings settings( locale );
     *     settings.load();
     *     settings.setPracticeFont( font );
     * }
     */
    void storePracticeFont( QString, QFont );

    /**
     * Sample corresponding slot for calling class :
     * 
     * void saveKeyboardLayout( QString locale, QString keyboardLayout ) {
     *     LanguageSettings settings( locale );
     *     settings.load();
     *     settings.setKeyboardLayout( keyboardLayout );
     * }
     */
    void storeKeyboardLayout( QString, QString );

    /**
     * Sample corresponding slot for calling class :
     * 
     * void saveSpellChecker( QString locale, QString spellChecker ) {
     *     LanguageSettings settings( locale );
     *     settings.load();
     *     settings.setSpellChecker( spellChecker );
     * }
     */
    void storeSpellChecker( QString, QString );

    /**
     * Sample corresponding slot for calling class :
     * 
     * void storeSettings( QString locale ) {
     *     LanguageSettings settings( locale );
     *     settings.load();
     *     settings.save();
     * }
     */
    void saveSettings( QString );


    /**
     * Sample corresponding slot for calling class
     * 
     * void loadAutoBackup()
     * {
     *     editorDoc->setAutoBackup( Prefs::autoBackup() );
     * }
     * 
     * Where editorDoc is a KEduVocEditorDocument object
     */
    void autoBackupChanged();

    /**
     * Call ParleyMainWindow::showEditor in corresponding slot
     */
    void showEditor();

    /** Sample corresponding slot for calling class (in ParleyMainWindow)
     * 
     * void setCheckBox( QCheckBox* box )
     * {
     *     box->setChecked( currentComponent() != ParleyMainWindow::EditorComponent );
     * }
     */
    void checkBoxChanged( QCheckBox* box );

    /**
     * Call ParleyMainWindow::showPracticeConfiguration in corresponding slot
     */
    void showPracticeConfiguration();

    /**
     * Sample corresponding slot for calling class
     * 
     * void loadSeparator()
     * {
     *     editorDoc->setSeparator( Prefs::separator() );
     * }
     * 
     * Where editorDoc is a KEduVocEditorDocument object
     */
    void separatorChanged();

    /**
     * Sample corresponding slot for calling class (in ParleyMainWindow)
     * 
     * void loadRecentFile( const QUrl& url, QString title )
     * {
     *     addRecentFile( url, title );
     * }
     */
    void recentFileChanged( const QUrl& url, QString title );

    /**
     * Call Prefs::self()->save() in corresponding slot
     */
    void prefsSelfSave();

    /**
     * Sample corresponding slot for calling class
     * 
     * void loadAutoSave()
     * {
     *     editorDoc->setAutoSave( Prefs::autoSave() );
     * }
     * 
     * Where editorDoc is a KEduVocEditorDocument object
     */
    void autoSaveChanged();

    /**
     * Sample corresponding slot for calling class (in ParleyMainWindow)
     * 
     * void loadQueryClose()
     * {
     *     editorDoc->setQueryClose( queryClose() );
     * }
     * 
     * Where editorDoc is a KEduVocEditorDocument object
     */
    void queryCloseChanged();

    /**
     * Call ParleyMainWindow::updateRecentFilesModel() in corresponding slot
     */
    void updateRecentFilesModel();

    /**
     * Sample corresponding slot for calling class (in ParleyMainWindow)
     * 
     * void loadBackupTime()
     * {
     *     editorDoc->setBackupTime( Prefs::backupTime()) );
     * }
     * 
     * Where editorDoc is a KEduVocEditorDocument object
     */
    void backupTimeChanged();

private:
    void initializeDefaultGrammar( KEduVocDocument *doc );
    void setDefaultDocumentProperties( KEduVocDocument *doc );

    class Private;
    Private * const d;
};

#endif
