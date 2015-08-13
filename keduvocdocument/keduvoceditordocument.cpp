/***************************************************************************

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

#include "keduvoceditordocument.h"

/*#include "../config-parley.h"
#include "parleymainwindow.h"
#include "editor/editor.h"
#include "version.h"
#include "prefs.h"
#include "languagesettings.h"

#include "dashboard/dashboard.h"*/

#include <keduvoclesson.h>
#include <keduvocleitnerbox.h>
#include <keduvocexpression.h>
#include <keduvocwordtype.h>
#include <keduvocvocabularyview.h>
#include <keduvocdocumentproperties.h>
#include <keduvoclanguageproperties.h>

#include <KRecentFilesAction>
#include <kns3/downloaddialog.h>
#include <kns3/uploaddialog.h>
#include <KEMailSettings>
#include <KMessageBox>
#include <KProcess>
#include <KLocalizedString>

#include <QDialog>
#include <QDialogButtonBox>
#include <QTimer>
#include <QCheckBox>
#include <QFileDialog>
#include <QMimeDatabase>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QVBoxLayout>
#include <QApplication>

#ifdef HAVE_LIBXSLT
//#include "exportdialog.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#endif


class KEduVocEditorDocument::Private
{
public:
    Private( QWidget* parent );

    QWidget *m_parent;   // Pointer to the owner of this document
    QTimer *m_backupTimer; // Timer for next autosave

    // The contents of the document
    KEduVocDocument *m_doc;

    bool m_autoBackup; // Prefs::autoBackup()
    QString m_separator; // Prefs::separator()
    bool m_autoSave; // Prefs::autoSave()
    bool m_queryClose; // ParleyMainWindow::queryClose()
    int m_backupTime; // Prefs::backupTime()
};

KEduVocEditorDocument::Private::Private( QWidget* parent )
{
    m_parent = parent;
    m_backupTimer = 0;
    m_doc = 0;
}

namespace DocumentHelper
{
void fetchGrammar( KEduVocDocument* doc, int languageIndex )
{
    QString locale = doc->identifier( languageIndex ).locale();
    //QString locale = "en";

    QUrl location( QUrl::fromUserInput( QString( "http://edu.kde.org/parley/locale/" ) + locale + QString( ".kvtml" ) ) );

    KEduVocDocument grammarDoc;
    if( grammarDoc.open( location ) == KEduVocDocument::NoError ) {
        doc->identifier( languageIndex ).setArticle( grammarDoc.identifier( 0 ).article() );
        doc->identifier( languageIndex ).setPersonalPronouns( grammarDoc.identifier( 0 ).personalPronouns() );
        // @todo        m_doc->identifier(index).setDeclension(grammarDoc.identifier(0).declension());
        doc->identifier( languageIndex ).setTenseList( grammarDoc.identifier( 0 ).tenseList() );
    } else {
        qDebug() << "Download of " << location << " failed.";
    }
}
} // namespace DocumentHelper


KEduVocEditorDocument::KEduVocEditorDocument( QWidget* parent )
    : QObject( parent )
    ,d( new Private( parent ) )
{
    d->m_doc = new KEduVocDocument( this );
}

KEduVocEditorDocument::~KEduVocEditorDocument()
{
    close();
    delete d;
    emit documentChanged( 0 );
}


void KEduVocEditorDocument::loadLanguageSettings( QString locale ) {
    emit languageSettingsChanged( locale );
}

void KEduVocEditorDocument::loadEditorFont( QString locale, KEduVocLanguagePropertiesPage* page ) {
    emit editorFontChanged( locale, page );
}

void KEduVocEditorDocument::loadPracticeFont( QString locale, KEduVocLanguagePropertiesPage* page ) {
    emit practiceFontChanged( locale, page );
}

void KEduVocEditorDocument::loadKeyboardLayout( QString locale, KEduVocLanguagePropertiesPage* page ) {
    emit keyboardLayoutChanged( locale, page );
}

void KEduVocEditorDocument::loadSpellChecker( QString locale, KEduVocLanguagePropertiesPage* page ) {
    emit spellCheckerChanged( locale, page );
}

void KEduVocEditorDocument::saveEditorFont( QString locale, QFont font ) {
    emit storeEditorFont( locale, font );
}

