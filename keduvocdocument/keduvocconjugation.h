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

///@todo get rid of this old strange prefix thing: used to mark personal pronouns.
#define CONJ_PREFIX            "--"   // definition of prefixes (I, you, ..)


class KEDUVOCDOCUMENT_EXPORT KEduVocTenseRelation
{
public:
    /** default constructor */
    KEduVocTenseRelation();
    KEduVocTenseRelation( const QString & _short, const QString & _long );

    /** default destructor, deletes the d-pointer */
    ~KEduVocTenseRelation();

    QString shortStr() const;
    QString longStr()  const;

    KEduVocTenseRelation &operator= ( const KEduVocTenseRelation &other );

private:
    class Private;
    Private * const d;
};


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

    int entryCount() const;

    QString getType( int index );
    void setType( int index, const QString & type );
    void cleanUp();
    bool isEmpty( int idx );

    QString pers1Singular( const QString &type ) const;
    QString pers2Singular( const QString &type ) const;
    bool    pers3SingularCommon( const QString &type ) const;
    QString pers3FemaleSingular( const QString &type ) const;
    QString pers3MaleSingular( const QString &type ) const;
    QString pers3NaturalSingular( const QString &type ) const;

    QString pers1Plural( const QString &type ) const;
    QString pers2Plural( const QString &type ) const;
    bool    pers3PluralCommon( const QString &type ) const;
    QString pers3FemalePlural( const QString &type ) const;
    QString pers3MalePlural( const QString &type ) const;
    QString pers3NaturalPlural( const QString &type ) const;

    void setPers1Singular( const QString &type, const QString &str );
    void setPers2Singular( const QString &type, const QString &str );
    void setPers3SingularCommon( const QString &type, bool f );
    void setPers3FemaleSingular( const QString &type, const QString &str );
    void setPers3MaleSingular( const QString &type, const QString &str );
    void setPers3NaturalSingular( const QString &type, const QString &str );

    void setPers1Plural( const QString &type, const QString &str );
    void setPers2Plural( const QString &type, const QString &str );
    void setPers3PluralCommon( const QString &type, bool f );
    void setPers3FemalePlural( const QString &type, const QString &str );
    void setPers3MalePlural( const QString &type, const QString &str );
    void setPers3NaturalPlural( const QString &type, const QString &str );

    KEduVocConjugation& operator = ( const KEduVocConjugation& a );
    bool operator == ( const KEduVocConjugation& a ) const;

private:
    class Private;
    Private* const d;
};



#endif // KEDUVOCCONJUGATION_H

