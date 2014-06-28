/***************************************************************************
                        Vocabulary Document for KDE Edu
    -----------------------------------------------------------------------
    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                     (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                     (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvocdocument.h"

#include <QtCore/QFileInfo>
#include <QtCore/QTextStream>
#include <QtCore/QtAlgorithms>
#include <QtCore/QIODevice>
#include <QTemporaryFile>

#include <KLocalizedString>
#include <QDebug>
#include <kio/filecopyjob.h>
#include <krandomsequence.h>
#include <kfilterdev.h>
#include <kautosavefile.h>

#include "keduvocexpression.h"
#include "keduvoclesson.h"
#include "keduvocleitnerbox.h"
#include "keduvocwordtype.h"
#include "keduvockvtmlwriter.h"
#include "keduvockvtml2writer.h"
#include "keduvoccsvreader.h"
#include "keduvoccsvwriter.h"
#include "keduvockvtml2reader.h"
#include "keduvocwqlreader.h"
#include "keduvocpaukerreader.h"
#include "keduvocvokabelnreader.h"
#include "keduvocxdxfreader.h"

#define WQL_IDENT      "WordQuiz"

#define KVTML_EXT        "kvtml"
#define CSV_EXT          "csv"
#define TXT_EXT          "txt"
#define WQL_EXT          "wql"

class KEduVocDocument::KEduVocDocumentPrivate
{
public:
    KEduVocDocumentPrivate( KEduVocDocument* qq )
            : q( qq )
    {
        m_lessonContainer = 0;
        m_wordTypeContainer = 0;
        m_leitnerContainer = 0;
        m_autosave = new KAutoSaveFile;
        init();
    }

    ~KEduVocDocumentPrivate();

    void init();

    KEduVocDocument* q;

    /** autosave file used to provide locking access to the underlying file
     * Note: It is a pointer to allow locking a new file, saving results and
     * then transfering the lock to m_autosave without risking loss of lock.
     * See saveAs for clarification*/
    KAutoSaveFile            *m_autosave;

    bool                      m_dirty;

    // save these to document
    QList<KEduVocIdentifier>  m_identifiers;

    QList<int>                m_extraSizeHints;
    QList<int>                m_sizeHints;

    QString                   m_generator;
    QString                   m_queryorg;
    QString                   m_querytrans;

    QStringList               m_tenseDescriptions;
    QSet<QString>             m_usages;

    QString                   m_title;
    QString                   m_author;
    QString                   m_authorContact;
    QString                   m_license;
    QString                   m_comment;
    QString                   m_version;
    QString                   m_csvDelimiter;

    /** Categories that can later be used to sork kvtml files:
      * language, music, children, anatomy
      */
    QString                   m_category;

    KEduVocLesson * m_lessonContainer;
    KEduVocWordType * m_wordTypeContainer;
    KEduVocLeitnerBox * m_leitnerContainer;

    /** Creates an autosave file for fpath in order to lock the file
     *   @param autosave a reference to an allocated KAutosave
     *   @param fpath File path to the file to be locked
     *   @param flags Describes how to deal with locked file etc.
     *   @return ErrorCode where NoError is success
     *   */
    KEduVocDocument::ErrorCode initializeKAutoSave(KAutoSaveFile &autosave,  QString const &fpath,  FileHandlingFlags flags) const;
};

KEduVocDocument::KEduVocDocumentPrivate::~KEduVocDocumentPrivate()
{
    delete m_lessonContainer;
    delete m_wordTypeContainer;
    delete m_leitnerContainer;

    m_autosave->releaseLock();
    delete m_autosave;
}

void KEduVocDocument::KEduVocDocumentPrivate::init()
{
    delete m_lessonContainer;
    m_lessonContainer = new KEduVocLesson(i18nc("The top level lesson which contains all other lessons of the document.", "Document Lesson"));
    m_lessonContainer->setContainerType(KEduVocLesson::Lesson);
    delete m_wordTypeContainer;
    m_wordTypeContainer = new KEduVocWordType(i18n( "Word types" ));

    delete m_leitnerContainer;
    m_leitnerContainer = new KEduVocLeitnerBox(i18n( "Leitner Box" ));

    m_tenseDescriptions.clear();
    m_identifiers.clear();
    m_extraSizeHints.clear();
    m_sizeHints.clear();
    m_dirty = false;
    m_queryorg = "";
    m_querytrans = "";
    m_autosave->setManagedFile( i18n( "Untitled" ) );
    m_author = "";
    m_title = "";
    m_comment = "";
    m_version = "";
    m_generator = "";
    m_csvDelimiter = QString( '\t' );
    m_usages.clear();
    m_license.clear();
    m_category.clear();
}


