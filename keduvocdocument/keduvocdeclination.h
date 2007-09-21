/***************************************************************************

    C++ Interface: keduvocdeclination

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
#ifndef KEDUVOCDECLINATION_H
#define KEDUVOCDECLINATION_H

/**
A declination contains all forms that a NOUN possibly can have.

	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class KEduVocDeclination{
public:
    enum DeclinationNumber {
        Singular = 1,
        Dual,
        Plural
    };
/*
    enum DeclinationGender {
        Undefined,
        Masculine,
        Feminine,
        Neuter
    };*/

    enum DeclinationCase {
        Nominative = 1,
        Genitive,
        Dative,
        Accusative,
        Ablative,
        Locative,
        Vocative
    };


    /**
     * The constructor without arguments
     */
    explicit KEduVocDeclination();

    /** copy constructor
     * @param other comparison object to copy
     */
    KEduVocDeclination( const KEduVocDeclination &other );

    ~KEduVocDeclination();

    /** equality operator
     * @param a object to compare to
     * @returns true if comparisons are the same, false otherwise
     */
//     will probably not be necessary
//     bool operator == ( const KEduVocDeclination& a ) const;

    /** assignment operator for d-pointer copying
     * @param other object to copy from
     * @returns reference to this object
     */
    KEduVocDeclination& operator= ( const KEduVocDeclination& other );

    /**
     * The grammatical number, there is singular and plural for english, some languages have dual for exactly two items.
     * @param gender
     * @param decCase
     * @return
     */
    QString declination(DeclinationNumber number, DeclinationCase decCase);

    /**
     * Set a declination
     * @param
     * @param number
     * @param decCase
     */
    void setDeclination(const QString& declination, DeclinationNumber number, DeclinationCase decCase);

    bool isEmpty();

private:
    int indexOf(DeclinationNumber number, DeclinationCase decCase);

    class Private;
    Private * const d;
};

#endif
