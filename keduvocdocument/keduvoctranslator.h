/***************************************************************************

    Copyright 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef KEDUVOCTRANSLATOR_H
#define KEDUVOCTRANSLATOR_H

#include <QMap>
#include <QStringList>
#include <QStandardPaths>
#include <QObject>

#include <keduvocdocument.h>
#include <keduvocvocabularymodel.h>
#include <keduvocdocument_export.h>


/**
 Keeps the translated words
 
 @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
 */
class KEDUVOCDOCUMENT_EXPORT KEduVocTranslator
{
public:
    KEduVocTranslator();
    KEduVocTranslator(QObject * parent);

    ~KEduVocTranslator();

    /**
     * Stores the translation of @p word from language @p fromLanguage, to language @p toLanguage.
     * @param word word that was translated
     * @param fromLanguage language of @p word
     * @param toLanguage language of @p translation
     * @param translation translation of @p word
     */
    void addTranslation(QString word, QString fromLanguage, QString toLanguage, QString translation);

    /**
     * Returns a QStringList with all the translations of @p word from @p fromLanguage to @p toLanguage.
     * This function will call the translateWord function of the translation scripts if this word
     * wasn't translated before.
     * @param word
     * @param fromLanguage
     * @param toLanguage
     * @return QStringList with the translations (or an empty QStringList if no translations found)
     */
    QSet<QString>* getTranslation(QString word, QString fromLanguage, QString toLanguage);

private:
    class Private;
    Private * const d;
};


/*namespace Editor
{
    class EditorWindow;
}*/

namespace Scripting
{
    /**
     * @class KEduVocDocument
     * @brief KEduVocDocument scripting class
     *
     * KEduVocDocument class is the main entry point of KEduVocDocument scripting classes. Through it you can access the Document class (KEduVocDocument.doc or KEduVocDocument.document) which provides functionality for viewing/modifying a %KEduVocDocument document , that means access lessons, entries, document languages etc.
     *
     * Here follows a list of possible usages of KEduVocDocument class:
     *
     * - Add a new Action to the script menu (see KEduVocDocument::newAction() function)
     * - Add a new translation script (see KEduVocDocument::translateWord() signal)
     * - Have direct access to the active lesson (see KEduVocDocument::activeLesson property)
     * - Have access to various enumerations (see KEduVocDocument::Number, KEduVocDocument::Case, KEduVocDocument::Person, KEduVocDocument::Gender and KeduVocDocument::Definiteness enumerations)
     * - Create a new %KEduVocDocument %Document (see KEduVocDocument::newDocument() function)
     *
     * Signals and Slots: To connect a script function (slot) to a signal you <b> just define a function with the same name as the signal </b> or use the KEduVocDocument.connect function:
     * @code
     * #how to connect a function to a signal (example with KEduVocDocument::translateWord() signal)
     * def translateFromInternet(word,fromLang,toLang):
     *     print "Translating from Internet!!.."
     *
     * KEduVocDocument.connect("translateWord(const QString &,const QString &,const QString &)",translateFromInternet)
     * @endcode
     *
     *   @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
     */
    class KEDUVOCDOCUMENT_EXPORT KEduVocDocument : public QObject
    {
        Q_OBJECT
        /// Read-only property of the active document
        Q_PROPERTY(QObject * document READ getDocument)
        /// Abreviation of document property (same as KEduVocDocument.document)
        Q_PROPERTY(QObject * doc READ getDocument)
        /// Currently active lesson
        //Q_PROPERTY(QObject * activeLesson READ activeLesson)
        //             Q_ENUMS ( Number Case Person Gender Definiteness )
        Q_ENUMS(Flags)

    public:

        enum Flags {
            // This is used for both empty flags and to denote no flags of the correct type were set.
            NoInformation = 0x0,

            // Gender
            Masculine = 0x1,
            Feminine = 0x2,
            Neuter = 0x4,

            // Plurality
            Singular = 0x10,
            Dual = 0x20,
            Plural = 0x40,