KEduVocDocument::ErrorCode KEduVocDocument::KEduVocDocumentPrivate::initializeKAutoSave(KAutoSaveFile &autosave,  QString const & fpath,  FileHandlingFlags flags) const {

    QList<KAutoSaveFile *> staleFiles = KAutoSaveFile::staleFiles( fpath );
    if ( !staleFiles.isEmpty()) {
        if ( flags & FileIgnoreLock ) {
            foreach( KAutoSaveFile *f,  staleFiles ) {
                f->open( QIODevice::ReadWrite );
                f->remove();
                delete f;
            }
        } else {
            qCritical() << i18n( "Cannot lock file %1", fpath );
            return FileLocked;
        }
    }

    autosave.setManagedFile( fpath );
    if ( !autosave.open( QIODevice::ReadWrite ) ) {
        qCritical() << i18n( "Cannot lock file %1", autosave.fileName() );
        return FileCannotLock;
    }

    return NoError;
}


KEduVocDocument::KEduVocDocument( QObject *parent )
        : QObject( parent ), d( new KEduVocDocumentPrivate( this ) )
{
}


KEduVocDocument::~KEduVocDocument()
{
    delete d;
}


void KEduVocDocument::setModified( bool dirty )
{
    d->m_dirty = dirty;
    emit docModified( d->m_dirty );
}


KEduVocDocument::FileType KEduVocDocument::detectFileType( const QString &fileName )
{
    KFilterDev * f = new KFilterDev( fileName );
    if ( !f->open( QIODevice::ReadOnly ) ) {
        qDebug() << "Warning, could not open QIODevice for file: " << fileName;
        delete f;
        return Csv;
    }

    QTextStream ts( f );
    QString line1;
    QString line2;

    line1 = ts.readLine();
    if ( !ts.atEnd() ) {
        line2 = ts.readLine();
    }

    /*
     * Vokabeln.de files:
    The header seems to be something like this:

    "Name
    Lang1 - Lang2",123,234,456
    0

    or something longer:

    "Name
    Lang1 - Lang2
    [..]
    Blah, blah, blah...",123,234,456
    0
    */

    QString tmp;

    if ( line1.startsWith(QChar::fromLatin1('"'))) {
        ts.seek(0);
        tmp = ts.readLine();
        // There shouldn't be headers longer than 10 lines.
        for ( int x=0; x < 10; x++) {
            if (tmp.contains( "\"," )) {
                tmp = ts.readLine();
                if (tmp.endsWith('0')) {
                    f->close();
                    delete f;
                    return Vokabeln;
                }
            }
            tmp = ts.readLine();
        }
    }
    f->close();
    delete f;


    if ( line1.startsWith(QString::fromLatin1("<?xml")) ) {
        if ( line2.indexOf( "pauker", 0 ) >  0 ) {
            return Pauker;
        }
        if ( line2.indexOf( "xdxf", 0 ) >  0 ) {
            return Xdxf;
        } else {
            return Kvtml;
        }
    }

    if ( line1 == WQL_IDENT ) {
        return Wql;
    }

    return Csv;
}


