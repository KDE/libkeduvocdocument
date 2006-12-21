/***************************************************************************
                        Vocabulary Document for KDE Edu
    -----------------------------------------------------------------------
    copyright           : (C) 1999-2001 Ewald Arnold
                          (C) 2001 The KDE-EDU team
                          (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCDOCUMENT_H
#define KEDUVOCDOCUMENT_H

#include <libkdeedu_core_export.h>

#include <QString>

#define KVD_ZERO_TIME  934329599   // 1999-08-10 23:59:59, never change
#define KVD_VERS_PREFIX " v"       // kvoctrain v0.1.0

/** XML tags and attribute names */


static const QLatin1String KV_DOCTYPE("kvtml"); // doctype
#define KV_TITLE       "title"     // doc title
#define KV_AUTHOR      "author"    // doc author
#define KV_LICENSE     "license"   // doc license
#define KV_DOC_REM     "remark"    // doc remark
#define KV_LINES       "lines"     // entries
#define KV_GENERATOR   "generator" // who generated the doc
#define KV_COLS        "cols"      // columns
#define KV_ENCODING    "encoding"  // document encoding  (obsolete!)

#define KV_EXPR        "e"         // entry for one expression
static const QLatin1String KV_ORG("o");         // original expression in specified language
static const QLatin1String KV_TRANS("t");       // translated expression in specified language
#define KV_LANG        "l"         // language: en, de, it, fr ...
#define KV_QUERY       "q"         // query: org or translation
#define KV_O           "o"         // org
#define KV_T           "t"         // translation
#define KV_GRADE       "g"         // grade of knowledge: 0=well known, x=not known for x times
#define KV_LESS_MEMBER "m"         // member of lesson 1 .. x
#define KV_COUNT       "c"         // number of times queried
#define KV_SIZEHINT    "width"     // recommended column width
#define KV_CHARSET     "charset"   // recommended charset (obsolete!)
#define KV_BAD         "b"         // number of times failed
#define KV_DATE        "d"         // last query date
#define KV_DATE2       "w"         // last query date, compressed format, deprecated, currently ignored
#define KV_REMARK      "r"         // remark for this entry
#define KV_FAUX_AMI_F  "ff"        // false friend of this entry from org
#define KV_FAUX_AMI_T  "tf"        // false friend of this entry to org
#define KV_SYNONYM     "y"         // synonym (same meaning) of expr
#define KV_ANTONYM     "a"         // antonym (oppositite) of expr
#define KV_PRONUNCE    "p"         // how to pronounce this expression
#define KV_SELECTED    "s"         // entry selected for queries
#define KV_INACTIVE    "i"         // entry inactive (for queries)
#define KV_EXPRTYPE    "t"         // type of expression
#define KV_EXAMPLE     "x"         // example string with word
#define KV_USAGE       "u"         // usage label
#define KV_PARAPHRASE  "h"         // paraphrase for expression

/*
 <type>
  <desc no="1">My type 1</desc>
  <desc no="2">My type 2</desc>
 </type>
*/

#define KV_TYPE_GRP    "type"      // type descriptor group
#define KV_TYPE_DESC   "desc"      // type descriptor
#define KV_TYPE_NO     "no"        // type descriptor number

/*
 <usage>
  <desc no="1">My usage 1</desc>
  <desc no="2">My usage 2</desc>
 </type>
*/

#define KV_USAGE_GRP    "usage"     // usage descriptor group
#define KV_USAGE_DESC   "desc"      // usage descriptor
#define KV_USAGE_NO     "no"        // usage descriptor number

/*
 <lesson width="138">
  <desc no="1">Lesson #1</desc>
  <desc no="2" query="1">Lesson #2</desc>
 </lesson>
*/

#define KV_LESS_GRP    "lesson"    // lesson descriptor group
#define KV_LESS_CURR   "current"   // is current lesson
#define KV_LESS_DESC   "desc"      // lesson descriptor
#define KV_LESS_QUERY  "query"     // lesson contained in query
#define KV_LESS_NO     "no"        // lesson descriptor number

