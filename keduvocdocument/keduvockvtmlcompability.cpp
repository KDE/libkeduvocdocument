/*
 * C++ Implementation: keduvockvtml1compability_p
 * SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "keduvockvtmlcompability.h"
#include "keduvocwordtype.h"

#include <KLocalizedString>

#include <QDebug>

const QString KEduVocKvtmlCompability::KVTML_1_USER_DEFINED = QStringLiteral( "#" );
const QString KEduVocKvtmlCompability::KVTML_1_SEPERATOR = QStringLiteral( ":" );


KEduVocKvtmlCompability::KEduVocKvtmlCompability()
{
    m_userdefinedTenseCounter = 0;
    m_userdefinedTypeCounter = 0;

    initOldTypeLists();
    initOldTenses();
}


////////////////// TYPES /////////////////////////////////////////
void KEduVocKvtmlCompability::initOldTypeLists()
{
    m_oldMainTypeNames.clear();
    m_oldMainTypeNames.insert( QStringLiteral("v"), i18nc( "@item:inlistbox The grammatical type of a word", "Verb" ) );
    m_oldMainTypeNames.insert( QStringLiteral("n"), i18nc( "@item:inlistbox The grammatical type of a word", "Noun" ) );
    m_oldMainTypeNames.insert( QStringLiteral("nm"), i18nc( "@item:inlistbox The grammatical type of a word", "Name" ) );
    m_oldMainTypeNames.insert( QStringLiteral("ar"), i18nc( "@item:inlistbox The grammatical type of a word", "Article" ) );
    m_oldMainTypeNames.insert( QStringLiteral("aj"), i18nc( "@item:inlistbox The grammatical type of a word", "Adjective" ) );
    m_oldMainTypeNames.insert( QStringLiteral("av"), i18nc( "@item:inlistbox The grammatical type of a word", "Adverb" ) );
    m_oldMainTypeNames.insert( QStringLiteral("pr"), i18nc( "@item:inlistbox The grammatical type of a word", "Pronoun" ) );
    m_oldMainTypeNames.insert( QStringLiteral("ph"), i18nc( "@item:inlistbox The grammatical type of an entry", "Phrase" ) );
    m_oldMainTypeNames.insert( QStringLiteral("num"), i18nc( "@item:inlistbox The grammatical type of a word", "Numeral" ) );
    m_oldMainTypeNames.insert( QStringLiteral("con"), i18nc( "@item:inlistbox The grammatical type of a word", "Conjunction" ) );
    m_oldMainTypeNames.insert( QStringLiteral("pre"), i18nc( "@item:inlistbox The grammatical type of a word", "Preposition" ) );
    m_oldMainTypeNames.insert( QStringLiteral("qu"), i18nc( "@item:inlistbox The grammatical type of an entry", "Question" ) );


    m_oldSubTypeNames.clear();
    m_oldSubTypeNames.insert( QStringLiteral("ord"), i18nc( "@item:inlistbox A subtype of the grammatical word type: Numeral Ordinal  (first, second, third, ...)","Ordinal" ) );
    m_oldSubTypeNames.insert( QStringLiteral("crd"), i18nc( "@item:inlistbox A subtype of the grammatical word type: Numeral Cardinal (one, two, three, ...)","Cardinal" ) );
    m_oldSubTypeNames.insert( QStringLiteral("def"), i18nc( "@item:inlistbox A subtype of the grammatical word type: Article (the)","Definite" ) );
    m_oldSubTypeNames.insert( QStringLiteral("ind"), i18nc( "@item:inlistbox A subtype of the grammatical word type: Article (a)","Indefinite" ) );
    m_oldSubTypeNames.insert( QStringLiteral("re"), i18nc( "@item:inlistbox A subtype of the grammatical word type: Verb with regular conjugation","Regular" ) );
    m_oldSubTypeNames.insert( QStringLiteral("ir"), i18nc( "@item:inlistbox A subtype of the grammatical word type: Verb with irregular conjugation","Irregular" ) );
    m_oldSubTypeNames.insert( QStringLiteral("pos"), i18nc( "@item:inlistbox A subtype of the grammatical word type: Pronoun (my, your, his, her...)", "Possessive" ) );
    m_oldSubTypeNames.insert( QStringLiteral("per"), i18nc( "@item:inlistbox A subtype of the grammatical word type: Pronoun (I, you, he...)", "Personal" ) );
    m_oldSubTypeNames.insert( QStringLiteral("m"), i18nc( "@item:inlistbox A subtype of the grammatical word type: Noun", "Male" ) );
    m_oldSubTypeNames.insert( QStringLiteral("f"), i18nc( "@item:inlistbox A subtype of the grammatical word type: Noun", "Female" ) );
    m_oldSubTypeNames.insert( QStringLiteral("s"), i18nc( "@item:inlistbox A subtype of the grammatical word type: Noun", "Neutral" ) );
}


KEduVocWordType* KEduVocKvtmlCompability::typeFromOldFormat(KEduVocWordType* parent, const QString & typeSubtypeString ) const
{
    // check if it's user defined
    if ( typeSubtypeString.length() >= 2 && typeSubtypeString.left( 1 ) == QM_USER_TYPE ) {
        // they started counting at 1, we need to know which index we are dealing with:
        int selfDefinedTypeIndex = typeSubtypeString.rightRef( typeSubtypeString.count()-1 ).toInt() -1;
        return static_cast<KEduVocWordType*>(parent->childContainer(selfDefinedTypeIndex));
    }

    // assume the parent is set up to contain the old types correctly
    QString mainType;
    QString subType;
    int i;

    if (( i = typeSubtypeString.indexOf( KVTML_1_SEPERATOR ) ) >= 0 ) {
        mainType = typeSubtypeString.left( i );
        subType = typeSubtypeString.right( i+1 );
    } else {
        mainType = typeSubtypeString;
    }

    // convert from pre-0.5 versions (I guess we can just leave that in here.
    // I seriously doubt that any such documents exist...
    if ( mainType == QLatin1Char('1') ) {
        mainType = QM_VERB;
    } else if ( mainType == QLatin1Char('2') ) {
        mainType = QM_NOUN;
    } else if ( mainType == QLatin1Char('3') ) {
        mainType = QM_NAME;
    }

    QString typeName = m_oldMainTypeNames.value( mainType );
    if ( typeName.isEmpty() ) {
        qDebug() << "Unknown old maintype: " << typeSubtypeString;
        return 0;
    }

    QString subTypeName = m_oldSubTypeNames.value( subType );

    foreach (KEduVocContainer* wordType, parent->childContainers()) {
        if (wordType->name() == typeName) {
            if (subType.isEmpty()) {
                return static_cast<KEduVocWordType*>(wordType);
            } else {
                foreach (KEduVocContainer* subWordType, wordType->childContainers()) {
                    if (subWordType->name() == subTypeName) {
                        return static_cast<KEduVocWordType*>(subWordType);
                    }
                }
            }
        }
    }

    return 0;
}






/*
if ( type.length() >= 2 && type.left( 1 ) == QM_USER_TYPE ) {
                // they started counting at 1, we need to know which index we are dealing with:
                int selfDefinedTypeIndex = type.right( type.count()-1 ).toInt() -1;

                // append invented types (do we not trust our own writer?)
                if ( selfDefinedTypeIndex >= m_oldSelfDefinedTypes.count() ) {
                    while ( selfDefinedTypeIndex >= m_oldSelfDefinedTypes.count() ) {
                        m_oldSelfDefinedTypes.append( i18n( "User defined word type %1", m_oldSelfDefinedTypes.count() - 1 ) );
                    }
                }
                type = m_oldSelfDefinedTypes.value( selfDefinedTypeIndex );
            } else {
                type = m_compability.mainTypeFromOldFormat( oldType );
                subType = m_compability.subTypeFromOldFormat( oldType );
            } // not user defined - preset types


if ( oldType.length() >= 2 && type.left( 1 ) == QM_USER_TYPE ) {
            // they started counting at 1
            int selfDefinedTypeIndex = oldType.right( type.count()-1 ).toInt() -1;
            // append invented types (do we not trust our own writer?)
            if ( selfDefinedTypeIndex >= m_oldSelfDefinedTypes.count() ) {
                while ( selfDefinedTypeIndex >= m_oldSelfDefinedTypes.count() ) {
                    m_oldSelfDefinedTypes.append( i18n( "User defined word type %1", m_oldSelfDefinedTypes.count() - 1 ) );
                }
            }
            type = m_oldSelfDefinedTypes.value( selfDefinedTypeIndex );
        } else {
            type = m_compability.mainTypeFromOldFormat( oldType );
            subType = m_compability.subTypeFromOldFormat( oldType );
        } // not user defined - preset types
    }
*/


