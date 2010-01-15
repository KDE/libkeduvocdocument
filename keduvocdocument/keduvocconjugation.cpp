/***************************************************************************

    C++ Implementation: keduvocconjugation

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

#include "keduvocconjugation.h"
#include "keduvoccommon_p.h"
#include "keduvoctext.h"
#include "kvtml2defs.h"

#include <QtCore/QMap>
#include <QtXml/QDomDocument>

class KEduVocConjugation::Private
{
public:
    QMap<KEduVocWordFlags, KEduVocText> m_conjugations;
};


KEduVocConjugation::KEduVocConjugation()
        : d( new Private )
{}


KEduVocConjugation::KEduVocConjugation( const KEduVocConjugation& other )
        : d( new Private )
{
    d->m_conjugations = other.d->m_conjugations;
}


KEduVocConjugation::~KEduVocConjugation()
{
    delete d;
}

KEduVocConjugation& KEduVocConjugation::operator = ( const KEduVocConjugation& other )
{
    d->m_conjugations = other.d->m_conjugations;
    return *this;
}

bool KEduVocConjugation::operator ==(const KEduVocConjugation& other) const
{
    return d->m_conjugations == other.d->m_conjugations;
}



KEduVocText& KEduVocConjugation::conjugation(KEduVocWordFlags flags) const
{
        return d->m_conjugations[flags & (KEduVocWordFlag::persons | KEduVocWordFlag::numbers | KEduVocWordFlag::genders)];
}

void KEduVocConjugation::setConjugation(const KEduVocText& conjugation, KEduVocWordFlags flags)
{
    d->m_conjugations[flags & (KEduVocWordFlag::persons | KEduVocWordFlag::numbers | KEduVocWordFlag::genders)] = conjugation;
}

bool KEduVocConjugation::isEmpty()
{
    return d->m_conjugations.count() == 0;
}

QList< KEduVocWordFlags > KEduVocConjugation::keys()
{
    return d->m_conjugations.keys();
}



void KEduVocConjugation::toKVTML2(QDomElement & parent, const QString &tense)
{
    if (isEmpty()) {
        return;
    }

    QMap<int, KEduVocWordFlag::Flags> numbers;
    numbers[0] = KEduVocWordFlag::Singular;
    numbers[1] = KEduVocWordFlag::Dual;
    numbers[2] = KEduVocWordFlag::Plural;
    QMap<int, KEduVocWordFlag::Flags> persons;
    persons[0] = KEduVocWordFlag::First;
    persons[1] = KEduVocWordFlag::Second;
    persons[2] = (KEduVocWordFlag::Flags)((int)KEduVocWordFlag::Third | (int)KEduVocWordFlag::Masculine);
    persons[3] = (KEduVocWordFlag::Flags)((int)KEduVocWordFlag::Third | (int)KEduVocWordFlag::Feminine);
    persons[4] = (KEduVocWordFlag::Flags)((int)KEduVocWordFlag::Third | (int)KEduVocWordFlag::Neuter);

    // write the tense tag
    QDomDocument domDoc = parent.ownerDocument();
    QDomElement tenseElement = domDoc.createElement( KVTML_TENSE );
    tenseElement.appendChild( domDoc.createTextNode(tense) );
    parent.appendChild(tenseElement);

    for ( int num = 0; num <= 2; ++num) {
        QDomElement numberElement = domDoc.createElement( KVTML_GRAMMATICAL_NUMBER[num] );
        for ( int person = 0; person < 5; ++person) {
            if (!conjugation(numbers[num] | persons[person]).isEmpty()) {
                QDomElement personElement = domDoc.createElement( KVTML_GRAMMATICAL_PERSON[person] );
                numberElement.appendChild(personElement);
                conjugation(persons[person] | numbers[num]).toKVTML2(personElement);
            }
        }
        if (numberElement.hasChildNodes()) {
            parent.appendChild( numberElement );
        }
    }
}

/*

        QString first = conjugation.conjugation(
                KEduVocConjugation::First, num );
        QString second = conjugation.conjugation(
                KEduVocConjugation::Second, num );
        QString third_male = conjugation.conjugation(
                KEduVocConjugation::ThirdMale, num );
        QString third_female = conjugation.conjugation(
                KEduVocConjugation::ThirdFemale, num );
        QString third_neutral = conjugation.conjugation(
                KEduVocConjugation::ThirdNeutralCommon, num );

        if ( !first.isEmpty() || !second.isEmpty() || !third_female.isEmpty() ||
              !third_male.isEmpty() || !third_neutral.isEmpty() ) {
            QDomElement number;
            switch (num) {
                case KEduVocConjugation::Singular:
                    number = m_domDoc.createElement( KVTML_SINGULAR );
                    break;
                case KEduVocConjugation::Dual:
                    number = m_domDoc.createElement( KVTML_DUAL );
                    break;
                case KEduVocConjugation::Plural:
                    number = m_domDoc.createElement( KVTML_PLURAL );
                    break;
            }

            number.appendChild( newTextElement( KVTML_1STPERSON, first ) );
            number.appendChild( newTextElement( KVTML_2NDPERSON, second ) );
            number.appendChild( newTextElement( KVTML_THIRD_MALE, third_male ) );
            number.appendChild( newTextElement( KVTML_THIRD_FEMALE, third_female ) );
            number.appendChild( newTextElement( KVTML_THIRD_NEUTRAL_COMMON, third_neutral ) );

            conjugationElement.appendChild( number );
              }
    }*/


    /*
    for ( KEduVocWordFlag::DeclensionNumber num = KEduVocWordFlag::Singular; num <= KEduVocWordFlag::Plural; num = KEduVocWordFlag::DeclensionNumber(num +1) ) {
        QDomElement numberElement = domDoc.createElement( KVTML_GRAMMATICAL_NUMBER[num] );
        for ( KEduVocWordFlag::DeclensionCase dcase = KEduVocWordFlag::Nominative; dcase < KEduVocWordFlag::DeclensionCaseMAX; dcase = KEduVocWordFlag::DeclensionCase(dcase +1) ) {
            QDomElement caseElement = domDoc.createElement( KVTML_DECLENSION_CASE[dcase] );
            declension(num, dcase).toKVTML2(caseElement);

            if (caseElement.hasChildNodes()) {
                numberElement.appendChild(caseElement);
            }
        }
        if (numberElement.hasChildNodes()) {
            declensionElement.appendChild(numberElement);
        }
    }

    */



