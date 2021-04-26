/*
 * SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KEDUVOCPERSONALPRONOUN_H
#define KEDUVOCPERSONALPRONOUN_H

#include "keduvocdocument_export.h"
#include "keduvocconjugation.h"
#include <QStringList>

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

