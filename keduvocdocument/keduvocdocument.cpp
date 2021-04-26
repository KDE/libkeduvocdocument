/*
 * Vocabulary Document for KDE Edu
 * SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
 * SPDX-FileCopyrightText: 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "keduvocdocument.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QTextStream>
#include <QtAlgorithms>
#include <QIODevice>
#include <QTemporaryFile>

#include <QDebug>
#include <kio/filecopyjob.h>
#include <KFilterDev>
#include <KLocalizedString>
#include <KAutoSaveFile>

#include "keduvocexpression.h"
#include "keduvoclesson.h"
#include "keduvocleitnerbox.h"
#include "keduvocwordtype.h"
#include "keduvockvtmlwriter.h"
#include "keduvockvtml2writer.h"
#include "keduvoccsvwriter.h"
#include "readerwriters/readermanager.h"
#include "readerwriters/readerbase.h"

#define WQL_IDENT      "WordQuiz"

#define KVTML_EXT        "kvtml"
#define CSV_EXT          "csv"
#define TXT_EXT          "txt"
#define WQL_EXT          "wql"

/** @details Private Data class for KEduVocDocument */
class KEduVocDocument::KEduVocDocumentPrivate
{
public:
    /** constructor */
    KEduVocDocumentPrivate( KEduVocDocument* qq )
            : q( qq )
    {
        m_lessonContainer = 0;
        m_wordTypeContainer = 0;
        m_leitnerContainer = 0;
        m_autosave = new KAutoSaveFile;
        init();
    }

    /** destructor */
    ~KEduVocDocumentPrivate();

    /** initializer */
    void init();

    KEduVocDocument* q;                               ///< The Front End

    /** autosave file used to provide locking access to the underlying file
     * Note: It is a pointer to allow locking a new file, saving results and
     * then transferring the lock to m_autosave without risking loss of lock.
     * See saveAs for clarification*/
    KAutoSaveFile            *m_autosave;

    bool                      m_dirty;                ///< dirty bit
    bool                      m_isReadOnly;	      ///< FileOpenReadOnly was used for opening

    // save these to document
    QList<KEduVocIdentifier>  m_identifiers;          ///< list of identifiers

    QList<int>                m_extraSizeHints;       ///< unused
    QList<int>                m_sizeHints;            ///< unused

    QString                   m_generator;            ///< name of generating application
    QString                   m_queryorg;             ///< unused
    QString                   m_querytrans;           ///< unused

    QStringList               m_tenseDescriptions;    ///< unused. Was used in merge
    QSet<QString>             m_usages;               ///< unused

    QString                   m_title;                ///< Document title
    QString                   m_author;               ///< Document author
    QString                   m_authorContact;        ///< Author contact information
    QString                   m_license;              ///< Document license
    QString                   m_comment;              ///< Document comment
    QString                   m_version;              ///< Document version
    QString                   m_csvDelimiter;         ///< CSV delimiter

    /** Categories that can later be used to sork kvtml files:
      * language, music, children, anatomy
      */
    QString                   m_category;            ///< Document category

    KEduVocLesson * m_lessonContainer;               ///< Root lesson container
    KEduVocWordType * m_wordTypeContainer;           ///< Root word types container
    /** Root Leitner container
     * (probably unused) This is not used in Parley
     * */
    KEduVocLeitnerBox * m_leitnerContainer;

    /** Creates an autosave file for fpath in order to lock the file
     *   @param autosave a reference to an allocated KAutosave
     *   @param fpath File path to the file to be locked
     *   @param flags Describes how to deal with locked file etc.
     *   @return ErrorCode where NoError is success
     *   */
    KEduVocDocument::ErrorCode initializeKAutoSave(KAutoSaveFile &autosave,
						   QString const &fpath,
						   FileHandlingFlags flags) const;
};

KEduVocDocument::KEduVocDocumentPrivate::~KEduVocDocumentPrivate()
{
    delete m_lessonContainer;
    delete m_wordTypeContainer;
    delete m_leitnerContainer;
    delete m_autosave;
}

void KEduVocDocument::KEduVocDocumentPrivate::init()
{
    delete m_lessonContainer;
    m_lessonContainer = new KEduVocLesson(i18nc("The top level lesson which contains all other lessons of the document.",
						"Document Lesson"),
					  q);
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
    m_isReadOnly = false;
    m_queryorg = QLatin1String("");
    m_querytrans = QLatin1String("");
    m_autosave->setManagedFile( QUrl() );
    m_author = QLatin1String("");
    m_title = QLatin1String("");
    m_comment = QLatin1String("");
    m_version = QLatin1String("");
    m_generator = QLatin1String("");
    m_csvDelimiter = QString( '\t' );
    m_usages.clear();
    m_license.clear();
    m_category.clear();
}


