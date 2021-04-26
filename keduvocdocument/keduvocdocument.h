/*
 * Vocabulary Document for KDE Edu
 * SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
 * SPDX-FileCopyrightText: 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef KEDUVOCDOCUMENT_H
#define KEDUVOCDOCUMENT_H

#include "keduvocdocument_export.h"

#include "keduvocidentifier.h"
#include "keduvocarticle.h"
#include "keduvocconjugation.h"

#include <QObject>
#include <QList>
#include <QMap>
#include <QUrl>

class KEduVocLesson;
class KEduVocWordType;
class KEduVocLeitnerBox;

/**
 * @brief The primary entry point to the hierarchy of objects describing vocabularies.
 * @details This class contains the expressions of your vocabulary
 * as well as other information about the vocabulary.
 */
class KEDUVOCDOCUMENT_EXPORT KEduVocDocument : public QObject
{
    Q_OBJECT
public:

    ///@todo in new API change enums to QFlags

    /// known vocabulary file types
    enum FileType {
        KvdNone,      ///< handles nothing
        Automatic,    ///< try to automatically determine file type
        Kvtml,        ///< Kvtml 2.0
        Wql,          ///< Word Quiz format
        Pauker,       ///< Pauker format
        Vokabeln,     ///< Vokabeln format
        Xdxf,         ///< XDXF format @see https://github.com/soshial/xdxf_makedict/blob/master/format_standard/xdxf_strict.dtd
        Csv,          ///< Command separated values
        Kvtml1        ///< KVTML 1.0
    };

    /// the return code when opening/saving
    enum ErrorCode {
        NoError = 0,         ///< no error
        Unknown,             ///< unspecified error
        InvalidXml,          ///< malformed xml or bad file formatting
        FileTypeUnknown,     ///< unknown file type
        FileCannotWrite,     ///< unwritable file
        FileWriterFailed,    ///< file writer failed
        FileCannotRead,      ///< unreadable file
        FileReaderFailed,    ///< file reader failed
        FileDoesNotExist,    ///< unknown file type
        FileLocked,          ///< An autosave file exists for this document
        FileCannotLock,      ///< Can't create an autosave file for this document
        FileIsReadOnly       ///< Can't save this file because it was opened read-only
    };

    /// indicates file open/save status locking or readonly
    enum FileHandlingFlags
    {
        FileDefaultHandling = 0x0, ///< Default status
        FileIgnoreLock = 0x1,      ///< Ignore the file lock
	FileOpenReadOnly = 0x2	   ///< Open without any intention to change and save back later. This also implies FileIgnoreLock.
    };

    /// used as parameter for pattern
    enum FileDialogMode
    {
        Reading,
        Writing
    };

    /// delete only empty lessons or also if they have entries
    enum LessonDeletion
    {
        DeleteEmptyLesson,
        DeleteEntriesAndLesson
    };

    /**
     * Constructor for a KDEEdu vocabulary document
     *
     * @param parent calling object
     */
    explicit KEduVocDocument( QObject* parent = nullptr );

    /**
     * Destructor
     */
    ~KEduVocDocument();


    /** @name Whole Document Methods
     * Methods related to saving/loading and locating the document
     * @nosubgrouping
     */
    ///@{

    /**
     * Opens and locks a document file
     *
     * Note: This is intended to be preserve binary compatible with int open(const QUrl&)
     *       When the API increments the major version number, then merge them
     *
     * @param url      url to file to open
     * @param flags How to handle expected unusual conditions (i.e. locking)
     * @returns        ErrorCode
     */
    ErrorCode open(const QUrl &url,  FileHandlingFlags flags = FileDefaultHandling);

    /**
     * Close a document file and release the lock on the file
     *
     */
    void close();

    /**
     * Saves the data under the given name
     *
     * @pre generator is set.
     *
     * Note: This is intended to be preserve binary compatible with
     *       int saveAs(const QUrl&, FileType ft, const QString & generator );
     *       When the API increments the major version number, then merge them
     *
     * @param url        if url is empty (or NULL) actual name is preserved
     * @param ft         the filetype to be used when saving the document
     * @param flags How to handle expected unusual conditions (i.e. locking)
     * @returns          ErrorCode
     */
    ErrorCode saveAs( const QUrl & url, FileType ft,  FileHandlingFlags flags  = FileDefaultHandling);

    /** @details  returns a QByteArray KVTML2 representation of this doc
     * @param generator name of the application creating the QByteArray
     * @return KVTML2 XML
     * @todo in new API if this should be part of save*/
    QByteArray toByteArray(const QString &generator);

    /**
     * Merges data from another document (UNIMPLEMENTED)
     *
     * @param docToMerge       document containing the data to be merged
     * @param matchIdentifiers if true only entries having identifiers present in the
     *                         current document will be mergedurl is empty (or NULL) actual name is preserved
     */
    void merge( KEduVocDocument *docToMerge, bool matchIdentifiers );

    /**
     * Indicates if the document is modified
     *
     * @param dirty   new state
     */
    void setModified( bool dirty = true );

    /** @returns the modification state of the doc */
    bool isModified() const;

    /**
     * Sets the URL of the XML file
     * @param url URL
     */
    void setUrl( const QUrl& url );

    /** @returns the URL of the XML file */
    QUrl url() const;

