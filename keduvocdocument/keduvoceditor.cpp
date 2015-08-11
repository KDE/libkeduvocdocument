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

#include "keduvoceditor.h"

#include "keduvoceditoradaptor.h"

//KEduVoc
#include <keduvocvocabularymodel.h>
#include <keduvoccontainermodel.h>
#include <keduvoclessonmodel.h>
#include <keduvocwordclassmodel.h>
#include <keduvocvocabularyfilter.h>
#include <keduvoctranslator.h>
#include <keduvoccontainerview.h>
#include <keduvocvocabularyview.h>
#include <keduvocwordtypeview.h>
#include <keduvoclessonview.h>
#include <keduvocaudiowidget.h>
#include <keduvocbrowserwidget.h>
#include <keduvocmultiplechoicewidget.h>
#include <keduvoccomparisonwidget.h>
#include <keduvocinflectionwidget.h>
#include <keduvocimagechooserwidget.h>
#include <keduvocsynonymwidget.h>
#include <keduvocsummarywordwidget.h>
#include <keduvoclatexwidget.h>
#include <keduvocscriptmanager.h>
#include <keduvocscriptdialog.h>

#include <KActionCollection>
#include <KToggleAction>
#include <KActionMenu>
#include <KCharSelect>
#include <KLocalizedString>
#include <KConfigGroup>
#include <KRecentFilesAction>
#include <KNewStuff3/kns3/knewstuffaction.h>

#include <QTimer>
#include <QSignalMapper>
#include <QDockWidget>
#include <QHeaderView>
#include <QMenu>
#include <QStackedWidget>
#include <QScrollArea>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QDebug>
#include <QDBusConnection>

using namespace Editor;


class KEduVocEditorWindow::Private
{
public:
    Private( QWidget* parent, KEduVocEditorDocument* editorDocument );

    QWidget *m_parent;
    KEduVocEditorDocument *m_editorDocument;

    QAction* m_vocabShowSearchBarAction;

    KEduVocVocabularyModel *m_vocabularyModel;
    KEduVocVocabularyView *m_vocabularyView;
    KEduVocVocabularyFilter *m_vocabularyFilter;

    QLineEdit *m_searchLine;
    QWidget *m_searchWidget;
    QAction *m_spellCheckMenu;
    QSignalMapper *m_spellCheckMapper;

    /** Show a single conjugation and let the user edit it */
    KEduVocInflectionWidget *m_inflectionWidget;
    KEduVocSummaryWordWidget *m_summaryWordWidget;
    KEduVocComparisonWidget *m_comparisonWidget;
    KEduVocLatexWidget *m_latexWidget;
    KEduVocSynonymWidget *m_synonymWidget;
    KEduVocSynonymWidget *m_antonymWidget;
    KEduVocSynonymWidget *m_falseFriendWidget;

    /// dock widgets to display lessons, word types, ...
    KEduVocLessonView *m_lessonView;
    KEduVocLessonModel *m_lessonModel;

    KEduVocWordClassModel *m_wordTypeModel;
    KEduVocWordTypeView *m_wordTypeView;

    KEduVocScriptManager* m_scriptManager;

    ///stores all the translations of a vocabulary word
    KEduVocTranslator* m_translator;

    QList<QDockWidget*> m_dockWidgets;
    QList <bool> m_dockWidgetVisibility;

    QList <int> m_visibleColumns; // DocumentSettings::visibleColumns()
    int m_entriesPerLesson; // Prefs::entriesPerLesson()
    bool m_subLessonEntries; // Prefs::showSubLessonEntries()
    bool m_automaticTranslation; // Prefs::automaticTranslation()
    bool m_showSearch; // Prefs::showSearch()
    QString m_spellChecker; // LanguageSettings::spellChecker()
    QString m_separator; // Prefs::separator()
};

KEduVocEditorWindow::Private::Private( QWidget* parent, KEduVocEditorDocument* editorDocument )
{
    m_parent = parent;
    m_editorDocument = editorDocument;
    m_vocabShowSearchBarAction = 0;
    m_vocabularyModel = 0;
    m_vocabularyView = 0;
    m_vocabularyFilter = 0;
    m_searchLine = 0;
    m_searchWidget = 0;
    m_spellCheckMenu = 0;
    m_spellCheckMapper = 0;
    m_inflectionWidget = 0;
    m_summaryWordWidget = 0;
    m_comparisonWidget = 0;
    m_latexWidget = 0;
    m_synonymWidget = 0;
    m_antonymWidget = 0;
    m_falseFriendWidget = 0;
    m_lessonView = 0;
    m_lessonModel = 0;
    m_wordTypeModel = 0;
    m_wordTypeView = 0;
    m_scriptManager = 0;
    m_translator = 0;
}

KEduVocEditorWindow::KEduVocEditorWindow( QWidget* parent, KEduVocEditorDocument* editorDocument )
    : KXmlGuiWindow( parent )
    , d( new Private( parent, editorDocument ) )
{
    // KXmlGui
    setXMLFile( "editorui.rc" );
    setObjectName( "Editor" );

    setCorner( Qt::TopLeftCorner, Qt::LeftDockWidgetArea );
    setCorner( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea );
    setCorner( Qt::TopRightCorner, Qt::RightDockWidgetArea );
    setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea );

    initView();
    initModel();

    initDockWidgets();
    initActions();

    KConfigGroup cfg( KSharedConfig::openConfig( QString( QApplication::applicationName().toLower() + "rc") ), objectName() );
    applyMainWindowSettings(cfg);

    connect( d->m_editorDocument, SIGNAL( documentChanged( KEduVocDocument* ) ), this, SLOT( updateDocument( KEduVocDocument* ) ) );
    connect( d->m_editorDocument, SIGNAL( languagesChanged() ), this, SLOT( slotLanguagesChanged() ) );
    connect( d->m_editorDocument, SIGNAL( statesNeedSaving() ), this, SLOT( saveState() ) );
    connect( parent, SIGNAL( preferencesChanged() ), this, SLOT( applyPrefs() ) );

    QTimer::singleShot( 0, this, SLOT( initScripts() ) );
}

