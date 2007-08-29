/***************************************************************************
                        Vocabulary Document for KDE Edu
    -----------------------------------------------------------------------
    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
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

#include <klocale.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kio/netaccess.h>
#include <krandomsequence.h>
#include <kfilterdev.h>

#include "keduvocexpression.h"
#include "keduvoclesson.h"
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
#include "leitnersystem.h"

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
        init();
    }

    void init();

    KEduVocDocument* q;

    bool                      m_dirty;
    KUrl                      m_url;
    QList<bool>               m_sortIdentifier;
    bool                      m_sortLesson;

    // save these to document
    QStringList               m_identifiers;      //0= origin, 1,.. translations
    int                       m_currentLesson;
    QList<int>                m_extraSizeHints;
    QList<int>                m_sizeHints;

    QString                   m_generator;
    QString                   m_queryorg;
    QString                   m_querytrans;
    QList<KEduVocExpression>  m_vocabulary;
    QList<int>                m_lessonsInQuery;

    QStringList               m_tenseDescriptions;
    QSet<QString>             m_usages;
    QString                   m_title;
    QString                   m_author;
    QString                   m_license;
    QString                   m_comment;
    QString                   m_version;
    QString                   m_csvDelimiter;

    /** Categories that can later be used to sork kvtml files:
      * language, music, children, anatomy
      */
    QString                   m_category;

    QList<KEduVocArticle>     m_articles;
    QList<KEduVocConjugation> m_conjugations;

    // make this a map so removals don't require renumbering :)
    QMap<int, KEduVocLesson*>  m_lessons;

    KEduVocWordType*          m_wordTypes;

    LeitnerSystem*            m_leitnerSystem;
    bool                      m_activeLeitnerSystem;
};


void KEduVocDocument::KEduVocDocumentPrivate::init()
{
    m_lessons.clear();
    m_articles.clear();
    m_tenseDescriptions.clear();
    m_identifiers.clear();
    m_sortIdentifier.clear();
    m_extraSizeHints.clear();
    m_sizeHints.clear();
    m_vocabulary.clear();
    m_dirty = false;
    m_sortLesson = false;
    m_currentLesson = 0;
    m_queryorg = "";
    m_querytrans = "";
    m_url.setFileName( i18n( "Untitled" ) );
    m_title = "";
    m_author = "";
    m_comment = "";
    m_version = "";
    m_generator = "";
    m_csvDelimiter = QString( '\t' );
    m_activeLeitnerSystem = false;
    m_leitnerSystem = NULL;

    m_wordTypes = new KEduVocWordType();
}


KEduVocDocument::KEduVocDocument( QObject *parent )
        : QObject( parent ), d( new KEduVocDocumentPrivate( this ) )
{}


KEduVocDocument::~KEduVocDocument()
{
    delete d->m_wordTypes;
    delete d;
}


void KEduVocDocument::setModified( bool dirty )
{
    d->m_dirty = dirty;
    emit docModified( d->m_dirty );
}


void KEduVocDocument::appendEntry( KEduVocExpression *expression )
{
    d->m_vocabulary.append( *expression );
    setModified();
}


void KEduVocDocument::insertEntry( KEduVocExpression *expression, int index )
{
    d->m_vocabulary.insert( index, *expression );
    setModified();
}


KEduVocDocument::FileType KEduVocDocument::detectFileType( const QString &fileName )
{
    QIODevice * f = KFilterDev::deviceForFile( fileName );
    if ( !f->open( QIODevice::ReadOnly ) )
        return Csv;

    QDataStream is( f );

    qint8 c1, c2, c3, c4, c5;
    is >> c1
    >> c2
    >> c3
    >> c4
    >> c5;  // guess filetype by first x bytes

    QTextStream ts( f );
    QString line;
    QString line2;

    line = ts.readLine();
    if ( !ts.atEnd() )
        line2 = ts.readLine();
    line.prepend( c5 );
    line.prepend( c4 );
    line.prepend( c3 );
    line.prepend( c2 );
    line.prepend( c1 );

    if ( !is.device()->isOpen() )
        return KvdNone;

    f->close();
    if ( c1 == '<' && c2 == '?' && c3 == 'x' && c4 == 'm' && c5 == 'l' ) {
        if ( line2.indexOf( "pauker", 0 ) >  0 )
            return Pauker;
        else if ( line2.indexOf( "xdxf", 0 ) >  0 )
            return Xdxf;
        else
            return Kvtml;
    }

    if ( line == WQL_IDENT )
        return Wql;

    if ( c1 == '"' && ( line.contains( '"' ) == 1 || line.contains( QRegExp( "\",[0-9]" ) ) ) )
        return Vokabeln;

    return Csv;
}


