/*
 * C++ Interface: keduvocdeclension
 * SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KEDUVOCDECLENSION_H
#define KEDUVOCDECLENSION_H

#include "keduvocdocument_export.h"

#include "keduvoctext.h"
#include "keduvocwordflags.h"

/**
A declension contains all forms that a NOUN possibly can have.

	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class KEDUVOCDOCUMENT_EXPORT KEduVocDeclension{
public:

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
     * @param flags
     * @return
     */
    KEduVocText& declension(KEduVocWordFlags flags);

    /**
     * Set a declension
     * @param declension
     * @param flags
     */
    void setDeclension(const KEduVocText& declension, KEduVocWordFlags flags);

    bool isEmpty();


    /**
     * Create xml for this declension
     * @param parent
     */
    void toKVTML2(QDomElement& parent);

    /**
     * Reads a declension from xml, returns 0 if it is empty
     * @param parent
     * @return
     */
    static KEduVocDeclension* fromKVTML2(QDomElement& parent);

private:
    class Private;
    Private * const d;
};

#endif