KEduVocEditorWindow::~KEduVocEditorWindow()
{
    saveState();
    KConfigGroup cfg( KSharedConfig::openConfig( QString( QApplication::applicationName().toLower() + "rc" ) ), objectName() );
    saveMainWindowSettings( cfg );
    delete d;
}

void KEduVocEditorWindow::updateDocument( KEduVocDocument *doc )
{
    d->m_vocabularyView->setDocument( doc );

    if( doc ) {
        emit visibleColumnsChanged( doc );
        KConfig appConfig( QString( QApplication::applicationName().toLower() + "rc" ) );
        KConfigGroup documentGroup( &appConfig, "Document " + doc->url().url() ); 
        QByteArray state = documentGroup.readEntry( "VocabularyColumns", QByteArray() );
        d->m_vocabularyView->setVisibleColumns( d->m_visibleColumns );
        d->m_vocabularyView->setState( state );
        QByteArray saveState = d->m_vocabularyView->horizontalHeader()->saveState();
        documentGroup.writeEntry( "VocabularyColumns", d->m_vocabularyView->horizontalHeader()->saveState() );
    }
    d->m_vocabularyModel->setDocument( doc );

    d->m_lessonModel->setDocument( doc );
    d->m_wordTypeModel->setDocument( doc );

    d->m_summaryWordWidget->slotDocumentChanged( doc );
    d->m_inflectionWidget->setDocument( doc );
    d->m_comparisonWidget->setDocument( doc );
    d->m_synonymWidget->setDocument( doc );
    d->m_antonymWidget->setDocument( doc );
    d->m_falseFriendWidget->setDocument( doc );

    if( !d->m_editorDocument->document() ) {
        return;
    }

    if( !doc ) {
        return;
    }

    // expand the root items
    d->m_lessonView->expandToDepth( 0 );
    d->m_wordTypeView->expandToDepth( 0 );

    connect( d->m_vocabularyView->selectionModel(),
            SIGNAL( selectionChanged( const QItemSelection &, const QItemSelection & ) ),
            d->m_summaryWordWidget, SLOT( slotSelectionChanged( const QItemSelection &, const QItemSelection & ) ) );
    connect( d->m_vocabularyView->selectionModel(),
            SIGNAL( selectionChanged( const QItemSelection &, const QItemSelection & ) ),
            d->m_latexWidget, SLOT( slotSelectionChanged( const QItemSelection &, const QItemSelection & ) ) );

    d->m_spellCheckMenu->menu()->clear();
    for( int i = 0; i < doc->identifierCount(); ++i ) {
        QAction* languageSpellCheck = new QAction( doc->identifier( i ).name(), d->m_spellCheckMenu->menu() );
        d->m_spellCheckMenu->menu()->addAction( languageSpellCheck );
        d->m_spellCheckMapper->setMapping( languageSpellCheck, i );
        connect( languageSpellCheck, SIGNAL( triggered() ), d->m_spellCheckMapper, SLOT( map() ) );
    }
}

void KEduVocEditorWindow::setVisibleColumns( QList <int> visibleColumns )
{
    d->m_visibleColumns = visibleColumns;
}

QList <int> KEduVocEditorWindow::visibleColumns()
{
    return d->m_visibleColumns;
}

void KEduVocEditorWindow::setEntriesPerLesson( int entriesPerLesson )
{
    d->m_entriesPerLesson = entriesPerLesson;
}

int KEduVocEditorWindow::entriesPerLesson()
{
    return d->m_entriesPerLesson;
}

void KEduVocEditorWindow::setSubLessonEntries( bool subLessonEntries )
{
    d->m_subLessonEntries = subLessonEntries;
}

bool KEduVocEditorWindow::subLessonEntries()
{
    return d->m_subLessonEntries;
}

void KEduVocEditorWindow::storeAutomaticTranslation( bool automaticTranslation )
{
    d->m_automaticTranslation = automaticTranslation;
}

bool KEduVocEditorWindow::automaticTranslation()
{
    return d->m_automaticTranslation;
}

void KEduVocEditorWindow::setShowSearch( bool showSearch )
{
    d->m_showSearch = showSearch;
}

bool KEduVocEditorWindow::showSearch()
{
    return d->m_showSearch;
}

void KEduVocEditorWindow::setSpellChecker( QString spellChecker )
{
    d->m_spellChecker = spellChecker;
}

QString KEduVocEditorWindow::spellChecker()
{
    return d->m_spellChecker;
}

void KEduVocEditorWindow::setSeparator( QString separator )
{
    d->m_separator = separator;
}

QString KEduVocEditorWindow::separator()
{
    return d->m_separator;
}

void KEduVocEditorWindow::setAutomaticTranslation(bool v)
{
    emit saveAutomaticTranslation( v );
}