bool KEduVocDocument::open( const KUrl& url )
{
    d->init();
    if ( !url.isEmpty() )
        d->m_url = url;

    bool read = false;
    QString errorMessage = i18n( "<qt>Cannot open file<br /><b>%1</b></qt>", url.path() );
    QString temporaryFile;
    if ( KIO::NetAccess::download( url, temporaryFile, 0 ) ) {
        QIODevice * f = KFilterDev::deviceForFile( temporaryFile );

        if ( !f->open( QIODevice::ReadOnly ) ) {
            KMessageBox::error( 0, errorMessage );
            return false;
        }

        FileType ft = detectFileType( url.path() );

        switch ( ft ) {
            case Kvtml: {
                KEduVocKvtml2Reader kvtmlReader( f );
                read = kvtmlReader.readDoc( this );
                if ( !read )
                    errorMessage = kvtmlReader.errorMessage();
            }
            break;

            case Wql: {
                KEduVocWqlReader wqlReader( f );
                d->m_url.setFileName( i18n( "Untitled" ) );
                read = wqlReader.readDoc( this );
                if ( !read )
                    errorMessage = wqlReader.errorMessage();
            }
            break;

            case Pauker: {
                KEduVocPaukerReader paukerReader( this );
                d->m_url.setFileName( i18n( "Untitled" ) );
                read = paukerReader.read( f );
                if ( !read )
                    errorMessage = i18n( "Parse error at line %1, column %2:\n%3", paukerReader.lineNumber(), paukerReader.columnNumber(), paukerReader.errorString() );
            }
            break;

            case Vokabeln: {
                KEduVocVokabelnReader vokabelnReader( f );
                d->m_url.setFileName( i18n( "Untitled" ) );
                read = vokabelnReader.readDoc( this );
                if ( !read )
                    errorMessage = vokabelnReader.errorMessage();
            }
            break;

            case Csv: {
                KEduVocCsvReader csvReader( f );
                read = csvReader.readDoc( this );
                if ( !read )
                    errorMessage = csvReader.errorMessage();
            }
            break;

            case Xdxf: {
                KEduVocXdxfReader xdxfReader( this );
                d->m_url.setFileName( i18n( "Untitled" ) );
                read = xdxfReader.read( f );
                if ( !read )
                    errorMessage = i18n( "Parse error at line %1, column %2:\n%3", xdxfReader.lineNumber(), xdxfReader.columnNumber(), xdxfReader.errorString() );
            }
            break;

            default: {
                KEduVocKvtml2Reader kvtmlReader( f );
                read = kvtmlReader.readDoc( this );
                if ( !read )
                    errorMessage = kvtmlReader.errorMessage();
            }
        }

        if ( !read ) {
            QString msg = i18n( "Could not open or properly read \"%1\"\n(Error reported: %2)", url.path(), errorMessage );
            KMessageBox::error( 0, msg, i18n( "Error Opening File" ) );
        }

        f->close();
        KIO::NetAccess::removeTempFile( temporaryFile );
    }
    return read;
}


