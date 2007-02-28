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
#include <QList>
#include <QTextStream>
#include <QtAlgorithms>
#include <QIODevice>

#include <klocale.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kio/netaccess.h>
#include <krandomsequence.h>
#include <kfilterdev.h>

#include "keduvockvtmlwriter.h"
#include "keduvoccsvreader.h"
#include "keduvockvtmlreader.h"
#include "keduvocwqlreader.h"
#include "keduvocpaukerreader.h"
#include "keduvocvokabelnreader.h"
#include "leitnersystem.h"

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
{
  Q_UNUSED(parent);
  Init();
}


KEduVocDocument::~KEduVocDocument()
{
}


void KEduVocDocument::setVersion (const QString & vers)
{
  m_version = vers;
}


void KEduVocDocument::Init ()
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
  setCurrentLesson (0);
  m_queryorg = "";
  m_querytrans = "";
  m_url.setFileName(i18n("Untitled"));
  m_title = "";
  m_author = "";
  m_remark = "";
  m_version = "";
  m_generator = "";
//  m_font = NULL;

  m_activeLeitnerSystem = false;
  m_leitnerSystem = NULL;
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
  Init();
  if (!url.isEmpty())
    m_url = url;

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
          Init();
          return false;
        } else {
          Init();
          if (!url.isEmpty())
            m_url = url;
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
  connect( this, SIGNAL(progressChanged(KEduVocDocument*,int)), parent, SLOT(slotProgress(KEduVocDocument*,int)) );

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
        QTextStream os( &f );                       // serialize using f
        //TODO saved = saveToCsv(os, title);
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
      // TODO new writers provide an explicite error message
      // the two messages should be merged
      QString msg = i18n("Could not save \"%1\"\nDo you want to try again?", tmp.path());
      int result = KMessageBox::warningContinueCancel(0, msg, i18n("Error Saving File"), KGuiItem(i18n("&Retry")));
      if (result == KMessageBox::Cancel) 
        return false;
    }
  }
  m_url = tmp;
  setModified(false);
  return true;
}


KEduVocExpression *KEduVocDocument::entry(int index)
{
  if (index < 0 || index >= (int)m_vocabulary.size() )
    return 0;
  else
    return &m_vocabulary[index];
}


void KEduVocDocument::removeEntry(int index)
{
  if (index >= 0 && index < (int)m_vocabulary.size() )
    m_vocabulary.removeAt( index );
}


int KEduVocDocument::findIdentifier(const QString &lang) const
{
  QStringList::const_iterator first = m_identifiers.begin();
  int count = 0;
  while (first != m_identifiers.end()) {
    if ( *first == lang)
      return count;
    first++;
    count++;
  }
  return -1;
}


QString KEduVocDocument::identifier(int index) const
{
  if (index >= (int)m_identifiers.size() || index < 1 )
    return "";
  else
    return m_identifiers[index];
}


void KEduVocDocument::setIdentifier(int idx, const QString &id)
{
  if (idx < (int)m_identifiers.size() && idx >= 1 ) {
    m_identifiers[idx] = id;
  }
}


QString KEduVocDocument::typeName (int index) const
{
  if (index >= (int)m_typeDescriptions.count())
    return "";
  else
    return m_typeDescriptions[index];
}


void KEduVocDocument::setTypeName(int idx, QString &id)
{
  if (idx >= (int)m_typeDescriptions.size())
    for (int i = (int)m_typeDescriptions.size(); i <= idx; i++)
      m_typeDescriptions.push_back ("");

  m_typeDescriptions[idx] = id;
}


QString KEduVocDocument::tenseName(int index) const
{
  if (index >= (int)m_tenseDescriptions.size())
    return "";
  else
    return m_tenseDescriptions[index];
}


void KEduVocDocument::setTenseName(int idx, QString &id)
{
  if (idx >= (int)m_tenseDescriptions.size())
    for (int i = (int)m_tenseDescriptions.size(); i <= idx; i++)
      m_tenseDescriptions.push_back ("");

  m_tenseDescriptions[idx] = id;
}


QString KEduVocDocument::usageName(int index) const
{
  if (index >= (int)m_usageDescriptions.size())
    return "";
  else
    return m_usageDescriptions[index];
}