void KEduVocEditorWindow::initDockWidgets()
{
    // Lesson dockwidget
    QDockWidget *lessonDockWidget = new QDockWidget( i18n( "Units" ), this );
    lessonDockWidget->setObjectName( "LessonDock" );
    emit entriesPerLessonChanged();
    d->m_lessonView = new KEduVocLessonView( this, this->actionCollection(), d->m_entriesPerLesson );
    lessonDockWidget->setWidget( d->m_lessonView );
    addDockWidget( Qt::LeftDockWidgetArea, lessonDockWidget );
    d->m_dockWidgets.append( lessonDockWidget );
    actionCollection()->addAction( "show_units_dock", lessonDockWidget->toggleViewAction() );

    d->m_lessonModel = new KEduVocLessonModel( this );
    d->m_lessonView->setModel( d->m_lessonModel );
    d->m_lessonView->setToolTip( i18n( "Right click to add, delete, or rename units. \n"
                                  "With the checkboxes you can select which units you want to practice. \n"
                                  "Only checked units [x] will be asked in the practice!" ) );

    connect( d->m_lessonView, SIGNAL( selectedLessonChanged( KEduVocLesson* ) ),
            d->m_vocabularyModel, SLOT( setLesson( KEduVocLesson* ) ) );

    connect( d->m_lessonView, SIGNAL( signalShowContainer( KEduVocContainer* ) ),
            d->m_vocabularyModel, SLOT( showContainer( KEduVocContainer* ) ) );

    connect( d->m_vocabularyView, SIGNAL( translationChanged( KEduVocExpression*, int ) ),
            d->m_lessonView, SLOT( setTranslation( KEduVocExpression*, int ) ) );


    // Word classes dock widget
    QDockWidget* wordTypeDockWidget = new QDockWidget( i18n( "Word Types" ), this );
    wordTypeDockWidget->setObjectName( "WordTypeDock" );
    d->m_wordTypeView = new KEduVocWordTypeView( this );
    wordTypeDockWidget->setWidget( d->m_wordTypeView );
    addDockWidget( Qt::LeftDockWidgetArea, wordTypeDockWidget );
    d->m_dockWidgets.append( wordTypeDockWidget );

    d->m_wordTypeModel = new KEduVocWordClassModel( this );
    wordTypeDockWidget->setVisible( false );
    actionCollection()->addAction( "show_wordtype_dock", wordTypeDockWidget->toggleViewAction() );

///@todo test, should be fixed with the lesson one though
///@todo remove before release
//     new ModelTest(m_wordTypeModel, this);

    d->m_wordTypeView->setModel( d->m_wordTypeModel );

    connect( d->m_vocabularyView, SIGNAL( translationChanged( KEduVocExpression*, int ) ),
            d->m_wordTypeView, SLOT( setTranslation( KEduVocExpression*, int ) ) );

// Inflections
    QDockWidget *inflectionDock = new QDockWidget( i18n( "Inflection (verbs, adjectives, nouns)" ), this );
    inflectionDock->setObjectName( "InflectionDock" );
    d->m_inflectionWidget = new KEduVocInflectionWidget( this );
    QScrollArea *inflectionScrollArea = new QScrollArea( this );
    inflectionScrollArea->setWidgetResizable( true );
    inflectionScrollArea->setWidget( d->m_inflectionWidget );
    inflectionDock->setWidget( inflectionScrollArea );
    addDockWidget( Qt::RightDockWidgetArea, inflectionDock );
    d->m_dockWidgets.append( inflectionDock );
    actionCollection()->addAction( "show_inflection_dock", inflectionDock->toggleViewAction() );
    connect( d->m_editorDocument, SIGNAL( documentChanged( KEduVocDocument* ) ),
                d->m_inflectionWidget, SLOT( setDocument( KEduVocDocument* ) ) );
    connect( d->m_vocabularyView, SIGNAL( translationChanged( KEduVocExpression*, int ) ),
            d->m_inflectionWidget, SLOT( setTranslation( KEduVocExpression*, int ) ) );

// Comparison forms
    QDockWidget *comparisonDock = new QDockWidget( i18n( "Comparison forms" ), this );
    comparisonDock->setObjectName( "ComparisonDock" );
    d->m_comparisonWidget = new KEduVocComparisonWidget( this );
    QScrollArea *comparisonScrollArea = new QScrollArea( this );
    comparisonScrollArea->setWidgetResizable( true );
    comparisonScrollArea->setWidget( d->m_comparisonWidget );
    comparisonDock->setWidget( comparisonScrollArea );
    addDockWidget( Qt::RightDockWidgetArea, comparisonDock );
    d->m_dockWidgets.append( comparisonDock );
    actionCollection()->addAction( "show_comparison_dock", comparisonDock->toggleViewAction() );
    comparisonDock->setVisible( false );
    connect( d->m_vocabularyView, SIGNAL( translationChanged( KEduVocExpression*, int ) ),
            d->m_comparisonWidget, SLOT( setTranslation( KEduVocExpression*, int ) ) );
    tabifyDockWidget( comparisonDock, inflectionDock );

// Multiple choice
    QDockWidget *multipleChoiceDock = new QDockWidget( i18n( "Multiple Choice" ), this );
    multipleChoiceDock->setObjectName( "MultipleChoiceDock" );
    KEduVocMultipleChoiceWidget *multipleChoiceWidget = new KEduVocMultipleChoiceWidget( this );
    QScrollArea *multipleChoiceScrollArea = new QScrollArea( this );
    multipleChoiceScrollArea->setWidgetResizable( true );
    multipleChoiceScrollArea->setWidget( multipleChoiceWidget );
    multipleChoiceDock->setWidget( multipleChoiceScrollArea );
    addDockWidget( Qt::RightDockWidgetArea, multipleChoiceDock );
    d->m_dockWidgets.append( multipleChoiceDock );
    actionCollection()->addAction( "show_multiplechoice_dock", multipleChoiceDock->toggleViewAction() );
    multipleChoiceDock->setVisible( false );
    connect( d->m_vocabularyView, SIGNAL( translationChanged( KEduVocExpression*, int ) ),
            multipleChoiceWidget, SLOT( setTranslation( KEduVocExpression*, int ) ) );

// Synonym (and the same for antonym and false friends)
    QDockWidget *synonymDock = new QDockWidget( i18n( "Synonyms" ), this );
    synonymDock->setObjectName( "SynonymDock" );
    d->m_synonymWidget = new KEduVocSynonymWidget( KEduVocSynonymWidget::Synonym, this );
    QScrollArea *synonymScrollArea = new QScrollArea( this );
    synonymScrollArea->setWidgetResizable( true );
    synonymScrollArea->setWidget( d->m_synonymWidget );
    synonymDock->setWidget( synonymScrollArea );
    addDockWidget( Qt::RightDockWidgetArea, synonymDock );
    d->m_dockWidgets.append( synonymDock );
    actionCollection()->addAction( "show_synonym_dock", synonymDock->toggleViewAction() );
    synonymDock->setVisible(false);
    connect( d->m_vocabularyView, SIGNAL( translationChanged( KEduVocExpression*, int ) ),
                d->m_synonymWidget, SLOT( setTranslation( KEduVocExpression*, int ) ) );

    QDockWidget *antonymDock = new QDockWidget( i18n( "Antonyms" ), this );
    antonymDock->setObjectName( "AntonymDock" );
    d->m_antonymWidget = new KEduVocSynonymWidget( KEduVocSynonymWidget::Antonym, this );
    QScrollArea *antonymScrollArea = new QScrollArea( this );
    antonymScrollArea->setWidgetResizable( true );
    antonymScrollArea->setWidget( d->m_antonymWidget );
    antonymDock->setWidget( antonymScrollArea );
    addDockWidget( Qt::RightDockWidgetArea, antonymDock );
    d->m_dockWidgets.append( antonymDock );
    actionCollection()->addAction( "show_antonym_dock", antonymDock->toggleViewAction() );
    antonymDock->setVisible( false );
    connect( d->m_vocabularyView, SIGNAL( translationChanged( KEduVocExpression*, int ) ),
            d->m_antonymWidget, SLOT( setTranslation( KEduVocExpression*, int ) ) );
    tabifyDockWidget( synonymDock, antonymDock );

    QDockWidget *falseFriendDock = new QDockWidget( i18n( "False Friends" ), this );
    falseFriendDock->setObjectName( "FalseFriendDock" );
    d->m_falseFriendWidget = new KEduVocSynonymWidget( KEduVocSynonymWidget::FalseFriend, this );
    QScrollArea *falseFriendScrollArea = new QScrollArea( this );
    falseFriendScrollArea->setWidgetResizable( true );
    falseFriendScrollArea->setWidget( d->m_falseFriendWidget );
    falseFriendDock->setWidget( falseFriendScrollArea );
    addDockWidget( Qt::RightDockWidgetArea, falseFriendDock );
    d->m_dockWidgets.append( falseFriendDock );
    actionCollection()->addAction( "show_falsefriend_dock", falseFriendDock->toggleViewAction() );
    falseFriendDock->setVisible( false );
    connect( d->m_vocabularyView, SIGNAL( translationChanged( KEduVocExpression*, int ) ),
                d->m_falseFriendWidget, SLOT( setTranslation( KEduVocExpression*, int ) ) );
    tabifyDockWidget( antonymDock, falseFriendDock );

// Pronunciation symbols - Use KCharSelect
    QDockWidget *charSelectDock = new QDockWidget( i18n( "Phonetic Symbols" ), this );
    charSelectDock->setObjectName( "IPADock" );
    KCharSelect *charSelectWidget = new KCharSelect( this, 0, KCharSelect::SearchLine | KCharSelect::BlockCombos | KCharSelect::CharacterTable );
    charSelectWidget->setCurrentChar( 0x0250 );
    QScrollArea *charSelectScrollArea = new QScrollArea( this );
    charSelectScrollArea->setWidgetResizable( true );
    charSelectScrollArea->setWidget( charSelectWidget );
    charSelectDock->setWidget( charSelectScrollArea );
    addDockWidget( Qt::BottomDockWidgetArea, charSelectDock );
    d->m_dockWidgets.append( charSelectDock );
    actionCollection()->addAction( "show_pronunciation_dock", charSelectDock->toggleViewAction() );
    charSelectDock->setVisible( false );
    connect( charSelectWidget, SIGNAL( charSelected( const QChar & ) ), d->m_vocabularyView, SLOT( appendChar( const QChar & ) ) );

// Image
    QDockWidget* imageDock = new QDockWidget( i18n( "Image" ), this );
    imageDock->setObjectName( "ImageDock" );
    KEduVocImageChooserWidget *imageChooserWidget = new KEduVocImageChooserWidget( this );
    QScrollArea* imageScrollArea = new QScrollArea( this );
    imageScrollArea->setWidgetResizable( true );
    imageScrollArea->setWidget( imageChooserWidget );
    imageDock->setWidget( imageScrollArea );
    addDockWidget( Qt::RightDockWidgetArea, imageDock );
    d->m_dockWidgets.append( imageDock );
    actionCollection()->addAction( "show_image_dock", imageDock->toggleViewAction() );
    imageDock->setVisible( false );
    connect( d->m_vocabularyView, SIGNAL( translationChanged( KEduVocExpression*, int ) ),
            imageChooserWidget, SLOT( setTranslation( KEduVocExpression*, int ) ) );
    tabifyDockWidget( multipleChoiceDock, imageDock );

// Summary word
    QDockWidget *summaryDock = new QDockWidget( i18n( "Summary" ), this );
    summaryDock->setObjectName( "SummaryDock" );
    d->m_summaryWordWidget = new KEduVocSummaryWordWidget( d->m_vocabularyFilter, d->m_editorDocument->document(), this );
    QScrollArea *summaryScrollArea = new QScrollArea( this );
    summaryScrollArea->setWidgetResizable( true );
    summaryScrollArea->setWidget( d->m_summaryWordWidget );
    summaryDock->setWidget( summaryScrollArea );
    addDockWidget( Qt::BottomDockWidgetArea, summaryDock );
    actionCollection()->addAction( "show_summary_dock", summaryDock->toggleViewAction() );
    summaryDock->setVisible( false );
    d->m_dockWidgets.append( summaryDock );
    connect( d->m_vocabularyView, SIGNAL( translationChanged( KEduVocExpression*, int ) ),
            d->m_summaryWordWidget, SLOT( setTranslation( KEduVocExpression*, int ) ) );

// Sound
    QDockWidget *audioDock = new QDockWidget( i18n( "Sound" ), this );
    audioDock->setObjectName( "AudioDock" );
    KEduVocAudioWidget *audioWidget = new KEduVocAudioWidget( this );
    QScrollArea *audioScrollArea = new QScrollArea( this );
    audioScrollArea->setWidgetResizable( true );
    audioScrollArea->setWidget( audioWidget );
    audioDock->setWidget( audioScrollArea );
    addDockWidget( Qt::RightDockWidgetArea, audioDock );
    d->m_dockWidgets.append( audioDock );
    actionCollection()->addAction( "show_audio_dock", audioDock->toggleViewAction() );
    audioDock->setVisible( false );
    connect( d->m_vocabularyView, SIGNAL( translationChanged( KEduVocExpression*, int ) ),
            audioWidget, SLOT( setTranslation( KEduVocExpression*, int ) ) );
    tabifyDockWidget( imageDock, audioDock );

// browser
    QDockWidget *browserDock = new QDockWidget( i18n( "Internet" ), this );
    browserDock->setObjectName( "BrowserDock" );
    //TinyWebBrowser *browserWidget = new TinyWebBrowser(this);
    KEduVocBrowserWidget *htmlPart = new KEduVocBrowserWidget( browserDock );
    QScrollArea *browserScrollArea = new QScrollArea( this );
    browserScrollArea->setWidgetResizable( true );
    browserScrollArea->setWidget( htmlPart );
    browserDock->setWidget( browserScrollArea );
    addDockWidget( Qt::BottomDockWidgetArea, browserDock );
    d->m_dockWidgets.append( browserDock );
    actionCollection()->addAction( "show_browser_dock", browserDock->toggleViewAction() );
    browserDock->setVisible( false );
        connect( d->m_vocabularyView, SIGNAL( translationChanged( KEduVocExpression*, int ) ),
                htmlPart, SLOT( setTranslation( KEduVocExpression*, int ) ) );
    tabifyDockWidget( summaryDock, browserDock );

// LaTeX
    QDockWidget *latexDock = new QDockWidget( i18n( "LaTeX" ), this );
    latexDock->setObjectName( "LatexDock" );
    d->m_latexWidget = new KEduVocLatexWidget( d->m_vocabularyFilter, d->m_editorDocument->document(), this );
    QScrollArea *latexScrollArea = new QScrollArea( this );
    latexScrollArea->setWidgetResizable( true );
    latexScrollArea->setWidget( d->m_latexWidget );
    latexDock->setWidget( latexScrollArea );
    addDockWidget( Qt::RightDockWidgetArea, latexDock );
    actionCollection()->addAction( "show_latex_dock", latexDock->toggleViewAction() );
    latexDock->setVisible( false );
    d->m_dockWidgets.append( latexDock );
    connect( d->m_vocabularyView, SIGNAL( translationChanged( KEduVocExpression*, int ) ),
                d->m_latexWidget, SLOT( setTranslation( KEduVocExpression*, int ) ) );
    tabifyDockWidget( audioDock, latexDock );

// Grades
//     QDockWidget *gradeDock = new QDockWidget(i18n("Grade"), this);
//     gradeDock->setObjectName("gradeDock");
//     QLabel *gradeWidget = new QLabel("grade placeholder", this);
//     gradeDock->setWidget(gradeWidget);
//     addDockWidget(Qt::RightDockWidgetArea, gradeDock);
//     connect(this, SIGNAL(signalSetData(KEduVocTranslation*)), m_declensionWidget, SLOT(setTranslation(KEduVocTranslation*)));

}