bool KEduVocDocument::saveAs( const KUrl & url, FileType ft, const QString & generator )
{
    KUrl tmp( url );

    if ( ft == Automatic ) {
        if ( tmp.path().right( strlen( "." KVTML_EXT ) ) == "." KVTML_EXT )
            ft = Kvtml;
        else if ( tmp.path().right( strlen( "." CSV_EXT ) ) == "." CSV_EXT )
            ft = Csv;
        else {
            return false;
        }
    }

    bool saved = false;

    while ( !saved ) {
        QFile f( tmp.path() );

        if ( !f.open( QIODevice::WriteOnly ) ) {
            KMessageBox::error( 0, i18n( "<qt>Cannot write to file<br /><b>%1</b></qt>", tmp.path() ) );
            return false;
        }

        switch ( ft ) {
            case Kvtml: {
                // write version 2 file
                KEduVocKvtml2Writer kvtmlWriter( &f );
                saved = kvtmlWriter.writeDoc( this, generator );
            }
            break;
            case Kvtml1: {
                // write old version 1 file
                KEduVocKvtmlWriter kvtmlWriter( &f );
                saved = kvtmlWriter.writeDoc( this, generator );
            }
            break;
            case Csv: {
                KEduVocCsvWriter csvWriter( &f );
                saved = csvWriter.writeDoc( this, generator );
            }
            break;
            default: {
                kError() << "kvcotrainDoc::saveAs(): unknown filetype" << endl;
            }
            break;
        }
        f.close();

        if ( !saved ) {
            QString msg = i18n( "Could not save \"%1\"\nDo you want to try again?", tmp.path() );
            int result = KMessageBox::warningContinueCancel( 0, msg, i18n( "Error Saving File" ), KGuiItem( i18n( "&Retry" ) ) );
            if ( result == KMessageBox::Cancel )
                return false;
        }
    }
    d->m_url = tmp;
    setModified( false );
    return true;
}



