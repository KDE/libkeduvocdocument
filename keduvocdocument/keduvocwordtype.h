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

#ifndef KEDUVOCWORDTYPE_H
#define KEDUVOCWORDTYPE_H

#include "libkeduvocdocument_export.h"

#include "keduvoccontainer.h"

#include <QtCore/QList>
#include <QtCore/QString>

class KEduVocExpression;
class KEduVocTranslation;

/** class to store translation word types */
class KEDUVOCDOCUMENT_EXPORT KEduVocWordType :public KEduVocContainer
{
public:

    enum EnumWordType {
        General,
        Noun,
        NounMale,
        NounFemale,
        NounNeutral,
        Verb,
        Adjective,
        Adverb
    };


    /** default constructor */
    explicit KEduVocWordType(const QString& name, KEduVocWordType *parent = 0);

    /** copy constructor for d-pointer safe copying */
    KEduVocWordType( const KEduVocWordType &other );

    /** destructor */
    ~KEduVocWordType();

    /** assignment operator */
    KEduVocWordType& operator= ( const KEduVocWordType& );

    void setWordType(EnumWordType type);
    KEduVocWordType::EnumWordType wordType() const;


    KEduVocTranslation * translation(int row);

    /** get a list of all entries in the lesson */
    QList < KEduVocExpression* > entries();

    KEduVocExpression* entry(int row);

    /** get the number of entries in the lesson */
    int entryCount();

//     KEduVocWordType *parent();

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
