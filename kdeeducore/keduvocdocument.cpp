/***************************************************************************
                        Vocabulary Document for KDE Edu
    -----------------------------------------------------------------------
    copyright      : (C) 1999-2001 Ewald Arnold
                     (C) 2001 The KDE-EDU team
                     (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
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

#include <QApplication>
#include <QFileInfo>
#include <QTextStream>
#include <QtAlgorithms>
#include <QIODevice>

#include <klocale.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kio/netaccess.h>
#include <krandomsequence.h>
#include <kfilterdev.h>

#include "keduvocexpression.h"
#include "keduvockvtmlwriter.h"
#include "keduvoccsvreader.h"
#include "keduvoccsvwriter.h"
#include "keduvockvtmlreader.h"
#include "keduvocwqlreader.h"
#include "keduvocpaukerreader.h"
#include "keduvocvokabelnreader.h"
#include "leitnersystem.h"

class KEduVocDocument::Private
{
public:
  Private(KEduVocDocument* qq)
    : q(qq)
  {
    init();
  }

  void init();

  KEduVocDocument* q;

  bool                      m_dirty;
  KUrl                      m_url;
  QList<bool>               m_sortIdentifier;
  bool                      m_sortLesson;
  bool                      m_sortingEnabled;

  // save these to document
  QStringList               m_identifiers;      //0= origin, 1,.. translations
  int                       m_currentLesson;
  QList<int>                m_extraSizeHints;
  QList<int>                m_sizeHints;

  QString                   m_generator;
  QString                   m_queryorg;
  QString                   m_querytrans;
  QList<KEduVocExpression>  m_vocabulary;
  QList<bool>               m_lessonsInQuery;
  QStringList               m_lessonDescriptions;
  QStringList               m_typeDescriptions;
  QStringList               m_tenseDescriptions;
  QStringList               m_usageDescriptions;
  QString                   m_title;
  QString                   m_author;
  QString                   m_license;
  QString                   m_remark;
  QString                   m_version;
  QString                   m_csvDelimiter;

  QList<KEduVocArticle>     m_articles;
  QList<KEduVocConjugation> m_conjugations;

  LeitnerSystem*            m_leitnerSystem;
  bool                      m_activeLeitnerSystem;
};


void KEduVocDocument::Private::init()
{
  m_lessonDescriptions.clear();
  m_articles.clear();
  m_typeDescriptions.clear();
  m_tenseDescriptions.clear();
  m_identifiers.clear();
  m_sortIdentifier.clear();
  m_extraSizeHints.clear();
  m_sizeHints.clear();
  m_vocabulary.clear();
  m_dirty = false;
  m_sortingEnabled = true;
  m_sortLesson = false;
  m_currentLesson = 0;
  m_queryorg = "";
  m_querytrans = "";
  m_url.setFileName(i18n("Untitled"));
  m_title = "";
  m_author = "";
  m_remark = "";
  m_version = "";
  m_generator = "";
  m_csvDelimiter = QString('\t');
  m_activeLeitnerSystem = false;
  m_leitnerSystem = NULL;
}



/**@todo possibly implement
  1. sorting based on lesson name
  2. sorting based on lesson index and original.
*/

class KEduVocDocumentSortHelper
{
public:
  inline KEduVocDocumentSortHelper(int column, Qt::SortOrder order) : sort_column(column), sort_order(order) {}

  inline bool operator()(const KEduVocExpression &e1, const KEduVocExpression &e2) const
    {
    if (sort_order == Qt::AscendingOrder)
      if (sort_column == 0)
        return e1.original().toLower() < e2.original().toLower();
      else
        return e1.translation(sort_column).toLower() < e2.translation(sort_column).toLower();
    else
      if (sort_column == 0)
        return !(e1.original().toLower() < e2.original().toLower());
      else
        return !(e1.translation(sort_column).toLower() < e2.translation(sort_column).toLower());
    }

private:
  int sort_column;
  Qt::SortOrder sort_order;
};


KEduVocDocument::KEduVocDocument(QObject *parent)
  : QObject(parent), d(new Private(this))
{
}


KEduVocDocument::~KEduVocDocument()
{
  delete d;
}


void KEduVocDocument::setModified(bool dirty)
{
  d->m_dirty = dirty;
  emit docModified(d->m_dirty);
}


void KEduVocDocument::appendEntry(KEduVocExpression *expression)
{
  d->m_vocabulary.append(*expression);
  setModified();
}