KEduVocDocument::ErrorCode KEduVocDocument::KEduVocDocumentPrivate::initializeKAutoSave(KAutoSaveFile &autosave,  QString const & fpath,  FileHandlingFlags flags) const {

    QList<KAutoSaveFile *> staleFiles = KAutoSaveFile::staleFiles( QUrl::fromLocalFile(fpath),
                                                                 QCoreApplication::instance()->applicationName());
    if ( !staleFiles.isEmpty()) {
        if ( flags & FileIgnoreLock ) {
            foreach( KAutoSaveFile *f,  staleFiles ) {
                f->open( QIODevice::ReadWrite );
                f->remove();
                delete f;
            }
        } else {
            qWarning() << i18n( "Cannot lock file %1", fpath );
            return FileLocked;
        }
    }

    autosave.setManagedFile( QUrl::fromLocalFile(fpath) );
    if ( !autosave.open( QIODevice::ReadWrite ) ) {
        qWarning() << i18n( "Cannot lock file %1", autosave.fileName() );
        return FileCannotLock;
    }

    return NoError;
}


KEduVocDocument::KEduVocDocument( QObject *parent )
    : QObject(parent)
    , d(new KEduVocDocumentPrivate(this))
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


KEduVocDocument::FileType KEduVocDocument::detectFileType(const QString &fileName)
{
    KFilterDev f(fileName);
    f.open(QIODevice::ReadOnly);

    ReaderManager::ReaderPtr reader(ReaderManager::reader(f));

    f.close();

    return reader->fileTypeHandled();
}


KEduVocDocument::ErrorCode KEduVocDocument::open(const QUrl &url, FileHandlingFlags flags)
{
    // save csv delimiter to preserve it in case this is a csv document
    QString csv = d->m_csvDelimiter;
    // clear all other properties
    d->init();
    if (!url.isEmpty()) {
        setUrl(url);
    }
    d->m_csvDelimiter = csv;

    KEduVocDocument::ErrorCode errStatus = Unknown;
    QString errorMessage = i18n("<qt>Cannot open file<br /><b>%1</b></qt>", url.toDisplayString());

    QString temporaryFile;
    QTemporaryFile tempFile;
    if (url.isLocalFile()) {
        temporaryFile = url.toLocalFile();
    } else {
        if (!tempFile.open()) {
            qWarning() << i18n("Cannot open tempfile %1",  tempFile.fileName());
            return Unknown;
        }
        KIO::FileCopyJob *job = KIO::file_copy(url, QUrl::fromLocalFile(tempFile.fileName()), -1, KIO::Overwrite);
        if (!job->exec()) {
            qWarning() << i18n("Cannot download %1: %2",  url.toDisplayString(), job->errorString());
            return FileDoesNotExist;
        }
        temporaryFile = tempFile.fileName();
    }

    if (flags & FileOpenReadOnly) {
        d->m_isReadOnly = true;
    }

    ErrorCode autosaveError = NoError;

    if (!d->m_isReadOnly) {
        autosaveError = d->initializeKAutoSave(*d->m_autosave,  temporaryFile, flags);
        if (autosaveError != NoError) {
            return autosaveError;
        }
    }

    KFilterDev f(temporaryFile);
    if (f.open(QIODevice::ReadOnly)) {

        ReaderManager::ReaderPtr reader(ReaderManager::reader(f));
        errStatus = reader->read(*this);

        if (errStatus != KEduVocDocument::NoError) {
            errorMessage = i18n("Could not open or properly read \"%1\"\n(Error reported: %2)"
                                , url.toDisplayString(), reader->errorMessage());
        }
    } else {
        errStatus = FileCannotRead;
    }

    f.close();

    if (errStatus == KEduVocDocument::NoError) {
        setModified(false);
    } else {
        qWarning() << errorMessage;
    }

    return errStatus;
}

void KEduVocDocument::close()
{
    if (!d->m_isReadOnly) {
	d->m_autosave->releaseLock();
    }
}

KEduVocDocument::ErrorCode KEduVocDocument::saveAs( const QUrl & url, FileType ft,
						    FileHandlingFlags flags)
{
    if (d->m_isReadOnly) {
        return FileIsReadOnly;
    }

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

    QString errorMessage = i18n( "Cannot write to file %1", tmp.toDisplayString() );

    KAutoSaveFile *autosave;

    //If we don't care about the lock always create a new one
    //If we are changing files create a new lock
    if ( ( flags & FileIgnoreLock )
        || ( d->m_autosave->managedFile() != tmp ) ) {

        autosave = new KAutoSaveFile;
        ErrorCode autosaveError = d->initializeKAutoSave( *autosave, tmp.toLocalFile(), flags );
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

    QFile f( tmp.toLocalFile() );
    if ( !f.open( QIODevice::WriteOnly ) ) {
        qCritical() << i18n( "Cannot write to file %1", f.fileName() );
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
            qCritical() << "kvcotrainDoc::saveAs(): unknown filetype";
        }
        break;
    } // switch

    f.close();

    if ( !saved ) {
        qCritical() << "Error Saving File" << tmp.toDisplayString();

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
    ///@todo decouple document title and root lesson title
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
            newfilters.append( i18n( filters[i].description ) + " (" + QLatin1String( filters[i].extensions ) + ')' );
            allext.append( QLatin1String( filters[i].extensions ) );
        }
    }
    if ( mode == Reading ) {
        newfilters.prepend( allext.join(QLatin1Char(' ') ) + '|' + i18n( "All supported documents" ) );
    }
    return newfilters.join(QLatin1String(";;") );
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
