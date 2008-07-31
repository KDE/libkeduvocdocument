/***************************************************************************

    C++ Interface: keduvocconjugation

    -----------------------------------------------------------------------

    begin         : Di Aug 28 2007

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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


#ifndef KEDUVOCCONJUGATION_H
#define KEDUVOCCONJUGATION_H

#include "libkeduvocdocument_export.h"
#include "keduvoctext.h"
#include "keduvocwordflags.h"

#include <QtCore/QStringList>

/**
 * The conjugation of a verb
 */
class KEDUVOCDOCUMENT_EXPORT KEduVocConjugation
{
public:

    /**
     * The constructor
     */
    explicit KEduVocConjugation();

    KEduVocConjugation( const KEduVocConjugation& rhs );

    ~KEduVocConjugation();

    KEduVocConjugation& operator = ( const KEduVocConjugation& a );
    bool operator == ( const KEduVocConjugation& a ) const;

    KEduVocText& conjugation(KEduVocWordFlags flags) const;
    void setConjugation(const KEduVocText& conjugation, KEduVocWordFlags flags);

    QList<KEduVocWordFlags> keys();

    bool isEmpty();

    /**
     * Create xml for this declension
     * @param parent
     */
    void toKVTML2(QDomElement& parent, const QString &tense);

    /**
     * Reads a declension from xml, returns 0 if it is empty
     * @param parent
     * @return
     */
    static KEduVocConjugation* fromKVTML2(QDomElement& parent);

private:
    class Private;
    Private* const d;
};



#endif // KEDUVOCCONJUGATION_H

