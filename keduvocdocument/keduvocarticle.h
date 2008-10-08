/***************************************************************************

    Copyright 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    Copyright 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
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

#ifndef KEDUVOCARTICLE_H
#define KEDUVOCARTICLE_H

#include "libkeduvocdocument_export.h"

#include <QtCore/QStringList>

#include "keduvocwordflags.h"

class KEduVocWordType;
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
    KDE_DEPRECATED KEduVocArticle( const QString &fem_def, const QString &fem_indef, const QString &mal_def, const QString &mal_indef, const QString &nat_def, const QString &nat_indef );

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
