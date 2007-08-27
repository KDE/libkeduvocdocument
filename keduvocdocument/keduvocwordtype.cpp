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

class KEduVocWordType::Private {

public:
    struct subWordType{
        QString m_subTypeName;
        QString m_specialType;
        QString m_specialTypeExplanation;
    };
    struct wordType{
        QString m_typeName;
        QString m_specialType;
        QString m_specialTypeExplanation;
        QList<subWordType> m_subWordTypeList;
    };

    /// Map containing the word type name and its properties.
    QList<wordType> m_wordTypeList;

    QMap<QString, QString> m_oldMainTypeNames;
    QMap<QString, QString> m_oldSubTypeNames;

    static const QString KVTML_1_TYPE_USER;
    static const QString KVTML_1_TYPE_DIV;

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
};

const QString KEduVocWordType::Private::KVTML_1_TYPE_USER = QString("#");
const QString KEduVocWordType::Private::KVTML_1_TYPE_DIV = QString(":");

const QString KEduVocWordType::Private::WORDTYPE_NOUN = QString("noun");
const QString KEduVocWordType::Private::WORDTYPE_NOUN_MALE = QString("noun:male");
const QString KEduVocWordType::Private::WORDTYPE_NOUN_FEMALE = QString("noun:female");
const QString KEduVocWordType::Private::WORDTYPE_NOUN_NEUTRAL = QString("noun:neutral");

const QString KEduVocWordType::Private::WORDTYPE_VERB = QString("verb");
const QString KEduVocWordType::Private::WORDTYPE_VERB_REGULAR = QString("verb:regular");
const QString KEduVocWordType::Private::WORDTYPE_VERB_IRREGULAR = QString("verb:irregular");

const QString KEduVocWordType::Private::WORDTYPE_ADJECTIVE = QString("adjective");
const QString KEduVocWordType::Private::WORDTYPE_ADVERB = QString("adverb");




const QString KEduVocWordType::Private::WORDTYPE_NOUN_EXPLANATION = QString(i18n("This holds the words of type noun. You can rename it but not delete since the article training relies on it!"));

/*

const QString KEduVocWordType::Private::WORDTYPE_NOUN_MALE = QString("male", "This holds the words of type noun male. You can rename it but not delete since the article training relies on it!");
const QString KEduVocWordType::Private::WORDTYPE_NOUN_FEMALE = QString("female", "This holds the words of type noun female. You can rename it but not delete since the article training relies on it!");
const QString KEduVocWordType::Private::WORDTYPE_NOUN_NEUTRAL = QString("neutral", "This holds the words of type noun neutral. You can rename it but not delete since the article training relies on it!");

const QString KEduVocWordType::Private::WORDTYPE_VERB = QString("verb", "This holds the words of type verb. You can rename it but not delete since the article training relies on it!");
const QString KEduVocWordType::Private::WORDTYPE_VERB_REGULAR = QString("regular", "This holds the words of type regular verbs. You can rename it but not delete since the article training relies on it!");
const QString KEduVocWordType::Private::WORDTYPE_VERB_IRREGULAR = QString("irregular", "This holds the words of type irregular verbs. You can rename it but not delete since the article training relies on it!");

const QString KEduVocWordType::Private::WORDTYPE_ADJECTIVE = QString("adjective", "This holds the words of type adjective. You can rename it but not delete since the article training relies on it!");
const QString KEduVocWordType::Private::WORDTYPE_ADVERB = QString("adverb", "This holds the words of type adverb. You can rename it but not delete since the article training relies on it!");*/



KEduVocWordType::KEduVocWordType()
: d(new Private)
{
    // this should go into the old reader/writer
    initOldTypeLists();
}

KEduVocWordType::KEduVocWordType(const KEduVocWordType & other)
: d(new Private)
{
    d->m_wordTypeList = other.d->m_wordTypeList;
    d->m_oldMainTypeNames = other.d->m_oldMainTypeNames;
    d->m_oldSubTypeNames = other.d->m_oldSubTypeNames;
}

KEduVocWordType::~KEduVocWordType()
{
    delete d;
}


KEduVocWordType & KEduVocWordType::operator =(const KEduVocWordType & other)
{
    d->m_wordTypeList = other.d->m_wordTypeList;
    return *this;
}