namespace PrivateAction
{
    QAction* createCustomAction( const QObject* recvr, const char* slot, QObject* parent,
                                 const QString& name,
                                 const QString& text,
                                 const QString& helpText,
                                 const QString& iconName = QString(),
                                 bool toggle = false )
    {
        // Create QAction or KToggleAction
        QAction* pAction;
        if( toggle ) {
            pAction = new KToggleAction( parent );
        } else {
            pAction = new QAction( parent );
        }
        // Set ObjectName, Text and HelpText
        pAction->setObjectName( name );
        pAction->setText( text );
        pAction->setToolTip( helpText );

        // Icon
        if( !iconName.isEmpty() ) {
            QIcon foundIcon( QIcon::fromTheme( iconName ) );
            if( foundIcon.isNull() ) {
                //Note: If you are using an alternative /usr/share/icons directory you need to
                //copy the /usr/share/icons/<theme>/index.theme into you alternate directory
                qDebug() << "Missing QIcon "<< iconName;
            }
            pAction->setIcon( foundIcon );
        }

        // Connect the action
        pAction->connect( pAction, SIGNAL( triggered( bool ) ), recvr, slot );

        // If parent is a KActionCollection, add the new action to it
        KActionCollection *collection = qobject_cast<KActionCollection *>(parent);
        if( pAction && collection )
            collection->addAction( pAction->objectName(), pAction );
        return pAction;
    }
}

