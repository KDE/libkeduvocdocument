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

// #define QM_VERB           "v"    // go
// #define   QM_VERB_IRR     "ir"
// #define   QM_VERB_REG     "re"
// #define QM_NOUN           "n"    // table, coffee
// #define   QM_NOUN_F       "f"
// #define   QM_NOUN_M       "m"
// #define   QM_NOUN_S       "s"
// #define QM_NAME           "nm"
// #define QM_ART            "ar"   // article
// #define   QM_ART_DEF      "def"  // definite    a/an
// #define   QM_ART_IND      "ind"  // indefinite  the
// #define QM_ADJ            "aj"   // adjective   expensive, good
// #define QM_ADV            "av"   // adverb      today, strongly
// #define QM_PRON           "pr"   // pronoun     you, she
// #define   QM_PRON_POS     "pos"  // possessive  my, your
// #define   QM_PRON_PER     "per"  // personal
// #define QM_PHRASE         "ph"
// #define QM_NUM            "num"  // numeral
// #define   QM_NUM_ORD      "ord"  // ordinal     first, second
// #define   QM_NUM_CARD     "crd"  // cardinal    one, two
// #define QM_INFORMAL       "ifm"
// #define QM_FIG            "fig"
// #define QM_CON            "con"  // conjuncton  and, but
// #define QM_PREP           "pre"  // preposition behind, between
// #define QM_QUEST          "qu"   // question    who, what

// type delimiters

// #define QM_USER_TYPE  "#"   // designates number of user type
// #define QM_TYPE_DIV   ":"   // divide main from subtype

class KEduVocWordType::Private
{
public:
    struct subWordType
    {
        QString m_subTypeName;
        QString m_specialType;
        QString m_specialTypeExplanation;
        bool operator== ( const subWordType& other );
    };
    struct wordType
    {
        QString m_typeName;
        QString m_specialType;
        QString m_specialTypeExplanation;
        QList<subWordType> m_subWordTypeList;
        bool operator== ( const wordType& other );
    };

    /// Map containing the word type name and its properties.
    QList<wordType> m_wordTypeList;

    static const QString WORDTYPE_NOUN;
    static const QString WORDTYPE_NOUN_MALE;
    static const QString WORDTYPE_NOUN_FEMALE;
    static const QString WORDTYPE_NOUN_NEUTRAL;

    static const QString WORDTYPE_VERB;
    static const QString WORDTYPE_VERB_REGULAR;
    static const QString WORDTYPE_VERB_IRREGULAR;
    static const QString WORDTYPE_ADJECTIVE;
    static const QString WORDTYPE_ADVERB;

    static const QString WORDTYPE_NOUN_EXPLANATION;
    static const QString WORDTYPE_NOUN_MALE_EXPLANATION;
    static const QString WORDTYPE_NOUN_FEMALE_EXPLANATION;
    static const QString WORDTYPE_NOUN_NEUTRAL_EXPLANATION;

    static const QString WORDTYPE_VERB_EXPLANATION;
    static const QString WORDTYPE_VERB_REGULAR_EXPLANATION;
    static const QString WORDTYPE_VERB_IRREGULAR_EXPLANATION;
    static const QString WORDTYPE_ADJECTIVE_EXPLANATION;
    static const QString WORDTYPE_ADVERB_EXPLANATION;
};

const QString KEduVocWordType::Private::WORDTYPE_NOUN = QString( "noun" );
const QString KEduVocWordType::Private::WORDTYPE_NOUN_MALE = QString( "noun male" );
const QString KEduVocWordType::Private::WORDTYPE_NOUN_FEMALE = QString( "noun female" );
const QString KEduVocWordType::Private::WORDTYPE_NOUN_NEUTRAL = QString( "noun neutral" );

const QString KEduVocWordType::Private::WORDTYPE_VERB = QString( "verb" );
const QString KEduVocWordType::Private::WORDTYPE_VERB_REGULAR = QString( "verb regular" );
const QString KEduVocWordType::Private::WORDTYPE_VERB_IRREGULAR = QString( "verb irregular" );

