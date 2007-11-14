/***************************************************************************

    C++ Implementation: keduvocwordtype

    -----------------------------------------------------------------------

    begin         : Mi Aug 22 2007

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

#include "keduvocwordtype.h"

#include <KLocale>
#include <kdebug.h>

class KEduVocWordType::Private
{
public:
    struct subWordType
    {
        QString m_subTypeName;
        QString m_specialType;
        bool operator== ( const subWordType& other );
    };
    struct wordType
    {
        QString m_typeName;
        QString m_specialType;
        QList<subWordType> m_subWordTypeList;
        bool operator== ( const wordType& other );
    };

    /// Map containing the word type name and its properties.
    QList<wordType> m_wordTypeList;

    static const QString WORDTYPE_SPECIAL_NOUN;
    static const QString WORDTYPE_SPECIAL_NOUN_MALE;
    static const QString WORDTYPE_SPECIAL_NOUN_FEMALE;
    static const QString WORDTYPE_SPECIAL_NOUN_NEUTRAL;

    static const QString WORDTYPE_SPECIAL_VERB;
    static const QString WORDTYPE_SPECIAL_VERB_REGULAR;
    static const QString WORDTYPE_SPECIAL_VERB_IRREGULAR;
    static const QString WORDTYPE_SPECIAL_ADJECTIVE;
    static const QString WORDTYPE_SPECIAL_ADVERB;
};

const QString KEduVocWordType::Private::WORDTYPE_SPECIAL_NOUN =
    QString( i18nc( "@item:inlistbox The grammatical type of a word", "Noun" ) );
const QString KEduVocWordType::Private::WORDTYPE_SPECIAL_NOUN_MALE =
    QString( i18nc( "@item:inlistbox A subtype of the grammatical word type: Noun", "Male" ) );
const QString KEduVocWordType::Private::WORDTYPE_SPECIAL_NOUN_FEMALE =
    QString( i18nc( "@item:inlistbox A subtype of the grammatical word type: Noun", "Female" ) );
const QString KEduVocWordType::Private::WORDTYPE_SPECIAL_NOUN_NEUTRAL =
    QString( i18nc( "@item:inlistbox A subtype of the grammatical word type: Noun", "Neutral" ) );

const QString KEduVocWordType::Private::WORDTYPE_SPECIAL_VERB =
    QString( i18nc( "@item:inlistbox The grammatical type of a word", "Verb" ) );
const QString KEduVocWordType::Private::WORDTYPE_SPECIAL_ADJECTIVE =
    QString( i18nc( "@item:inlistbox The grammatical type of a word", "Adjective" ) );
const QString KEduVocWordType::Private::WORDTYPE_SPECIAL_ADVERB =
    QString( i18nc( "@item:inlistbox The grammatical type of a word", "Adverb" ) );


bool KEduVocWordType::Private::wordType::operator ==(const wordType & other)
{
    return m_typeName == other.m_typeName &&
        m_specialType == other.m_specialType &&
        m_subWordTypeList == other.m_subWordTypeList;
}

bool KEduVocWordType::Private::subWordType::operator ==(const subWordType & other)
{
    return m_subTypeName == other.m_subTypeName &&
        m_specialType == other.m_specialType;
}

bool KEduVocWordType::operator ==(const KEduVocWordType & other)
{
    return d->m_wordTypeList == other.d->m_wordTypeList;
}

KEduVocWordType::KEduVocWordType()
        : d( new Private )
{}

KEduVocWordType::~KEduVocWordType()
{
    delete d;
}

KEduVocWordType & KEduVocWordType::operator = ( const KEduVocWordType & other )
{
    d->m_wordTypeList = other.d->m_wordTypeList;
    return *this;
}

KEduVocWordType::KEduVocWordType( const KEduVocWordType & other )
        : d( new Private )
{
    d->m_wordTypeList = other.d->m_wordTypeList;
}


void KEduVocWordType::createDefaultWordTypes()
{
    // first the special types
    addType(  d->WORDTYPE_SPECIAL_NOUN, d->WORDTYPE_SPECIAL_NOUN );

    addSubType( d->WORDTYPE_SPECIAL_NOUN,
        d->WORDTYPE_SPECIAL_NOUN_MALE, d->WORDTYPE_SPECIAL_NOUN_MALE );

    addSubType( d->WORDTYPE_SPECIAL_NOUN,
        d->WORDTYPE_SPECIAL_NOUN_FEMALE, d->WORDTYPE_SPECIAL_NOUN_FEMALE );

    addSubType( d->WORDTYPE_SPECIAL_NOUN,
        d->WORDTYPE_SPECIAL_NOUN_NEUTRAL, d->WORDTYPE_SPECIAL_NOUN_NEUTRAL );

    addType( d->WORDTYPE_SPECIAL_VERB, d->WORDTYPE_SPECIAL_VERB );

    addSubType( d->WORDTYPE_SPECIAL_VERB,
        i18nc( "@item:inlistbox A subtype of the grammatical word type: Verb with regular conjugation","Regular" ) );

    addSubType( d->WORDTYPE_SPECIAL_VERB,
        i18nc( "@item:inlistbox A subtype of the grammatical word type: Verb with irregular conjugation","Irregular" ) );

    addType( d->WORDTYPE_SPECIAL_ADJECTIVE, d->WORDTYPE_SPECIAL_ADJECTIVE );
    addType( d->WORDTYPE_SPECIAL_ADVERB, d->WORDTYPE_SPECIAL_ADVERB );

    addType( i18nc( "@item:inlistbox The grammatical type of an entry", "Question" ) );
    addType( i18nc( "@item:inlistbox The grammatical type of a word", "Name" ) );
    addType( i18nc( "@item:inlistbox The grammatical type of a word", "Article" ) );
    addSubType( i18nc( "@item:inlistbox The grammatical type of a word", "Article" ),
        i18nc( "@item:inlistbox A subtype of the grammatical word type: Article (the)","Definite" ) );
    addSubType( i18nc( "@item:inlistbox The grammatical type of a word", "Article" ),
        i18nc( "@item:inlistbox A subtype of the grammatical word type: Article (a)","Indefinite" ) );

    addType( i18nc( "@item:inlistbox The grammatical type of a word", "Pronoun" ) );
    addSubType( i18nc( "@item:inlistbox The grammatical type of a word", "Pronoun" ),
         i18nc( "@item:inlistbox A subtype of the grammatical word type: Pronoun (my, your, his, her...)", "Possessive" ) );
    addSubType( i18nc( "@item:inlistbox The grammatical type of a word", "Pronoun" ),
        i18nc( "@item:inlistbox A subtype of the grammatical word type: Pronoun (I, you, he...)", "Personal" ) );

    addType( i18nc( "@item:inlistbox The grammatical type of an entry", "Phrase" ) );
    addType( i18nc( "@item:inlistbox The grammatical type of a word", "Numeral" ) );
    addSubType( i18nc( "@item:inlistbox The grammatical type of a word", "Numeral" ),
        i18nc( "@item:inlistbox A subtype of the grammatical word type: Numeral Ordinal  (one, two, three, ...)","Ordinal" ) );
    addSubType( i18nc( "@item:inlistbox The grammatical type of a word", "Numeral" ),
        i18nc( "@item:inlistbox A subtype of the grammatical word type: Numeral Cardinal (first, second, third, ...)","Cardinal" ) );

    addType( i18nc( "@item:inlistbox The grammatical type of a word", "Conjunction" ) );
    addType( i18nc( "@item:inlistbox The grammatical type of a word", "Preposition" ) );
}

QString KEduVocWordType::mainTypeName( int index ) const
{
    return d->m_wordTypeList[index].m_typeName;
}

int KEduVocWordType::mainTypeIndex( const QString& name ) const
{
    for ( int i=0; i < d->m_wordTypeList.count(); i++ ) {
        if ( d->m_wordTypeList.value( i ).m_typeName == name ) {
            return i;
        }
    }
    return -1;
}

QStringList KEduVocWordType::typeNameList() const
{
    QStringList list;
    foreach( Private::wordType wt, d->m_wordTypeList ) {
        list.append( wt.m_typeName );
    }
    return list;
}

QStringList KEduVocWordType::subTypeNameList( const QString & mainType ) const
{
    int mainIndex = mainTypeIndex( mainType );
    kDebug() << "Get subtypes for " << mainType << " = " << mainIndex;
    QStringList list;
    foreach( Private::subWordType wt, d->m_wordTypeList.value( mainIndex ).m_subWordTypeList ) {
        list.append( wt.m_subTypeName );
    }
    return list;
}


void KEduVocWordType::addType( const QString & typeName, const QString & specialType )
{
    if ( typeName.isEmpty() ) {
        kDebug() << "Attempting to add empty type. When opening old kvtml documents this is ok.";
        return;
    }
    if ( typeNameList().contains( typeName ) ) {
        kDebug() << "Attempting to add type \"" << typeName << "\" twice.";
        return;
    }
    d->m_wordTypeList.append( Private::wordType() );
    d->m_wordTypeList[d->m_wordTypeList.count()-1].m_typeName = typeName;
    d->m_wordTypeList[d->m_wordTypeList.count()-1].m_specialType = specialType;
}

void KEduVocWordType::addSubType( const QString & mainType, const QString & typeName, const QString & specialType )
{
    int mt = mainTypeIndex( mainType );

    d->m_wordTypeList[mt].m_subWordTypeList.append( Private::subWordType() );

    d->m_wordTypeList[mt].m_subWordTypeList[d->m_wordTypeList[mt].m_subWordTypeList.count()-1].m_subTypeName = typeName;
    d->m_wordTypeList[mt].m_subWordTypeList[d->m_wordTypeList[mt].m_subWordTypeList.count()-1].m_specialType = specialType;
}

void KEduVocWordType::renameType( const QString & oldTypeName, const QString & newTypeName )
{
    int index = mainTypeIndex( oldTypeName );
    d->m_wordTypeList[index].m_typeName= newTypeName;
}

void KEduVocWordType::renameSubType( const QString & mainTypeName, const QString & oldTypeName, const QString & newTypeName )
{
    kDebug() << "Rename subtype: " << mainTypeName << oldTypeName << newTypeName;
    int mainIndex = mainTypeIndex( mainTypeName );
    if ( mainIndex < 0 ) {
        kDebug() << "Renaming of subtype faild - parent not found";
        return;
    }
    int subIndex = subTypeIndex( mainTypeName, oldTypeName );
    if ( subIndex < 0 ) {
        kDebug() << "Renaming of subtype faild - old subtype not found";
        return;
    }

    d->m_wordTypeList[mainIndex].m_subWordTypeList[subIndex].m_subTypeName= newTypeName;
}

bool KEduVocWordType::removeType( const QString & typeName )
{
    // only if NOT special type
    int index = mainTypeIndex( typeName );
    if ( index < 0 ) {
        return false;
    }
    d->m_wordTypeList.removeAt( index );
    return true;
}

bool KEduVocWordType::removeSubType( const QString & mainTypeName, const QString & typeName )
{
    int mainIndex = mainTypeIndex( mainTypeName );
    if ( mainIndex < 0 ) {
        return false;
    }
    int subIndex = subTypeIndex( mainTypeName, typeName );
    if ( subIndex < 0 ) {
        return false;
    }
    d->m_wordTypeList[mainIndex].m_subWordTypeList.removeAt( subIndex );
    return true;
}

int KEduVocWordType::subTypeIndex( const QString & mainTypeName, const QString & subTypeName ) const
{
    int main = mainTypeIndex( mainTypeName );
    if ( main < 0 ) {
        kDebug() << "Main word type not found (" << mainTypeName << ")";
        return -1;
    }
    for ( int i=0; i < d->m_wordTypeList[main].m_subWordTypeList.count(); i++ ) {
        if ( d->m_wordTypeList[main].m_subWordTypeList.value( i ).m_subTypeName == subTypeName ) {
            return i;
        }
    }
    return -1;
}

QString KEduVocWordType::specialType( const QString & typeName )
{
    int index = mainTypeIndex( typeName );
    if ( index >= 0 ) {
        return d->m_wordTypeList[index].m_specialType;
    }
    return QString();
}

QString KEduVocWordType::specialSubType( const QString & mainTypeName, const QString & subTypeName )
{
    int mainIndex = mainTypeIndex( mainTypeName );
    if ( mainIndex >= 0 ) {
        int subIndex = subTypeIndex( mainTypeName, subTypeName );
        if ( subIndex >= 0 ) {
            return d->m_wordTypeList[mainIndex].m_subWordTypeList[subIndex].m_specialType;
        }
    }
    return QString();
}


QString KEduVocWordType::specialTypeNoun() const
{
    return d->WORDTYPE_SPECIAL_NOUN;
}
QString KEduVocWordType::specialTypeNounMale() const
{
    return d->WORDTYPE_SPECIAL_NOUN_MALE;
}
QString KEduVocWordType::specialTypeNounFemale() const
{
    return d->WORDTYPE_SPECIAL_NOUN_FEMALE;
}
QString KEduVocWordType::specialTypeNounNeutral() const
{
    return d->WORDTYPE_SPECIAL_NOUN_NEUTRAL;
}


QString KEduVocWordType::specialTypeVerb() const
{
    return d->WORDTYPE_SPECIAL_VERB;
}

QString KEduVocWordType::specialTypeAdjective() const
{
    return d->WORDTYPE_SPECIAL_ADJECTIVE;
}

QString KEduVocWordType::specialTypeAdverb() const
{
    return d->WORDTYPE_SPECIAL_ADVERB;
}

void KEduVocWordType::setSpecialType(const QString & typeName, const QString & newSpecialType)
{
    int mainIndex = mainTypeIndex( typeName );
    if ( mainIndex >= 0 ) {
        d->m_wordTypeList[mainIndex].m_specialType = newSpecialType;
    }
}

void KEduVocWordType::setSpecialSubType(const QString & mainTypeName, const QString & subTypeName, const QString & newSpecialType)
{
    int mainIndex = mainTypeIndex( mainTypeName );
    if ( mainIndex >= 0 ) {
        int subIndex = subTypeIndex( mainTypeName, subTypeName );
        if ( subIndex >= 0 ) {
            d->m_wordTypeList[mainIndex].m_subWordTypeList[subIndex].m_specialType = newSpecialType;
        }
    }
}

void KEduVocWordType::clear()
{
    d->m_wordTypeList.clear();
}