void KEduVocDocument::insertEntry(KEduVocExpression *expression, int index)
{
  d->m_vocabulary.insert(index, *expression);
  setModified();
}


KEduVocDocument::FileType KEduVocDocument::detectFileType(const QString &fileName)
{
  QIODevice * f = KFilterDev::deviceForFile(fileName);
  if (!f->open(QIODevice::ReadOnly))
    return csv;

  QDataStream is(f);

  qint8 c1, c2, c3, c4, c5;
  is >> c1
    >> c2
    >> c3
    >> c4
    >> c5;  // guess filetype by first x bytes

  QTextStream ts(f);
  QString line;
  QString line2;

  line = ts.readLine();
  if (!ts.atEnd())
    line2 = ts.readLine();
  line.prepend(c5);
  line.prepend(c4);
  line.prepend(c3);
  line.prepend(c2);
  line.prepend(c1);

  if (!is.device()->isOpen())
    return kvd_none;

  f->close();
  if (c1 == '<' && c2 == '?' && c3 == 'x' && c4 == 'm' && c5 == 'l') {
    if (line2.indexOf("pauker", 0) >  0)
      return pauker;
    else
      return kvtml;
  }

  if (line == WQL_IDENT)
    return wql;

  if (c1 == '"' && (line.contains('"') == 1 || line.contains(QRegExp("\",[0-9]"))))
    return vokabeln;

  return csv;
}


bool KEduVocDocument::open(const KUrl& url, bool /*append*/)
{
  d->init();
  if (!url.isEmpty())
    d->m_url = url;

  // TODO EPT  connect( this, SIGNAL(progressChanged(KEduVocDocument*,int)), parent, SLOT(slotProgress(KEduVocDocument*,int)) );

  QString errorMessage = i18n("<qt>Cannot open file<br><b>%1</b></qt>", url.path());
  QString temporaryFile;
  if (KIO::NetAccess::download(url, temporaryFile, 0))
  {
    QIODevice * f = KFilterDev::deviceForFile(temporaryFile);

    if (!f->open(QIODevice::ReadOnly))
    {
      KMessageBox::error(0, errorMessage);
      return false;
    }

    FileType ft = detectFileType(url.path());

    bool read = false;
    while (!read) {
      QApplication::setOverrideCursor(Qt::WaitCursor);
      switch (ft) {
        case kvtml:
        {
          KEduVocKvtmlReader kvtmlReader(f);
          read = kvtmlReader.readDoc(this);
          if (!read)
            errorMessage = kvtmlReader.errorMessage();
        }
        break;

        case wql:
        {
          KEduVocWqlReader wqlReader(f);
          read = wqlReader.readDoc(this);
          if (!read)
            errorMessage = wqlReader.errorMessage();
        }
        break;

        case pauker:
        {
          KEduVocPaukerReader paukerReader(f);
          read = paukerReader.readDoc(this);
          if (!read)
            errorMessage = paukerReader.errorMessage();
        }
        break;

        case vokabeln:
        {
          KEduVocVokabelnReader vokabelnReader(f);
          read = vokabelnReader.readDoc(this);
          if (!read)
            errorMessage = vokabelnReader.errorMessage();
        }
        break;

        case csv:
        {
          KEduVocCsvReader csvReader(f);
          read = csvReader.readDoc(this);
          if (!read)
            errorMessage = csvReader.errorMessage();
        }
        break;

        default:
        {
          KEduVocKvtmlReader kvtmlReader(f);
          read = kvtmlReader.readDoc(this);
          if (!read)
            errorMessage = kvtmlReader.errorMessage();
        }
      }

      QApplication::restoreOverrideCursor();

      if (!read) {
        QString msg = i18n("Could not open \"%1\"\nDo you want to try again?\n(Error reported: %2)", url.path(), errorMessage);
        int result = KMessageBox::warningContinueCancel(0, msg, i18n("Error Opening File"), KGuiItem(i18n("&Retry")));
        if (result == KMessageBox::Cancel) {
          d->init();
          return false;
        } else {
          d->init();
          if (!url.isEmpty())
            d->m_url = url;
        }
      }
    }
    f->close();
    KIO::NetAccess::removeTempFile(temporaryFile);
  }
  return true;
}