QAction* KEduVocEditorWindow::createAction( Action id, const QObject* recvr, const char* slot, KActionCollection* parent )
{
    QAction* pAction = 0;

    switch( id ) {
        case FileNew:
            pAction = KStandardAction::openNew( recvr, slot, parent );
            pAction->setToolTip( i18n( "Creates a new vocabulary collection" ) );
            parent->setDefaultShortcut( pAction, QKeySequence::New );
            break;
        case FileOpen:
            pAction = KStandardAction::open( recvr, slot, parent );
            pAction->setToolTip( i18n( "Opens an existing vocabulary collection" ) );
            parent->setDefaultShortcut( pAction, QKeySequence::Open );
            break;
        case FileOpenDownloaded:
            pAction = PrivateAction::createCustomAction( recvr, slot, parent,
                                                  "file_open_downloaded", i18n( "Open &Downloaded Vocabularies..." ),
                                                  i18n( "Open downloaded vocabulary collections" ), "get-hot-new-stuff" );
            break;
        case FileSave:
            pAction = KStandardAction::save( recvr, slot, parent );
            pAction->setToolTip( i18n( "Save the active vocabulary collection" ) );
            parent->setDefaultShortcut( pAction, QKeySequence::Save );
            break;
        case FileSaveAs:
            pAction = KStandardAction::saveAs( recvr, slot, parent );
            pAction->setShortcut( QKeySequence::SaveAs );
            parent->setDefaultShortcut( pAction, QKeySequence::SaveAs );
            pAction->setToolTip( i18n( "Save the active vocabulary collection with a different name" ) );
            break;
        case FileExport:
            pAction = PrivateAction::createCustomAction( recvr, slot, parent,
                                                  "file_export", i18n( "&Export..." ),
                                                  i18n( "Export to HTML or CSV" ), "document-export" );
            break;
        case FileProperties:
            pAction = PrivateAction::createCustomAction( recvr, slot, parent,
                                                  "file_properties", i18n( "&Properties..." ),
                                                  i18n( "Edit document properties" ), "document-properties" );
            break;
        case FileClose:
            pAction = PrivateAction::createCustomAction( recvr, slot, parent,
                                                  "file_close", i18n( "Dashboard" ),
                                                  i18n( "Close the current vocabulary collection and show the dashboard" ), "go-home" );
            break;
        case FileQuit:
            pAction = KStandardAction::quit( recvr, slot, parent );
            parent->setDefaultShortcut( pAction, QKeySequence::Quit );
            pAction->setToolTip( i18n( "Quit Parley" ) );
            break;
        case Preferences:
            pAction = KStandardAction::preferences( recvr, slot, parent );
            parent->setDefaultShortcut( pAction, QKeySequence::Preferences );
            pAction->setToolTip( i18n( "Show the configuration dialog" ) );
            break;
        case LanguagesProperties:
            pAction = PrivateAction::createCustomAction( recvr, slot, parent,
                                                  "edit_languages", i18n( "&Languages..." ),
                                                  i18n( "Edit which languages are in the collection and their grammar properties." ), "set-language" );
            break;
        case RemoveGrades:
            pAction = PrivateAction::createCustomAction( recvr, slot, parent,
                                                  "vocab_remove_grades", i18n( "Remove Confidence Levels" ),
                                                  i18n( "Remove all confidence levels from the current document" ), "edit-clear" );
            break;
        case CheckSpelling:
            pAction = KStandardAction::spelling( recvr, slot, parent );
            break;
        case ToggleShowSublessons:
            pAction = PrivateAction::createCustomAction( recvr, slot, parent,
                                                  "lesson_showsublessonentries", i18n( "Show Entries from Child Units" ),
                                                  i18n( "Enable to also see the entries of child units in each unit." ),
                                                  QString(), true );
            emit subLessonEntriesChanged();
            pAction->setChecked( d->m_subLessonEntries );
            break;
        case AutomaticTranslation:
            pAction = PrivateAction::createCustomAction( recvr, slot, parent,
                                                  "lesson_automatictranslation", i18n( "Automatic Translation" ),
                                                  i18n( "Enable automatic translation of the unit entries."),
                                                  QString(), true );
            emit automaticTranslationChanged();
            pAction->setChecked( d->m_automaticTranslation );
            break;
        case StartPractice:
            pAction = PrivateAction::createCustomAction( recvr, slot, parent,
                                                  "practice_start", i18n( "Start Practice..." ),
                                                  i18n( "Start practicing" ), "practice-start" );
            break;
        case ConfigurePractice:
            pAction = PrivateAction::createCustomAction( recvr, slot, parent,
                                                  "practice_configure", i18n( "Configure Practice..." ),
                                                  i18n( "Change practice settings" ), "practice-setup" );
            break;
        case ExportPracticeResults:
            pAction = PrivateAction::createCustomAction( recvr, slot, parent,
                                                  "practice_export", i18n( "Export results..." ),
                                                  i18n( "Write a file with the results of the practice" ), "document-export" );
            break;
        case EnterEditMode:
            pAction = PrivateAction::createCustomAction( recvr, slot, parent,
                                                  "document_edit", i18n( "Editor" ),
                                                  i18n( "Switch to vocabulary editor" ), "document-edit" );
            break;
        case ToggleSearchBar:
            pAction = PrivateAction::createCustomAction( recvr, slot, parent,
                                                  "config_show_search", i18n( "Show Se&arch" ),
                                                  i18n( "Toggle display of the search bar" ),
                                                  QString(), true );
            emit showSearchChanged();
            pAction->setChecked( d->m_showSearch );
            break;
        case SearchVocabulary:
            pAction = KStandardAction::find( recvr, slot, parent );
            parent->setDefaultShortcut( pAction, QKeySequence::Find );
            break;
        case ShowScriptManager:
            pAction = PrivateAction::createCustomAction( recvr, slot, parent,
                                                  "show_script_manager", i18n( "&Script Manager" ),
                                                  i18n( "Enable and disable scripts" ), "set-language" );
            break;
    }

    Q_ASSERT( pAction );
    return pAction;
}

