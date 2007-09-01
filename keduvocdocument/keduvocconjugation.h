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

    /**
     * The constructor
     */
    explicit KEduVocConjugation();

    KEduVocConjugation( const KEduVocConjugation& rhs );

    ~KEduVocConjugation();

    QString pers1Singular( ) const;
    QString pers2Singular( ) const;
    bool    pers3SingularCommon( ) const;
    QString pers3FemaleSingular( ) const;
    QString pers3MaleSingular( ) const;
    QString pers3NaturalSingular( ) const;

    QString pers1Plural( ) const;
    QString pers2Plural( ) const;
    bool    pers3PluralCommon( ) const;
    QString pers3FemalePlural( ) const;
    QString pers3MalePlural( ) const;
    QString pers3NaturalPlural( ) const;

    void setPers1Singular( const QString &str );
    void setPers2Singular( const QString &str );
    void setPers3SingularCommon( bool f );
    void setPers3FemaleSingular( const QString &str );
    void setPers3MaleSingular( const QString &str );
    void setPers3NaturalSingular( const QString &str );

    void setPers1Plural( const QString &str );
    void setPers2Plural( const QString &str );
    void setPers3PluralCommon( bool f );
    void setPers3FemalePlural( const QString &str );
    void setPers3MalePlural( const QString &str );
    void setPers3NaturalPlural( const QString &str );

    KEduVocConjugation& operator = ( const KEduVocConjugation& a );
    bool operator == ( const KEduVocConjugation& a ) const;

private:
    class Private;
    Private* const d;
};



#endif // KEDUVOCCONJUGATION_H

