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


#ifndef KEDUVOCLANGUAGEPROPERTIESPAGE_H
#define KEDUVOCLANGUAGEPROPERTIESPAGE_H

#include <keduvocdocument_export.h>

#include <QWidget>

class KEduVocDocument;

class KEDUVOCDOCUMENT_EXPORT KEduVocLanguagePropertiesPage : public QWidget
{
    Q_OBJECT
public:
    KEduVocLanguagePropertiesPage( KEduVocDocument *doc, int identifierIndex, QWidget *parent = 0 );
    ~KEduVocLanguagePropertiesPage();

    int languageIdentifierIndex();
    void setLanguageIdentifierIndex( int newIndex );

    QFont editorFont();
    void setEditorFont( QFont editorFont );

    QFont practiceFont();
    void setPracticeFont( QFont practiceFont );

    QString keyboardLayout();
    void setKeyboardLayout( QString keyboardLayout );

    QString spellChecker();
    void setSpellChecker( QString spellChecker );

public slots:
    void accept();
    void downloadGrammar();

signals:
    void nameChanged( const QString );

    /**
     * Sample corresponding slot for calling class :
     * 
     * void loadLanguageSettings( QString locale) {
     *     LanguageSettings settings( locale );
     *     settings.load();
     * }
     */
    void languageSettingsChanged( QString );

    /**
     * Sample corresponding slot for calling class :
     * 
     * void loadEditorFont( QString locale ) {
     *     LanguageSettings settings( locale );
     *     settings.load();
     *     languagePropertiesPage.setEditorFont( settings.editorFont() );
     * }
     * 
     * Where languagePropertiesPage is a KEduVocLanguagePropertiesPage object
     */
    void editorFontChanged( QString );

    /**
     * Sample corresponding slot for calling class :
     * 
     * void loadPracticeFont( QString locale ) {
     *     LanguageSettings settings( locale );
     *     settings.load();
     *     languagePropertiesPage.setPracticeFont( settings.practiceFont() );
     * }
     * 
     * Where languagePropertiesPage is a KEduVocLanguagePropertiesPage object
     */
    void practiceFontChanged( QString );

    /**
     * Sample corresponding slot for calling class :
     * 
     * void loadKeyboardLayout( QString locale ) {
     *     LanguageSettings settings( locale );
     *     settings.load();
     *     languagePropertiesPage.setKeyboardLayout( settings.keyboardLayout() );
     * }
     * 
     * Where languagePropertiesPage is a KEduVocLanguagePropertiesPage object
     */
    void keyboardLayoutChanged( QString );

    /**
     * Sample corresponding slot for calling class :
     * 
     * void loadSpellChecker( QString locale ) {
     *     LanguageSettings settings( locale );
     *     settings.load();
     *     languagePropertiesPage.setSpellChecker( settings.spellChecker() );
     * }
     * 
     * Where languagePropertiesPage is a KEduVocLanguagePropertiesPage object
     */
    void spellCheckerChanged( QString );

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
    void localeChanged( const QString& );

    // pronouns/conjugations
    void updateCheckBoxes();

    // tenses
    void slotDeleteTense();
    void slotNewTense();
    void slotTenseChosen(int);
    void slotModifyTense();

private:
    void updateListBox(int start);

    // initialize widgets with contents
    void loadGrammarFromDocument();

    class Private;
    Private * const d;
};

#endif