void KEduVocDocument::merge( KEduVocDocument *docToMerge, bool matchIdentifiers )
{
    kDebug() << "Merging of docs is not implemented"; /// @todo IMPLEMENT ME
    // This code was really horribly broken.
    // Now with the new classes we could attempt to reactivate it.
    // A rewrite might be easier.

    /*
      if (docToMerge) {

        QStringList new_names = docToMerge->lessonDescriptions();

        QStringList new_types = docToMerge->typeDescriptions();

        QStringList new_tenses = docToMerge->tenseDescriptions();

        QList<int> old_in_query = lessonsInQuery();
        QList<int> new_in_query = docToMerge->lessonsInQuery();

        QStringList new_usages = docToMerge->usageDescriptions();

        int lesson_offset = d->m_lessonDescriptions.count();
        for (int i = 0; i < new_names.count(); i++) {
          d->m_lessonDescriptions.append(new_names[i]);
        }

        for (int i = 0; i < new_in_query.count(); i++)
          old_in_query.append(new_in_query[i] + lesson_offset);
        setLessonsInQuery(old_in_query);

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


KEduVocExpression *KEduVocDocument::entry( int index )
{
    if ( index < 0 || index >= d->m_vocabulary.size() )
        return 0;
    else
        return &d->m_vocabulary[index];
}


void KEduVocDocument::removeEntry( int index )
{
    if ( index >= 0 && index < d->m_vocabulary.size() )
        d->m_vocabulary.removeAt( index );
}


int KEduVocDocument::indexOfIdentifier( const QString &lang ) const
{
    QStringList::const_iterator first = d->m_identifiers.begin();
    int count = 0;
    while ( first != d->m_identifiers.end() ) {
        if ( *first == lang )
            return count;
        first++;
        count++;
    }
    return -1;
}


QString KEduVocDocument::identifier( int index ) const
{
    if ( index >= d->m_identifiers.size() || index < 0 )
        return "";
    else
        return d->m_identifiers[index];
}


void KEduVocDocument::setIdentifier( int idx, const QString &id )
{
    if ( idx < d->m_identifiers.size() && idx >= 0 ) {
        d->m_identifiers[idx] = id;
    }
}


QString KEduVocDocument::tenseName( int index ) const
{
    if ( index >= d->m_tenseDescriptions.size() )
        return "";
    else
        return d->m_tenseDescriptions[index];
}


void KEduVocDocument::setTenseName( int idx, QString &id )
{
    if ( idx >= d->m_tenseDescriptions.size() )
        for ( int i = d->m_tenseDescriptions.size(); i <= idx; i++ )
            d->m_tenseDescriptions.append( "" );

    d->m_tenseDescriptions[idx] = id;
}


QStringList KEduVocDocument::tenseDescriptions() const
{
    return d->m_tenseDescriptions;
}


void KEduVocDocument::setTenseDescriptions( const QStringList &names )
{
    d->m_tenseDescriptions = names;
}


void KEduVocDocument::setConjugation( int idx, const KEduVocConjugation &con )
{
    if ( idx < 0 ) return;

    // extend conjugation with empty elements
    if ( d->m_conjugations.size() <= idx )
        for ( int i = d->m_conjugations.size(); i < idx+1; i++ )
            d->m_conjugations.append( KEduVocConjugation() );

    d->m_conjugations[idx] = con;
}


int KEduVocDocument::conjugationCount() const
{
    return d->m_conjugations.count();
}


KEduVocConjugation KEduVocDocument::conjugation( int idx ) const
{
    if ( idx >= d->m_conjugations.size() || idx < 0 ) {
        return KEduVocConjugation();
    } else {
        return d->m_conjugations[idx];
    }
}


void KEduVocDocument::setArticle( int idx, const KEduVocArticle &art )
{
    if ( idx < 0 ) return;

    // extend conjugation with empty elements
    if ( d->m_articles.size() <= idx )
        for ( int i = d->m_articles.size(); i < idx+1; i++ )
            d->m_articles.append( KEduVocArticle() );

    d->m_articles[idx] = art;
}


KEduVocArticle KEduVocDocument::article( int idx ) const
{
    if ( idx >= d->m_articles.size() || idx < 0 ) {
        return KEduVocArticle();
    } else {
        return d->m_articles[idx];
    }
}


int KEduVocDocument::articleCount() const
{
    return d->m_articles.count();
}


int KEduVocDocument::sizeHint( int idx ) const
{
    if ( idx < 0 ) {
        idx = -idx;
        if ( idx >= d->m_extraSizeHints.size() )
            return 80; // make a good guess about column size
        else {
//      cout << "gsh " << idx << "  " << extraSizehints[idx] << endl;
            return d->m_extraSizeHints[idx];
        }
    } else {
        if ( idx >= d->m_sizeHints.size() )
            return 150; // make a good guess about column size
        else {
//      cout << "gsh " << idx << "  " << sizehints[idx] << endl;
            return d->m_sizeHints[idx];
        }
    }
}


void KEduVocDocument::setSizeHint( int idx, const int width )
{
//  cout << "ssh " << idx << "  " << width << endl;
    if ( idx < 0 ) {
        idx = -idx;
        if ( idx >= d->m_extraSizeHints.size() ) {
            for ( int i = d->m_extraSizeHints.size(); i <= idx; i++ )
                d->m_extraSizeHints.append( 80 );
        }
        d->m_extraSizeHints[idx] = width;

    } else {
        if ( idx >= d->m_sizeHints.size() ) {
            for ( int i = d->m_sizeHints.size(); i <= idx; i++ )
                d->m_sizeHints.append( 150 );
        }
        d->m_sizeHints[idx] = width;
    }
}


void KEduVocDocument::removeIdentifier( int index )
{
    if ( index < d->m_identifiers.size() && index >= 1 ) {
        d->m_identifiers.removeAt( index );
        for ( int i = 0; i < d->m_vocabulary.count(); i++ )
            d->m_vocabulary[i].removeTranslation( index );
    }
}


bool KEduVocDocument::isModified() const
{
    return d->m_dirty;
}


bool KEduVocDocument::leitnerSystemActive()
{
    return d->m_activeLeitnerSystem;
}

void KEduVocDocument::setLeitnerSystemActive( bool yes )
{
    if ( yes ) {
        if ( d->m_leitnerSystem == 0 )
            createStandardLeitnerSystem(); //if nothing is loaded yet

        d->m_activeLeitnerSystem = true;
    } else if ( !yes )
        d->m_activeLeitnerSystem = false;
}

void KEduVocDocument::createStandardLeitnerSystem()
{
    LeitnerSystem* tmpSystem = new LeitnerSystem();
    QString name = "Standard";

    tmpSystem->setSystemName( name );
    tmpSystem->insertBox( "Box 1" );
    tmpSystem->insertBox( "Box 2" );
    tmpSystem->insertBox( "Box 3" );
    tmpSystem->insertBox( "Box 4" );
    tmpSystem->insertBox( "Box 5" );

    tmpSystem->setCorrectBox( "Box 1", "Box 2" );
    tmpSystem->setWrongBox( "Box 1", "Box 1" );

    tmpSystem->setCorrectBox( "Box 2", "Box 3" );
    tmpSystem->setWrongBox( "Box 2", "Box 1" );

    tmpSystem->setCorrectBox( "Box 3", "Box 4" );
    tmpSystem->setWrongBox( "Box 3", "Box 1" );

    tmpSystem->setCorrectBox( "Box 4", "Box 5" );
    tmpSystem->setWrongBox( "Box 4", "Box 1" );

    tmpSystem->setCorrectBox( "Box 5", "Box 1" );
    tmpSystem->setWrongBox( "Box 5", "Box 1" );

    d->m_leitnerSystem = tmpSystem;
}

void KEduVocDocument::setLeitnerSystem( LeitnerSystem* system )
{
    d->m_leitnerSystem = system;
}

LeitnerSystem* KEduVocDocument::leitnerSystem()
{
    return d->m_leitnerSystem;
}


int KEduVocDocument::entryCount() const
{
    return d->m_vocabulary.count();
}


void KEduVocDocument::resetEntry( int index, int lesson )
{
    for ( int i = 0; i < d->m_vocabulary.count(); i++ )
        if ( /*lesson == 0 ||*/ lesson == d->m_vocabulary[i].lesson() ) {
            // index is the translation number whose grades are reset
            d->m_vocabulary[i].resetGrades( index );
        }
}