KEduVocDocument::ErrorCode KEduVocDocument::open( const QUrl& url, FileHandlingFlags flags)
{
    // save csv delimiter to preserve it in case this is a csv document
    QString csv = d->m_csvDelimiter;
    // clear all other properties
    d->init();
    if ( !url.isEmpty() ) {
        setUrl( url );
    }
    d->m_csvDelimiter = csv;

    bool read = false;
    QString errorMessage = i18n( "<qt>Cannot open file<br /><b>%1</b></qt>", url.path() );
    QTemporaryFile tempFile;
    QString fileName;
    if (url.isLocalFile()) {
        fileName = url.toLocalFile();
    } else {
        if (tempFile.open()) {
            fileName = tempFile.fileName();
            KIO::FileCopyJob *job = KIO::file_copy(url, tempFile.fileName());
            if (!job->exec()) {
                qCritical() << "Couldn't open document file " << url.toString();
                return FileCannotRead;
            }
        }
    }

    ErrorCode autosaveError = d->initializeKAutoSave( *d->m_autosave,  fileName, flags );
    if ( autosaveError != NoError) {
        return autosaveError;
    }


    KFilterDev f( fileName );

    if ( !f.open( QIODevice::ReadOnly ) ) {
        qCritical() << errorMessage;
        return FileCannotRead;
    }

    FileType ft = detectFileType( fileName );

    switch ( ft ) {
        case Kvtml: {
            qDebug() << "Reading KVTML document...";
            KEduVocKvtml2Reader kvtmlReader( &f );
            read = kvtmlReader.readDoc( this );
            if ( !read ) {
                errorMessage = kvtmlReader.errorMessage();
            }
        }
        break;

        case Wql: {
            qDebug() << "Reading WordQuiz (WQL) document...";
            KEduVocWqlReader wqlReader( &f );
            d->m_autosave->setManagedFile( i18n( "Untitled" ) );
            read = wqlReader.readDoc( this );
            if ( !read ) {
                errorMessage = wqlReader.errorMessage();
            }
        }
        break;

        case Pauker: {
            qDebug() << "Reading Pauker document...";
            KEduVocPaukerReader paukerReader( this );
            d->m_autosave->setManagedFile( i18n( "Untitled" ) );
            read = paukerReader.read( &f );
            if ( !read ) {
                errorMessage = i18n( "Parse error at line %1, column %2:\n%3", paukerReader.lineNumber(), paukerReader.columnNumber(), paukerReader.errorString() );
            }
        }
        break;

        case Vokabeln: {
            qDebug() << "Reading Vokabeln document...";
            KEduVocVokabelnReader vokabelnReader( &f );
            d->m_autosave->setManagedFile( i18n( "Untitled" ) );
            read = vokabelnReader.readDoc( this );
            if ( !read ) {
                errorMessage = vokabelnReader.errorMessage();
            }
        }
        break;

        case Csv: {
            qDebug() << "Reading CVS document...";
            KEduVocCsvReader csvReader( &f );
            read = csvReader.readDoc( this );
            if ( !read ) {
                errorMessage = csvReader.errorMessage();
            }
        }
        break;

       case Xdxf: {
           qDebug() << "Reading XDXF document...";
           KEduVocXdxfReader xdxfReader( this );
           d->m_autosave->setManagedFile( i18n( "Untitled" ) );
           read = xdxfReader.read( &f );
           if ( !read ) {
               errorMessage = i18n( "Parse error at line %1, column %2:\n%3", xdxfReader.lineNumber(), xdxfReader.columnNumber(), xdxfReader.errorString() );
           }
        }
        break;

        default: {
            qDebug() << "Reading KVTML document (fallback)...";
            KEduVocKvtml2Reader kvtmlReader( &f );
            read = kvtmlReader.readDoc( this );
            if ( !read ) {
                errorMessage = kvtmlReader.errorMessage();
            }
        }
    }

    if ( !read ) {
        QString msg = i18n( "Could not open or properly read \"%1\"\n(Error reported: %2)", url.path(), errorMessage );
        qCritical() << msg << i18n( "Error Opening File" );
        ///@todo make the readers return int, pass on the error message properly
        return FileReaderFailed;
    }

    f.close();

    setModified(false);
    return NoError;
}

void KEduVocDocument::close() {
    d->m_autosave->releaseLock();
}

