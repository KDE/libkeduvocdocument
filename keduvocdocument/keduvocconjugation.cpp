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
#include "kvtml2defs.h"

#include <KLocalizedString>
#include <KDebug>
#include <QtXml/QDomDocument>

class KEduVocConjugation::Private
{
public:
    QMap<int, KEduVocText> m_conjugations;
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


KEduVocText& KEduVocConjugation::conjugation(int index)
{
    return d->m_conjugations[index];
}


KEduVocText& KEduVocConjugation::conjugation(ConjugationPerson person, ConjugationNumber number)
{
    return conjugation(indexOf(person, number));
}

void KEduVocConjugation::setConjugation(const KEduVocText& conjugation, ConjugationPerson person, ConjugationNumber number)
{
    setConjugation(conjugation, indexOf(person, number));
}


void KEduVocConjugation::setConjugation(const KEduVocText& conjugation, int index)
{
    d->m_conjugations[index] = conjugation;
}


int KEduVocConjugation::indexOf(ConjugationPerson person, ConjugationNumber number)
{
    return person + (ThirdNeutralCommon+1) * number;
}


bool KEduVocConjugation::isEmpty()
{
    return d->m_conjugations.count() == 0;
}

QList< int > KEduVocConjugation::keys()
{
    return d->m_conjugations.keys();
}



void KEduVocConjugation::toKVTML2(QDomElement & parent, const QString &tense)
{
    if (isEmpty()) {
        return;
    }

    // write the tense tag
    QDomDocument domDoc = parent.ownerDocument();
    QDomElement tenseElement = domDoc.createElement( KVTML_TENSE );
    tenseElement.appendChild( domDoc.createTextNode(tense) );
    parent.appendChild(tenseElement);

    for ( KEduVocConjugation::ConjugationNumber num = KEduVocConjugation::Singular; num <= KEduVocConjugation::Plural; num = KEduVocConjugation::ConjugationNumber(num +1) ) {

        QDomElement numberElement = domDoc.createElement( KVTML_GRAMMATICAL_NUMBER[num] );
        for ( KEduVocConjugation::ConjugationPerson person = KEduVocConjugation::First; person <= KEduVocConjugation::ThirdNeutralCommon; person = KEduVocConjugation::ConjugationPerson(person +1) ) {

            if (!conjugation(indexOf(person, num)).isEmpty()) {
                QDomElement personElement = domDoc.createElement( KVTML_GRAMMATICAL_PERSON[person] );
                numberElement.appendChild(personElement);
                conjugation(indexOf(person, num)).toKVTML2(personElement);
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
    for ( KEduVocDeclension::DeclensionNumber num = KEduVocDeclension::Singular; num <= KEduVocDeclension::Plural; num = KEduVocDeclension::DeclensionNumber(num +1) ) {
        QDomElement numberElement = domDoc.createElement( KVTML_GRAMMATICAL_NUMBER[num] );
        for ( KEduVocDeclension::DeclensionCase dcase = KEduVocDeclension::Nominative; dcase < KEduVocDeclension::DeclensionCaseMAX; dcase = KEduVocDeclension::DeclensionCase(dcase +1) ) {
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

    KEduVocConjugation* conjugation = new KEduVocConjugation;

    for ( int num = KEduVocConjugation::Singular; num <= KEduVocConjugation::Plural; num++ ) {
        QDomElement numberElement = parent.firstChildElement( KVTML_GRAMMATICAL_NUMBER[num] );

        if (numberElement.hasChildNodes()) {
            for (int person = KEduVocConjugation::First; person <= KEduVocConjugation::ThirdNeutralCommon; person++) {
                QDomElement personElement = numberElement.firstChildElement( KVTML_GRAMMATICAL_PERSON[person] );
                if (!personElement.isNull()) {
                    KEduVocText text;
                    text.fromKVTML2(personElement);
                    if (text.text().isEmpty()) {
                        // compatibility for kde 4.0. There the text was directly below the person, not enabling grades per conjugation form.
                        text.setText(personElement.text());
                    }
                    conjugation->setConjugation(text, ConjugationPerson(person), KEduVocConjugation::ConjugationNumber(num));
                }
            }
        }
    }
    return conjugation;
}