int KEduVocDocument::identifierCount() const
{
    return d->m_identifiers.count();  // number of translations
}

int KEduVocDocument::appendIdentifier( const QString & id )
{
    d->m_identifiers.append( id );
    return d->m_identifiers.size() - 1;
}

//QString KEduVocDocument::lessonDescription(int idx) const
//{
//  if (idx == 0)
//    return i18nc("@label:listbox","<placeholder>no lesson</placeholder>");

//  if (idx <= 0 || idx > d->m_lessons.size() )
//    return "";

//  return d->m_lessons[idx-1].description();
//}

//int KEduVocDocument::lessonIndex(const QString &description) const
//{
//  return d->m_lessonDescriptions.indexOf(description) +1;
//}


int KEduVocDocument::addLesson( const QString &lessonName, int position )
{
    if ( position == -1 ) {
        // no position was specified, so put it wherever there's a slot
        position = 1;
        while ( d->m_lessons.contains( position ) ) {
            ++position;
        }
    }

    KEduVocLesson *lesson = new KEduVocLesson;
    lesson->setDescription( lessonName );
    d->m_lessons.insert( position, lesson );
    return position;
}

QMap<int, KEduVocLesson *> KEduVocDocument::lessons() const
{
    return d->m_lessons;
}

KEduVocLesson * KEduVocDocument::lesson( int index )
{
    KEduVocLesson * retval( NULL );
    if ( d->m_lessons.contains( index ) ) {
        retval = d->m_lessons[index];
    }
    return retval;
}

//void KEduVocDocument::renameLesson(const int lessonIndex, const QString &lessonName)
//{
//  d->m_lessonDescriptions.replace(lessonIndex-1, lessonName); // counting from 1
//}


bool KEduVocDocument::lessonInQuery( int lessonIndex ) const
{
    return d->m_lessonsInQuery.contains( lessonIndex );
}


void KEduVocDocument::addLessonToQuery( int lessonIndex )
{
    if ( !lessonInQuery( lessonIndex ) )
        d->m_lessonsInQuery.append( lessonIndex );
}


void KEduVocDocument::removeLessonFromQuery( int lessonIndex )
{
    if ( lessonInQuery( lessonIndex ) )
        d->m_lessonsInQuery.removeAt( d->m_lessonsInQuery.indexOf( lessonIndex ) );
}


QList<int> KEduVocDocument::lessonsInQuery() const
{
    return d->m_lessonsInQuery;
}


void KEduVocDocument::setLessonsInQuery( const QList<int> &lesson_iq )
{
    d->m_lessonsInQuery = lesson_iq;
}

KUrl KEduVocDocument::url() const
{
    return d->m_url;
}


void KEduVocDocument::setUrl( const KUrl& url )
{
    d->m_url = url;
}


QString KEduVocDocument::title() const
{
    if ( d->m_title.isEmpty() )
        return d->m_url.fileName();
    else
        return d->m_title;
}


QString KEduVocDocument::author() const
{
    return d->m_author;
}


QString KEduVocDocument::license() const
{
    return d->m_license;
}


QString KEduVocDocument::documentComment() const
{
    return d->m_comment;
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
}


