/***************************************************************************
                   Vocabulary Document for KDE Edu
    -----------------------------------------------------------------------
    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
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

#ifndef KEDUVOCDOCUMENT_H
#define KEDUVOCDOCUMENT_H

#include "libkeduvocdocument_export.h"

#define KVD_VERS_PREFIX " v"       // kvoctrain v0.1.0

#define WQL_IDENT      "WordQuiz"

#define KVTML_EXT        "kvtml"
#define CSV_EXT          "csv"
#define TXT_EXT          "txt"
#define WQL_EXT          "wql"

#include <QtCore/QObject>
#include <QtCore/QList>

#include <KUrl>

#include "keduvocgrammar.h"
#include "keduvocwordtype.h"

class QStringList;
class KEduVocExpression;
class KEduVocLesson;
class LeitnerSystem;
class KEduVocWordType;

/**
 * This class contains the expressions of your vocabulary
 * as well as other information about the vocabulary
 */
class KEDUVOCDOCUMENT_EXPORT KEduVocDocument : public QObject
{
  Q_OBJECT
public:

  enum FileType { kvd_none,
                  automatic,
                  kvtml,
                  wql,
                  pauker,
                  vokabeln,
                  xdxf,
                  csv,
				  kvtml1 };

  /**
   * Constructor for a KdeEdu vocabulary document
   *
   * @param parent calling object
   */
  explicit KEduVocDocument(QObject* parent = 0);

  /**
   * Destructor
   */
  ~KEduVocDocument();

  // *** whole document methods ***

  /**
   * Open a document file
   *
   * @param url      url to file to open
   * @returns        true if successful
   */
  bool open(const KUrl& url);

  /**
   * Saves the data under the given name
   *
   * @param url        if url is empty (or NULL) actual name is preserved
   * @param ft         the filetype to be used when saving the document
   * @param generator  the name of the application saving the document
   * @returns          true if successful
   */
  bool saveAs(const KUrl & url, FileType ft, const QString & generator);

  /**
   * Merges data from another document
   *
   * @param docToMerge       document containing the data to be merged
   * @param matchIdentifiers if true only entries having identifiers present in the
   *                         current document will be mergedurl is empty (or NULL) actual name is preserved
   */
  void merge(KEduVocDocument *docToMerge, bool matchIdentifiers);

  /**
   * Indicates if the document is modified
   *
   * @param dirty   new state
   */
  void setModified(bool dirty = true);

  /** @returns the modification state of the doc */
  bool isModified() const;

  /**
   * Enables sorting
   */
  void setSortingEnabled(bool enable);

  /** @returns whether sorting is enabled */
  bool isSortingEnabled() const;

  /**
   * Sets the URL of the XML file
   */
  void setUrl(const KUrl& url);

  /** @returns the URL of the XML file */
  KUrl url() const;

  /** set the title of the XML file
   * @param title title to set */
  void setTitle(const QString & title);

  /** @returns the title of the XML file */
  QString title() const;

  /** set the author of the file
   * @param author author to set */
  void setAuthor(const QString & author);

  /** @returns the author of the file */
  QString author() const;

  /** set the license of the file
   * @param license license to set */
  void setLicense(const QString & license);

  /** @returns the license of the file */
  QString license() const;

  /** set the remark of the file
   * @param rem remark to set */
  void setDocumentRemark(const QString & rem);

  /** @return the remark of the file */
  QString documentRemark() const;

  /**
   * Sets the generator of the file
   */
  void setGenerator(const QString & generator);

  /** @returns the generator of the file */
  QString generator() const;

  /** Sets version of the loaded file
   * @param ver the new version */
  void setVersion(const QString & ver);

  /** @returns the version of the loaded file */
  QString version() const;

  // *** entry methods ***

  /**
   * Appends a new expression to the end of the vocabulary
   *
   * @param expression      expression to append
   */
  void appendEntry(KEduVocExpression *expression);

  /**
   * Inserts a new expression
   *
   * @param expression      expression to insert
   * @param index           index of entry
   */
  void insertEntry(KEduVocExpression *expression, int index);

  /**
   * Removes an expression from the document
   *
   * @param index           index of expression
   */
  void removeEntry(int index);

  /**
   * Shuffles vocabulary in a random order
   */
  void shuffle();

