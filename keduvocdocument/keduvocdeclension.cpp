/***************************************************************************

    C++ Implementation: keduvocdeclension

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

#include "keduvocdeclension.h"

#include "kvtml2defs.h"
#include "keduvockvtml2writer.h"
#include <QtXml/QDomDocument>

#include <QtCore/QMap>

class KEduVocDeclension::Private
{
public:
    QMap<KEduVocWordFlags, KEduVocText> m_declensions;
};

KEduVocDeclension::KEduVocDeclension()
    :d (new Private)
{
}

KEduVocDeclension::KEduVocDeclension(const KEduVocDeclension & other)
    :d (new Private)
{
    d->m_declensions = other.d->m_declensions;
}

KEduVocDeclension & KEduVocDeclension::operator =(const KEduVocDeclension & other)
{
    d->m_declensions = other.d->m_declensions;
    return *this;
}

KEduVocDeclension::~KEduVocDeclension()
{
    delete d;
}

KEduVocText& KEduVocDeclension::declension(KEduVocWordFlags flags)
{
    return d->m_declensions[flags];
}

void KEduVocDeclension::setDeclension(const KEduVocText & declension, KEduVocWordFlags flags)
{
    d->m_declensions[flags] = declension;
}

bool KEduVocDeclension::isEmpty()
{
    return d->m_declensions.isEmpty();
}

void KEduVocDeclension::toKVTML2(QDomElement & parent)
{
    if (isEmpty()) {
        return;
    }
    QDomDocument domDoc = parent.ownerDocument();
    QDomElement declensionElement = domDoc.createElement( KVTML_DECLENSION );

    QMap<int, KEduVocWordFlags> genders;
    genders[0] = KEduVocWordFlag::Masculine;
    genders[1] = KEduVocWordFlag::Feminine;
    genders[2] = KEduVocWordFlag::Neuter;

    QMap<int, KEduVocWordFlags> numbers;
    numbers[0] = KEduVocWordFlag::Singular;
    numbers[1] = KEduVocWordFlag::Dual;
    numbers[2] = KEduVocWordFlag::Plural;

    QMap<int, KEduVocWordFlags> cases;
    cases[0] = KEduVocWordFlag::Nominative;
    cases[1] = KEduVocWordFlag::Genitive;
    cases[2] = KEduVocWordFlag::Dative;
    cases[3] = KEduVocWordFlag::Accusative;
    cases[4] = KEduVocWordFlag::Ablative;
    cases[5] = KEduVocWordFlag::Locative;
    cases[6] = KEduVocWordFlag::Vocative;


    for ( int gen = 0; gen <= 2; ++gen)
    {
        QDomElement genderElement = domDoc.createElement( KVTML_GRAMMATICAL_GENDER[gen] );
        for ( int num = 0; num <= 2; ++num) {
            QDomElement numberElement = domDoc.createElement( KVTML_GRAMMATICAL_NUMBER[num] );
            for ( int dcase = 0; dcase <= 6; ++dcase ) {
                QDomElement caseElement = domDoc.createElement( KVTML_DECLENSION_CASE[dcase] );
                declension(genders[gen] | numbers[num] | cases[dcase]).toKVTML2(caseElement);

                if (caseElement.hasChildNodes()) {
                    numberElement.appendChild(caseElement);
                }
            }
            if (numberElement.hasChildNodes()) {
                genderElement.appendChild(numberElement);
            }
        }
        if (genderElement.hasChildNodes()) {
            declensionElement.appendChild(genderElement);
        }
    }
    if (declensionElement.hasChildNodes()) {
        parent.appendChild(declensionElement);
    }
}

KEduVocDeclension* KEduVocDeclension::fromKVTML2(QDomElement & parent)
{
    QDomElement declensionElement = parent.firstChildElement( KVTML_DECLENSION );
    // we don't create empty objects, if necessary, create later on demand.
    if (declensionElement.isNull()) {
        return 0;
    }


    QMap<int, KEduVocWordFlags> genders;
    genders[0] = KEduVocWordFlag::Masculine;
    genders[1] = KEduVocWordFlag::Feminine;
    genders[2] = KEduVocWordFlag::Neuter;

    QMap<int, KEduVocWordFlags> numbers;
    numbers[0] = KEduVocWordFlag::Singular;
    numbers[1] = KEduVocWordFlag::Dual;
    numbers[2] = KEduVocWordFlag::Plural;

    QMap<int, KEduVocWordFlags> cases;
    cases[0] = KEduVocWordFlag::Nominative;
    cases[1] = KEduVocWordFlag::Genitive;
    cases[2] = KEduVocWordFlag::Dative;
    cases[3] = KEduVocWordFlag::Accusative;
    cases[4] = KEduVocWordFlag::Ablative;
    cases[5] = KEduVocWordFlag::Locative;
    cases[6] = KEduVocWordFlag::Vocative;


    KEduVocDeclension* declension = new KEduVocDeclension;

    for ( int gen = 0; gen <= 2; ++gen ) {
        QDomElement genderElement = declensionElement.firstChildElement( KVTML_GRAMMATICAL_GENDER[gen] );
        if (!genderElement.isNull())
        {
            for ( int num = 0; num <= 2; ++num ) {
                QDomElement numberElement = genderElement.firstChildElement( KVTML_GRAMMATICAL_NUMBER[num] );
                if (!numberElement.isNull()) {
                    for ( int dcase = 0; dcase <= 6; ++dcase) {
                        QDomElement caseElement = numberElement.firstChildElement( KVTML_DECLENSION_CASE[dcase] );
                        if (!caseElement.isNull()) {
                            KEduVocText text;
                            text.fromKVTML2(caseElement);
                            declension->setDeclension(text, genders[gen] | numbers[num] | cases[dcase]);
                        }
                    }
                }
            }
        }
    }
    return declension;
}