            // Part of Speech
            Verb = 0x100,
            Noun = 0x200,
            Pronoun = 0x400,
            Adjective = 0x800,
            Adverb = 0x1000,
            Article = 0x2000,
            Conjunction = 0x4000,

            // Person
            First = 0x10000,
            Second = 0x20000,
            Third = 0x40000,

            // Declension Case
            Nominative = 0x80000,
            Genitive = 0x100000,
            Dative = 0x200000,
            Accusative = 0x400000,
            Ablative = 0x800000,
            Locative = 0x1000000,
            Vocative = 0x2000000,

            // Other assorted flags
            Definite = 0x4000000, // The article is definite
            Indefinite = 0x8000000, // The article is indefinite
            Regular = 0x10000000,
            Irregular = 0x20000000
        };

        /**
         * @param doc Represents a KEduVococument object
         *      For example, when using this function in Parley, pass the doc value like this:
         *      m_editor->m_mainWindow->parleyDocument()->document()
         *      where m_editor is a EditorWindow object
         * @param vocabularyModel Represents the VocabularyModel object for the KEduVocDocument
         *      For example, when using this function in Parley, pass the vocabularyModel value like this:
         *      m_editor->m_vocabularyModel
         *      where m_editor is a EditorWindow object
         * @param separator Represents Prefs::separator()
         */
        KEduVocDocument( ::KEduVocDocument * doc, KEduVocVocabularyModel * vocabularyModel, QString separator );

        ~KEduVocDocument();

        /*
         * Emits translateWord signal so the script function connected to it translate the @p word
         * @param word Word to translate
         * @param fromLanguage The language of @p word to translate from
         * @param toLanguage The language you want to translate to
         */
        void callTranslateWord(const QString & word, const QString& fromLanguage, const QString& toLanguage);

        //sets the translator object
        //             void setTranslator ( Translator* translator ) { m_translator = translator; }
        KEduVocTranslator * translator() {
            return m_translator;
        }

        //Returns the active document
        QObject* getDocument() {
            return m_doc;
        }

        //Returns the active lesson
        //KEduVocLesson* activeLesson();

        QString separator()
        {
            return m_separator;
        }

        void setSeparator( QString v )
        {
            m_separator = v;
        }

    public Q_SLOTS:

        QStringList dataDirs() {
            return QStandardPaths::standardLocations(QStandardPaths::DataLocation);
        }

        QStringList pluginDirs() {
            QStringList basedirs(QStandardPaths::standardLocations(QStandardPaths::DataLocation));
            QStringList ret;
            foreach ( const QString dir , basedirs){
                ret << (dir + "/plugins");
            }

            return ret;
        }

        /** close the document*/
        void close();

        /** Returns a list of Expression objects (the selected entries of the active lesson) 
            Usage example:
                While using in Parley, pass the 'indexes' value like this
                indexes = m_editor->m_vocabularyView->getSelectedIndexes();
                where m_editor is a EditorWindow object
            TODO  Add a KEduVocVocabularyView object to this class (after is is moved to keduvoc from parley) and access getSelectedIndexes() from there
         */
        QVariantList selectedEntries( QModelIndexList indexes );

        /** Returns a list of Translation objects (the selected translations of the active lesson) 
            Usage example:
                While using in Parley, pass the 'indexes' value like this
                indexes = m_editor->m_vocabularyView->getSelectedIndexes();
                where m_editor is a EditorWindow object
            TODO  Add a KEduVocVocabularyView object to this class (after is is moved to keduvoc from parley) and access getSelectedIndexes() from there
         */
        QVariantList selectedTranslations( QModelIndexList indexes );

        /**
         * Adds the found @p translation of the @p word from language @p fromLanguage to language @p toLanguage to %KEduVocDocument translations to be used for translating lesson entries (or anything else). This function is ment to be used by scripts to add a translation of a word by parsing either online or offline dictionaries.
         *
         * @code
         * #example usage of addTranslation function
         * import KEduVocDocument
         * #function called by KEduVocDocument whenever a translation of a word is needed
         * def translateWord(word,fromLang,toLang):
         *     <<look for the word translation>>
         *     KEduVocDocument.addTranslation(word,fromLang,toLang,foundWord)
         *     <<look for more translations>>
         * @endcode
         *
         * @param word Translated word
         * @param fromLanguage From language
         * @param toLanguage To language
         * @param translation %Translation of word
         */
        void addTranslation(QString word, QString fromLanguage, QString toLanguage, QString translation);
        /**
         * Returns a list of all available locales (to be used by the scripts)
         * @return
         */
        QStringList locales();