KEduVocDocument::ErrorCode KEduVocDocument::saveAs( const QUrl & url, FileType ft, FileHandlingFlags flags)
{

    QUrl tmp( url );

    if ( ft == Automatic ) {
        if ( tmp.path().right( strlen( "." KVTML_EXT ) ) == "." KVTML_EXT )
            ft = Kvtml;
        else if ( tmp.path().right( strlen( "." CSV_EXT ) ) == "." CSV_EXT )
            ft = Csv;
        else {
            return FileTypeUnknown;
        }
    }

    QString errorMessage = i18n( "Cannot write to file %1", tmp.path() );

    KAutoSaveFile *autosave;

    //If we don't care about the lock always create a new one
    //If we are changing files create a new lock
    if ( ( flags & FileIgnoreLock )
        || ( d->m_autosave->managedFile() != tmp.path() ) ) {

        autosave = new KAutoSaveFile;
        ErrorCode autosaveError = d->initializeKAutoSave( *autosave,  tmp.path(), flags );
        if ( autosaveError != NoError) {
            delete autosave;
            return autosaveError;
        }
    }

    //We care about the lock and we think we still have it.
    else {
         autosave = d->m_autosave;
        //Is the lock still good?
        if ( !autosave->exists( ) ) {
            return FileCannotLock;
        }
    }

    QFile f( tmp.path() );
    if ( !f.open( QIODevice::WriteOnly ) ) {
        qCritical() << i18n( "Cannot write to file %1", tmp.path() );
        return FileCannotWrite;
    }

    bool saved = false;

    switch ( ft ) {
        case Kvtml: {
            // write version 2 file
            KEduVocKvtml2Writer kvtmlWriter( &f );
            saved = kvtmlWriter.writeDoc( this, d->m_generator );
        }
        break;
        ///@todo port me
//         case Kvtml1: {
//             // write old version 1 file
//             KEduVocKvtmlWriter kvtmlWriter( &f );
//             saved = kvtmlWriter.writeDoc( this, d->m_generator );
//         }
//         break;
        case Csv: {
            KEduVocCsvWriter csvWriter( &f );
            saved = csvWriter.writeDoc( this, d->m_generator );
        }
        break;
        default: {
            qCritical() << "kvcotrainDoc::saveAs(): unknown filetype" << endl;
        }
        break;
    } // switch

    f.close();

    if ( !saved ) {
        qCritical() << "Error Saving File" << tmp.path();

        if ( autosave != d->m_autosave ) {
            delete autosave;
        }
        return FileWriterFailed;
    }

    if ( autosave != d->m_autosave ) {
        //The order is important: release old lock, delete old locker and then claim new locker.
        d->m_autosave->releaseLock();
        delete d->m_autosave;
        d->m_autosave = autosave;
    }

    setModified( false );
    return NoError;
}

QByteArray KEduVocDocument::toByteArray(const QString &generator)
{
    // no file needed
    KEduVocKvtml2Writer kvtmlWriter(0);
    return kvtmlWriter.toByteArray( this, generator );
}