/*
 <tense>
  <desc no="1">user tense #1</desc>
  <desc no="2">user tense #2</desc>
 </tense>
*/

#define KV_TENSE_GRP    "tense"     // tense descriptor group
#define KV_TENSE_DESC   "desc"      // tense descriptor
#define KV_TENSE_NO     "no"        // tense descriptor number

/*
 <options>
  <sort on="1"/>
 </options>
*/

#define KV_OPTION_GRP  "options"   // internal options group
#define KV_OPT_SORT    "sort"      // allow sorting
#define KV_BOOL_FLAG   "on"        // general boolean flag

/*
 <article>
  <e l="de">        lang determines also lang order in entries !!
   <fi>eine</fi>    which must NOT differ
   <fd>die</fd>
   <mi>ein</mi>
   <md>der</md>
   <ni>ein</ni>
   <nd>das</nd>
  </e>
 </article>
*/

#define KV_ARTICLE_GRP "article"   // article descriptor group
static const QLatin1String KV_ART_ENTRY("e");  // article entry
#define KV_ART_FD      "fd"        // female definite
#define KV_ART_MD      "md"        // male definite
#define KV_ART_ND      "nd"        // natural definite
#define KV_ART_FI      "fi"        // female indefinite
#define KV_ART_MI      "mi"        // male indefinite
#define KV_ART_NI      "ni"        // natural indefinite

/*
 <comparison>
   <l1>good</l1>
   <l2>better</l2>
   <l3>best</l3>
 </comparison>
*/

#define KV_COMPARISON_GRP "comparison"   // comparison descriptor group
#define KV_COMP_L1        "l1"           // base form
#define KV_COMP_L2        "l2"           // next form
#define KV_COMP_L3        "l3"           // last form

/*
 <multiplechoice>
   <mc1>good</mc1>
   <mc2>better</mc2>
   <mc3>best</mc3>
   <mc4>best 2</mc4>
   <mc5>best 3</mc5>
 </multiplechoice>
*/

#define KV_MULTIPLECHOICE_GRP "multiplechoice"   // multiple choice descriptor group
#define KV_MC_1        "mc1"           // choice 1
#define KV_MC_2        "mc2"           // choice 2
#define KV_MC_3        "mc3"           // choice 3
#define KV_MC_4        "mc4"           // choice 4
#define KV_MC_5        "mc5"           // choice 5

/*
 <conjugation>        used in header for definiton of "prefix"
  <e l="de">          lang determines also lang order in entries !!
   <s1>I</s1>         which must NOT differ in subsequent <e>-tags
   <s2>you<2>
   <s3f>he</s3f>
   <s3m>she</s3m>
   <s3n>it</s3n>
   <p1>we</p1>
   <p2>you</p2>
   <p3f>they</p3f>
   <p3m>they</p3m>
   <p3n>they</p3n>
  </e>
 </conjugation>

 <conjugation>        and in entry for definition of tenses of (irreg.) verbs
  <t n="sipa">
   <s1>go</s1>
   <s2>go</s2>
   <s3f>goes</s3f>
   <s3m>goes</s3m>
   <s3n>goes</s3n>
   <p1>go</p1>
   <p2>go</p2>
   <p3f>go</p3f>
   <p3m>go</p3m>
   <p3n>go</p3n>
  </t>
 </conjugation>
*/