KRecentFilesAction* KEduVocEditorWindow::createRecentFilesAction( const QObject* recvr, const char* slot, QObject* parent )
{
    return KStandardAction::openRecent( recvr, slot, parent );
}

QAction* KEduVocEditorWindow::createDownloadAction( const QObject* recvr, const char* slot, KActionCollection* collection )
{
    QAction *pAction = KNS3::standardAction( i18n( "Download New Vocabularies..." ), recvr, slot, collection, "file_ghns" );
    pAction->setIcon( QIcon::fromTheme( "get-hot-new-stuff" ) );

    pAction->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_G ) );
    collection->setDefaultShortcut( pAction, QKeySequence( Qt::CTRL + Qt::Key_G ) );
    pAction->setToolTip( i18n( "Downloads new vocabulary collections" ) );
    return pAction;
}

QAction* KEduVocEditorWindow::createUploadAction( const QObject* recvr, const char* slot, KActionCollection* collection )
{
    QAction *pAction = KNS3::standardActionUpload( i18n( "&Upload Vocabulary Document..." ), recvr, slot, collection, "file_upload" );
    pAction->setIcon( QIcon::fromTheme( "get-hot-new-stuff" ) );

    pAction->setToolTip( i18n( "Share the current vocabulary collection with other users" ) );
    return pAction;
}