QString KEduVocWordType::mainTypeFromOldFormat(const QString & typeSubtypeString) const
{
    QString mainType;
    int i;

    if ((i = typeSubtypeString.indexOf(d->KVTML_1_TYPE_DIV)) >= 0)
        mainType = typeSubtypeString.left(i);
    else
        mainType = typeSubtypeString;

    QString wt = d->m_oldMainTypeNames.value( mainType );
    if ( wt == QString() ) {
        kDebug() << "Unknown old maintype: " << typeSubtypeString;
        return typeSubtypeString;
    }
    return wt;
}


QString KEduVocWordType::subTypeFromOldFormat(const QString & typeSubtypeString) const
{
    int i;
    QString t = typeSubtypeString;
    if ((i = t.indexOf(d->KVTML_1_TYPE_DIV)) >= 0) {
        t.remove(0, i+1);
    } else {
        return QString();
    }

    QString wt = d->m_oldSubTypeNames.value( t );
    if ( wt == QString() ) {
        kDebug() << "Unknown old maintype: " << typeSubtypeString;
        return typeSubtypeString;
    }
    return wt;
}


void KEduVocWordType::initOldTypeLists()
{
    d->m_oldMainTypeNames.clear();
    d->m_oldMainTypeNames.insert("v", i18n("Verb"));
    d->m_oldMainTypeNames.insert("n", i18n("Noun"));
    d->m_oldMainTypeNames.insert("nm", i18n("Name"));
    d->m_oldMainTypeNames.insert("ar", i18n("Article"));
    d->m_oldMainTypeNames.insert("aj", i18n("Adjective"));
    d->m_oldMainTypeNames.insert("av", i18n("Adverb"));
    d->m_oldMainTypeNames.insert("pr", i18n("Pronoun"));
    d->m_oldMainTypeNames.insert("ph", i18n("Phrase"));
    d->m_oldMainTypeNames.insert("num", i18n("Numeral"));
    d->m_oldMainTypeNames.insert("con", i18n("Conjunction"));
    d->m_oldMainTypeNames.insert("pre", i18n("Preposition"));
    d->m_oldMainTypeNames.insert("qu", i18n("Question"));
    d->m_oldMainTypeNames.insert("ifm", i18n("Informal"));
    d->m_oldMainTypeNames.insert("fig", i18n("Figuratively"));

    d->m_oldSubTypeNames.clear();
    d->m_oldSubTypeNames.insert("ord", i18n("Ordinal"));
    d->m_oldSubTypeNames.insert("crd", i18n("Cardinal"));
    d->m_oldSubTypeNames.insert("def", i18n("Definite"));
    d->m_oldSubTypeNames.insert("ind", i18n("Indefinite"));
    d->m_oldSubTypeNames.insert("re", i18n("Regular"));
    d->m_oldSubTypeNames.insert("ir", i18n("Irregular"));
    d->m_oldSubTypeNames.insert("pos", i18n("Possessive"));
    d->m_oldSubTypeNames.insert("per", i18n("Personal"));
    d->m_oldSubTypeNames.insert("m", i18n("Male"));
    d->m_oldSubTypeNames.insert("f", i18n("Female"));
    d->m_oldSubTypeNames.insert("s", i18n("Neutral"));

}

/*

QString KEduVocWordType::oldType(const QString & mainType, const QString & subType) const
{
    QString oldType;
    oldType = d->m_oldMainTypeNames.key(mainType);
    if ( subType != QString() ) {
        oldType.append(KVTML_1_TYPE_DIV);
        oldType.append(d->m_oldSubTypeNames.key(subType));
    }

    if ( oldType.isEmpty() ) {
        kDebug() << "Not found in preset types.";
        int index = m_userTypeDescriptions.indexOf(mainType);
        if ( index >= 0 ) {
            kDebug() << "Found user type.";
            // for some reason we count from one
            oldType = KVTML_1_TYPE_USER;
            oldType.append(QString::number(index + 1));
        }
    }

    kDebug() << "KEduVocWordType::getOldType(): " << mainType << ", "<< subType << " gives: " << oldType;
    return oldType;
}
*/