  /**
   * Removes duplicate entries (original plus all translations)
   *
   * @returns                 number of removed entries
   */
  int cleanUp();

  /**
   * @returns the number of entries
   */
  int entryCount() const;

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

  // *** identifier methods ***

  /**
   * @returns the number of different identifiers (usually languages)
   */
  int identifierCount() const;

  /**
   * Appends a new identifier (usually a language)
   *
   * @param id         the identifier to append
   * @returns the identifier number
   */
  int appendIdentifier(const QString & id);

  /**
   * Sets the original identifier
   *
   * @param id              the new identifier
   */
  void setOriginalIdentifier(const QString &id);

  /**
   * @returns the original identifier
   */
  QString originalIdentifier() const;

  /**
   * Sets the identifier of translation
   *
   * @param index            number of translation 1..x
   * @param lang             thr language identifier: en=english, de=german, ...
   */
  void setIdentifier(int index, const QString &lang);

  /**
   * Returns the identifier of translation @p index
   *
   * @param index            number of translation 1..x
   * @returns                the language identifier: en=english, de=german, ...
   */
  QString identifier(int index) const;

  /**
   * Removes identifier an the according translation in all entries
   *
   * @param index            number of translation 1..x
   */
  void removeIdentifier(int index);

  /**
   * Determines the index of a given identifier
   *
   * @param lang             identifier of language
   * @returns                index of identifier, 0 = original, 1..n = translation, -1 = not found
   */
  int indexOfIdentifier(const QString &lang) const;

  // *** type methods ***

  /**
   * Returns the type handling class. See KEduVocWordType.
   *
   * @returns the KEduVocWordType* type handling class
   */
  KEduVocWordType* wordTypes();


  // *** tense methods ***

  /**
   * Sets the tense string
   *
   * @param index            number of tense
   * @param str              name of tense
   */
  void setTenseName(int index, QString &str);

  /**
   * Returns the tense string
   *
   * @param index            number of tense
   * @returns                string
   */
  QString tenseName(int index) const;

  /**
   * Sets the description of the tenses
   */
  void setTenseDescriptions(const QStringList &names);

  /**
   * Gets the descriptions of the tenses
   */
  QStringList tenseDescriptions() const;

  // *** usage methods ***

  /**
   * Sets usage string
   *
   * @param index            number of usage
   * @param str              name of usage
   */
  void setUsageName(int index, QString &str);

  /**
   * Returns usage string
   *
   * @param index            number of usage
   * @returns                string
   */
  QString usageName(int index) const;

  /**
   * Sets the descriptions of the usages
   */
  void setUsageDescriptions(const QStringList &names);

  /**
   * Gets the descriptions of the usages
   */
  QStringList usageDescriptions() const;

  // *** grade methods ***

  /**
   * Sets grades to KV_NORM_GRADE, counts to 0 ...
   *
   * @param index    index of language 0..x, -1 = all
   * @param lesson   lesson id, if this is 0 all lesson are affected,
   *                 otherwise only matching numbers
   */
  void resetEntry(int index = -1, int lesson = 0);

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
   * Retrieves the identifiers for the current query
   *
   * @param org        identifier for original
   * @param trans      identifier for translation
   */
  void queryIdentifier(QString &org, QString &trans) const;

  /**
   * Sets the identifiers for the current query
   *
   * @param org        identifier for original
   * @param trans      identifier for translation
   */
  void setQueryIdentifier(const QString &org, const QString &trans);

  // *** lesson methods ***

  /**
   * @returns the current lesson index
   */
  int currentLesson() const;

  /**
   * Sets current lesson index
   * @param lesson    index of lesson
   */
  void setCurrentLesson(int lesson);

  /** get a lesson object
   * @returns a pointer to the lesson object at the specified index or NULL if there isn't one
   */
  KEduVocLesson *lesson(int index);

  /** get all lesson objects
   * @returns a map of pointers to lesson objects
   */
  QMap<int, KEduVocLesson *> lessons() const;

  /**
   * @returns                the number of lessons defined
   */
  int lessonCount() const;

  /**
   * Get list of ALL lessons that are selected for query.
   * @returns a list with the lessons in the current query
   */
  QList<int> lessonsInQuery() const;