void KEduVocEditorDocument::savePracticeFont( QString locale, QFont font ) {
    emit storePracticeFont( locale, font );
}

void KEduVocEditorDocument::saveKeyboardLayout( QString locale, QString keyboardLayout ) {
    emit storeKeyboardLayout( locale, keyboardLayout );
}

void KEduVocEditorDocument::saveSpellChecker( QString locale, QString spellChecker ) {
    emit storeSpellChecker( locale, spellChecker );
}

void KEduVocEditorDocument::storeSettings( QString locale ) {
    emit saveSettings( locale );
}


KEduVocDocument *KEduVocEditorDocument::document()
{
    // If there is no present vocabulary document, create an empty one.
    if( !d->m_doc ) {
        d->m_doc = new KEduVocDocument();
    }

    return d->m_doc;
}

void KEduVocEditorDocument::setTitle( const QString& title )
{
    d->m_doc->setTitle( title );
}

void KEduVocEditorDocument::slotFileNew()
{
    if( queryClose() ) {
        newDocument( true );
    }
}

bool KEduVocEditorDocument::autoBackup()
{
    return d->m_autoBackup;
}

void KEduVocEditorDocument::setAutoBackup( bool autoBackup )
{
    d->m_autoBackup = autoBackup;
}

QString KEduVocEditorDocument::separator()
{
    return d->m_separator;
}

void KEduVocEditorDocument::setSeparator( QString separator )
{
    d->m_separator = separator;
}

bool KEduVocEditorDocument::autoSave()
{
    return d->m_autoSave;
}

void KEduVocEditorDocument::setAutoSave( bool autoSave )
{
    d->m_autoSave = autoSave;
}

int KEduVocEditorDocument::backupTime()
{
    return d->m_backupTime;
}

void KEduVocEditorDocument::setBackupTime( int backupTime )
{
    d->m_backupTime = backupTime;
}

void KEduVocEditorDocument::setQueryClose( bool queryClose )
{
    d->m_queryClose = queryClose;
}