void KEduVocDocument::setTitle( const QString & title )
{
    d->m_title = title.simplified();
}


void KEduVocDocument::setAuthor( const QString & s )
{
    d->m_author = s.simplified();
}


void KEduVocDocument::setLicense( const QString & s )
{
    d->m_license = s.simplified();
}


void KEduVocDocument::setDocumentComment( const QString & s )
{
    d->m_comment = s.simplified();
}


void KEduVocDocument::setGenerator( const QString & generator )
{
    d->m_generator = generator;
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
}


int KEduVocDocument::currentLesson() const
{
    return d->m_currentLesson;
}


void KEduVocDocument::setCurrentLesson( int lesson )
{
    d->m_currentLesson = lesson;
}


QStringList KEduVocDocument::lessonDescriptions() const
{
    QStringList descriptions;
    QList<KEduVocLesson*> lessonObjects = lessons().values();
    for ( int i = 0; i < lessonObjects.count(); ++i ) {
        descriptions.append( lessonObjects[i]->description() );
    }
    return descriptions;
}


int KEduVocDocument::lessonCount() const
{
    return d->m_lessons.count();
}

bool KEduVocDocument::deleteLesson( int lessonIndex, int deleteMode )
{  // too bad we count from one!
    lessonIndex++;
    for ( int ent = entryCount() - 1; ent  >= 0 ; ent-- ) {
        if ( entry( ent )->lesson() == lessonIndex ) {
            if ( deleteMode == DeleteEmptyLesson )
                return false; // stop if there are vocabs left in the lesson
            if ( deleteMode == DeleteEntriesAndLesson )
                // delete entries of this lesson with this lesson
                removeEntry( ent );
        }
    }//for entries

    // for all above this lesson number - reduce lesson by one.
    for ( int ent = 0; ent < entryCount(); ent++ ) {
        if ( entry( ent )->lesson() > lessonIndex ) {
            entry( ent )->setLesson( entry( ent )->lesson()-1 );
        }
    } // reduce lesson

    // finally just remove the lesson name
    //d->m_lessonDescriptions.removeAt(lessonIndex-1); // because of the damned 0 arghh

    int currentInQuery = d->m_lessonsInQuery.indexOf( lessonIndex );
    if ( currentInQuery != -1 )
        d->m_lessonsInQuery.removeAt( currentInQuery );

    // move query entries
    for ( int queryLesson = 0; queryLesson < d->m_lessonsInQuery.count(); queryLesson++ ) {
        if ( d->m_lessonsInQuery.at( queryLesson ) > lessonIndex )
            d->m_lessonsInQuery.replace( queryLesson, d->m_lessonsInQuery.at( queryLesson )-1 );
    }
    return true;
}


//void KEduVocDocument::setLessonDescriptions(const QStringList &names)
//{
//  d->m_lessonDescriptions = names;
//}

//void KEduVocDocument::moveLesson(int from, int to)
//{
/////@todo move in query as well!
//  // still counting from 1
//  d->m_lessonDescriptions.move(from -1, to -1);

//  /*
//  to > from?
//    lesson >= from && lesson < to: lesson++
//  to < from?
//    lesson >= to && lesson < from: lesson++
//  */
//  for (int ent = 0; ent < entryCount(); ent++) {
//    // put from directly to to
//    if (entry(ent)->lesson() == from) {
//      entry(ent)->setLesson(to);
//    }
//    else
//    {
//      if(to > from)
//      {
//        if(entry(ent)->lesson() >= from && entry(ent)->lesson() < to)
//          entry(ent)->setLesson(entry(ent)->lesson()-1);
//      }
//      else
//      {
//        if(entry(ent)->lesson() >= to && entry(ent)->lesson() < from)
//          entry(ent)->setLesson(entry(ent)->lesson()+1);
//      }
//    }
//  }
//}


QString KEduVocDocument::csvDelimiter() const
{
    return d->m_csvDelimiter;
}


void KEduVocDocument::setCsvDelimiter( const QString &delimiter )
{
    d->m_csvDelimiter = delimiter;
}


class ExpRef
{

public:
    ExpRef()
    {}
    ExpRef( KEduVocExpression *_exp, int _idx )
    {
        idx    = _idx;
        exp    = _exp;
    }

