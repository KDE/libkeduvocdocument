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

#ifndef KEDUVOCLESSON_H
#define KEDUVOCLESSON_H

#include "libkeduvocdocument_export.h"

#include "keduvoccontainer.h"

class KEduVocExpression;

/** class to store information about a lesson */
class KEDUVOCDOCUMENT_EXPORT KEduVocLesson :public KEduVocContainer
{
public:
    /** default constructor */
    explicit KEduVocLesson(const QString& name, KEduVocContainer *parent = 0);

//     void appendChildLesson(KEduVocLesson *child);

//     QList<KEduVocLesson *> childLessons();
//     KEduVocLesson *childLesson(int row);

//     int childLessonCount() const;

//     int row() const;
//     KEduVocLesson *parent();

    /** copy constructor for d-pointer safe copying */
    KEduVocLesson( const KEduVocLesson &other );

    /** destructor */
    ~KEduVocLesson();

    /** assignment operator */
    KEduVocLesson& operator= ( const KEduVocLesson& );

    KEduVocExpression* entry(int row, EnumEntriesRecursive recursive = NotRecursive);

    /** get a list of all entries in the lesson */
    QList < KEduVocExpression* > entries(EnumEntriesRecursive recursive = NotRecursive);

    /** get the number of entries in the lesson */
    int entryCount(EnumEntriesRecursive recursive = NotRecursive);

    /** append an entry to the lesson
     * @param entryid id of the entry to add
     */
    void appendEntry(KEduVocExpression* entry);

    /**
     * insert an entry at a specific position
     * @param index 
     * @param entry 
     */
    void insertEntry(int index, KEduVocExpression* entry);

    /** remove an entry from the lesson
     * @param entryid id of the entry to remove
     */
    void removeEntry(KEduVocExpression* entry);

private:
    class Private;
    Private * const d;
};

#endif
