/***************************************************************************

    Copyright 2007 Jeremy Whiting <jeremywhiting@scitools.com>
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

#include <KDE/KUrl>

#include <QtCore/QList>
#include <QtCore/QString>

class KEduVocExpression;

/** class to store information about a lesson */
class KEDUVOCDOCUMENT_EXPORT KEduVocContainer
{
public:
    enum EnumContainerType{
        LessonContainer,
        LeitnerContainer,
        WordTypeContainer,
        WordTypeNounContainer,
        WordTypeNounMaleContainer,
        WordTypeNounFemaleContainer,
        WordTypeNounNeutralContainer,
        WordTypeVerbContainer,
        WordTypeAdjectiveContainer,
        WordTypeAdverbContainer
    };

    /** default constructor */
    explicit KEduVocContainer(const QString& name, EnumContainerType type = LessonContainer, KEduVocContainer *parent = 0);

    void appendChildContainer(KEduVocContainer *child);

    QList<KEduVocContainer *> childContainers();

    KEduVocContainer *childContainer(int row);

    /**
     * Find a child container
     * @param name 
     * @return 
     */
//     KEduVocContainer *childContainer(const QString& name);

    int childContainerCount() const;

    int row() const;
    KEduVocContainer *parent();

    /** copy constructor for d-pointer safe copying */
    KEduVocContainer( const KEduVocContainer &other );

    /** destructor */
    ~KEduVocContainer();

    /** assignment operator */
    KEduVocContainer& operator= ( const KEduVocContainer& );

    /** set the container name
     * @param name text to set for the name
     */
    void setName( const QString &name );

    /** get the container name */
    QString name();

    /** get a list of all entries in the container */
    virtual QList < KEduVocExpression* > entries() =0;

    /** get a list of all entries in the container and its child containers */
    QList < KEduVocExpression* > entriesRecursive();

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

private:
    class Private;
    Private * const d;
};

#endif