    ///@}



    /** @name General Document Properties
     *
     */
    ///@{

    /** Set the title of the file
     * @param title title to set */
    void setTitle( const QString & title );

    /** @returns the title of the file */
    QString title() const;

    /** Set the author of the file
     * @param author author to set */
    void setAuthor( const QString & author );

    /** @returns the author of the file */
    QString author() const;

    /** Set the author contact info
     * @param authorContact contact email/contact info to set */
    void setAuthorContact( const QString & authorContact );

    /** @returns the author contact information */
    QString authorContact() const;

    /** Set the license of the file
     * @param license license to set */
    void setLicense( const QString & license );

    /** @returns the license of the file */
    QString license() const;

    /** Set the comment of the file
     * @param comment comment to set */
    void setDocumentComment( const QString & comment );

    /** @return the comment of the file */
    QString documentComment() const;

    /** Set the category of the file
     * @param category category to set */
    void setCategory( const QString & category );

    /** @return the category of the file */
    QString category() const;

    /**
     * Sets the generator of the file
     * @param generator name of the application generating the file
     */
    void setGenerator( const QString & generator );

    /** @returns the generator of the file */
    QString generator() const;

    /** Sets version of the loaded file
     * @param ver the new version */
    void setVersion( const QString & ver );

    /** @returns the version of the loaded file */
    QString version() const;

    ///@}



    /** @name Identifier Methods
     *
     */
    ///@{

    /**
     * @returns the number of different identifiers (usually languages)
     */
    int identifierCount() const;

    /**
     * Appends a new identifier (usually a language)
     *
     * @param identifier the identifier to append. If empty default names are used.
     * @returns the identifier number
     */
    int appendIdentifier( const KEduVocIdentifier & identifier = KEduVocIdentifier());

    /**
     * Sets the identifier of translation
     *
     * @param index            number of translation 0..x
     * @param lang             the language identifier: en=english, de=german, ...
     */
    void setIdentifier( int index, const KEduVocIdentifier& lang );

    /**
     * Returns the identifier of translation @p index
     *
     * @param index            number of translation 0..x
     * @returns                the language identifier: en=english, de=german, ...
     */
    KEduVocIdentifier& identifier( int index );

    /**
     * Const overload of identifier(int);
     * @param index of the identifier
     * @return reference to the identifier
     */
    const KEduVocIdentifier& identifier( int index ) const;

    /**
     * Removes identifier and the according translation in all entries
     *
     * @param index            number of translation 0..x
     */
    void removeIdentifier( int index );

    /**
     * Determines the index of a given identifier
     *
     * @param name             identifier of language
     * @returns                index of identifier, 0 = original, 1..n = translation, -1 = not found
     */
    int indexOfIdentifier( const QString &name ) const;


    ///@}



    /** @name Grade Methods
     *
     */
    ///@{

    /**
     * Retrieves the identifiers for the current query
     * not written in the new version!
     *
     * @param org        identifier for original
     * @param trans      identifier for translation
     */
    KEDUVOCDOCUMENT_DEPRECATED void queryIdentifier( QString &org, QString &trans ) const;

    /**
     * Sets the identifiers for the current query
     * not written in the new version!
     *
     * @param org        identifier for original
     * @param trans      identifier for translation
     */
    KEDUVOCDOCUMENT_DEPRECATED void setQueryIdentifier( const QString &org, const QString &trans );


    ///@}



    /** @name Lesson Methods
     *
     */
    ///@{

    /** @brief Get the lesson root object
     * @returns a pointer to the lesson object
     */
    KEduVocLesson * lesson();

    /** @brief Returns the root word type object
        @return poitner to the internal word type object */
    KEduVocWordType * wordTypeContainer();

    /** @brief Returns the root Leitner container
        @return poitner to the internal Leitner container object
        @todo in new API determine if this is used */
    KEduVocLeitnerBox * leitnerContainer();


    ///@}



    /** @name File Format Specific Methods
     *
     */
    ///@{

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
    void setCsvDelimiter( const QString &delimiter );

    ///@}

    /**
     * @details Detects the file type
     * @param fileName filename
     * @return enum of filetype
     */
    static FileType detectFileType(const QString &fileName);

    /**
     * Create a string with the supported document types, that can be used
     * as filter in KFileDialog. It includes also an entry to match all the
     * supported types.
     *
     * @param mode             the mode for the supported document types
     * @returns                the filter string
     */
    static QString pattern( FileDialogMode mode );

    /**
     * @brief Returns a QString description of the errorCode
     * @param errorCode the code
     * @returns a user useful error string.
     */
    static QString errorDescription( int errorCode );

Q_SIGNALS:
    /**
     * @brief never used
     * @param curr_percent
     */
    void progressChanged( KEduVocDocument *, int curr_percent );

    /**
     * @brief Emitted when the document becomes modified or saved.
     * @param mod the current modified/dirty state
     * @returns state (true=modified)
     */
    void docModified( bool mod );

private:
    // The private data of this - see KEduVocDocument::Private, implemented in keduvocdocument.cpp
    class KEduVocDocumentPrivate;
    KEduVocDocumentPrivate* const d;  ///< d pointer to private class

    Q_DISABLE_COPY( KEduVocDocument )
};


#endif // KEDUVOCDOCUMENT_H