void KEduVocDocument::merge( KEduVocDocument *docToMerge, bool matchIdentifiers )
{
    Q_UNUSED(docToMerge)
    Q_UNUSED(matchIdentifiers)
    qDebug() << "Merging of docs is not implemented"; /// @todo IMPLEMENT ME
    // This code was really horribly broken.
    // Now with the new classes we could attempt to reactivate it.
    // A rewrite might be easier.

    /*
      if (docToMerge) {

        QStringList new_names = docToMerge->lessonDescriptions();

        QStringList new_types = docToMerge->typeDescriptions();

        QStringList new_tenses = docToMerge->tenseDescriptions();

        QList<int> old_in_query = lessonsInPractice();
        QList<int> new_in_query = docToMerge->lessonsInPractice();

        QStringList new_usages = docToMerge->usageDescriptions();

        int lesson_offset = d->m_lessonDescriptions.count();
        for (int i = 0; i < new_names.count(); i++) {
          d->m_lessonDescriptions.append(new_names[i]);
        }

        for (int i = 0; i < new_in_query.count(); i++)
          old_in_query.append(new_in_query[i] + lesson_offset);
        setLessonsInPractice(old_in_query);

        int types_offset = d->m_typeDescriptions.count();
        for (int i = 0; i < new_types.count(); i++) {
          d->m_typeDescriptions.append(new_types[i]);
        }

        int tenses_offset = d->m_tenseDescriptions.count();
        for (int i = 0; i < new_tenses.count(); i++) {
          d->m_tenseDescriptions.append(new_tenses[i]);
        }

        int usages_offset = d->m_usageDescriptions.count();
        for (int i = 0; i < new_usages.count(); i++) {
          d->m_usageDescriptions.append(new_usages[i]);
        }

        bool equal = true;
        if (originalIdentifier() != docToMerge->originalIdentifier())
          equal = false;
        for (int i = 1; i < identifierCount(); i++)
          if (identifier(i) != docToMerge->identifier(i))
            equal = false;

        if (!matchIdentifiers)
          equal = true; ///@todo massive cheating, problem if docToMerge has more identifiers than this

        if (equal) {   // easy way: same language codes, just append

          for (int i = 0; i < docToMerge->entryCount(); i++) {
            KEduVocExpression *expr = docToMerge->entry(i);

            expr->setLesson(expr->lesson() + lesson_offset);

            for (int lang = 0; lang <= expr->translationCount(); lang++) {
              QString t = expr->translation(lang).type();
              // adjust type offset
              if (!t.isEmpty() && t.left(1) == QM_USER_TYPE) {
                QString t2;
                t.remove(0, 1);
                t2.setNum(t.toInt() + types_offset);
                t2.prepend(QM_USER_TYPE);
                expr->translation(lang).setType (t2);
              }

              t = expr->translation(lang).usageLabel();
              // adjust usage offset
              QString tg;
              if (!t.isEmpty()) {
                QString t2;
                while (t.left(strlen(":")) == UL_USER_USAGE) {
                  QString n;
                  t.remove(0, 1);
                  int next;
                  if ((next = t.indexOf(":")) >= 0) {
                    n = t.left(next);
                    t.remove(0, next + 1);
                  }
                  else {
                    n = t;
                    t = "";
                  }

                  t2.setNum(n.toInt() + usages_offset);
                  t2.prepend(UL_USER_USAGE);
                  if (tg.length() == 0)
                    tg = t2;
                  else
                    tg += ':' + t2;
                }

                if (tg.length() == 0)
                  tg = t;
                else if (t.length() != 0)
                  tg += ':' + t;

                expr->translation(lang).setUsageLabel (tg);
              }

              KEduVocConjugation conj = expr->translation(lang).conjugation();
              bool condirty = false;
              for (int ci = 0; ci < conj.entryCount(); ci++) {
                t = conj.getType(ci);
                if (!t.isEmpty() && t.left(1) == UL_USER_TENSE) {
                  t.remove(0, strlen(UL_USER_TENSE));
                  QString t2;
                  t2.setNum(t.toInt() + tenses_offset);
                  t2.prepend(UL_USER_TENSE);
                  conj.setType(ci, t2);
                  condirty = true;
                }
                if (condirty)
                  expr->translation(lang).setConjugation(conj);
              }
            }

            appendEntry(expr);
          }
          setModified();
        }
        else { // hard way: move entries around, most attributes get lost
          QList<int> move_matrix;
          QList<bool> cs_equal;
          QString s;

          for (int i = 0; i < qMax (identifierCount(), docToMerge->identifierCount()); i++)
            cs_equal.append(false);

          move_matrix.append(docToMerge->indexOfIdentifier(originalIdentifier()));
          for (int i = 1; i < identifierCount(); i++)
            move_matrix.append(docToMerge->indexOfIdentifier(identifier(i)));

          for (int j = 0; j < docToMerge->entryCount(); j++) {
            KEduVocExpression new_expr;
            KEduVocExpression *expr = docToMerge->entry(j);
            new_expr.setLesson(expr->lesson()+lesson_offset);

            for (int i = 0; i < move_matrix.count(); i++) {
              int lpos = move_matrix[i];
              if (lpos >= 0) {

                if (lpos == 0)
                  s = expr->original();
                else
                  s = expr->translation(lpos);

                if (!cs_equal[lpos]) {
                  cs_equal[lpos] = true;
                  QString id = lpos == 0 ? originalIdentifier() : identifier(lpos);
                }

                if (i == 0)
                  new_expr.setOriginal(s);
                else
                  new_expr.setTranslation(i, s);
                QString r = expr->remark(lpos);
                new_expr.setRemark (i, r);

                QString t = expr->type(lpos);
                if (!t.isEmpty() && t.left(1) == QM_USER_TYPE) {
                  QString t2;
                  t.remove(0, 1);
                  t2.setNum(t.toInt() + types_offset);
                  t2.prepend(QM_USER_TYPE);
                  new_expr.setType(i, t2);
                }

                t = expr->usageLabel(lpos);
                if (!t.isEmpty() && t.left(1) == QM_USER_TYPE) {
                  QString t2;
                  t.remove(0, 1);
                  t2.setNum(t.toInt() + usages_offset);
                  t2.prepend(QM_USER_TYPE);
                  new_expr.setUsageLabel(i, t2);
                }

                KEduVocConjugation conj = expr->conjugation(lpos);
                for (int ci = 0; ci < conj.entryCount(); ci++) {
                  t = conj.getType(ci);
                  if (!t.isEmpty() && t.left(1) == QM_USER_TYPE) {
                    t.remove (0, strlen(QM_USER_TYPE));
                    QString t2;
                    t2.setNum(t.toInt() + tenses_offset);
                    t2.prepend(QM_USER_TYPE);
                    conj.setType(ci, t2);
                  }
                }

              }
            }
            // only append if entries are used
            bool used = !new_expr.original().isEmpty();
            for (int i = 1; i < identifierCount(); i++)
              if (!new_expr.translation(i).isEmpty())
                used = true;

            if (used) {
              appendEntry(&new_expr);
              setModified();
            }
          }
        }
      }
    */
}

