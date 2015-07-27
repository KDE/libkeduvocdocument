/***************************************************************************
    Copyright 2008 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCLANGUAGEPROPERTIES_H
#define KEDUVOCLANGUAGEPROPERTIES_H

#include <KPageDialog>

#include <keduvocdocument_export.h>
#include <keduvoclanguagepropertiespage.h>

class KEduVocDocument;
class KEduVocLanguagePropertiesPage;

class KEDUVOCDOCUMENT_EXPORT KEduVocLanguageProperties : public KPageDialog
{
    Q_OBJECT
public:
    KEduVocLanguageProperties( KEduVocDocument* doc, QWidget* parent );
    ~KEduVocLanguageProperties();

public slots:
    virtual void accept();

    // connect these with the signals from KEduVocLanguagePropertiesPage
    void loadLanguageSettings( QString locale );
    void loadEditorFont( QString locale );
    void loadPracticeFont( QString locale );
    void loadKeyboardLayout( QString locale );
    void loadSpellChecker( QString locale );
    void saveEditorFont( QString locale, QFont font );
    void savePracticeFont( QString locale, QFont font );
    void saveKeyboardLayout( QString locale, QString keyboardLayout );
    void saveSpellChecker( QString locale, QString spellChecker );
    void storeSettings( QString locale );

signals:

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

private slots:
    void slotAppendIdentifier();
    void slotDeleteIdentifier();
    void pageNameChanged( const QString& newName );

private:
    KPageWidgetItem* createPage( int identifierIndex );

    class Private;
    Private * const d;
};

#endif
