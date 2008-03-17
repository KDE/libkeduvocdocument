/***************************************************************************
    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCTEXT_H
#define KEDUVOCTEXT_H

#include "libkeduvocdocument_export.h"
#include <QtCore/QDateTime>


#define KV_MAX_GRADE       7
#define KV_MIN_GRADE       0

#define KV_NORM_GRADE      0       // not practiced yet
#define KV_NORM_TEXT       I18N_NOOP("Not Practiced Yet")

#define KV_LEV1_GRADE      1
#define KV_LEV1_TEXT       I18N_NOOP("Level 1")

#define KV_LEV2_GRADE      2
#define KV_LEV2_TEXT       I18N_NOOP("Level 2")

#define KV_LEV3_GRADE      3
#define KV_LEV3_TEXT       I18N_NOOP("Level 3")

#define KV_LEV4_GRADE      4
#define KV_LEV4_TEXT       I18N_NOOP("Level 4")

#define KV_LEV5_GRADE      5
#define KV_LEV5_TEXT       I18N_NOOP("Level 5")

#define KV_LEV6_GRADE      6
#define KV_LEV6_TEXT       I18N_NOOP("Level 6")

#define KV_LEV7_GRADE      7
#define KV_LEV7_TEXT       I18N_NOOP("Level 7")


typedef unsigned short grade_t;
typedef unsigned short count_t;

class QDomElement;

/**
 * A text in vocabulary documents. Associated with it are grade and date information.
 * This should be used instead of strings for all things that can be tested and thus get a grade.
 @author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class KEDUVOCDOCUMENT_EXPORT KEduVocText
{
public:
    /** default constructor */
    KEduVocText(const QString& text = QString());

    /** copy constructor
     * provides safe copy of d pointer
     * @param other object to copy from
     */
    KEduVocText( const KEduVocText &other );

    /** default destructor */
    ~KEduVocText();

    /**
     * The translation as string (the word itself)
     * @return the translation
     */
    QString text() const;

    /**
     * Sets the translation
     * @param expr
     */
    void setText( const QString & expr );

    /**
     * Equal operator to copy grades.
     * @param other grades copied
     * @return reference to the new grades
     */
    KEduVocText& operator= ( const KEduVocText &other );
    /**
     * Compare two sets of grades.
     * @param other
     * @return true if equal
     */
    bool operator== ( const KEduVocText &other ) const;


    /** returns how often this entry has been practiced as int
    * @returns total count
    */
    count_t practiceCount() const;

    /** set how often this entry has been practiced as int
    * @param count the new count
    */
    void setPracticeCount( count_t count );

    /** returns bad query count as int
    * @returns bad query count
    */
    count_t badCount() const;

    /** set bad query count as int
    * @param count the new count
    */
    void setBadCount( count_t count );

    /** increment bad query count of given translation by 1 */
    void incBadCount();

    /** increment query count of given translation by 1 */
    void incPracticeCount();

    /**
     * Clears grading and date information.
     */
    void resetGrades();

    /** sets the grade
    * @param grade number of knowlegde: 0=known, x=numbers not knows
    */
    void setGrade( grade_t grade );

    /** returns grade as int
    * @returns number of knowlegde: 0=known, x=numbers not knows
    */
    grade_t grade() const;

    /** increments grade */
    void incGrade();

    /** decrements grade */
    void decGrade();

    /** returns last practice date as int
    */
    QDateTime practiceDate() const;

    /** Set last query date
    * @param date             the new date
    */
    void setPracticeDate( const QDateTime & date );

    /**
     * If the string inside is empty this returns true.
     * @return 
     */
    bool isEmpty();

    void fromKVTML2(QDomElement& parent);
    void toKVTML2(QDomElement& parent);

private:
    class KEduVocTextPrivate;
    KEduVocTextPrivate * const d;
};

#endif