    bool operator< ( const ExpRef& y ) const
    {
        QString s1;
        QString s2;
        int cmp;
        foreach( int i, exp->translationIndices() ) {

            s1 = exp->translation( i ).text();
            s2 = y.exp->translation( i ).text();
            cmp = QString::compare( s1.toUpper(), s2.toUpper() );
            if ( cmp != 0 )
                return cmp < 0;
        }
        return cmp < 0;
    }

    int idx;
    KEduVocExpression *exp;
};

typedef QList<ExpRef> ExpRefList;

int KEduVocDocument::cleanUp()
{
    int count = 0;
    KEduVocExpression *kve1, *kve2;
    ExpRefList shadow;
    QList<int> to_delete;

    for ( int i = 0; i < d->m_vocabulary.count(); i++ ) {
        shadow.append( ExpRef( entry( i ), i ) );
    }
    qStableSort( shadow.begin(), shadow.end() );

    int ent_no = 0;
    int ent_percent = d->m_vocabulary.size() / 100;
    float f_ent_percent = d->m_vocabulary.size() / 100.0;
    emit progressChanged( this, 0 );

    for ( int i = shadow.size() - 1; i > 0; i-- ) {
        kve1 = shadow[i].exp;
        kve2 = shadow[i - 1].exp;

        ent_no++;
        if ( ent_percent != 0 && ( ent_no % ent_percent ) == 0 ) {
            emit progressChanged( this, ( int )(( ent_no / f_ent_percent ) / 2.0 ) );
        }

        bool equal = true;
        for ( int l = 0; equal && l < identifierCount(); l++ ) {
            if ( kve1->translation( l ).text() != kve2->translation( l ).text() ) {
                equal = false;
            }
        }

        if ( equal ) {
            to_delete.append( shadow[i - 1].idx );
            count++;
        }
    }

    // removing might take very long
    ent_no = 0;
    ent_percent = to_delete.size() / 100;
    f_ent_percent = to_delete.size() / 100.0;
    emit progressChanged( this, 0 );

    qStableSort( to_delete.begin(), to_delete.end() );

    for ( int i = ( int ) to_delete.count() - 1; i >= 0; i-- ) {
        ent_no++;
        if ( ent_percent != 0 && ( ent_no % ent_percent ) == 0 )
            emit progressChanged( this, ( int )( 50 + ent_no / f_ent_percent / 2.0 ) );
        removeEntry( to_delete[i] );
        setModified();
    }

    return count;
}


void KEduVocDocument::shuffle()
{
    KRandomSequence rs;
    rs.randomize( d->m_vocabulary );
    setModified();
}


QString KEduVocDocument::pattern( Mode mode )
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
                    { true, true, "*.csv", I18N_NOOP( "Text" ) },
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
    newfilters.prepend( allext.join( " " ) + '|' + i18n( "All supported documents" ) );
    return newfilters.join( "\n" );
}


KEduVocWordType* KEduVocDocument::wordTypes()
{
    return d->m_wordTypes;
}


QStringList KEduVocDocument::usages()
{
    return d->m_usages.values();
}


void KEduVocDocument::addUsage( const QString &usage )
{
    d->m_usages.insert( usage );
}


void KEduVocDocument::renameUsage( const QString &oldName, const QString &newName )
{
    if ( d->m_usages.contains( oldName ) ) {
        d->m_usages.remove( oldName );
        d->m_usages.insert( newName );
    } else {
        return;
    }

    for ( int i = 0; i < d->m_vocabulary.count(); i++ ) {
        foreach( int translationIndex, d->m_vocabulary[i].translationIndices() ) {
            if ( d->m_vocabulary[i].translation( translationIndex ).usages().contains( oldName ) ) {
                d->m_vocabulary[i].translation( translationIndex ).usages().remove( oldName );
                d->m_vocabulary[i].translation( translationIndex ).usages().insert( newName );
            }
        }
    }
}


void KEduVocDocument::removeUsage( const QString &name )
{
    d->m_usages.remove( name );

    for ( int i = 0; i < d->m_vocabulary.count(); i++ ) {
        foreach( int translationIndex, d->m_vocabulary[i].translationIndices() ) {
            d->m_vocabulary[i].translation( translationIndex ).usages().remove( name );
        }
    }
}




#include "keduvocdocument.moc"