void KEduVocWordType::createDefaultWordTypes()
{
    // first the special types which cannot be deleted:
    addType("Noun", "noun", "This holds the words of type noun. You can rename it but not delete since the article training relies on it!");

    addSubType("Noun", "Male", "noun male", "This holds the words of type noun male. You can rename it but not delete since the article training relies on it!");

    addSubType("Noun", "Female", "noun female", "This holds the words of type noun female. You can rename it but not delete since the article training relies on it!");

    addSubType("Noun", "Neutral", "noun neutral", "This holds the words of type noun neutral. You can rename it but not delete since the article training relies on it!");

    addType("Verb", "verb", "This holds the words of type verb. You can rename it but not delete since the article training relies on it!");

    addSubType("Verb", "Regular", "regular", "This holds the words of type regular verbs. You can rename it but not delete since the article training relies on it!");

    addSubType("Verb", "Irregular", "irregular", "This holds the words of type irregular verbs. You can rename it but not delete since the article training relies on it!");

    addType("Adjective", "adjective", "This holds the words of type adjective. You can rename it but not delete since the article training relies on it!");

    addType("Adverb", "adverb", "This holds the words of type adverb. You can rename it but not delete since the article training relies on it!");

    addType("Question");
    addType("Name");
    addType("Article");
    addSubType("Article", "Definite");
    addSubType("Article", "Indefinite");

    addType("Pronoun");
    addSubType("Pronoun", "Possessive");
    addSubType("Pronoun", "Personal");

    addType("Phrase");
    addType("Numeral");
    addSubType("Numeral", "Ordinal");
    addSubType("Numeral", "Cardinal");

    addType("Conjunction");
    addType("Preposition");
    addType("Informal");
    addType("Figuratively");
}


QString KEduVocWordType::mainTypeName(int index) const
{
    return d->m_wordTypeList[index].m_typeName;
}

int KEduVocWordType::mainTypeIndex(const QString& name) const
{
    for ( int i=0; i < d->m_wordTypeList.count(); i++ ) {
        if ( d->m_wordTypeList.value(i).m_typeName == name ) {
            return i;
        }
    }
    return -1;
}

QStringList KEduVocWordType::typeNameList() const
{
    QStringList list;
    foreach (Private::wordType wt, d->m_wordTypeList) {
        list.append(wt.m_typeName);
    }
    return list;
}

QStringList KEduVocWordType::subTypeNameList(const QString & mainType) const
{
    int mainIndex = mainTypeIndex( mainType );
kDebug() << "Get subtypes for " << mainType << " = " << mainIndex;
    QStringList list;
    foreach (Private::subWordType wt, d->m_wordTypeList.value(mainIndex).m_subWordTypeList) {
        list.append(wt.m_subTypeName);
    }
    return list;
}


void KEduVocWordType::addType(const QString & typeName, const QString & specialType, const QString & specialTypeExplanation)
{
    if (typeName.isEmpty()) {
        kDebug() << "Attempting to add empty type. When opening old kvtml documents this is ok.";
        return;
    }
    if (typeNameList().contains(typeName)) {
        kDebug() << "Attempting to add type \"" << typeName << "\" twice.";
        return;
    }
    d->m_wordTypeList.append(Private::wordType());
    d->m_wordTypeList[d->m_wordTypeList.count()-1].m_typeName = typeName;
    d->m_wordTypeList[d->m_wordTypeList.count()-1].m_specialType = specialType;
    d->m_wordTypeList[d->m_wordTypeList.count()-1].m_specialTypeExplanation = specialTypeExplanation;
}

void KEduVocWordType::addSubType(const QString & mainType, const QString & typeName, const QString & specialType, const QString & specialTypeExplanation)
{
    int mt = mainTypeIndex(mainType);

    d->m_wordTypeList[mt].m_subWordTypeList.append(Private::subWordType());

    d->m_wordTypeList[mt].m_subWordTypeList[d->m_wordTypeList[mt].m_subWordTypeList.count()-1].m_subTypeName = typeName;
    d->m_wordTypeList[mt].m_subWordTypeList[d->m_wordTypeList[mt].m_subWordTypeList.count()-1].m_specialType = specialType;
    d->m_wordTypeList[mt].m_subWordTypeList[d->m_wordTypeList[mt].m_subWordTypeList.count()-1].m_specialTypeExplanation = specialTypeExplanation;
}