#define KV_CONJUG_GRP  "conjugation" // conjugation descriptor group
#define KV_CON_ENTRY   "e"           // conjugation entry (header)
#define KV_CON_TYPE    "t"           // conjugation type  (voc entries)
#define KV_CON_NAME    "n"           // conjugation type name (voc entries)
#define KV_CON_P1S     "s1"          // 1. person singular
#define KV_CON_P2S     "s2"          // 2. person singular
#define KV_CON_P3SF    "s3f"         // 3. person singular female
#define KV_CON_P3SM    "s3m"         // 3. person singular male
#define KV_CON_P3SN    "s3n"         // 3. person singular natural
#define KV_CON_P1P     "p1"          // 1. person plural
#define KV_CON_P2P     "p2"          // 2. person plural
#define KV_CON_P3PF    "p3f"         // 3. person plural female
#define KV_CON_P3PM    "p3m"         // 3. person plural male
#define KV_CON_P3PN    "p3n"         // 3. person plural natural
#define KV_CONJ_COMMON "common"      // female contains common for all three

#define LEX_IDENT_50   "Vocabulary Trainer V5.0"
#define WQL_IDENT      "WordQuiz"

#define KVTML_EXT        "kvtml"
#define VT5_LEX_EXT      "lex"
#define QVOCAB_EXT       "qvo"
#define VCB_EXT          "vocab"
#define KVL_EXT          "vl"
#define CSV_EXT          "csv"
#define TXT_EXT          "txt"
#define WQL_EXT          "wql"

#define VCB_SEPARATOR    "__"

#include <QObject>
#include <QFont>
#include <QList>
#include <QTextStream>
#include <kurl.h>

#include "keduvocexpression.h"

class QStringList;
class KEduVocMultipleChoice;
class LeitnerSystem;

/**
 * This class contains the expressions of your vocabulary
 * as well as other information about the vocabulary
 */
class KDEEDUCORE_EXPORT KEduVocDocument : public QObject
{
  Q_OBJECT
  friend class KEduVocKvtmlWriter;
  friend class KEduVocKvtmlReader;
  friend class KEduVocWqlReader;

 public:

  enum FileType { kvd_none, automatic,
                  kvtml,
                  wql,
                  kvtbin,
                  vt_lex, vt_vcb, csv /*, kvoclearn, qvocab*/ };

  /**
   * Constructor for a KdeEdu vocabulary document
   *
   * @param parent calling object
   */
  KEduVocDocument(QObject* parent);

  /**
   * Destructor for the document class
   */
  ~KEduVocDocument();

  /**
   * Indicates if the document is modified
   *
   * @param dirty   new state
   */
  inline void setModified (bool dirty = true) { emit docModified(m_dirty = dirty); }

  /**
   * Sppends a new expression to the end of the vocabulary
   *
   * @param expression      expression to append
   */
  inline void appendEntry (KEduVocExpression *expression)
    { m_vocabulary.append(*expression); setModified(); }

  /**
   * Inserts a new expression
   *
   * @param expression      expression to insert
   * @param index           index of entry
   */
  inline void insertEntry(KEduVocExpression *expression, int index)
    { m_vocabulary.insert(index, *expression); setModified(); }

  /**
   * Removes an expression from the document
   *
   * @param index           index of expression
   */
  void removeEntry(int index);

  /**
   * Removes all expressions from the document (clears vocabulary)
   */
  inline void removeAllEntries()
    { m_vocabulary.clear(); setModified(); }

  /**
   * Sorts vocabulary alphabetically
   *
   * @param index             index identifier (0 = original, 1..n = translations
   * @returns                 the direction of sorting: true = ascending
   */
  bool sort(int index);

  /**
   * Sorts vocabulary alphabetically
   *
   * @param index             index identifier (0 = original, 1..n = translations
   * @param order             the order used when sorting
   * @returns                 the direction of sorting: true = ascending
   */
  bool sort(int index, Qt::SortOrder order);

  /**
   * Shuffles vocabulary in a random order
   */
  void shuffle();

  /**
   * Removes equal entries (original plus all translations)
   *
   * @returns                 number of removed entries
   */
  int cleanUp();

  /**
   * Sorts vocabulary by lesson indices
   * @returns direction of sorting: true = ascending
   */
  bool sortByLessonIndex();