const QString KEduVocWordType::Private::WORDTYPE_ADJECTIVE = QString( "adjective" );
const QString KEduVocWordType::Private::WORDTYPE_ADVERB = QString( "adverb" );


const QString KEduVocWordType::Private::WORDTYPE_NOUN_EXPLANATION = QString( i18n( "This holds the words of type noun. You can rename it but not delete since the article training relies on it!" ) );
const QString KEduVocWordType::Private::WORDTYPE_NOUN_MALE_EXPLANATION = QString( "This holds the words of type noun male. You can rename it but not delete since the article training relies on it!" );
const QString KEduVocWordType::Private::WORDTYPE_NOUN_FEMALE_EXPLANATION = QString( "This holds the words of type noun female. You can rename it but not delete since the article training relies on it!" );
const QString KEduVocWordType::Private::WORDTYPE_NOUN_NEUTRAL_EXPLANATION = QString( "This holds the words of type noun neutral. You can rename it but not delete since the article training relies on it!" );

const QString KEduVocWordType::Private::WORDTYPE_VERB_EXPLANATION = QString( "This holds the words of type verb. You can rename it but not delete since the article training relies on it!" );
const QString KEduVocWordType::Private::WORDTYPE_VERB_REGULAR_EXPLANATION = QString( "This holds the words of type regular verbs. You can rename it but not delete since the article training relies on it!" );
const QString KEduVocWordType::Private::WORDTYPE_VERB_IRREGULAR_EXPLANATION = QString( "This holds the words of type irregular verbs. You can rename it but not delete since the article training relies on it!" );

const QString KEduVocWordType::Private::WORDTYPE_ADJECTIVE_EXPLANATION = QString( "This holds the words of type adjective. You can rename it but not delete since the article training relies on it!" );
const QString KEduVocWordType::Private::WORDTYPE_ADVERB_EXPLANATION = QString( "This holds the words of type adverb. You can rename it but not delete since the article training relies on it!" );


bool KEduVocWordType::Private::wordType::operator ==(const wordType & other)
{
    return m_typeName == other.m_typeName &&
        m_specialType == other.m_specialType &&
        m_specialTypeExplanation == other.m_specialTypeExplanation &&
        m_subWordTypeList == other.m_subWordTypeList;
}

bool KEduVocWordType::Private::subWordType::operator ==(const subWordType & other)
{
    return m_subTypeName == other.m_subTypeName &&
        m_specialType == other.m_specialType &&
        m_specialTypeExplanation == other.m_specialTypeExplanation;
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
    // first the special types which cannot be deleted:
    addType( i18nc( "@item:inlistbox The grammatical type of a word", "Noun" ),
        d->WORDTYPE_NOUN, d->WORDTYPE_NOUN_EXPLANATION );

    addSubType( i18nc( "@item:inlistbox The grammatical type of a word", "Noun" ),
        i18nc( "@item:inlistbox A subtype of the grammatical word type: Noun", "Male" ),
        d->WORDTYPE_NOUN_MALE, d->WORDTYPE_NOUN_MALE_EXPLANATION );

    addSubType( i18nc( "@item:inlistbox The grammatical type of a word", "Noun" ),
        i18nc( "@item:inlistbox A subtype of the grammatical word type: Noun", "Female" ),
        d->WORDTYPE_NOUN_FEMALE, d->WORDTYPE_NOUN_FEMALE_EXPLANATION );

    addSubType( i18nc( "@item:inlistbox The grammatical type of a word", "Noun" ),
        i18nc( "@item:inlistbox A subtype of the grammatical word type: Noun", "Neutral" ),
        d->WORDTYPE_NOUN_NEUTRAL, d->WORDTYPE_NOUN_NEUTRAL_EXPLANATION );

    addType( i18nc( "@item:inlistbox The grammatical type of a word", "Verb" ),
        d->WORDTYPE_VERB, d->WORDTYPE_VERB_EXPLANATION );

    addSubType( i18nc( "@item:inlistbox The grammatical type of a word", "Verb" ),
        i18nc( "@item:inlistbox A subtype of the grammatical word type: Verb with regular conjugation","Regular" ),
        d->WORDTYPE_VERB_REGULAR, d->WORDTYPE_VERB_REGULAR_EXPLANATION );

    addSubType( i18nc( "@item:inlistbox The grammatical type of a word", "Verb" ),
        i18nc( "@item:inlistbox A subtype of the grammatical word type: Verb with irregular conjugation","Irregular" ),
        d->WORDTYPE_VERB_IRREGULAR, d->WORDTYPE_VERB_IRREGULAR_EXPLANATION );

    addType(  i18nc( "@item:inlistbox The grammatical type of a word", "Adjective" ) ,
        d->WORDTYPE_ADJECTIVE, d->WORDTYPE_ADJECTIVE_EXPLANATION );

    addType(  i18nc( "@item:inlistbox The grammatical type of a word", "Adverb" ),
        d->WORDTYPE_ADVERB, d->WORDTYPE_ADVERB_EXPLANATION );


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


void KEduVocWordType::addType( const QString & typeName, const QString & specialType, const QString & specialTypeExplanation )
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
    d->m_wordTypeList[d->m_wordTypeList.count()-1].m_specialTypeExplanation = specialTypeExplanation;
}

