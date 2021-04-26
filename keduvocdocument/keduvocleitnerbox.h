/*
 * SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KEDUVOCLEITNERBOX_H
#define KEDUVOCLEITNERBOX_H

#include "keduvocdocument_export.h"

#include "keduvoccontainer.h"

#include <QList>
#include <QString>

class KEduVocExpression;
class KEduVocTranslation;

/**
 * Leitner Boxes are an alternative grading system.
 * Classically flash cards are kept in boxes and moved corresponding to the users knowledge level.
 */
class KEDUVOCDOCUMENT_EXPORT KEduVocLeitnerBox :public KEduVocContainer
{
public:
    /** default constructor */
    explicit KEduVocLeitnerBox(const QString& name, KEduVocLeitnerBox *parent = nullptr);

    /** destructor */
    ~KEduVocLeitnerBox();

    /**
     * The leitner box class keeps track of individual translations, because for one entry, the translations can have different grades.
     * @param row 
     * @return 
     */
    KEduVocTranslation * translation(int row);

    /**
     * get a list of all entries in the box
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
