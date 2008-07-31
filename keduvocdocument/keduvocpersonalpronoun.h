/***************************************************************************
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef KEDUVOCPERSONALPRONOUN_H
#define KEDUVOCPERSONALPRONOUN_H

#include "libkeduvocdocument_export.h"
#include "keduvocconjugation.h"
#include <QtCore/QStringList>

/**
 * The conjugation of a verb
 */
class KEDUVOCDOCUMENT_EXPORT KEduVocPersonalPronoun
{
public:

    /**
     * The constructor
     */
    explicit KEduVocPersonalPronoun();

    KEduVocPersonalPronoun( const KEduVocPersonalPronoun& rhs );

    ~KEduVocPersonalPronoun();

    KEduVocPersonalPronoun& operator = ( const KEduVocPersonalPronoun& a );
    bool operator == ( const KEduVocPersonalPronoun& a ) const;

    QString personalPronoun(KEduVocWordFlags flags) const;
    void setPersonalPronoun(const QString& conjugation, KEduVocWordFlags flags);

    bool maleFemaleDifferent() const;
    void setMaleFemaleDifferent(bool different);

    bool neutralExists() const;
    void setNeutralExists(bool exists);

    bool dualExists() const;
    void setDualExists(bool exists);
private:
    class Private;
    Private* const d;

};



#endif // KEDUVOCCONJUGATION_H

