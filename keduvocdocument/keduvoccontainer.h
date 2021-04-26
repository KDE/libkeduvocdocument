/*
 * SPDX-FileCopyrightText: 2007 Jeremy Whiting <jpwhiting@kde.org>
 * SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KEDUVOCCONTAINER_H
#define KEDUVOCCONTAINER_H

#include "keduvocdocument_export.h"

#include "keduvoctext.h"

#include <QList>
#include <QUrl>

class KEduVocDocument;
class KEduVocExpression;

/** class to store information about a container - that can be a lesson or word types */
class KEDUVOCDOCUMENT_EXPORT KEduVocContainer
{
    // make this a template?

public:
    enum EnumContainerType{
        Container,
        Lesson,
        WordType,
        Leitner
    };

    enum EnumEntriesRecursive{
        NotRecursive = 0,
        Recursive = 1
    };

    /** default constructor */
    explicit KEduVocContainer(const QString& name, EnumContainerType type,
                  KEduVocContainer *parent = nullptr);

    void appendChildContainer(KEduVocContainer *child);
    void insertChildContainer(int row, KEduVocContainer *child);
    void deleteChildContainer(int row);
    void removeChildContainer(int row);
    KEduVocContainer *childContainer(int row);

    /**
     * Retrieve a child container by its name
     * Returns 0 if no container is found
     * @param name container name
     * @return the child container
     */
    KEduVocContainer *childContainer(const QString& name);

    QList<KEduVocContainer *> childContainers();

    /**
     * Find a child container
     * @param name
     * @return
     */
//     KEduVocContainer *childContainer(const QString& name);

    int childContainerCount() const;

    int row() const;
    virtual KEduVocContainer *parent();

    /** copy constructor for d-pointer safe copying */
    KEduVocContainer( const KEduVocContainer &other );

    /** destructor */
    virtual ~KEduVocContainer();

    /** assignment operator */
    KEduVocContainer& operator= ( const KEduVocContainer& );

    /** @return the containing KEduVocDocument */
    KEduVocDocument *document() const;

    /** set the container name
     * @param name text to set for the name
     */
    void setName( const QString &name );

    /** get the container name */
    QString name();

    /** get a list of all entries in the container */
    virtual QList < KEduVocExpression* > entries(EnumEntriesRecursive recursive = NotRecursive) =0;
    virtual int entryCount(EnumEntriesRecursive recursive = NotRecursive) =0;
    virtual KEduVocExpression* entry(int row, EnumEntriesRecursive recursive = NotRecursive) =0;

    /**
     * Removes a translation. This has to be called when a language is removed from a document.
     * @param translation
     */
    void removeTranslation(int translation);

    bool inPractice();
    void setInPractice( bool inPractice );

    /** equality operator */
    bool operator==(const KEduVocContainer &other) const;

    /**
     * The type of this container. @see EnumContainerType
     * @return
     */
    KEduVocContainer::EnumContainerType containerType();

    /**
     * Set the type of container.
     * For convenience by default this is taken over from the parent, so no need to set.
     * @param type the new type
     */
    void setContainerType(KEduVocContainer::EnumContainerType type);


    /** get the image url for this container if it exists */
    QUrl imageUrl();

    /** set the image url for this container
     * @param url               url of the image
     */
    void setImageUrl(const QUrl &url);

    double averageGrade(int translation, EnumEntriesRecursive recursive);

    int expressionsOfGrade(int translation, grade_t grade, EnumEntriesRecursive recursive);

    /**
     * Remove grades from all entries of this lessons
     * @param translation which translation to remove. -1 for all.
     * @param recursive whether to include child lessons
     */
    void resetGrades(int translation, EnumEntriesRecursive recursive);

protected:
    QList< KEduVocExpression * > entriesRecursive();

    /**
     * Set the child entry cache to invalid
     */
    void invalidateChildLessonEntries();

    /**
     * Recreate the cache of entries in this lesson's child lessons.
     */
    void updateChildLessonEntries();

    // Used by KEduVocLesson when the Document creates the top level lesson.
    explicit KEduVocContainer(const QString& name, EnumContainerType type,
			      KEduVocDocument *document);
private:
    class Private;
    Private * const d;
};

#endif
