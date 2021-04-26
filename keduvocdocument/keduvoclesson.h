/*
 * SPDX-FileCopyrightText: 2007 Jeremy Whiting <jpwhiting@kde.org>
 * SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KEDUVOCLESSON_H
#define KEDUVOCLESSON_H

#include "keduvocdocument_export.h"

#include "keduvoccontainer.h"

class KEduVocExpression;

/** class to store information about a lesson */
class KEDUVOCDOCUMENT_EXPORT KEduVocLesson :public KEduVocContainer
{
public:
    /** default constructor */
    explicit KEduVocLesson(const QString& name, KEduVocContainer *parent = nullptr);

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

    KEduVocExpression* entry(int row, EnumEntriesRecursive recursive = NotRecursive) Q_DECL_OVERRIDE;

    /** get a list of all entries in the lesson */
    QList < KEduVocExpression* > entries(EnumEntriesRecursive recursive = NotRecursive) Q_DECL_OVERRIDE;

    /** get the number of entries in the lesson */
    int entryCount(EnumEntriesRecursive recursive = NotRecursive) Q_DECL_OVERRIDE;

    /** append an entry to the lesson
     * @param entry the id of the entry to add
     */
    void appendEntry(KEduVocExpression* entry);

    /**
     * insert an entry at a specific position
     * @param index 
     * @param entry 
     */
    void insertEntry(int index, KEduVocExpression* entry);

    /** remove an entry from the lesson
     * @param entry the id of the entry to remove
     */
    void removeEntry(KEduVocExpression* entry);

private:
    friend class KEduVocDocument;
    // This constructor is used by KEduVocDocument when creating the top level lesson.
    explicit KEduVocLesson(const QString& name, KEduVocDocument *document);

private:
    class Private;
    Private * const d;
};

#endif