void KEduVocWordType::addSubType( const QString & mainType, const QString & typeName, const QString & specialType, const QString & specialTypeExplanation )
{
    int mt = mainTypeIndex( mainType );

    d->m_wordTypeList[mt].m_subWordTypeList.append( Private::subWordType() );

    d->m_wordTypeList[mt].m_subWordTypeList[d->m_wordTypeList[mt].m_subWordTypeList.count()-1].m_subTypeName = typeName;
    d->m_wordTypeList[mt].m_subWordTypeList[d->m_wordTypeList[mt].m_subWordTypeList.count()-1].m_specialType = specialType;
    d->m_wordTypeList[mt].m_subWordTypeList[d->m_wordTypeList[mt].m_subWordTypeList.count()-1].m_specialTypeExplanation = specialTypeExplanation;
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
kDebug() << "special for " << typeName << ":" << d->m_wordTypeList[index].m_specialType;
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
    return d->WORDTYPE_NOUN;
}
QString KEduVocWordType::specialTypeNounMale() const
{
    return d->WORDTYPE_NOUN_MALE;
}
QString KEduVocWordType::specialTypeNounFemale() const
{
    return d->WORDTYPE_NOUN_FEMALE;
}
QString KEduVocWordType::specialTypeNounNeutral() const
{
    return d->WORDTYPE_NOUN_NEUTRAL;
}


QString KEduVocWordType::specialTypeVerb() const
{
    return d->WORDTYPE_VERB;
}

QString KEduVocWordType::specialTypeAdjective() const
{
    return d->WORDTYPE_ADJECTIVE;
}

QString KEduVocWordType::specialTypeAdverb() const
{
    return d->WORDTYPE_ADVERB;
}

void KEduVocWordType::setSpecialType(const QString & typeName, const QString & newSpecialType)
{
    int mainIndex = mainTypeIndex( typeName );
    if ( mainIndex >= 0 ) {
        d->m_wordTypeList[mainIndex].m_specialType = newSpecialType;
kDebug() << "set special type" << typeName << ": " << d->m_wordTypeList[mainIndex].m_specialType;
    }

}

void KEduVocWordType::setSpecialSubType(const QString & mainTypeName, const QString & subTypeName, const QString & newSpecialType)
{
    int mainIndex = mainTypeIndex( mainTypeName );
    if ( mainIndex >= 0 ) {
        int subIndex = subTypeIndex( mainTypeName, subTypeName );
        if ( subIndex >= 0 ) {
            d->m_wordTypeList[mainIndex].m_subWordTypeList[subIndex].m_specialType = newSpecialType;
kDebug() << "set special sub type" << subTypeName << ": " << d->m_wordTypeList[mainIndex].m_subWordTypeList[subIndex].m_specialType;
        }
    }
}