KEduVocConjugation* KEduVocConjugation::fromKVTML2(QDomElement & parent)
{
    // sanity check
    if (parent.isNull()) {
        return 0;
    }

    QMap<int, KEduVocWordFlag::Flags> numbers;
    numbers[0] = KEduVocWordFlag::Singular;
    numbers[1] = KEduVocWordFlag::Dual;
    numbers[2] = KEduVocWordFlag::Plural;
    QMap<int, KEduVocWordFlag::Flags> persons;
    persons[0] = KEduVocWordFlag::First;
    persons[1] = KEduVocWordFlag::Second;
    persons[2] = (KEduVocWordFlag::Flags)((int)KEduVocWordFlag::Third | (int)KEduVocWordFlag::Masculine);
    persons[3] = (KEduVocWordFlag::Flags)((int)KEduVocWordFlag::Third | (int)KEduVocWordFlag::Feminine);
    persons[4] = (KEduVocWordFlag::Flags)((int)KEduVocWordFlag::Third | (int)KEduVocWordFlag::Neuter);


    KEduVocConjugation* conjugation = new KEduVocConjugation;

    for ( int num = 0; num <= 2; num++ ) {
        QDomElement numberElement = parent.firstChildElement( KVTML_GRAMMATICAL_NUMBER[num] );

        if (numberElement.hasChildNodes()) {
            for (int person = 0; person < 5; person++) {
                QDomElement personElement = numberElement.firstChildElement( KVTML_GRAMMATICAL_PERSON[person] );
                if (!personElement.isNull()) {
                    KEduVocText text;
                    text.fromKVTML2(personElement);
                    if (text.text().isEmpty()) {
                        // compatibility for kde 4.0. There the text was directly below the person, not enabling grades per conjugation form.
                        text.setText(personElement.text());
                    }
                    conjugation->setConjugation(text, persons[person] | numbers[num]);
                }
            }
        }
    }
    return conjugation;
}