void KEduVocEditorWindow::initActions()
{
    createAction( Action::RemoveGrades, this, SLOT( removeGrades() ), actionCollection() );
    d->m_spellCheckMenu = createAction( Action::CheckSpelling, 0, "", actionCollection() );
    d->m_spellCheckMenu->setMenu( new QMenu( this ) );
    d->m_spellCheckMapper = new QSignalMapper( this );
    connect( d->m_spellCheckMapper, SIGNAL( mapped( int ) ), this, SLOT( setSpellCheck( int ) ) );

    createAction( Action::ToggleShowSublessons, d->m_vocabularyModel, SLOT( setRecursive( bool ) ), actionCollection() );
    createAction( Action::AutomaticTranslation, this, SLOT( setAutomaticTranslation( bool ) ), actionCollection() );
    createAction( Action::StartPractice, d->m_parent, SLOT( showPracticeConfiguration() ), actionCollection() );
    actionCollection()->action( "practice_start" )->setText( i18n( "Practice" ) );
    actionCollection()->action( "practice_start" )->setToolTip( i18n( "Practice" ) );
    createAction( Action::ConfigurePractice, d->m_parent, SLOT( configurePractice() ), actionCollection() );
    createAction( Action::ToggleSearchBar, this, SLOT( slotConfigShowSearch() ), actionCollection() );
    createAction( Action::SearchVocabulary, this, SLOT( startSearch() ), actionCollection() );
    createAction( Action::ShowScriptManager, this, SLOT( slotShowScriptManager() ), actionCollection() );
    createAction( Action::LanguagesProperties, d->m_editorDocument, SLOT( languageProperties() ), actionCollection() );
    createUploadAction( d->m_editorDocument, SLOT( uploadFile() ), actionCollection() );

    new KEduVocEditorWindowAdaptor( this );

    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject( "/AddWithTranslation", this );
}

void KEduVocEditorWindow::setSpellCheck( int language )
{
    QString locale = d->m_vocabularyView->document()->identifier( language ).locale();
    emit spellCheckerChanged( locale );
    QString spellCheck = d->m_spellChecker.isEmpty() ? locale : d->m_spellChecker;
    d->m_vocabularyView->setSpellCheck( spellCheck );
    d->m_vocabularyView->checkSpelling( language );
}

void KEduVocEditorWindow::unplugAction( QString list )
{
    unplugAction( list );
}

void KEduVocEditorWindow::plugAction( QString list, QList< QAction* > scriptActions )
{
    plugAction( list, scriptActions );
}

void KEduVocEditorWindow::editorAddActionCollection( const QString& name, QAction* action )
{
    actionCollection()->addAction( name, action );
}