  /**
   * Sets ALL lessons in the query. Better use addLessonToQuery and removeLessonFromQuery.
   */
  void setLessonsInQuery(const QList<int> &lesson_iq);

  /**
   * Check if @p lessonIndex is in the query.
   * @param lessonIndex - index of the lesson
   * @return true if in query
   */
  bool lessonInQuery(int lessonIndex) const;

  /**
   * Add @p lessonIndex to the query.
   * @param lessonIndex - index of the lesson
   */
  void addLessonToQuery(int lessonIndex);

  /**
   * Remove @p lessonIndex from the query.
   * @param lessonIndex - index of the lesson
   */
  void removeLessonFromQuery(int lessonIndex);

  /**
   * Append a new lesson to the list of lessons.
   * @param lessonName name for the new lesson
   * @param position lesson number to use (-1 to find the next hole to put it in)
   * @returns the index of the new lesson
   */
  int addLesson(const QString &lessonName, int position = -1);

  /**
   * Delete a lesson.
   * @param lessonIndex which lesson
   * @param deleteMode either KVocDocument::DeleteEmptyLesson (delete only if empty) or KVocDocument::DeleteEntriesAndLesson (delete including vocabulary in that lesson)
   * @returns if the deletion was successful. If there are vocabularies in the lesson, but DeleteEmptyLesson, this will return false and not delete the lesson.
   */
  bool deleteLesson(int lessonIndex, int deleteMode);

  ///**
  // * Get the real name of a lesson from it's index as QString.
  // * @param index lesson index
  // * @returns the description (Name) of the lesson with index @p index .
  // */
  //QString lessonDescription(int index) const;

  /**
   * Get the index from the long name of a lesson.
   * @param description lesson name
   * @returns the index of the lesson (from its name)
   * -1 if the lesson does not exist
   */
  //int lessonIndex(const QString &description) const;

  /**
   * Rename a lesson.
   * @param lessonIndex index of lesson
   * @param lessonName new name for the lesson
   */
  //void renameLesson(const int lessonIndex, const QString &lessonName);

  /**
   * All lesson descriptions as stringlist.
   * @returns a list of defined lessons
   */
  QStringList lessonDescriptions() const;

  /**
   * Sets the description of the lesson
   * @param names list of all names of the lessons
   */
  //void setLessonDescriptions(const QStringList &names);

  /**
   * Moves the lesson at index position from to index position to.
   * @param from the lesson to be moved
   * @param to the new position
   */
  //void moveLesson(int from, int to);

  // *** conjugation methods ***

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
   * @returns                the number of conjugations
  */
  int conjugationCount() const;

  // *** article methods ***

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
   * @returns                the number of articles
  */
  int articleCount() const;

  // *** file format specific methods ***

  /**
   * Returns the delimiter (separator) used for csv import and export.
   * The default is a single tab character
   *
   * @returns                the delimiter used
   */
  QString csvDelimiter() const;

  /**
   * Sets the delimiter (separator) used for csv import and export
   *
   * @param delimiter        the delimiter to use
   */
  void setCsvDelimiter(const QString &delimiter);

  bool leitnerSystemActive();
  void setLeitnerSystemActive(bool yes);
  void createStandardLeitnerSystem();
  void setLeitnerSystem( LeitnerSystem* system );
  LeitnerSystem* leitnerSystem();

  FileType detectFileType(const QString &fileName);

  enum Mode {
    Reading,
    Writing
  };

  enum LessonDeletion {
    DeleteEmptyLesson,
    DeleteEntriesAndLesson
  };

  /**
   * Create a string with the supported document types, that can be used
   * as filter in KFileDialog. It includes also an entry to match all the
   * supported types.
   *
   * @param mode             the mode for the supported document types
   * @returns                the filter string
   */
  static QString pattern(Mode mode);

Q_SIGNALS:
  void progressChanged (KEduVocDocument *, int curr_percent);
  void docModified (bool mod);

private:
  // The private data of this - see KEduVocDocument::Private, implemented in keduvocdocument.cpp
  class KEduVocDocumentPrivate;
  KEduVocDocumentPrivate* const d;

  Q_DISABLE_COPY(KEduVocDocument)
};


#endif // KEDUVOCDOCUMENT_H