  /**
   * Sorts vocabulary by lesson name
   * @returns direction of sorting: true = ascending
   */
  bool sortByLessonAlpha();

  /**
   * Enables sorting
   */
  inline void enableSorting(bool enable) { m_enableSorting = enable; }

  /**
   * @returns whether sorting is enabled
   */
  inline bool isSortingEnabled() { return m_enableSorting; }

  /**
   * @returns the modification state of the doc
   */
  inline bool isModified() const { return m_dirty; }

  /**
   * @returns the original identifier
   */
  QString originalIdentifier() const;

  /**
   * Sets the original identifier
   *
   * @param id              the new identifier
   */
  void setOriginalIdentifier(const QString &id);

  /**
   * Returns the identifier of translation @p index
   *
   * @param index            number of translation 1..x
   * @returns                the language identifier: en=english, de=german, ...
   */
  QString identifier(int index) const;

  /**
   * Sets the identifier of translation
   *
   * @param index            number of translation 1..x
   * @param lang             thr language identifier: en=english, de=german, ...
   */
  void setIdentifier(int index, const QString &lang);

  /**
   * Removes identifier an the according translation in all entries
   *
   * @param index            number of translation 1..x
   */
  void removeIdentifier(int index);

  /**
   * Determines if given translation is available and where
   *
   * @param lang             identifier of language
   * @returns                index of translation, 0=original, -1=none
   */
  int findIdentifier(const QString &lang) const;

  /**
   * Returns the attribute string
   *
   * @param index            number of attribute
   * @returns                string
   */
  QString typeName(int index) const;

  /**
   * Sets attribute string
   *
   * @param index            number of attribute
   * @param str              name of attribute
   */
  void setTypeName(int index, QString &str);

  /**
   * Gets the descriptions of the types
   */
  inline QStringList typeDescriptions() const { return m_typeDescriptions; }

  /**
   * Sets the descriptions of the types
   */
  inline void setTypeDescriptions(QStringList names) { m_typeDescriptions = names; }

  /**
   * Returns the tense string
   *
   * @param index            number of tense
   * @returns                string
   */
  QString tenseName(int index) const;

  /**
   * Sets the tense string
   *
   * @param index            number of tense
   * @param str              name of tense
   */
  void setTenseName(int index, QString &str);

  /**
   * Gets the descriptions of the tenses
   */
  inline QStringList tenseDescriptions() const { return m_tenseDescriptions; }

  /**
   * Sets the description of the tenses
   */
  inline void setTenseDescriptions(QStringList names) { m_tenseDescriptions = names; }

  /**
   * Returns usage string
   *
   * @param index            number of usage
   * @returns                string
   */
  QString usageName(int index) const;

  /**
   * Sets usage string
   *
   * @param index            number of usage
   * @param str              name of usage
   */
  void setUsageName(int index, QString &str);

  /**
   * Gets the descriptions of the usages
   */
  inline QStringList usageDescriptions() const { return m_usageDescriptions; }

  /**
   * Sets the descriptions of the usages
   */
  inline void setUsageDescriptions(QStringList names) { m_usageDescriptions = names; }

  /**
   * Open a document file
   *
   * @param url      url to file to open
   * @param append   if @c true, the words from @p url are appended to the
   *                 existing collection; if @c false, the collection is
   *                 emptied first so that only the words from @p url
   *                 are in the result.
   * @returns        true if successful
   */
  bool open(const KUrl& url, bool append);

  /**
   * Saves the data under the given name
   *
   * @param parent     the object that owns the document
   * @param url        if url is empty (or NULL) actual name is preserved
   * @param ft         the filetype to be used when saving the document
   * @param generator  the name of the application saving the document
   * @returns          true if successful
   */
  bool saveAs(QObject *parent, const KUrl & url, FileType ft, const QString & generator);

  /**
   * @returns the number of entries
   */
  inline int numEntries() const { return m_vocabulary.count(); }