const KEduVocIdentifier& KEduVocDocument::identifier( int index ) const
{
    if ( index < 0 || index >= d->m_identifiers.size() ) {
        qCritical() << " Error: Invalid identifier index: " << index;
    }
    return d->m_identifiers[index];
}

KEduVocIdentifier& KEduVocDocument::identifier( int index )
{
    if ( index < 0 || index >= d->m_identifiers.size() ) {
        qCritical() << " Error: Invalid identifier index: " << index;
    }
    return d->m_identifiers[index];
}

void KEduVocDocument::setIdentifier( int idx, const KEduVocIdentifier &id )
{
    if ( idx >= 0 && idx < d->m_identifiers.size() ) {
        d->m_identifiers[idx] = id;
    }
    setModified(true);
}

// works if const is removed
int KEduVocDocument::indexOfIdentifier( const QString &name ) const
{
    for (int i = 0; i < identifierCount(); i++)
        if (identifier(i).locale() == name)
            return i;
    return -1;
}

void KEduVocDocument::removeIdentifier( int index )
{
    if ( index < d->m_identifiers.size() && index >= 0 ) {
        d->m_identifiers.removeAt( index );
        d->m_lessonContainer->removeTranslation( index );
    }
}


bool KEduVocDocument::isModified() const
{
    return d->m_dirty;
}


int KEduVocDocument::identifierCount() const
{
    return d->m_identifiers.count();  // number of translations
}

int KEduVocDocument::appendIdentifier( const KEduVocIdentifier& id )
{
    int i = d->m_identifiers.size();
//qDebug() << "appendIdentifier: " << i << id.name() << id.locale();
    d->m_identifiers.append( id );
    if ( id.name().isEmpty() ) {
        if ( i == 0 ) {
            identifier(i).setName(i18nc("The name of the first language/column of vocabulary, if we have to guess it.", "Original"));
        } else {
            identifier(i).setName(i18nc( "The name of the second, third ... language/column of vocabulary, if we have to guess it.", "Translation %1", i ) );
        }
    }


    return i;
}

KEduVocLesson * KEduVocDocument::lesson()
{
    return d->m_lessonContainer;
}

KEduVocWordType * KEduVocDocument::wordTypeContainer()
{
    return d->m_wordTypeContainer;
}

KEduVocLeitnerBox * KEduVocDocument::leitnerContainer()
{
    return d->m_leitnerContainer;
}

QUrl KEduVocDocument::url() const
{
    return d->m_autosave->managedFile();
}

void KEduVocDocument::setUrl( const QUrl& url )
{
    d->m_autosave->setManagedFile(url);
}

QString KEduVocDocument::title() const
{
    if ( d->m_title.isEmpty() )
        return d->m_autosave->managedFile().fileName();
    else
        return d->m_title;
}

void KEduVocDocument::setTitle( const QString & title )
{
    d->m_title = title;
    d->m_lessonContainer->setName(title);
    setModified(true);
}

QString KEduVocDocument::author() const
{
    return d->m_author;
}

void KEduVocDocument::setAuthor( const QString & s )
{
    d->m_author = s.simplified();
    setModified(true);
}

