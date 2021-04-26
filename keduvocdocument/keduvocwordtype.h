/*
 * SPDX-FileCopyrightText: 2007 Jeremy Whiting <jpwhiting@kde.org>
 * SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KEDUVOCWORDTYPE_H
#define KEDUVOCWORDTYPE_H

#include "keduvocdocument_export.h"

#include "keduvoccontainer.h"

#include "keduvocwordflags.h"

#include <QList>
#include <QString>

class KEduVocExpression;
class KEduVocTranslation;

/** class to store translation word types */
class KEDUVOCDOCUMENT_EXPORT KEduVocWordType :public KEduVocContainer
{
public:


    /** default constructor */
    explicit KEduVocWordType(const QString& name, KEduVocWordType *parent = nullptr);

    /** destructor */
    ~KEduVocWordType();

    /** assignment operator */
//     KEduVocWordType& operator= ( const KEduVocWordType& );

    /**
     * Internally (different from the name) the class can have one of the preset word types. These are used to determine special properties (verbs have conjugations available for example).
     * @param flags
     */
    void setWordType(KEduVocWordFlags flags);

    /**
     * Return the raw WordTypeFlags. Returns NoInformation if no flags are set.
     * @return WordTypeFlags
     */
     KEduVocWordFlags wordType() const;

    /**
     * Return a child class (or this class) that is of the specified type. Returns 0 if no class of that type is found.
     * @param flags
     * @return
     */
    KEduVocWordType* childOfType(const KEduVocWordFlags& flags);

    /**
     * The word type class does keep track of individual translations, because for one entry, the translations can have different word types (eg. genders of nouns tend to be different in different languages).
     * @param row
     * @return
     */
    KEduVocTranslation * translation(int row);

    /**
     * get a list of all entries in the lesson
     * @param recursive include entries in sublessons
     * @return
     */
    QList < KEduVocExpression* > entries(EnumEntriesRecursive recursive = NotRecursive) Q_DECL_OVERRIDE;

    KEduVocExpression* entry(int row, EnumEntriesRecursive recursive = NotRecursive) Q_DECL_OVERRIDE;

    /** get the number of entries in the lesson */
    int entryCount(EnumEntriesRecursive recursive = NotRecursive) Q_DECL_OVERRIDE;

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
