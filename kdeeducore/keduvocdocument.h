/***************************************************************************
                   Vocabulary Document for KDE Edu
    -----------------------------------------------------------------------
    copyright     : (C) 1999-2001 Ewald Arnold
                    (C) 2001 The KDE-EDU team
                    (C) 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
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

#define KVD_VERS_PREFIX " v"       // kvoctrain v0.1.0

#define WQL_IDENT      "WordQuiz"

#define KVTML_EXT        "kvtml"
#define CSV_EXT          "csv"
#define TXT_EXT          "txt"
#define WQL_EXT          "wql"

#include <QtCore/QObject>
#include <QtCore/QList>

#include <kurl.h>

#include "keduvocgrammar.h"

class QStringList;
class KEduVocExpression;
class KEduVocMultipleChoice;
class LeitnerSystem;

/**
 * This class contains the expressions of your vocabulary
 * as well as other information about the vocabulary
 */
class KDEEDUCORE_EXPORT KEduVocDocument : public QObject
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
                  csv };

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

  /**
   * Indicates if the document is modified
   *
   * @param dirty   new state
   */
  void setModified(bool dirty = true);

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
   * Removes duplicate entries (original plus all translations)
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
  void setSortingEnabled(bool enable);

  /**
   * @returns whether sorting is enabled
   */
  bool isSortingEnabled() const;

  /**
   * @returns the modification state of the doc
   */
  bool isModified() const;

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
   * Determines the index of a given identifier
   *
   * @param lang             identifier of language
   * @returns                index of identifier, 0 = original, 1..n = translation, -1 = not found
   */
  int indexOfIdentifier(const QString &lang) const;

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
  QStringList typeDescriptions() const;

  /**
   * Sets the descriptions of the types
   */
  void setTypeDescriptions(const QStringList &names);

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
  QStringList tenseDescriptions() const;

  /**
   * Sets the description of the tenses
   */
  void setTenseDescriptions(const QStringList &names);

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
  QStringList usageDescriptions() const;

  /**
   * Sets the descriptions of the usages
   */
  void setUsageDescriptions(const QStringList &names);

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
   * @param parent     the object that owns the document
   * @param url        if url is empty (or NULL) actual name is preserved
   * @param ft         the filetype to be used when saving the document
   * @param generator  the name of the application saving the document
   * @returns          true if successful
   */
  bool saveAs(QObject *parent, const KUrl & url, FileType ft, const QString & generator);

  /**
   * Merges data from another document
   *
   * @param docToMerge       document containing the data to be merged
   * @param matchIdentifiers if true only entries having identifiers present in the
   *                         current document will be mergedurl is empty (or NULL) actual name is preserved
   */
  void merge(KEduVocDocument *docToMerge, bool matchIdentifiers);

  /**
   * @returns the number of entries
   */
  int entryCount() const;

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
  int identifierCount() const;

  /**
   * Appends a new identifier (usually a language)
   *
   * @param id         the identifier to append
   */
  void appendIdentifier(const QString & id);

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
  KUrl url() const;

  /**
   * Sets the URL of the XML file
   */
  void setUrl(const KUrl& url);

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
  QString documentRemark() const;

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
  void setDocumentRemark(const QString & rem);

  /**
   * Sets the generator of the file
   */
  void setGenerator(const QString & generator);

  /**
   * Gets the generator of the file
   */
  QString generator() const;

  /**
   * Gets the version of the loaded file
   */
  QString version() const;

  /**
   * Sets version of the loaded file
   * @param ver the new version
   */
  void setVersion(const QString & ver);

  /**
   * @returns the current lesson index
   */
  int currentLesson() const;

  /**
   * Sets current lesson index
   * @param lesson    index of lesson
   */
  void setCurrentLesson(int lesson);

  /**
   * @returns the description of the lesson
   */
  QString lessonDescription(int index) const;

  /**
   * @returns the index of the lesson (from its name)
   * -1 if the lesson does not exist
   */
  int lessonIndex(const QString description) const;

  /**
   * @returns a list with the lessons in the current query
   */
  QList<int> lessonsInQuery() const;

  /**
   * Sets the lessons in the current query
   */
  void setLessonsInQuery(const QList<int> &lesson_iq);

  /**
   * @returns                a list of defined lessons
   */
  QStringList lessonDescriptions() const;

  /**
   * @returns                the number of lessons defined
   */
  int lessonCount() const;
  /**
   * Delete a lesson.
   * @param lessonIndex which lesson
   * @param deleteMode either KVocDocument::DeleteEmptyLesson (delete only if empty) or KVocDocument::DeleteEntriesAndLesson (delete including vocabulary in that lesson)
   * @returns if the deletion was successfull. If there are vocabularies in the lesson, but DeleteEmptyLesson, this will return false and not delete the lesson.
   */
  bool deleteLesson(int lessonIndex, int deleteMode);

  /**
   * Sets the description of the lesson
   */
  void setLessonDescriptions(const QStringList &names);

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
  class Private;
  Private* const d;

  Q_DISABLE_COPY(KEduVocDocument)
};


#endif // KEDUVOCDOCUMENT_H
