/***************************************************************************
              manage lessons
    -----------------------------------------------------------------------

    begin        : August 11, 2007

    copyright    : (C) 2007 Jeremy Whiting <jeremywhiting@scitools.com>

    -----------------------------------------------------------------------
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

#include <QtCore/QList>
#include <QtCore/QString>

class KEduVocExpression;

/** class to store information about a lesson */
class KEDUVOCDOCUMENT_EXPORT KEduVocLesson
{

public:
     /** default constructor */
     explicit KEduVocLesson(const QString& name, KEduVocLesson *parent = 0);

     void appendChildLesson(KEduVocLesson *child);

     QList<KEduVocLesson *> childLessons();
     KEduVocLesson *childLesson(int row);
     int childLessonCount() const;

     int row() const;
     KEduVocLesson *parent();


    /** copy constructor for d-pointer safe copying */
    KEduVocLesson( const KEduVocLesson &other );

    /** destructor */
    ~KEduVocLesson();

    /** assignment operator */
    KEduVocLesson& operator= ( const KEduVocLesson& );

    /** set the lesson name
     * @param name text to set for the name
     */
    void setName( const QString &name );

    /** get the lesson name */
    QString name();

    KEduVocExpression* entry(int row);

    /** get a list of all entries in the lesson */
    QList < KEduVocExpression* > entries();

    /** get a list of all entries in the lesson and its child lessons */
    QList < KEduVocExpression* > entriesRecursive();

    /** get the number of entries in the lesson */
    int entryCount();

    /** add an entry to the lesson
     * @param entryid id of the entry to add
     */
    void addEntry(KEduVocExpression* entry);

    /** remove an entry from the lesson
     * @param entryid id of the entry to remove
     */
    void removeEntry(KEduVocExpression* entry);


    void removeTranslation(int translation);

    //void resetGrades()?

    bool inPractice();
    void setInPractice( bool inPractice );

    /** equality operator */
    bool operator==(const KEduVocLesson &other);

private:
    class Private;
    Private * const d;
};

#endif