void KEduVocWordType::renameType(const QString & oldTypeName, const QString & newTypeName)
{
    int index = mainTypeIndex( oldTypeName );
    d->m_wordTypeList[index].m_typeName= newTypeName;
}

void KEduVocWordType::renameSubType(const QString & mainTypeName, const QString & oldTypeName, const QString & newTypeName)
{
kDebug() << "Rename subtype: " << mainTypeName << oldTypeName << newTypeName;
    int mainIndex = mainTypeIndex( mainTypeName );
    if (mainIndex < 0) {
        kDebug() << "Renaming of subtype faild - parent not found";
        return;
    }
    int subIndex = subTypeIndex( mainTypeName, oldTypeName );
    if (subIndex < 0) {
        kDebug() << "Renaming of subtype faild - old subtype not found";
        return;
    }

    d->m_wordTypeList[mainIndex].m_subWordTypeList[subIndex].m_subTypeName= newTypeName;
}

bool KEduVocWordType::removeType(const QString & typeName)
{
    // only if NOT special type
    int index = mainTypeIndex( typeName );
    if ( d->m_wordTypeList[index].m_specialType.isEmpty() ) {
        d->m_wordTypeList.removeAt( index );
        return true;
    }
    return false;
}

bool KEduVocWordType::removeSubType(const QString & mainTypeName, const QString & typeName)
{
    kDebug() << " delete subtype: " << mainTypeName << "/" << typeName;
    // only if NOT special type
    int mainIndex = mainTypeIndex( mainTypeName );
    int subIndex = subTypeIndex( mainTypeName, typeName );
    kDebug() << "Index: " << mainIndex << "/" << subIndex;
    if ( d->m_wordTypeList[mainIndex].m_subWordTypeList[subIndex].m_specialType.isEmpty() ) {
        d->m_wordTypeList[mainIndex].m_subWordTypeList.removeAt( subIndex );
        return true;
    }
    return false;
}

int KEduVocWordType::subTypeIndex(const QString & mainTypeName, const QString & subTypeName) const
{
    int main = mainTypeIndex( mainTypeName );
    if ( main < 0 ) {
        kDebug() << "Main word type not found (" << mainTypeName << ")";
        return -1;
    }
    for ( int i=0; i < d->m_wordTypeList[main].m_subWordTypeList.count(); i++ ) {
        if ( d->m_wordTypeList[main].m_subWordTypeList.value(i).m_subTypeName == subTypeName ) {
            return i;
        }
    }
    return -1;
}

void KEduVocWordType::printDebugWordTypes()
{
    foreach ( Private::wordType wt, d->m_wordTypeList ) {
        kDebug() << wt.m_typeName;
        foreach ( Private::subWordType swt, wt.m_subWordTypeList ) {
            kDebug() << "    " << swt.m_subTypeName;
        }
    }
}

QString KEduVocWordType::specialType(const QString & typeName)
{
    int index = mainTypeIndex( typeName );
    if (index >= 0) {
        return d->m_wordTypeList[index].m_specialType;
    }
    return QString();
}

QString KEduVocWordType::specialSubType(const QString & mainTypeName, const QString & subTypeName)
{
    int mainIndex = mainTypeIndex( mainTypeName );
    if (mainIndex >= 0) {
        int subIndex = subTypeIndex( mainTypeName, subTypeName );
        if (subIndex >= 0) {
            return d->m_wordTypeList[mainIndex].m_subWordTypeList[subIndex].m_specialType;
        }
    }
    return QString();
}



QString KEduVocWordType::nounSpecialType() const
{
    return d->WORDTYPE_NOUN;
}

QString KEduVocWordType::verbSpecialType() const
{
    return d->WORDTYPE_VERB;
}

QString KEduVocWordType::adjectiveSpecialType() const
{
    return d->WORDTYPE_ADJECTIVE;
}

QString KEduVocWordType::adverbSpecialType() const
{
    return d->WORDTYPE_ADVERB;
}

QString KEduVocWordType::typeOfSpecialType(const QString & specialType) const
{
    for ( int i=0; i < d->m_wordTypeList.count(); i++ ) {
        if ( d->m_wordTypeList.value(i).m_specialType == specialType ) {
            return d->m_wordTypeList.value(i).m_typeName;
        }
    }
    return QString();
}