void KEduVocDocument::setUsageName(int idx, QString &id)
{
  if (idx >= (int)m_usageDescriptions.size())
    for (int i = (int)m_usageDescriptions.size(); i <= idx; i++)
      m_usageDescriptions.push_back ("");

  m_usageDescriptions[idx] = id;
}


void KEduVocDocument::setConjugation(int idx, const KEduVocConjugation &con)
{
  if ( idx < 0) return;

  // extend conjugation with empty elements
  if ((int)m_conjugations.size() <= idx )
    for (int i = m_conjugations.size(); i < idx+1; i++)
      m_conjugations.push_back (KEduVocConjugation());

  m_conjugations[idx] = con;
}


KEduVocConjugation KEduVocDocument::conjugation (int idx) const
{
  if (idx >= (int)m_conjugations.size() || idx < 0) {
    return KEduVocConjugation();
  }
  else {
    return m_conjugations[idx];
  }
}


void KEduVocDocument::setArticle(int idx, const KEduVocArticle &art)
{
  if ( idx < 0) return;

  // extend conjugation with empty elements
  if ((int)m_articles.size() <= idx )
    for (int i = m_articles.size(); i < idx+1; i++)
      m_articles.push_back (KEduVocArticle());

  m_articles[idx] = art;
}


KEduVocArticle KEduVocDocument::article (int idx) const
{
  if (idx >= (int)m_articles.size() || idx < 0) {
    return KEduVocArticle();
  }
  else {
    return m_articles[idx];
  }
}


int KEduVocDocument::sizeHint (int idx) const
{
  if (idx < 0) {
    idx = -idx;
    if (idx >= (int)m_extraSizeHints.size() )
      return 80; // make a good guess about column size
    else {
//      cout << "gsh " << idx << "  " << extraSizehints[idx] << endl;
      return m_extraSizeHints[idx];
    }
  }
  else {
    if (idx >= (int)m_sizeHints.size() )
      return 150; // make a good guess about column size
    else {
//      cout << "gsh " << idx << "  " << sizehints[idx] << endl;
      return m_sizeHints[idx];
    }
  }
}


void KEduVocDocument::setSizeHint (int idx, const int width)
{
//  cout << "ssh " << idx << "  " << width << endl;
  if (idx < 0) {
    idx = -idx;
    if (idx >= (int)m_extraSizeHints.size()) {
      for (int i = (int)m_extraSizeHints.size(); i <= idx; i++)
        m_extraSizeHints.push_back (80);
    }
    m_extraSizeHints[idx] = width;

  }
  else {
    if (idx >= (int)m_sizeHints.size()) {
      for (int i = (int)m_sizeHints.size(); i <= idx; i++)
        m_sizeHints.push_back (150);
    }
    m_sizeHints[idx] = width;
  }
}


void KEduVocDocument::removeIdentifier(int index)
{
  if (index < (int)m_identifiers.size() && index >= 1 )
  {
    m_identifiers.removeAt(index);
    for (int i = 0; i < m_vocabulary.count(); i++)
      m_vocabulary[i].removeTranslation(index);
  }
}


QString KEduVocDocument::originalIdentifier() const
{
  if (m_identifiers.size() > 0)
    return m_identifiers[0];
  else
    return "";
}


void KEduVocDocument::setOriginalIdentifier(const QString &id)
{
  if (m_identifiers.size() > 0) {
    m_identifiers[0] = id;
  }
}


bool KEduVocDocument::sort(int index, Qt::SortOrder order)
{
  bool result = false;
  if (m_sortingEnabled && index < identifierCount())
  {
    if (m_sortIdentifier.count() < m_identifiers.count())
      for (int i = m_sortIdentifier.count(); i < (int) m_identifiers.count(); i++)
          m_sortIdentifier.append(false);

    m_sortIdentifier[index] = (order == Qt::AscendingOrder);
    result = sort(index);
  }
  return result;
}

