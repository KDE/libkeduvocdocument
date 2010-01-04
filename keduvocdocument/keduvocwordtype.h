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

#ifndef KEDUVOCWORDTYPE_H
#define KEDUVOCWORDTYPE_H

#include "libkeduvocdocument_export.h"

#include "keduvoccontainer.h"

#include "keduvocwordflags.h"

#include <QtCore/QList>
#include <QtCore/QString>

class KEduVocExpression;
class KEduVocTranslation;

/** class to store translation word types */
class KEDUVOCDOCUMENT_EXPORT KEduVocWordType :public KEduVocContainer
{
public:


    /** default constructor */
    explicit KEduVocWordType(const QString& name, KEduVocWordType *parent = 0);

    /** destructor */
    ~KEduVocWordType();

    /** assignment operator */
//     KEduVocWordType& operator= ( const KEduVocWordType& );

    /**
     * Internally (different from the name) the class can have one of the preset word types. These are used to determine special properties (verbs have conjugations available for example).
     * @param type
     */
    void setWordType(KEduVocWordFlags flags);

    /**
     * Return the raw WordTypeFlags. Returns NoInformation if no flags are set.
     * @return WordTypeFlags
     */
     KEduVocWordFlags wordType() const;

    /**
     * Return a child class (or this class) that is of the specified type. Returns 0 if no class of that type is found.
     * @param type
     * @return
     */
    KEduVocWordType* childOfType(const KEduVocWordFlags& flags);

    /**
     * The word type class does keep track of individual translations, because for one entry, the translations can have different word types (eg. genders of nouns tend to be different in different langues).
     * @param row
     * @return
     */
    KEduVocTranslation * translation(int row);

    /**
     * get a list of all entries in the lesson
     * @param recursive include entries in sublessons
     * @return
     */
    QList < KEduVocExpression* > entries(EnumEntriesRecursive recursive = NotRecursive);

    KEduVocExpression* entry(int row, EnumEntriesRecursive recursive = NotRecursive);

    /** get the number of entries in the lesson */
    int entryCount(EnumEntriesRecursive recursive = NotRecursive);

private:
    class Private;
    Private * const d;

    /** add an entry to the lesson
     * @param entryid id of the entry to add
     */
    void addTranslation(KEduVocTranslation* translation);

    /** remove an entry from the lesson
     * @param entryid id of the entry to remove
     */
    void removeTranslation(KEduVocTranslation* translation);

    friend class KEduVocTranslation;
};

#endif
