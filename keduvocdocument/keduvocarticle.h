/*
 * SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
 * SPDX-FileCopyrightText: 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-FileCopyrightText: 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>                                                                    *
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KEDUVOCARTICLE_H
#define KEDUVOCARTICLE_H

#include "keduvocdocument_export.h"

#include <QStringList>

#include "keduvocwordflags.h"

/**
 * Class representing the articles of a language
 *
 * It contains all available articles of the language as QString
 */
class KEDUVOCDOCUMENT_EXPORT KEduVocArticle
{
public:



    /**
     * The constructor without arguments
     */
    explicit KEduVocArticle();

    /** copy constructor for d-pointer safety
     * @param other article object to copy
     */
    KEduVocArticle( const KEduVocArticle &other );

    /**
     * DEPRECATED The constructor with arguments, assumes singular!
     * @param fem_def reference to a QString with the definite female article
     * @param fem_indef reference to a QString with the indefinite female article
     * @param mal_def reference to a QString with the definite male article
     * @param mal_indef reference to a QString with the indefinite male article
     * @param nat_def reference to a QString with the definite neutral article
     * @param nat_indef reference to a QString with the indefinite neutral article
     */
    KEDUVOCDOCUMENT_DEPRECATED KEduVocArticle( const QString &fem_def, const QString &fem_indef, const QString &mal_def, const QString &mal_indef, const QString &nat_def, const QString &nat_indef );

    /**
     * default destructor, deletes the d pointer
     */
    ~KEduVocArticle();

    /**
     * assignment operator for d-pointer copying
     */
    KEduVocArticle &operator= ( const KEduVocArticle& other );

    QString article(const KEduVocWordFlags&);

    void setArticle(const QString& article, const KEduVocWordFlags&);

    bool isArticle(const QString& article) const;

    bool isEmpty();

private:
    class Private;
    Private * const d;
};


#endif
