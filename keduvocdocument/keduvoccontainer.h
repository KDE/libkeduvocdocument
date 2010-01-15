/***************************************************************************

    Copyright 2007 Jeremy Whiting <jpwhiting@kde.org>
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCCONTAINER_H
#define KEDUVOCCONTAINER_H

#include "libkeduvocdocument_export.h"

#include "keduvoctext.h"

#include <KDE/KUrl>
#include <QtCore/QList>

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
    explicit KEduVocContainer(const QString& name, EnumContainerType type, KEduVocContainer *parent = 0);

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
    bool operator==(const KEduVocContainer &other);

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
    KUrl imageUrl();

    /** set the image url for this container
     * @param url               url of the image
     */
    void setImageUrl(const KUrl &url);

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

private:
    class Private;
    Private * const d;
};

#endif