  /**
   * Sets grades to KV_NORM_GRADE, counts to 0 ...
   *
   * @param index    index of language 0..x, -1 = all
   * @param lesson   lesson id, if this is 0 all lesson are affected,
   *                 otherwise only matching numbers
   */
  void resetEntry(int index = -1, int lesson = 0);

  /**
   * @returns the number of different identifiers (usually languages)
   */
  inline int numIdentifiers() const { return m_identifiers.count(); } // org + translations

  /**
   * Appends a new identifier (usually a language)
   *
   * @param id         the identifier to append
   */
  inline void appendIdentifier(const QString & id) { m_identifiers.append(id); }

  /**
   * Returns pointer to expression object @p index
   *
   * @param index     index of desired entry
   * @returns         pointer to object or NULL if index out of range
   */
  KEduVocExpression *entry(int index);

  /**
   * Search substring in vocabulary (case insensitive always)
   *
   * @param substr     partial string to search
   * @param id         which language to search: 0=org, 1..x=translation
   * @param first      index from where to start
   * @param last       index of last entry, -1 goes till end
   * @param word_start false: search partial string,
   *                   true:always from beginning of word
   * @returns          the index of found entry, -1 if none
   */
  int search(const QString &substr, int id, int first=0, int last=-1, bool word_start = false);

  /**
   * @returns the URL of the XML file
   */
  inline KUrl URL() const {return m_url; }

  /**
   * Sets the URL of the XML file
   */
  inline void setURL(const KUrl& url) {m_url = url;}

  /**
   * @returns the title of the XML file
   */
  QString title() const;

  /**
   * @returns the author of the file
   */
  QString author() const;

  /**
   * @returns the license of the file
   */
  QString license() const;

  /**
   * @return the remark of the file
   */
  QString docRemark() const;

  /**
   * @returns the font
   */
  QFont* font() const;

  /**
   * Retrieves the identifiers for the current query
   *
   * @param org        identifier for original
   * @param trans      identifier for translation
   */
  inline void queryIdentifier(QString &org, QString &trans) const
    { org = m_queryorg; trans = m_querytrans; }

  /**
   * Sets the identifiers for the current query
   *
   * @param org        identifier for original
   * @param trans      identifier for translation
   */
  inline void setQueryIdentifier(const QString &org, const QString &trans)
    { m_queryorg = org; m_querytrans = trans; }

  /**
   * Sets the title of the XML file
   */
  void setTitle(const QString & title);

  /**
   * Sets the author of the file
   */
  void setAuthor(const QString & author);

  /**
   * Sets the license of the file
   */
  void setLicense(const QString & license);

  /**
   * Sets the remark of the file
   */
  void setDocRemark(const QString & rem);

  /**
   * Sets the font
   */
  void setFont(QFont *font);

  /**
   * Gets the version of the loaded file
   */
  void version(int &major, int &minor, int &patch);

  /**
   * @returns the current lesson index
   */
  inline int currentLesson() const { return m_currentLesson; }

  /**
   * Sets current lesson index
   * @param lesson    index of lesson
   */
  inline void setCurrentLesson(int lesson) { m_currentLesson = lesson; }

  /**
   * @returns the description of the lesson
   */
  QString lessonDescription(int index) const;

  /**
   * @returns a list with the lessons in the current query
   */
  QList<int> lessonsInQuery() const;

  /**
   * Sets the lessons in the current query
   */
  void setLessonsInQuery(QList<int>);

  inline QStringList lessonDescriptions() const { return m_lessonDescriptions; }

  inline int numLessons () const {return (int) m_lessonDescriptions.count(); }

  /**
   * Sets the description of the lesson
   */
  inline void setLessonDescriptions(QStringList names) { m_lessonDescriptions = names; }

  /**
   * @param index            index of translation
   * @returns                a pointer to conjugations if available
   */
  KEduVocConjugation conjugation(int index) const;

  /**
   * Sets conjugations
   *
   * @param index            index of translation
   * @param con              conjugation block
   */
  void setConjugation(int index, const KEduVocConjugation &con);

