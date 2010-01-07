/***************************************************************************
                   Vocabulary Document for KDE Edu
    -----------------------------------------------------------------------
    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

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

#include "keduvocidentifier.h"
#include "keduvocarticle.h"
#include "keduvocconjugation.h"

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QMap>

#include <KDE/KUrl>

class QStringList;
class KEduVocExpression;
class KEduVocLesson;
class KEduVocWordType;
class KEduVocLeitnerBox;

/**
 * This class contains the expressions of your vocabulary
 * as well as other information about the vocabulary
 */
class KEDUVOCDOCUMENT_EXPORT KEduVocDocument : public QObject
{
    Q_OBJECT
public:

    /// known vocabulary file types
    enum FileType {
        KvdNone,
        Automatic,
        Kvtml,
        Wql,
        Pauker,
        Vokabeln,
        Xdxf,
        Csv,
        Kvtml1
    };

    /// the return code when opening/saving
    enum ErrorCode {
        NoError = 0,
        Unknown,
        InvalidXml,
        FileTypeUnknown,
        FileCannotWrite,
        FileWriterFailed,
        FileCannotRead,
        FileReaderFailed,
        FileDoesNotExist
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
     * Constructor for a KdeEdu vocabulary document
     *
     * @param parent calling object
     */
    explicit KEduVocDocument( QObject* parent = 0 );

    /**
     * Destructor
     */
    ~KEduVocDocument();

    // *** whole document methods ***

    /**
     * Open a document file
     *
     * @param url      url to file to open
     * @returns        ErrorCode
     */
    int open( const KUrl& url );

    /**
     * Saves the data under the given name
     *
     * @param url        if url is empty (or NULL) actual name is preserved
     * @param ft         the filetype to be used when saving the document
     * @param generator  the name of the application saving the document
     * @returns          ErrorCode
     */
    int saveAs( const KUrl & url, FileType ft, const QString & generator );

    QByteArray toByteArray(const QString &generator);

    /**
     * Merges data from another document
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
     */
    void setUrl( const KUrl& url );

    /** @returns the URL of the XML file */
    KUrl url() const;


    /** set the title of the file
     * @param title title to set */
    void setTitle( const QString & title );

    /** @returns the title of the file */
    QString title() const;

    /** set the author of the file
     * @param author author to set */
    void setAuthor( const QString & author );

    /** @returns the author of the file */
    QString author() const;

    /** set the author contact info
     * @param contact email/contact info to set */
    void setAuthorContact( const QString & authorContact );

    /** @returns the author contact information */
    QString authorContact() const;

    /** set the license of the file
     * @param license license to set */
    void setLicense( const QString & license );

    /** @returns the license of the file */
    QString license() const;

    /** set the comment of the file
     * @param comment comment to set */
    void setDocumentComment( const QString & comment );

    /** @return the comment of the file */
    QString documentComment() const;

    /** set the category of the file
     * @param category category to set */
    void setCategory( const QString & category );

    /** @return the category of the file */
    QString category() const;

    /**
     * Sets the generator of the file
     */
    void setGenerator( const QString & generator );

    /** @returns the generator of the file */
    QString generator() const;

    /** Sets version of the loaded file
     * @param ver the new version */
    void setVersion( const QString & ver );

    /** @returns the version of the loaded file */
    QString version() const;

    // *** identifier methods ***

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
     * @param lang             identifier of language
     * @returns                index of identifier, 0 = original, 1..n = translation, -1 = not found
     */
    int indexOfIdentifier( const QString &name ) const;

    // *** grade methods ***

    /**
     * Retrieves the identifiers for the current query
     * not written in the new version!
     *
     * @param org        identifier for original
     * @param trans      identifier for translation
     */
    KDE_DEPRECATED void queryIdentifier( QString &org, QString &trans ) const;

    /**
     * Sets the identifiers for the current query
     * not written in the new version!
     *
     * @param org        identifier for original
     * @param trans      identifier for translation
     */
    KDE_DEPRECATED void setQueryIdentifier( const QString &org, const QString &trans );

    // *** lesson methods ***

    /** get the lesson root object
     * @returns a pointer to the lesson object
     */
    KEduVocLesson * lesson();

    KEduVocWordType * wordTypeContainer();

    KEduVocLeitnerBox * leitnerContainer();

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
    void setCsvDelimiter( const QString &delimiter );

    static FileType detectFileType( const QString &fileName );

    /**
     * Create a string with the supported document types, that can be used
     * as filter in KFileDialog. It includes also an entry to match all the
     * supported types.
     *
     * @param mode             the mode for the supported document types
     * @returns                the filter string
     */
    static QString pattern( FileDialogMode mode );

    static QString errorDescription( int errorCode );

Q_SIGNALS:
    void progressChanged( KEduVocDocument *, int curr_percent );

    /**
     * Emitted when the document becomes modified or saved.
     * @returns state (true=modified)
     */
    void docModified( bool mod );

private:
    // The private data of this - see KEduVocDocument::Private, implemented in keduvocdocument.cpp
    class KEduVocDocumentPrivate;
    KEduVocDocumentPrivate* const d;

    Q_DISABLE_COPY( KEduVocDocument )
};


#endif // KEDUVOCDOCUMENT_H