void KEduVocKvtmlCompability::initOldTenses()
{
    m_oldTenses[QStringLiteral("PrSi")] = i18n( "Simple Present" );
    m_oldTenses[QStringLiteral("PrPr")] = i18n( "Present Progressive" );
    m_oldTenses[QStringLiteral("PrPe")] = i18n( "Present Perfect" );
    m_oldTenses[QStringLiteral("PaSi")] = i18n( "Simple Past" );
    m_oldTenses[QStringLiteral("PaPr")] = i18n( "Past Progressive" );
    m_oldTenses[QStringLiteral("PaPa")] = i18n( "Past Participle" );
    m_oldTenses[QStringLiteral("FuSi")] = i18n( "Future" );
}


void KEduVocKvtmlCompability::addUserdefinedTense(const QString & tense)
{
    m_userdefinedTenseCounter++;
    m_oldTenses[KVTML_1_USER_DEFINED + QString::number( m_userdefinedTenseCounter )] = tense;
    m_tenses.insert(tense);

    qDebug() << " Add tense: " << KVTML_1_USER_DEFINED + QString::number( m_userdefinedTenseCounter ) << " - " << tense;
}


QString KEduVocKvtmlCompability::tenseFromKvtml1(const QString & oldTense)
{
    // in case the document got chaged, at least make up something as tense
    if (!m_oldTenses.keys().contains(oldTense)) {
        m_oldTenses[oldTense] = oldTense;
        qDebug() << "Warning, tense " << oldTense << " not found in document!";
    }
    m_tenses.insert(m_oldTenses.value(oldTense));
    return m_oldTenses.value(oldTense);
}