bool KEduVocDocument::sort(int index)
{
  bool result = false;
  if (m_sortingEnabled && index < identifierCount())
  {
    if (m_sortIdentifier.count() < m_identifiers.count())
      for (int i = m_sortIdentifier.count(); i < (int) m_identifiers.count(); i++)
          m_sortIdentifier.append(false);

    KEduVocDocumentSortHelper sh(index, m_sortIdentifier[index] ? Qt::AscendingOrder : Qt::DescendingOrder);
    qSort(m_vocabulary.begin(), m_vocabulary.end(), sh);
    m_sortIdentifier[index] = !m_sortIdentifier[index];
    result = m_sortIdentifier[index];
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

bool KEduVocDocument::leitnerSystemActive()
{
	return m_activeLeitnerSystem;
}

void KEduVocDocument::setLeitnerSystemActive( bool yes )
{
	if( yes )
	{
		if (m_leitnerSystem == 0)
			createStandardLeitnerSystem(); //if nothing is loaded yet

		m_activeLeitnerSystem = true;
	}
	else if( !yes )
		m_activeLeitnerSystem = false;
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

	m_leitnerSystem = tmpSystem;
}

void KEduVocDocument::setLeitnerSystem( LeitnerSystem* system )
{
	m_leitnerSystem = system;

	/*KWordQuizApp* app = (KWordQuizApp*) parent();
	app->slotLeitnerSystem();*/
}

LeitnerSystem* KEduVocDocument::leitnerSystem()
{
  return m_leitnerSystem;
}


void KEduVocDocument::resetEntry(int index, int lesson)
{
  for (int i = 0; i < m_vocabulary.count(); i++)
    if (/*lesson == 0 ||*/ lesson == m_vocabulary[i].lesson())
    {
      m_vocabulary[i].setGrade(index, KV_NORM_GRADE, false);
      m_vocabulary[i].setGrade(index, KV_NORM_GRADE, true);
      m_vocabulary[i].setQueryCount(index, 0, true);
      m_vocabulary[i].setQueryCount(index, 0, false);
      m_vocabulary[i].setBadCount(index, 0, true);
      m_vocabulary[i].setBadCount(index, 0, false);
      QDateTime dt;
      dt.setTime_t(0);
      m_vocabulary[i].setQueryDate(index, dt, true);
      m_vocabulary[i].setQueryDate(index, dt, false);
    }
}


QString KEduVocDocument::lessonDescription(int idx) const
{
  if (idx == 0)
    return i18n("<no lesson>");

  if (idx <= 0 || idx > (int) m_lessonDescriptions.size() )
    return "";

  return m_lessonDescriptions[idx-1];
}


QList<int> KEduVocDocument::lessonsInQuery() const
{
  QList<int> iqvec;
  for (int i = 0; i < m_lessonsInQuery.size(); i++)
    if (m_lessonsInQuery[i]) {
      iqvec.push_back(i+1);   // Offset <no lesson>
//      cout << "getliq: " << i+1 << endl;
    }
  return iqvec;
}


void KEduVocDocument::setLessonsInQuery(QList<int> lesson_iq)
{
  m_lessonsInQuery.clear();
  for (int i = 0; i < m_lessonDescriptions.count(); i++)
    m_lessonsInQuery.append(false);

  foreach(int i, lesson_iq)
    if (i <= m_lessonsInQuery.count())
      m_lessonsInQuery[i - 1] = true;
}


QString KEduVocDocument::title() const
{
  if (m_title.isEmpty())
    return m_url.fileName();
  else
    return m_title;
}


QString KEduVocDocument::author() const
{
  return m_author;
}


QString KEduVocDocument::license() const
{
  return m_license;
}


QString KEduVocDocument::docRemark() const
{
  return m_remark;
}


void KEduVocDocument::setTitle(const QString & title)
{
  m_title = title.simplified();
}


void KEduVocDocument::setAuthor(const QString & s)
{
  m_author = s.simplified();
}


void KEduVocDocument::setLicense(const QString & s)
{
  m_license = s.simplified();
}


void KEduVocDocument::setDocRemark(const QString & s)
{
  m_remark = s.simplified();
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

  for (int i = 0; i < (int) m_vocabulary.count(); i++)
    shadow.append(ExpRef (entry(i), i));
  qStableSort(shadow.begin(), shadow.end());

  int ent_no = 0;
  int ent_percent = m_vocabulary.size () / 100;
  float f_ent_percent = m_vocabulary.size () / 100.0;
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
  rs.randomize(m_vocabulary);
  setModified();
}

#include "keduvocdocument.moc"