bool KEduVocDocument::saveAs(QObject *parent, const KUrl & url, FileType ft, const QString & generator)
{
  connect(this, SIGNAL(progressChanged(KEduVocDocument*, int)), parent, SLOT(slotProgress(KEduVocDocument*, int)));

  KUrl tmp (url);

  if (ft == automatic)
  {
    if (tmp.path().right(strlen("." KVTML_EXT)) == "." KVTML_EXT)
      ft = kvtml;
    else if (tmp.path().right(strlen("." CSV_EXT)) == "." CSV_EXT)
      ft = csv;
    else
    {
      tmp.setFileName(tmp.path() + "." KVTML_EXT);
      ft = kvtml;
    }
  }

  bool saved = false;

  while (!saved)
  {
    QFile f(tmp.path());

    if (!f.open(QIODevice::WriteOnly))
    {
      KMessageBox::error(0, i18n("<qt>Cannot write to file<br><b>%1</b></qt>", tmp.path()));
      return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    switch (ft) {
      case kvtml: {
        KEduVocKvtmlWriter kvtmlWriter(&f);
        saved = kvtmlWriter.writeDoc(this, generator);
      }
      break;

      case csv: {
        KEduVocCsvWriter csvWriter(&f);
        saved = csvWriter.writeDoc(this, generator);
      }
      break;

      default: {
        kError() << "kvcotrainDoc::saveAs(): unknown filetype" << endl;
      }
      break;
    }
    f.close();
    QApplication::restoreOverrideCursor();

    if (!saved) {
      QString msg = i18n("Could not save \"%1\"\nDo you want to try again?", tmp.path());
      int result = KMessageBox::warningContinueCancel(0, msg, i18n("Error Saving File"), KGuiItem(i18n("&Retry")));
      if (result == KMessageBox::Cancel) 
        return false;
    }
  }
  d->m_url = tmp;
  setModified(false);
  return true;
}


KEduVocExpression *KEduVocDocument::entry(int index)
{
  if (index < 0 || index >= d->m_vocabulary.size() )
    return 0;
  else
    return &d->m_vocabulary[index];
}


void KEduVocDocument::removeEntry(int index)
{
  if (index >= 0 && index < d->m_vocabulary.size() )
    d->m_vocabulary.removeAt( index );
}


int KEduVocDocument::findIdentifier(const QString &lang) const
{
  QStringList::const_iterator first = d->m_identifiers.begin();
  int count = 0;
  while (first != d->m_identifiers.end()) {
    if ( *first == lang)
      return count;
    first++;
    count++;
  }
  return -1;
}


QString KEduVocDocument::identifier(int index) const
{
  if (index >= d->m_identifiers.size() || index < 1 )
    return "";
  else
    return d->m_identifiers[index];
}


void KEduVocDocument::setIdentifier(int idx, const QString &id)
{
  if (idx < d->m_identifiers.size() && idx >= 1 ) {
    d->m_identifiers[idx] = id;
  }
}


QString KEduVocDocument::typeName (int index) const
{
  if (index >= d->m_typeDescriptions.count())
    return "";
  else
    return d->m_typeDescriptions[index];
}


void KEduVocDocument::setTypeName(int idx, QString &id)
{
  if (idx >= d->m_typeDescriptions.size())
    for (int i = d->m_typeDescriptions.size(); i <= idx; i++)
      d->m_typeDescriptions.push_back ("");

  d->m_typeDescriptions[idx] = id;
}


QStringList KEduVocDocument::typeDescriptions() const
{
  return d->m_typeDescriptions;
}


void KEduVocDocument::setTypeDescriptions(const QStringList &names)
{
  d->m_typeDescriptions = names;
}


QString KEduVocDocument::tenseName(int index) const
{
  if (index >= d->m_tenseDescriptions.size())
    return "";
  else
    return d->m_tenseDescriptions[index];
}


void KEduVocDocument::setTenseName(int idx, QString &id)
{
  if (idx >= d->m_tenseDescriptions.size())
    for (int i = d->m_tenseDescriptions.size(); i <= idx; i++)
      d->m_tenseDescriptions.push_back ("");

  d->m_tenseDescriptions[idx] = id;
}


QStringList KEduVocDocument::tenseDescriptions() const
{
  return d->m_tenseDescriptions;
}


void KEduVocDocument::setTenseDescriptions(const QStringList &names)
{
  d->m_tenseDescriptions = names;
}


QString KEduVocDocument::usageName(int index) const
{
  if (index >= d->m_usageDescriptions.size())
    return "";
  else
    return d->m_usageDescriptions[index];
}


void KEduVocDocument::setUsageName(int idx, QString &id)
{
  if (idx >= d->m_usageDescriptions.size())
    for (int i = d->m_usageDescriptions.size(); i <= idx; i++)
      d->m_usageDescriptions.push_back ("");

  d->m_usageDescriptions[idx] = id;
}


QStringList KEduVocDocument::usageDescriptions() const
{
  return d->m_usageDescriptions;
}


void KEduVocDocument::setUsageDescriptions(const QStringList &names)
{
  d->m_usageDescriptions = names;
}


void KEduVocDocument::setConjugation(int idx, const KEduVocConjugation &con)
{
  if ( idx < 0) return;

  // extend conjugation with empty elements
  if (d->m_conjugations.size() <= idx )
    for (int i = d->m_conjugations.size(); i < idx+1; i++)
      d->m_conjugations.push_back (KEduVocConjugation());

  d->m_conjugations[idx] = con;
}


int KEduVocDocument::conjugationCount() const
{
  return d->m_conjugations.count();
}


KEduVocConjugation KEduVocDocument::conjugation (int idx) const
{
  if (idx >= d->m_conjugations.size() || idx < 0) {
    return KEduVocConjugation();
  }
  else {
    return d->m_conjugations[idx];
  }
}


void KEduVocDocument::setArticle(int idx, const KEduVocArticle &art)
{
  if ( idx < 0) return;

  // extend conjugation with empty elements
  if (d->m_articles.size() <= idx )
    for (int i = d->m_articles.size(); i < idx+1; i++)
      d->m_articles.push_back (KEduVocArticle());

  d->m_articles[idx] = art;
}


KEduVocArticle KEduVocDocument::article (int idx) const
{
  if (idx >= d->m_articles.size() || idx < 0) {
    return KEduVocArticle();
  }
  else {
    return d->m_articles[idx];
  }
}


int KEduVocDocument::articleCount() const
{
  return d->m_articles.count();
}


int KEduVocDocument::sizeHint (int idx) const
{
  if (idx < 0) {
    idx = -idx;
    if (idx >= d->m_extraSizeHints.size() )
      return 80; // make a good guess about column size
    else {
//      cout << "gsh " << idx << "  " << extraSizehints[idx] << endl;
      return d->m_extraSizeHints[idx];
    }
  }
  else {
    if (idx >= d->m_sizeHints.size() )
      return 150; // make a good guess about column size
    else {
//      cout << "gsh " << idx << "  " << sizehints[idx] << endl;
      return d->m_sizeHints[idx];
    }
  }
}


void KEduVocDocument::setSizeHint (int idx, const int width)
{
//  cout << "ssh " << idx << "  " << width << endl;
  if (idx < 0) {
    idx = -idx;
    if (idx >= d->m_extraSizeHints.size()) {
      for (int i = d->m_extraSizeHints.size(); i <= idx; i++)
        d->m_extraSizeHints.push_back (80);
    }
    d->m_extraSizeHints[idx] = width;

  }
  else {
    if (idx >= d->m_sizeHints.size()) {
      for (int i = d->m_sizeHints.size(); i <= idx; i++)
        d->m_sizeHints.push_back (150);
    }
    d->m_sizeHints[idx] = width;
  }
}


void KEduVocDocument::removeIdentifier(int index)
{
  if (index < d->m_identifiers.size() && index >= 1 )
  {
    d->m_identifiers.removeAt(index);
    for (int i = 0; i < d->m_vocabulary.count(); i++)
      d->m_vocabulary[i].removeTranslation(index);
  }
}


QString KEduVocDocument::originalIdentifier() const
{
  if (d->m_identifiers.size() > 0)
    return d->m_identifiers[0];
  else
    return "";
}


void KEduVocDocument::setOriginalIdentifier(const QString &id)
{
  if (d->m_identifiers.size() > 0) {
    d->m_identifiers[0] = id;
  }
}


bool KEduVocDocument::sort(int index, Qt::SortOrder order)
{
  bool result = false;
  if (d->m_sortingEnabled && index < identifierCount())
  {
    if (d->m_sortIdentifier.count() < d->m_identifiers.count())
      for (int i = d->m_sortIdentifier.count(); i < d->m_identifiers.count(); i++)
          d->m_sortIdentifier.append(false);

    d->m_sortIdentifier[index] = (order == Qt::AscendingOrder);
    result = sort(index);
  }
  return result;
}

bool KEduVocDocument::sort(int index)
{
  bool result = false;
  if (d->m_sortingEnabled && index < identifierCount())
  {
    if (d->m_sortIdentifier.count() < d->m_identifiers.count())
      for (int i = d->m_sortIdentifier.count(); i < d->m_identifiers.count(); i++)
          d->m_sortIdentifier.append(false);

    KEduVocDocumentSortHelper sh(index, d->m_sortIdentifier[index] ? Qt::AscendingOrder : Qt::DescendingOrder);
    qSort(d->m_vocabulary.begin(), d->m_vocabulary.end(), sh);
    d->m_sortIdentifier[index] = !d->m_sortIdentifier[index];
    result = d->m_sortIdentifier[index];
  }
  return result;
}


bool KEduVocDocument::sortByLessonAlpha ()
{
  ///@todo remove?
  return false;
}


bool KEduVocDocument::sortByLessonIndex ()
{
  ///@todo remove?
  return false;
}


void KEduVocDocument::setSortingEnabled(bool enable)
{
  d->m_sortingEnabled = enable;
}


bool KEduVocDocument::isSortingEnabled() const
{
  return d->m_sortingEnabled;
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
	if( yes )
	{
		if (d->m_leitnerSystem == 0)
			createStandardLeitnerSystem(); //if nothing is loaded yet

		d->m_activeLeitnerSystem = true;
	}
	else if( !yes )
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


void KEduVocDocument::resetEntry(int index, int lesson)
{
  for (int i = 0; i < d->m_vocabulary.count(); i++)
    if (/*lesson == 0 ||*/ lesson == d->m_vocabulary[i].lesson())
    {
      d->m_vocabulary[i].setGrade(index, KV_NORM_GRADE, false);
      d->m_vocabulary[i].setGrade(index, KV_NORM_GRADE, true);
      d->m_vocabulary[i].setQueryCount(index, 0, true);
      d->m_vocabulary[i].setQueryCount(index, 0, false);
      d->m_vocabulary[i].setBadCount(index, 0, true);
      d->m_vocabulary[i].setBadCount(index, 0, false);
      QDateTime dt;
      dt.setTime_t(0);
      d->m_vocabulary[i].setQueryDate(index, dt, true);
      d->m_vocabulary[i].setQueryDate(index, dt, false);
    }
}


int KEduVocDocument::identifierCount() const
{
  return d->m_identifiers.count();  // org + translations
}


void KEduVocDocument::appendIdentifier(const QString & id)
{
  d->m_identifiers.append(id);
}


QString KEduVocDocument::lessonDescription(int idx) const
{
  if (idx == 0)
    return i18n("<no lesson>");

  if (idx <= 0 || idx > d->m_lessonDescriptions.size() )
    return "";

  return d->m_lessonDescriptions[idx-1];
}


QList<int> KEduVocDocument::lessonsInQuery() const
{
  QList<int> iqvec;
  for (int i = 0; i < d->m_lessonsInQuery.size(); i++)
    if (d->m_lessonsInQuery[i]) {
      iqvec.push_back(i+1);   // Offset <no lesson>
//      cout << "getliq: " << i+1 << endl;
    }
  return iqvec;
}


void KEduVocDocument::setLessonsInQuery(const QList<int> &lesson_iq)
{
  d->m_lessonsInQuery.clear();
  for (int i = 0; i < d->m_lessonDescriptions.count(); i++)
    d->m_lessonsInQuery.append(false);

  foreach(int i, lesson_iq)
    if (i <= d->m_lessonsInQuery.count())
      d->m_lessonsInQuery[i - 1] = true;
}


KUrl KEduVocDocument::URL() const
{
  return d->m_url;
}


void KEduVocDocument::setURL(const KUrl& url)
{
  d->m_url = url;
}


QString KEduVocDocument::title() const
{
  if (d->m_title.isEmpty())
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


QString KEduVocDocument::docRemark() const
{
  return d->m_remark;
}


void KEduVocDocument::queryIdentifier(QString &org, QString &trans) const
{
  org = d->m_queryorg;
  trans = d->m_querytrans;
}


void KEduVocDocument::setQueryIdentifier(const QString &org, const QString &trans)
{
  d->m_queryorg = org;
  d->m_querytrans = trans;
}


void KEduVocDocument::setTitle(const QString & title)
{
  d->m_title = title.simplified();
}


void KEduVocDocument::setAuthor(const QString & s)
{
  d->m_author = s.simplified();
}


void KEduVocDocument::setLicense(const QString & s)
{
  d->m_license = s.simplified();
}


void KEduVocDocument::setDocRemark(const QString & s)
{
  d->m_remark = s.simplified();
}


void KEduVocDocument::setGenerator(const QString & generator)
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


void KEduVocDocument::setVersion(const QString & vers)
{
  d->m_version = vers;
}


int KEduVocDocument::currentLesson() const
{
  return d->m_currentLesson;
}


void KEduVocDocument::setCurrentLesson(int lesson)
{
  d->m_currentLesson = lesson;
}


QStringList KEduVocDocument::lessonDescriptions() const
{
  return d->m_lessonDescriptions;
}


int KEduVocDocument::lessonCount() const
{
  return d->m_lessonDescriptions.count();
}


void KEduVocDocument::setLessonDescriptions(const QStringList &names)
{
  d->m_lessonDescriptions = names;
}


int KEduVocDocument::search(const QString &substr, int id, int first, int last, bool word_start)
{
  if (last >= entryCount() || last < 0)
    last = entryCount();

  if (first < 0)
    first = 0;

  if (id >= identifierCount() || last < first)
    return -1;

  if (id == 0) {
    for (int i = first; i < last; i++) {
      if (word_start) {
        if (entry(i)->original().indexOf(substr, 0, Qt::CaseInsensitive) == 0)  // case insensitive
          return i;
      }
      else {
        if (entry(i)->original().indexOf(substr, 0, Qt::CaseInsensitive) > -1)  // case insensitive
          return i;
      }
    }
  }
  else {
    for (int i = first; i < last; i++) {
      if (word_start) {
        if (entry(i)->translation(id).indexOf(substr, 0, Qt::CaseInsensitive) == 0) // case insensitive
          return i;
      }
      else {
        if (entry(i)->translation(id).indexOf(substr, 0, Qt::CaseInsensitive) > -1) // case insensitive
          return i;
      }
    }
  }
  return -1;
}


QString KEduVocDocument::csvDelimiter() const
{
  return d->m_csvDelimiter;
}


void KEduVocDocument::setCsvDelimiter(const QString &delimiter)
{
  d->m_csvDelimiter = delimiter;
}


class ExpRef {

public:
  ExpRef() {}
  ExpRef (KEduVocExpression *_exp, int _idx)
  {
    idx    = _idx;
    exp    = _exp;
  }

  bool operator< (const ExpRef& y) const
  {
    QString s1 = exp->original();
    QString s2 = y.exp->original();
    int cmp = QString::compare(s1.toUpper(), s2.toUpper());
    if (cmp != 0)
      return cmp < 0;

    for (int i = 1; i < exp->translationCount(); i++) {

      s1 = exp->translation(i);
      s2 = y.exp->translation(i);
      cmp = QString::compare(s1.toUpper(), s2.toUpper() );
      if (cmp != 0)
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

  for (int i = 0; i < d->m_vocabulary.count(); i++)
    shadow.append(ExpRef (entry(i), i));
  qStableSort(shadow.begin(), shadow.end());

  int ent_no = 0;
  int ent_percent = d->m_vocabulary.size () / 100;
  float f_ent_percent = d->m_vocabulary.size () / 100.0;
  emit progressChanged(this, 0);

  for (int i = shadow.size() - 1; i > 0; i--) {
    kve1 = shadow[i].exp;
    kve2 = shadow[i - 1].exp;

    ent_no++;
    if (ent_percent != 0 && (ent_no % ent_percent) == 0 )
      emit progressChanged(this, (int)((ent_no / f_ent_percent) / 2.0));

    bool equal = true;
    if (kve1->original() == kve2->original() ) {
      for (int l = 1; equal && l < identifierCount(); l++ )
        if (kve1->translation(l) != kve2->translation(l))
          equal = false;

      if (equal) {
        to_delete.append(shadow[i - 1].idx);
        count++;
      }
    }
  }

  // removing might take very long
  ent_no = 0;
  ent_percent = to_delete.size () / 100;
  f_ent_percent = to_delete.size () / 100.0;
  emit progressChanged(this, 0);

  qStableSort(to_delete.begin(), to_delete.end());

  for (int i = (int) to_delete.count() - 1; i >= 0; i--) {
    ent_no++;
    if (ent_percent != 0 && (ent_no % ent_percent) == 0 )
      emit progressChanged(this, (int)(50 + ent_no / f_ent_percent / 2.0));
    removeEntry(to_delete[i]);
    setModified();
  }

  return count;
}

void KEduVocDocument::shuffle()
{
  KRandomSequence rs;
  rs.randomize(d->m_vocabulary);
  setModified();
}

#include "keduvocdocument.moc"
