/***************************************************************************

    C++ Implementation: keduvockvtml1compability_p

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

#include "keduvockvtmlcompability.h"

#include <KDebug>

const QString KEduVocKvtmlCompability::KVTML_1_USER_DEFINED = QString( "#" );
const QString KEduVocKvtmlCompability::KVTML_1_SEPERATOR = QString( ":" );


KEduVocKvtmlCompability::KEduVocKvtmlCompability()
{
    m_usages = usageMap();
    m_userdefinedUsageCounter = 0;
    m_userdefinedTenseCounter = 0;

    initOldTypeLists();
    initOldTenses();
}


QSet<QString> KEduVocKvtmlCompability::usageFromKvtml1( const QString & oldUsage ) const
{
    QSet<QString> usages;
    foreach( QString usage , oldUsage.split( KVTML_1_SEPERATOR, QString::SkipEmptyParts ) ) {
        usages.insert( m_usages[usage] );
    }
    return usages;
}


QMap< QString, QString > KEduVocKvtmlCompability::usageMap()
{
    QMap< QString, QString > usages;

    usages["Am"    ] = i18n( "Americanism" );
    usages["abbr"  ] = i18n( "abbreviation" );
    usages["anat"  ] = i18n( "anatomy" );
    usages["astr"  ] = i18n( "astronomy" );
    usages["biol"  ] = i18n( "biology" );
    usages["bs"    ] = i18n( "bad sense" );
    usages["contp" ] = i18n( "contemptuously" );
    usages["eccl"  ] = i18n( "ecclesiastical" );
    usages["fig"   ] = i18n( "figuratively" );
    usages["geol"  ] = i18n( "geology" );
    usages["hist"  ] = i18n( "historical" );
    usages["icht"  ] = i18n( "ichthyology" );
    usages["ifm"   ] = i18n( "informal" );
    usages["iro"   ] = i18n( "ironic" );
    usages["irr"   ] = i18n( "irregular" );
    usages["lit"   ] = i18n( "literary" );
    usages["metal" ] = i18n( "metallurgy" );
    usages["meteo" ] = i18n( "meteorology" );
    usages["miner" ] = i18n( "mineralogy" );
    usages["mot"   ] = i18n( "motoring" );
    usages["mount" ] = i18n( "mountaineering" );
    usages["myth"  ] = i18n( "mythology" );
    usages["npr"   ] = i18n( "proper name" );
    usages["opt"   ] = i18n( "optics" );
    usages["orn"   ] = i18n( "ornithology" );
    usages["os"    ] = i18n( "oneself" );
    usages["p"     ] = i18n( "person" );
    usages["parl"  ] = i18n( "parliamentary" );
    usages["pharm" ] = i18n( "pharmacy" );
    usages["phls"  ] = i18n( "philosophy" );
    usages["phot"  ] = i18n( "photography" );
    usages["phys"  ] = i18n( "physics" );
    usages["physio"] = i18n( "physiology" );
    usages["pl"    ] = i18n( "plural" );
    usages["poet"  ] = i18n( "poetry" );
    usages["pol"   ] = i18n( "politics" );
    usages["prov"  ] = i18n( "provincialism" );
    usages["psych" ] = i18n( "psychology" );
    usages["rhet"  ] = i18n( "rhetoric" );
    usages["surv"  ] = i18n( "surveying" );
    usages["telg"  ] = i18n( "telegraphy" );
    usages["telph" ] = i18n( "telephony" );
    usages["thea"  ] = i18n( "theater" );
    usages["typo"  ] = i18n( "typography" );
    usages["univ"  ] = i18n( "university" );
    usages["vet"   ] = i18n( "veterinary medicine" );
    usages["zoo"   ] = i18n( "zoology" );

    return usages;
}

void KEduVocKvtmlCompability::addUserdefinedUsage( const QString & usage )
{
    // start counting at 1 !!!
    m_userdefinedUsageCounter++;
    m_usages[KVTML_1_USER_DEFINED + QString::number( m_userdefinedUsageCounter )] = usage;
}

QSet< QString > KEduVocKvtmlCompability::documentUsages() const
{
    return QSet<QString>::fromList( m_usages.values() );
}



////////////////// TYPES /////////////////////////////////////////
void KEduVocKvtmlCompability::initOldTypeLists()
{

    m_oldMainTypeNames.clear();
    m_oldMainTypeNames.insert( "v", i18nc( "@item:inlistbox The grammatical type of a word", "Verb" ) );
    m_oldMainTypeNames.insert( "n", i18nc( "@item:inlistbox The grammatical type of a word", "Noun" ) );
    m_oldMainTypeNames.insert( "nm", i18nc( "@item:inlistbox The grammatical type of a word", "Name" ) );
    m_oldMainTypeNames.insert( "ar", i18nc( "@item:inlistbox The grammatical type of a word", "Article" ) );
    m_oldMainTypeNames.insert( "aj", i18nc( "@item:inlistbox The grammatical type of a word", "Adjective" ) );
    m_oldMainTypeNames.insert( "av", i18nc( "@item:inlistbox The grammatical type of a word", "Adverb" ) );
    m_oldMainTypeNames.insert( "pr", i18nc( "@item:inlistbox The grammatical type of a word", "Pronoun" ) );
    m_oldMainTypeNames.insert( "ph", i18nc( "@item:inlistbox The grammatical type of an entry", "Phrase" ) );
    m_oldMainTypeNames.insert( "num", i18nc( "@item:inlistbox The grammatical type of a word", "Numeral" ) );
    m_oldMainTypeNames.insert( "con", i18nc( "@item:inlistbox The grammatical type of a word", "Conjunction" ) );
    m_oldMainTypeNames.insert( "pre", i18nc( "@item:inlistbox The grammatical type of a word", "Preposition" ) );
    m_oldMainTypeNames.insert( "qu", i18nc( "@item:inlistbox The grammatical type of an entry", "Question" ) );


    m_oldSubTypeNames.clear();
    m_oldSubTypeNames.insert( "ord", i18nc( "@item:inlistbox A subtype of the grammatical word type: Numeral Ordinal  (one, two, three, ...)","Ordinal" ) );
    m_oldSubTypeNames.insert( "crd", i18nc( "@item:inlistbox A subtype of the grammatical word type: Numeral Cardinal (first, second, third, ...)","Cardinal" ) );
    m_oldSubTypeNames.insert( "def", i18nc( "@item:inlistbox A subtype of the grammatical word type: Article (the)","Definite" ) );
    m_oldSubTypeNames.insert( "ind", i18nc( "@item:inlistbox A subtype of the grammatical word type: Article (a)","Indefinite" ) );
    m_oldSubTypeNames.insert( "re", i18nc( "@item:inlistbox A subtype of the grammatical word type: Verb with regular conjugation","Regular" ) );
    m_oldSubTypeNames.insert( "ir", i18nc( "@item:inlistbox A subtype of the grammatical word type: Verb with irregular conjugation","Irregular" ) );
    m_oldSubTypeNames.insert( "pos", i18nc( "@item:inlistbox A subtype of the grammatical word type: Pronoun (my, your, his, her...)", "Possessive" ) );
    m_oldSubTypeNames.insert( "per", i18nc( "@item:inlistbox A subtype of the grammatical word type: Pronoun (I, you, he...)", "Personal" ) );
    m_oldSubTypeNames.insert( "m", i18nc( "@item:inlistbox A subtype of the grammatical word type: Noun", "Male" ) );
    m_oldSubTypeNames.insert( "f", i18nc( "@item:inlistbox A subtype of the grammatical word type: Noun", "Female" ) );
    m_oldSubTypeNames.insert( "s", i18nc( "@item:inlistbox A subtype of the grammatical word type: Noun", "Neutral" ) );
}


QString KEduVocKvtmlCompability::mainTypeFromOldFormat( const QString & typeSubtypeString ) const
{
    QString mainType;
    int i;

    if (( i = typeSubtypeString.indexOf( KVTML_1_SEPERATOR ) ) >= 0 )
        mainType = typeSubtypeString.left( i );
    else
        mainType = typeSubtypeString;

    // convert from pre-0.5 versions (I guess we can just leave that in here.
    // I seriously doubt that any such documents exist...
    if ( mainType == "1" ) {
        mainType = QM_VERB;
    } else if ( mainType == "2" ) {
        mainType = QM_NOUN;
    } else if ( mainType == "3" ) {
        mainType = QM_NAME;
    }

    QString wt = m_oldMainTypeNames.value( mainType );
    if ( wt == QString() ) {
        kDebug() << "Unknown old maintype: " << typeSubtypeString;
        return typeSubtypeString;
    }
    return wt;
}


QString KEduVocKvtmlCompability::subTypeFromOldFormat( const QString & typeSubtypeString ) const
{
    int i;
    QString t = typeSubtypeString;
    if (( i = t.indexOf( KVTML_1_SEPERATOR ) ) >= 0 ) {
        t.remove( 0, i+1 );
    } else {
        return QString();
    }

    QString wt = m_oldSubTypeNames.value( t );
    if ( wt == QString() ) {
        kDebug() << "Unknown old maintype: " << typeSubtypeString;
        return typeSubtypeString;
    }
    return wt;
}

QString KEduVocKvtmlCompability::oldType( const QString & mainType, const QString & subType ) const
{
    QString oldType;
    QString oldSubType;
    oldType = m_oldMainTypeNames.key( mainType );
    oldSubType = m_oldSubTypeNames.key( subType );
    if ( !oldSubType.isEmpty() ) {
        return oldType + KVTML_1_SEPERATOR + oldSubType;
    }
    return oldType;

}

void KEduVocKvtmlCompability::initOldTenses()
{
    m_oldTenses["PrSi"] = i18n( "Simple Present" );
    m_oldTenses["PrPr"] = i18n( "Present Progressive" );
    m_oldTenses["PrPe"] = i18n( "Present Perfect" );
    m_oldTenses["PaSi"] = i18n( "Simple Past" );
    m_oldTenses["PaPr"] = i18n( "Past Progressive" );
    m_oldTenses["PaPa"] = i18n( "Past Participle" );
    m_oldTenses["FuSi"] = i18n( "Future" );
}


void KEduVocKvtmlCompability::addUserdefinedTense(const QString & tense)
{
    m_userdefinedTenseCounter++;
    m_oldTenses[KVTML_1_USER_DEFINED + QString::number( m_userdefinedTenseCounter )] = tense;
    m_tenses.insert(tense);

    kDebug() << " Add tense: " << KVTML_1_USER_DEFINED + QString::number( m_userdefinedTenseCounter ) << " - " << tense;
}


QString KEduVocKvtmlCompability::tenseFromKvtml1(const QString & oldTense)
{
    // in case the document got chaged, at least make up something as tense
    if (!m_oldTenses.keys().contains(oldTense)) {
        m_oldTenses[oldTense] = oldTense;
        kDebug() << "Warning, tense not found in document!";
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