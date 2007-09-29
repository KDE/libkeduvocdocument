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

#include <QtCore/QStringList>

/**
 * The conjugation of a verb
 */
class KEDUVOCDOCUMENT_EXPORT KEduVocConjugation
{
public:

    enum ConjugationNumber {
        Singular,
        Dual,
        Plural,
        NumberMAX
    };

    // store third person neutral/common in the same sttr
    enum ConjugationPerson {
        First,
        Second,
        ThirdMale,
        ThirdFemale,
        ThirdNeutralCommon,
        PersonMAX
    };


    /**
     * The constructor
     */
    explicit KEduVocConjugation();

    KEduVocConjugation( const KEduVocConjugation& rhs );

    ~KEduVocConjugation();

    KEduVocConjugation& operator = ( const KEduVocConjugation& a );
    bool operator == ( const KEduVocConjugation& a ) const;

    QString conjugation(ConjugationPerson person, ConjugationNumber number) const;
    void setConjugation(const QString& conjugation, ConjugationPerson person, ConjugationNumber number);

private:
    class Private;
    Private* const d;

    int indexOf(ConjugationPerson person, ConjugationNumber number) const;
};



#endif // KEDUVOCCONJUGATION_H