  /**
   * @param index            index of translation
   * @returns                a pointer to articles if available
   */
  KEduVocArticle article(int index) const;

  /**
   * Sets the articles
   *
   * @param index            index of translation
   * @param art              article block
   */
  void setArticle(int index, const KEduVocArticle &art);

  /**
   * Returns the recommended size
   *
   * @param index            number of expr, -1 = lesson
   * @returns                width of column
   */
  int sizeHint(int index) const;

  /**
   * Sets the recommended size
   *
   * @param index            number of expr, -1 = lesson
   * @param width            width of column
   */
  void setSizeHint (int index, const int width);

  bool leitnerSystemActive();
  void setLeitnerSystemActive(bool yes);
  void createStandardLeitnerSystem();
  void setLeitnerSystem( LeitnerSystem* system );
  LeitnerSystem* leitnerSystem();

  bool unknownAttribute(int line, const QString &name, const QString &attr);
  void unknownElement(int line, const QString &elem );
  void errorKvtMl(int line, const QString &text );
  void warningKvtMl(int line, const QString &text );

  void errorLex(int line, const QString &text );

  void errorCsv(int line, const QString &text );

  FileType detectFileType(const QString &filename);

signals:
  void progressChanged (KEduVocDocument *, int curr_percent);
  void docModified (bool mod);

protected:

  void Init();

  /**
   * Sets version of the loaded file
   * @param ver the new version
   */
  void setVersion(const QString & ver);

  /* TODO
  bool saveToLex     (QTextStream& os, QString &title);
  bool loadFromLex   (QTextStream& is);
  bool loadLessonLex (QTextStream& is);
  bool saveLessonLex (QTextStream& os);
  bool saveTypeNameLex   (QTextStream& os);
  bool loadTypeNameLex   (QTextStream& is);

  bool saveToCsv     (QTextStream& os, QString &title);
  bool loadFromCsv   (QTextStream& is);
  bool loadLessonCsv (QTextStream& is);
  bool saveLessonCsv (QTextStream& os);
  bool saveTypeNameCsv   (QTextStream& os);
  bool loadTypeNameCsv   (QTextStream& is);

  bool saveTypeNameVcb (QTextStream &os);
  bool loadTypeNameVcb (QTextStream &is);
  bool saveLessonVcb (QTextStream &os);
  bool loadLessonVcb (QTextStream &is);
  bool saveToVcb (QTextStream& os, QString &title);
  bool loadFromVcb (QTextStream& is);
  void errorVcb (int line, const QString &text );
  */

 private:
  bool                  m_dirty;
  KUrl                  m_url;
  QList<bool>      m_sortIdentifier;
  bool                  m_sortLesson;
  bool                  m_unknownAttribute;
  bool                  m_unknownElement;
  bool                  m_enableSorting;

  // save these to document
  QStringList           m_identifiers;      //0= origin, 1,.. translations
  int                   m_cols;
  int                   m_lines;
  int                   m_currentLesson;
  QList<int>       m_extraSizeHints;
  QList<int>       m_sizeHints;
  QFont*                m_font;

  QString               m_generator;
  QString               m_queryorg;
  QString               m_querytrans;
  QList<KEduVocExpression>  m_vocabulary;
  QList<bool>      m_lessonsInQuery;
  QStringList           m_lessonDescriptions;
  QStringList           m_typeDescriptions;
  QStringList           m_tenseDescriptions;
  QStringList           m_usageDescriptions;
  QString               m_title;
  QString               m_author;
  QString               m_license;
  QString               m_remark;
  QString               m_version;

  QList<KEduVocArticle>   m_articles;
  QList<KEduVocConjugation> m_conjugations;

  LeitnerSystem* 	       m_leitnerSystem;
  bool			       m_activeLeitnerSystem;
};


#endif // KEDUVOCDOCUMENT_H