QStringList KEduVocKvtmlCompability::documentTenses() const
{
    return m_tenses.values();
}


QString KEduVocKvtmlCompability::oldTense(const QString & tense)
{
///@todo writing of the user defined tenses is probably messed up
    if ( !m_oldTenses.values().contains(tense) ) {
        m_userdefinedTenseCounter++;
        m_oldTenses[KVTML_1_USER_DEFINED + QString::number( m_userdefinedTenseCounter )] = tense;
    }
    return m_oldTenses.key(tense);
}

void KEduVocKvtmlCompability::setupWordTypes(KEduVocWordType * parent)
{
    QStringList wordTypeNames;
    wordTypeNames
        << i18nc( "The grammatical type of a word", "Verb" ) // 0
        << i18nc( "The grammatical type of a word", "Noun" ) // 1
        << i18nc( "The grammatical type of a word", "Name" )
        << i18nc( "The grammatical type of a word", "Article" ) // 3
        << i18nc( "The grammatical type of a word", "Adjective" ) // 4
        << i18nc( "The grammatical type of a word", "Adverb" ) // 5
        << i18nc( "The grammatical type of a word", "Pronoun" ) // 6
        << i18nc( "The grammatical type of an entry", "Phrase" )
        << i18nc( "The grammatical type of a word", "Numeral" ) // 8
        << i18nc( "The grammatical type of a word", "Conjunction" )
        << i18nc( "The grammatical type of a word", "Preposition" )
        << i18nc( "The grammatical type of an entry", "Question" );

    foreach (const QString &typeName, wordTypeNames) {
        KEduVocWordType* wordType = new KEduVocWordType(typeName, parent);
        parent->appendChildContainer(wordType);
        m_userdefinedTypeCounter++;
    }
    static_cast<KEduVocWordType*>(parent->childContainer(4))->setWordType(KEduVocWordFlag::Adjective);
    static_cast<KEduVocWordType*>(parent->childContainer(5))->setWordType(KEduVocWordFlag::Adverb);

    KEduVocWordType* numeral = static_cast<KEduVocWordType*>(parent->childContainer(8));
    KEduVocWordType* wordType = new KEduVocWordType(
        i18nc( "@item:inlistbox A subtype of the grammatical word type: Numeral Ordinal  (first, second, third, ...)","Ordinal" ), numeral);
    wordType->setWordType(KEduVocWordFlag::Adjective);
    numeral->appendChildContainer(wordType);
    wordType = new KEduVocWordType(
        i18nc( "@item:inlistbox A subtype of the grammatical word type: Numeral Cardinal (one, two, three, ...)","Cardinal" ), numeral);

    wordType->setWordType(KEduVocWordFlag::Adjective);
    numeral->appendChildContainer(wordType);

    KEduVocWordType* article = static_cast<KEduVocWordType*>(parent->childContainer(3));
    wordType = new KEduVocWordType(i18nc( "@item:inlistbox A subtype of the grammatical word type: Article (the)","Definite" ), article);
    wordType->setWordType(KEduVocWordFlag::Article | KEduVocWordFlag::Definite);
    article->appendChildContainer(wordType);
    wordType = new KEduVocWordType(i18nc( "@item:inlistbox A subtype of the grammatical word type: Article (a)","Indefinite" ), article);
    wordType->setWordType(KEduVocWordFlag::Article | KEduVocWordFlag::Indefinite);
    article->appendChildContainer(wordType);

    KEduVocWordType* verb = static_cast<KEduVocWordType*>(parent->childContainer(0));
    verb->setWordType(KEduVocWordFlag::Verb);
    wordType = new KEduVocWordType(i18nc( "@item:inlistbox A subtype of the grammatical word type: Verb with regular conjugation","Regular" ), verb);
    wordType->setWordType(KEduVocWordFlag::Verb | KEduVocWordFlag::Regular);
    verb->appendChildContainer(wordType);
    wordType = new KEduVocWordType(i18nc( "@item:inlistbox A subtype of the grammatical word type: Verb with irregular conjugation","Irregular" ), verb);
    verb->appendChildContainer(wordType);
    wordType->setWordType(KEduVocWordFlag::Verb | KEduVocWordFlag::Irregular);

    KEduVocWordType* noun = static_cast<KEduVocWordType*>(parent->childContainer(1));
    noun->setWordType(KEduVocWordFlag::Noun);
    wordType = new KEduVocWordType(i18nc( "@item:inlistbox A subtype of the grammatical word type: Noun", "Male" ), noun);
    noun->appendChildContainer(wordType);
    wordType->setWordType(KEduVocWordFlag::Noun | KEduVocWordFlag::Masculine);
    wordType = new KEduVocWordType(i18nc( "@item:inlistbox A subtype of the grammatical word type: Noun", "Female" ), noun);
    noun->appendChildContainer(wordType);
    wordType->setWordType(KEduVocWordFlag::Noun | KEduVocWordFlag::Feminine);
    wordType = new KEduVocWordType(i18nc( "@item:inlistbox A subtype of the grammatical word type: Noun", "Neutral" ), noun);
    noun->appendChildContainer(wordType);
    wordType->setWordType(KEduVocWordFlag::Noun | KEduVocWordFlag::Neuter);


    KEduVocWordType* pronoun = static_cast<KEduVocWordType*>(parent->childContainer(6));
    wordType = new KEduVocWordType(i18nc( "@item:inlistbox A subtype of the grammatical word type: Pronoun (my, your, his, her...)", "Possessive" ), pronoun);
    wordType->setWordType(KEduVocWordFlag::Pronoun);
    pronoun->appendChildContainer(wordType);
    wordType = new KEduVocWordType(i18nc( "@item:inlistbox A subtype of the grammatical word type: Pronoun (I, you, he...)", "Personal" ), pronoun);
    wordType->setWordType(KEduVocWordFlag::Pronoun);
    pronoun->appendChildContainer(wordType);
}
