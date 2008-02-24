/***************************************************************************

    C++ Interface: keduvocdeclension

    -----------------------------------------------------------------------

    begin         : Do Sep 20 2007

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
#ifndef KEDUVOCDECLENSION_H
#define KEDUVOCDECLENSION_H

#include "libkeduvocdocument_export.h"

#include "keduvoctext.h"

/**
A declension contains all forms that a NOUN possibly can have.

	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class KEDUVOCDOCUMENT_EXPORT KEduVocDeclension{
public:
    enum DeclensionNumber {
        Singular = 0,
        Dual,
        Plural
    };

    enum DeclensionCase {
        Nominative = 0,
        Genitive,
        Dative,
        Accusative,
        Ablative,
        Locative,
        Vocative,
        DeclensionCaseMAX
    };

    /**
     * The constructor without arguments
     */
    explicit KEduVocDeclension();

    /** copy constructor
     * @param other comparison object to copy
     */
    KEduVocDeclension( const KEduVocDeclension &other );

    ~KEduVocDeclension();

    /** equality operator
     * @param a object to compare to
     * @returns true if comparisons are the same, false otherwise
     */
//     will probably not be necessary
//     bool operator == ( const KEduVocDeclension& a ) const;

    /** assignment operator for d-pointer copying
     * @param other object to copy from
     * @returns reference to this object
     */
    KEduVocDeclension& operator= ( const KEduVocDeclension& other );

    /**
     * The grammatical number, there is singular and plural for english, some languages have dual for exactly two items.
     * @param gender
     * @param decCase
     * @return
     */
    KEduVocText& declension(DeclensionNumber number, DeclensionCase decCase);
    KEduVocText& declension(int index);

    /**
     * Set a declension
     * @param
     * @param number
     * @param decCase
     */
    void setDeclension(const KEduVocText& declension, DeclensionNumber number, DeclensionCase decCase);

    void setDeclension(const KEduVocText& declension, int index);

    bool isEmpty();

    void fromKVTML2(QDomElement& parent);
    void toKVTML2(QDomElement& parent);

    static int indexOf(DeclensionNumber number, DeclensionCase decCase);

private:
    class Private;
    Private * const d;
};

#endif