        /**
         * Gives the language name of the given @p locale
         * @param locale Language locale
         * @return Language name
         */
        QString localeName(QString locale);

        /// @todo Make this function working (not very important function)
        /*
         * Open the KEduVocDocument Document @p filename
         * Usage:
         * @code
         * import KEduVocDocument
         * KEduVocDocument.open("Vocab/MyVocab.kvtml")
         * @endcode
         * @param file KEduVocDocument Document file path (ex. /home/kde-user/MyVocab.kvtml)
         */
        bool open(QString filename);

        /**
         * Creates a new document and returns a reference to it
         *
         * @code
         * #how to create a new document, add lessons, add entries and save it to a kvtml file
         * import KEduVocDocument
         *
         * #create new document
         * doc = KEduVocDocument.newDocument()
         * doc.title = "New document"
         *
         * #set identifiers
         * doc.appendNewIdentifier("English","en_US")
         * doc.appendNewIdentifier("French","fr")
         *
         * #lessons
         * l1 = doc.newLesson("Lesson1")
         * doc.rootLesson.appendChildLesson(l1)
         *
         * #add a new entry (first way)
         * e = l1.newEntry()
         * e.setTranslation(0,"dog")
         * e.setTranslation(1,"chien")
         * l1.appendEntry(e)
         *
         * #add a new entry (second way)
         * ee = l1.newEntry(["glass","verre"])
         * l1.appendEntry(ee)
         * #third way
         * ee = l1.appendNewEntry(["book","livre"])
         *
         * #add a new lesson (fast way)
         * l2 = doc.appendNewLesson("Lesson 2")
         *
         * #add a new child lesson under "Lesson 2"
         * l3 = doc.appendNewLesson("Lesson 3",l2)
         *
         * #add a new entry (third way)
         * l2.appendNewEntry(["I","je"]);
         * l3.appendNewEntry(["good morning","bonjour"])
         *
         * #save document
         * doc.saveAs("/home/kde-devel/test_new_document.kvtml")
         * @endcode
         *
         * @return A Document object, the newly created lesson
         */
        QObject * newDocument() {
            return new ::KEduVocDocument();
        }


    Q_SIGNALS:
        /**
         * Slots (script functions) connected to this signal are called when a translation of @p word is requested. Note that a script function with the same name as a signal will be automatically connected to that signal when the script is activated.
         *
         * @code
         * #example usage of translateWord signal
         * import KEduVocDocument
         * #function called by KEduVocDocument whenever a translation of a word is needed
         * def translateWord(word,fromLang,toLang):
         *     <<look for the word translation>>
         *     KEduVocDocument.addTranslation(word,fromLang,toLang,foundWord)
         *     <<look for more translations>>
         * @endcode
         *
         * @param word Word to translate
         * @param fromLanguage The language of @p word to translate from
         * @param toLanguage The language you want to translate to
         */
        void translateWord(const QString & word, const QString& fromLanguage, const QString& toLanguage);
        /* Emits when the translation of a word is finished (from all the scripts) [not to be used by scripts] */
        void translationFinished(const QString & word, const QString& fromLanguage, const QString& toLanguage);
        /* Emits when the translation of a word starts [not to be used by scripts] */
        void translationStarted(const QString & word, const QString& fromLanguage, const QString& toLanguage);
        /** Emitted when the document pointer is changed. */
        void documentChanged(::KEduVocDocument *newDocument);
    private:
        KEduVocTranslator* m_translator;
        ::KEduVocDocument* m_doc;
        KEduVocVocabularyModel * m_vocabularyModel;
        QString m_separator;
    };
}

Q_DECLARE_METATYPE( KEduVocTranslation* )

#endif