void KEduVocEditorWindow::addWordWithTranslation( const QStringList &w )
{
    KEduVocExpression *kexpr = new KEduVocExpression( w );

    d->m_vocabularyModel->appendEntry( kexpr );
}

void KEduVocEditorWindow::initModel()
{
    emit subLessonEntriesChanged();
    d->m_vocabularyModel = new KEduVocVocabularyModel( d->m_subLessonEntries, this );
    d->m_vocabularyFilter = new KEduVocVocabularyFilter( this );
    d->m_vocabularyFilter->setSourceModel( d->m_vocabularyModel );
    d->m_vocabularyView->setFilter( d->m_vocabularyFilter );

//    connect(m_mainWindow->parleyDocument(), SIGNAL(documentChanged(KEduVocDocument*)), m_vocabularyModel, SLOT(setDocument(KEduVocDocument*)));
//    connect(m_mainWindow->parleyDocument(), SIGNAL(documentChanged(KEduVocDocument*)), m_vocabularyView, SLOT(setDocument(KEduVocDocument*)));
    connect( d->m_searchLine, SIGNAL( textChanged( const QString& ) ), d->m_vocabularyFilter, SLOT( setSearchString( const QString& ) ) );
}

/**
 * This initializes the main widgets and table.
 */
void KEduVocEditorWindow::initView()
{
    QWidget *mainWidget = new QWidget( this );
    setCentralWidget( mainWidget );
    QVBoxLayout *topLayout = new QVBoxLayout( mainWidget );

    d->m_searchLine = new QLineEdit( this );
    d->m_searchLine->show();
    d->m_searchLine->setFocusPolicy( Qt::ClickFocus );
    d->m_searchLine->setClearButtonEnabled( true );
    d->m_searchLine->setPlaceholderText( i18n( "Enter search terms here" ) );
    d->m_searchLine->setToolTip( i18n( "Search your vocabulary" ) );

    QLabel *label = new QLabel( i18n( "&Search:" ), this );
    label->setBuddy( d->m_searchLine );
    label->show();

    d->m_searchWidget = new QWidget( this );
    QHBoxLayout* layout = new QHBoxLayout( d->m_searchWidget );
    layout->setMargin( 0 );
    layout->addWidget( label );
    layout->addWidget( d->m_searchLine );

///@todo     centralWidget()-> delete layout
    QVBoxLayout * rightLayout = new QVBoxLayout();
    rightLayout->setMargin( 0 );
    rightLayout->addWidget( d->m_searchWidget );
    emit showSearchChanged();
    d->m_searchWidget->setVisible( d->m_showSearch );

    emit automaticTranslationChanged();
    d->m_vocabularyView = new KEduVocVocabularyView( this, this->actionCollection(), d->m_automaticTranslation );

    rightLayout->addWidget( d->m_vocabularyView, 1, 0 );

    topLayout->addLayout( rightLayout );
}

void KEduVocEditorWindow::slotConfigShowSearch()
{
    d->m_searchWidget->setVisible( d->m_searchWidget->isHidden() );
    emit saveShowSearch( d->m_searchWidget->isVisible() );
}

void KEduVocEditorWindow::startSearch()
{
    d->m_searchWidget->setVisible( true );
    d->m_searchLine->setFocus();
}

void KEduVocEditorWindow::slotShowScriptManager()
{
    KEduVocScriptDialog * dialog = new KEduVocScriptDialog( d->m_scriptManager );
    dialog->show();
}

void KEduVocEditorWindow::applyPrefs()
{
    emit visibleColumnsChanged( d->m_vocabularyView->document() );
    QList <int> visibleColumns = d->m_visibleColumns;

    KConfig appConfig( QString( QApplication::applicationName().toLower() + "rc") );
    KConfigGroup documentGroup( &appConfig, "Document " + d->m_vocabularyView->document()->url().url() ); 
    QByteArray state = documentGroup.readEntry( "VocabularyColumns", QByteArray() );

    d->m_vocabularyView->reset( visibleColumns, state );

    QByteArray saveState = d->m_vocabularyView->horizontalHeader()->saveState();
    documentGroup.writeEntry( "VocabularyColumns", d->m_vocabularyView->horizontalHeader()->saveState() );
}

void KEduVocEditorWindow::removeGrades()
{
    d->m_editorDocument->document()->lesson()->resetGrades( -1, KEduVocContainer::Recursive );
}

void KEduVocEditorWindow::initScripts()
{
    emit separatorChanged();
    d->m_scriptManager = new KEduVocScriptManager( this, this->d->m_editorDocument, this->d->m_vocabularyModel, d->m_separator );

    // signal-slot connections between EditorWindow and KEduVocScriptMangager
    connect( d->m_scriptManager, &KEduVocScriptManager::editorUnplugActionList, this, &KEduVocEditorWindow::unplugAction );
    connect( d->m_scriptManager, &KEduVocScriptManager::editorPlugActionList, this, &KEduVocEditorWindow::plugAction );
    connect( d->m_scriptManager, &KEduVocScriptManager::editorActionAdded, this, &KEduVocEditorWindow::editorAddActionCollection );

    d->m_vocabularyView->setTranslator( d->m_scriptManager->translator() );

    //Load scripts
    d->m_scriptManager->loadScripts();
}

void KEduVocEditorWindow::saveState()
{
    d->m_vocabularyView->saveColumnVisibility();
}

void KEduVocEditorWindow::slotLanguagesChanged()
{
    d->m_vocabularyModel->resetLanguages();
}