QString KEduVocDocument::authorContact() const
{
    return d->m_authorContact;
}

void KEduVocDocument::setAuthorContact( const QString & s )
{
    d->m_authorContact = s.simplified();
    setModified(true);
}

QString KEduVocDocument::license() const
{
    return d->m_license;
}

QString KEduVocDocument::documentComment() const
{
    return d->m_comment;
}

void KEduVocDocument::setCategory( const QString & category )
{
    d->m_category = category;
    setModified(true);
}

QString KEduVocDocument::category() const
{
    return d->m_category;
    ///@todo make writer/reader use this
}

void KEduVocDocument::queryIdentifier( QString &org, QString &trans ) const
{
    org = d->m_queryorg;
    trans = d->m_querytrans;
}

void KEduVocDocument::setQueryIdentifier( const QString &org, const QString &trans )
{
    d->m_queryorg = org;
    d->m_querytrans = trans;
    setModified(true);
}

void KEduVocDocument::setLicense( const QString & s )
{
    d->m_license = s.simplified();
    setModified(true);
}

void KEduVocDocument::setDocumentComment( const QString & s )
{
    d->m_comment = s.trimmed();
    setModified(true);
}

void KEduVocDocument::setGenerator( const QString & generator )
{
    d->m_generator = generator;
    setModified(true);
}

QString KEduVocDocument::generator() const
{
    return d->m_generator;
}

QString KEduVocDocument::version() const
{
    return d->m_version;
}

void KEduVocDocument::setVersion( const QString & vers )
{
    d->m_version = vers;
    setModified(true);
}

QString KEduVocDocument::csvDelimiter() const
{
    return d->m_csvDelimiter;
}

void KEduVocDocument::setCsvDelimiter( const QString &delimiter )
{
    d->m_csvDelimiter = delimiter;
    setModified(true);
}


QString KEduVocDocument::pattern( FileDialogMode mode )
{
    static const struct SupportedFilter {
        bool reading;
        bool writing;
        const char* extensions;
        const char* description;
    }
    filters[] = {
                    { true, true, "*.kvtml", I18N_NOOP( "KDE Vocabulary Document" ) },
                    { true, false, "*.wql", I18N_NOOP( "KWordQuiz Document" ) },
                    { true, false, "*.xml.qz *.pau.gz", I18N_NOOP( "Pauker Lesson" ) },
                    { true, false, "*.voc", I18N_NOOP( "Vokabeltrainer" ) },
                    { true, false, "*.xdxf", I18N_NOOP( "XML Dictionary Exchange Format" ) },
                    { true, true, "*.csv", I18N_NOOP( "Comma Separated Values (CSV)" ) },
                    // last is marker for the end, do not remove it
                    { false, false, 0, 0 }
                };
    QStringList newfilters;
    QStringList allext;
    for ( int i = 0; filters[i].extensions; ++i ) {
        if (( mode == Reading && filters[i].reading ) ||
                ( mode == Writing && filters[i].writing ) ) {
            newfilters.append( QLatin1String( filters[i].extensions ) + '|' + i18n( filters[i].description ) );
            allext.append( QLatin1String( filters[i].extensions ) );
        }
    }
    if ( mode == Reading ) {
        newfilters.prepend( allext.join( " " ) + '|' + i18n( "All supported documents" ) );
    }
    return newfilters.join( "\n" );
}

QString KEduVocDocument::errorDescription( int errorCode )
{
    switch (errorCode) {
    case NoError:
        return i18n("No error found.");

    case InvalidXml:
        return i18n("Invalid XML in document.");
    case FileTypeUnknown:
        return i18n("Unknown file type.");
    case FileCannotWrite:
        return i18n("File is not writeable.");
    case FileWriterFailed:
        return i18n("File writer failed.");
    case FileCannotRead:
        return i18n("File is not readable.");
    case FileReaderFailed:
        return i18n("The file reader failed.");
    case FileDoesNotExist:
        return i18n("The file does not exist.");
    case FileLocked:
        return i18n("The file is locked by another process.");
    case FileCannotLock:
        return i18n("The lock file can't be created.");
    case Unknown:
    default:
        return i18n("Unknown error.");
    }
}