void KEduVocEditorDocument::newDocument( bool wizard )
{
    KEduVocDocument *newDoc = new KEduVocDocument();

    initializeDefaultGrammar( newDoc );
    setDefaultDocumentProperties( newDoc );
    bool showGrammarDialog = false;
    bool fetchGrammarOnline = false;
    if( wizard ) {
        KEduVocDocumentProperties* titleAuthorWidget = new KEduVocDocumentProperties( newDoc, true, d->m_parent );

        QDialogButtonBox * button_dialog = new QDialogButtonBox;
        button_dialog->setStandardButtons( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget( titleAuthorWidget );
        layout->addWidget( button_dialog );

        QDialog * titleAuthorDialog = new QDialog;
        titleAuthorDialog->setLayout( layout );
        titleAuthorDialog->setWindowTitle( i18nc( "@title:window document properties", "Properties for %1", newDoc->url().url() ) );

        connect( titleAuthorDialog, &QDialog::accepted, titleAuthorWidget, &KEduVocDocumentProperties::accept );
        connect( button_dialog, &QDialogButtonBox::accepted, titleAuthorDialog, &QDialog::accept );
        connect( button_dialog, &QDialogButtonBox::rejected, titleAuthorDialog, &QDialog::reject );

        if( titleAuthorDialog->exec() ) {
            showGrammarDialog = titleAuthorWidget->isGrammarCheckBoxChecked();
            fetchGrammarOnline = titleAuthorWidget->isDownloadGrammarCheckBoxChecked();
            delete titleAuthorDialog;
        } else {
            delete titleAuthorDialog;
            delete newDoc;
            return;
        }
    }

    close();
    if( d->m_doc ) {
        d->m_doc->deleteLater();
    }
    d->m_doc = newDoc;
    emit documentChanged( d->m_doc );
    emit autoBackupChanged();
    enableAutoBackup( d->m_autoBackup );

    if (fetchGrammarOnline) {
        DocumentHelper::fetchGrammar( d->m_doc, 0 );
        DocumentHelper::fetchGrammar( d->m_doc, 1 );
    }
    if( showGrammarDialog ) {
        languageProperties();
    }

    emit showEditor();
}

void KEduVocEditorDocument::slotFileOpen()
{
    if( queryClose() ) {
        ///@todo frameworks check filters are fixed in kvocdoc
        ///@todo frameworks check how to add child checkbox to this dialog
        QFileDialog dialog( d->m_parent,  i18n( "Open Vocabulary Collection" ), QString(), KEduVocDocument::pattern( KEduVocDocument::Reading ) );
        QCheckBox *practiceCheckBox = new QCheckBox( i18n( "Open in practice &mode" ), &dialog );
        emit checkBoxChanged( practiceCheckBox );
        dialog.setFileMode( QFileDialog::ExistingFile );
        if( dialog.exec()
            && !dialog.selectedFiles().isEmpty()
            && open( QUrl::fromLocalFile( dialog.selectedFiles().first() ) )
            ) {
            if( practiceCheckBox->isChecked() ) {
                emit showPracticeConfiguration();
            } else {
                emit showEditor();
            }
        }
    }
}

void KEduVocEditorDocument::slotFileOpenRecent( const QUrl& url )
{
    if( queryClose() && open( url ) ) {
        ///@todo: start practice directly depending on current component
        emit showEditor();
    }
}

bool KEduVocEditorDocument::open( const QUrl & url )
{
    if( url.path().isEmpty() ) {
        return false;
    }

    close();

    if( d->m_doc ) {
        d->m_doc->deleteLater();
    }
    d->m_doc = new KEduVocDocument( this );
    emit documentChanged( d->m_doc );
    emit separatorChanged();
    d->m_doc->setCsvDelimiter( d->m_separator );

    bool isSuccess = false, isError = false;

    KEduVocDocument::ErrorCode ret = d->m_doc->open( url,  KEduVocDocument::FileDefaultHandling );
    switch( ret ) {
    case KEduVocDocument::NoError :
        isSuccess = true;
        break;
    case KEduVocDocument::FileLocked :
    {
        int exit = KMessageBox::warningYesNo(
            d->m_parent, i18n( "The vocabulary collection is locked by another process.  You can open the file if you take over the lock, but you will lose any changes from the other process.\n\nDo you want to take over the lock?\n" ), i18n( "Take Over Lock" ) ) ;
        if ( exit == KMessageBox::Yes ) { //attempt to steal lock

            ret = d->m_doc->open( url, KEduVocDocument::FileIgnoreLock );
            if ( ret == KEduVocDocument::NoError ) {
                qDebug() << "Lock stolen";
                isSuccess = true;
            } else {
                isError = true;
            }
        } else { //Don't Steal continue work without saving !!!
        }

        break;
    }
    default:
        isError = true;
    }

    if( isSuccess ) {
        qDebug() << "Open success.";
        //m_parleyApp->editor()->updateDocument();
        emit recentFileChanged( url, d->m_doc->title() );

        emit autoBackupChanged();
        enableAutoBackup( d->m_autoBackup );

    } else {
        if( isError ) {
            KMessageBox::error(
                d->m_parent, i18n( "Opening collection \"%1\" resulted in an error: %2", d->m_doc->url().url(),
                                  d->m_doc->errorDescription( ret ) ), i18nc( "@title:window", "Open Collection" ) );
        }
        d->m_doc->deleteLater();
        emit documentChanged( 0 );
        d->m_doc = 0;
    }

    return isSuccess;
}

void KEduVocEditorDocument::close()
{
    enableAutoBackup( false );
    if( d->m_doc ) {
        emit documentChanged( 0 );
        d->m_doc->deleteLater();
        d->m_doc = 0;
    }
}

bool KEduVocEditorDocument::queryClose()
{
    if( ( d->m_doc == NULL ) || !d->m_doc->isModified() ) {
        return true;
    }

    emit prefsSelfSave();

    emit autoSaveChanged(); //save without asking
    bool canSave = d->m_autoSave;

    if( !canSave ) {
        int exit = KMessageBox::warningYesNoCancel(
            d->m_parent, i18n( "Vocabulary is modified.\n\nSave file before exit?\n" ),
            "", KStandardGuiItem::save(), KStandardGuiItem::discard() );
        switch( exit ) {
        case KMessageBox::Yes :
            canSave = true;   // save and exit
            break;
        case KMessageBox::No :
            canSave = false;  // don't save but exit
            break;
        case KMessageBox::Continue :
        default:
            return false;  // continue work without saving !!!
        }
    }

    if( canSave ) {
        save();       // save and exit
    }

    close();
    return true;
}


void KEduVocEditorDocument::openGHNS()
{
    emit queryCloseChanged();
    if( d->m_queryClose ) {
        QDir downloadDir( QStandardPaths::writableLocation( QStandardPaths::DataLocation ) + '/' + "kvtml/" );

        downloadDir.mkpath( downloadDir.absolutePath() );

        QUrl url = QUrl::fromUserInput( QFileDialog::getOpenFileName(
            d->m_parent
            , i18n( "Open Downloaded Vocabulary Collection" )
            , downloadDir.path()
            , KEduVocDocument::pattern( KEduVocDocument::Reading ) ) );

        if( open( url ) ) {
            emit showPracticeConfiguration();
        }
    }
}

void KEduVocEditorDocument::save()
{
    if( d->m_doc->url().fileName() == i18n( "Untitled" ) ) {
        saveAs();
        return;
    }

    // remove previous backup
    QFile::remove( d->m_doc->url().toLocalFile() + '~' );
    QFile::copy( QFile::encodeName( d->m_doc->url().toLocalFile() ), QFile::encodeName( d->m_doc->url().toLocalFile() + '~' ) );

    emit separatorChanged();
    d->m_doc->setCsvDelimiter( d->m_separator );

    emit statesNeedSaving();

    QString newgenerator = QApplication::applicationName()  + QApplication::applicationVersion();
    d->m_doc->setGenerator( newgenerator );

    bool isSuccess = false,  isError = false ;

    KEduVocDocument::ErrorCode ret = d->m_doc->saveAs(
        d->m_doc->url() , KEduVocDocument::Automatic, KEduVocDocument::FileIgnoreLock );

    switch( ret ) {
    case KEduVocDocument::NoError :
        isSuccess = true;
        break;
    case KEduVocDocument::FileLocked :
    {
        int exit = KMessageBox::warningYesNo(
            d->m_parent, i18n( "File \"%1\" is locked by another process.  You can save to the file if you take over the lock, but you will lose any changes from the other process.\n\nDo you want to take over the lock?\n"
                , d->m_doc->url().url() ), "" );
        if( exit == KMessageBox::Yes ) {
            d->m_doc->setGenerator( newgenerator );
            ret = d->m_doc->saveAs( d->m_doc->url() , KEduVocDocument::Automatic, KEduVocDocument::FileIgnoreLock );

            if( ret == KEduVocDocument::NoError ) {
                isSuccess = true;
                qDebug() << "Lock stolen";
            } else {
                isError = true;
            }
        } else {
            //Intentionally empty else. Try to saveAs another filename
        }
        break;
    }
    default:
        isError = true;
    }

    if( isSuccess ) {
        emit recentFileChanged( d->m_doc->url(), d->m_doc->title() );
        emit autoBackupChanged();
        enableAutoBackup( d->m_autoBackup );
    } else {
        if( isError ) {
            KMessageBox::error(
                d->m_parent, i18n( "Writing file \"%1\" resulted in an error: %2", d->m_doc->url().url(),
                                  d->m_doc->errorDescription( ret ) ), i18nc( "@title:window", "Save File" ) );
        }
        qDebug() << "Save failed trying save as for "<< d->m_doc->url().url();
        saveAs();
    }

}

void KEduVocEditorDocument::saveAs( QUrl url )
{
    if( !d->m_doc ) {
        return;
    }

    if( url.isEmpty() ) {
        url = QUrl::fromLocalFile( QFileDialog::getSaveFileName(
            d->m_parent
            , i18n( "Save Vocabulary As" )
            , QString()
            , KEduVocDocument::pattern( KEduVocDocument::Writing ) )
        );

        if( url.isEmpty() ) {
            return;
        }
    }

    QFileInfo fileinfo( url.toLocalFile() );
    if( fileinfo.exists() ) {
        if( KMessageBox::warningContinueCancel(
                0, i18n( "<qt>The file<p><b>%1</b></p>already exists. Do you want to overwrite it?</qt>",
                        url.toLocalFile() ), QString(), KStandardGuiItem::overwrite() ) == KMessageBox::Cancel ) {
            return;
        }
    }

    QString msg = i18nc( "@info:status saving a file", "Saving %1", url.toLocalFile() );

    QFile::remove( url.toLocalFile() + '~' ); // remove previous backup
    QFile::copy( QFile::encodeName( url.toLocalFile() ), QFile::encodeName( QString( url.toLocalFile() + '~' ) ) );

    emit separatorChanged();
    d->m_doc->setCsvDelimiter( d->m_separator );

    if( !url.toLocalFile().contains( '.' ) ) {
        url.fromLocalFile( url.toLocalFile() + QString::fromLatin1( ".kvtml" ) );
    }

    bool isSuccess = false,  isError = false;
    d->m_doc->setGenerator( "Parley" );
    int ret = d->m_doc->saveAs( url, KEduVocDocument::Automatic );
    switch( ret ) {
    case KEduVocDocument::NoError :
        isSuccess = true;
        break;
    case KEduVocDocument::FileLocked :
    {
        int exit = KMessageBox::warningYesNo(
            d->m_parent, i18n( "File \"%1\" is locked by another process.  You can save to the file if you take over the lock, but you will lose any changes from the other process.\n\nDo you want to take over the lock?\n"
                , d->m_doc->url().url() ), "" );
        if( exit == KMessageBox::Yes ) { //attempt lock steal
            d->m_doc->setGenerator( QApplication::applicationName() + QApplication::applicationVersion() );
            ret = d->m_doc->saveAs(
                d->m_doc->url() , KEduVocDocument::Automatic, KEduVocDocument::FileIgnoreLock );

            if( ret == KEduVocDocument::NoError ) {
                isSuccess = true;
                qDebug() << "Lock stolen";
            } else {
                isError = true;
            }
            break;
        } else { //don't steal the lock
        }

        break;
    }
    default:
        isError = true;
        break;
    }

    if( isSuccess ) {
        qDebug() << "SaveAs success.";
        emit recentFileChanged( d->m_doc->url(), d->m_doc->title() );
        emit statesNeedSaving();

    } else {
        qDebug() << "SaveAs failed for "<< d->m_doc->url().url() << " \nwhy "<< d->m_doc->errorDescription( ret );
        if( isError ) {
            KMessageBox::error(
                d->m_parent, i18n( "Writing file \"%1\" resulted in an error: %2",
                                  d->m_doc->url().url(), d->m_doc->errorDescription( ret ) ), i18nc( "@title:window", "Save File" ) );

        }
    }

}

void KEduVocEditorDocument::initializeDefaultGrammar( KEduVocDocument *doc )
{
    KEduVocWordType *root = doc->wordTypeContainer();
    KEduVocWordType *noun = new KEduVocWordType( i18n( "Noun" ), root );
    noun->setWordType( KEduVocWordFlag::Noun );
    root->appendChildContainer( noun );

    KEduVocWordType *nounChild = new KEduVocWordType( i18n( "Masculine" ), noun );
    nounChild->setWordType( KEduVocWordFlag::Noun | KEduVocWordFlag::Masculine );
    noun->appendChildContainer( nounChild );
    nounChild = new KEduVocWordType( i18n( "Feminine" ), noun );
    nounChild->setWordType( KEduVocWordFlag::Noun | KEduVocWordFlag::Feminine );
    noun->appendChildContainer( nounChild );
    nounChild = new KEduVocWordType( i18n( "Neuter" ), noun );
    nounChild->setWordType( KEduVocWordFlag::Noun | KEduVocWordFlag::Neuter );
    noun->appendChildContainer( nounChild );

    KEduVocWordType *verb = new KEduVocWordType( i18n( "Verb" ), root );
    verb->setWordType( KEduVocWordFlag::Verb );
    root->appendChildContainer( verb );

    KEduVocWordType *adjective = new KEduVocWordType( i18n( "Adjective" ), root );
    adjective->setWordType( KEduVocWordFlag::Adjective );
    root->appendChildContainer( adjective );

    KEduVocWordType *adverb = new KEduVocWordType( i18n( "Adverb" ), root );
    adverb->setWordType( KEduVocWordFlag::Adverb );
    root->appendChildContainer( adverb );

    KEduVocWordType *conjunction = new KEduVocWordType( i18n( "Conjunction" ), root );
    conjunction->setWordType( KEduVocWordFlag::Conjunction );
    root->appendChildContainer( conjunction );
}


void KEduVocEditorDocument::setDefaultDocumentProperties( KEduVocDocument *doc )
{
    KEMailSettings emailSettings;
    emailSettings.setProfile( emailSettings.defaultProfileName() );
    doc->setAuthor( emailSettings.getSetting( KEMailSettings::RealName ) );
    doc->setAuthorContact( emailSettings.getSetting( KEMailSettings::EmailAddress ) );

    doc->setLicense( i18n( "Public Domain" ) );
    doc->setCategory( i18n( "Languages" ) );

    QString locale = QLocale().name();

    doc->appendIdentifier();
    doc->appendIdentifier();
    doc->identifier( 0 ).setName(QLocale( locale ).nativeLanguageName() );
    doc->identifier( 0 ).setLocale( locale );
    doc->identifier( 1 ).setName( i18n( "A Second Language" ) );
    doc->identifier( 1 ).setLocale( locale );

    KEduVocLesson* lesson = new KEduVocLesson( i18n( "Lesson 1" ), doc->lesson() );
    doc->lesson()->appendChildContainer( lesson );

    // add some entries
    for( int i = 0; i < 15 ; i++ ) {
        lesson->appendEntry( new KEduVocExpression() );
    }

    doc->setModified( false );
}

void KEduVocEditorDocument::slotGHNS()
{
    QMimeDatabase db;
    QString fileName;
    KNS3::DownloadDialog newStuffDialog( d->m_parent );
    newStuffDialog.exec();
    KNS3::Entry::List entries = newStuffDialog.installedEntries();
    int numberInstalled = entries.size();
    foreach( const KNS3::Entry & entry, entries ) {
        // check mime type and if kvtml, open it
        foreach( const QString & file, entry.installedFiles() ) {
            QMimeType mimeType = db.mimeTypeForFile( file );
            qDebug() << "KNS3 file of mime type:" << db.mimeTypeForFile( file ).name();
            if( mimeType.inherits( "application/x-kvtml" ) ) {
                emit recentFileChanged( QUrl::fromLocalFile( file ), QString() );
                fileName = file;
            }
        }
    }

    // to enable the display in the welcome screen
    emit prefsSelfSave();
    emit updateRecentFilesModel();
    if( numberInstalled > 1 ) {
        openGHNS();
    } else if( numberInstalled == 1 ) {
        if( open( QUrl::fromUserInput( fileName ) ) )
            emit showPracticeConfiguration();
        else
            KMessageBox::error( d->m_parent, i18n( "Could not open vocabulary collection \"%1\"", entries.first().name() ) );
    }
}

void KEduVocEditorDocument::documentProperties()
{
    KEduVocDocumentProperties* titleAuthorWidget = new KEduVocDocumentProperties( d->m_doc, false, d->m_parent );

    QDialogButtonBox * button_dialog = new QDialogButtonBox;
    button_dialog->setStandardButtons( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget( titleAuthorWidget );
    layout->addWidget( button_dialog );

    QDialog * titleAuthorDialog = new QDialog;
    titleAuthorDialog->setLayout( layout );
    titleAuthorDialog->setWindowTitle( i18nc( "@title:window document properties", "Properties for %1", d->m_doc->url().url() ) );

    // the language options are only shown, when this is used to create a new document.
    titleAuthorWidget->setLanguageGroupBoxVisible( false );

    connect( titleAuthorDialog, &QDialog::accepted, titleAuthorWidget, &KEduVocDocumentProperties::accept );
    connect( button_dialog, &QDialogButtonBox::accepted, titleAuthorDialog, &QDialog::accept );
    connect( button_dialog, &QDialogButtonBox::rejected, titleAuthorDialog, &QDialog::reject );

    titleAuthorDialog->exec();
    delete titleAuthorDialog;
    emit documentChanged( d->m_doc );
}

void KEduVocEditorDocument::languageProperties()
{
    KEduVocLanguageProperties *properties = new KEduVocLanguageProperties( d->m_doc, d->m_parent );
    connect( properties, &KEduVocLanguageProperties::languageSettingsChanged, this, &KEduVocEditorDocument::loadLanguageSettings );
    connect( properties, &KEduVocLanguageProperties::editorFontChanged, this, &KEduVocEditorDocument::loadEditorFont );
    connect( properties, &KEduVocLanguageProperties::practiceFontChanged, this, &KEduVocEditorDocument::loadPracticeFont );
    connect( properties, &KEduVocLanguageProperties::keyboardLayoutChanged, this, &KEduVocEditorDocument::loadKeyboardLayout );
    connect( properties, &KEduVocLanguageProperties::spellCheckerChanged, this, &KEduVocEditorDocument::loadSpellChecker );
    connect( properties, &KEduVocLanguageProperties::storeEditorFont, this, &KEduVocEditorDocument::saveEditorFont );
    connect( properties, &KEduVocLanguageProperties::storePracticeFont, this, &KEduVocEditorDocument::savePracticeFont );
    connect( properties, &KEduVocLanguageProperties::storeKeyboardLayout, this, &KEduVocEditorDocument::saveKeyboardLayout );
    connect( properties, &KEduVocLanguageProperties::storeSpellChecker, this, &KEduVocEditorDocument::saveSpellChecker );;
    connect( properties, &KEduVocLanguageProperties::saveSettings, this, &KEduVocEditorDocument::storeSettings );

    if( properties->exec() == QDialog::Accepted ) {
        emit languagesChanged();
    }
}

void KEduVocEditorDocument::uploadFile()
{
    // save file to temp location
    QTemporaryDir dir;
    if( ! dir.isValid() ) {
        KMessageBox::error( d->m_parent, i18n( "Could not create temporary directory \"%1\"", dir.path() ) );
        return;
    }
    QUrl url( QUrl::fromUserInput( dir.path() + d->m_doc->url().fileName() ) );
    qDebug() << "save in " << url;
    d->m_doc->setGenerator( "Parley" );
    if( d->m_doc->saveAs( url, KEduVocDocument::Automatic ) != KEduVocDocument::NoError ) {
        KMessageBox::error( d->m_parent, i18n( "Could not save vocabulary collection \"%1\"", url.toString() ) );
        return;
    }

    KEduVocDocument tempDoc( this );
    if( tempDoc.open( url ) != KEduVocDocument::NoError ) {
        KMessageBox::error( d->m_parent, i18n( "Could not open vocabulary collection \"%1\"", url.toString() ) );
        return;
    }

    // remove grades
    tempDoc.lesson()->resetGrades( -1, KEduVocContainer::Recursive );
    d->m_doc->setGenerator( "Parley" );
    if( tempDoc.saveAs( url, KEduVocDocument::Automatic ) != KEduVocDocument::NoError ) {
        KMessageBox::error( d->m_parent, i18n( "Could not save vocabulary collection \"%1\"", url.toString() ) );
        return;
    }

    // upload
    KNS3::UploadDialog dialog( d->m_parent );
    dialog.setUploadFile(url);
    dialog.exec();
}


void KEduVocEditorDocument::exportDialog()
{
#ifdef HAVE_LIBXSLT
    KEduVocExportDialog dialog( d->m_doc, m_parent );
    connect( dialog, &KEduVocExportDialog::saveEditorDocument, this, &KEduVocEditorDocument::saveAs );
    dialog.exec();
#endif

}


void KEduVocEditorDocument::enableAutoBackup( bool enable )
{
    if( !enable ) {
        if( d->m_backupTimer ) {
            d->m_backupTimer->stop();
        }
    } else {
        if( !d->m_backupTimer ) {
            d->m_backupTimer = new QTimer( this );
            connect( d->m_backupTimer, &QTimer::timeout, this, &KEduVocEditorDocument::save );
        }
        emit backupTimeChanged();
        d->m_backupTimer->start( d->m_backupTime * 60 * 1000 );
    }
}
