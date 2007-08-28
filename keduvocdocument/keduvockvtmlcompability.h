/***************************************************************************

    C++ Interface: keduvockvtml1compability_p

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

#ifndef KEDUVOCKVTML1COMPABILITY_P_H
#define KEDUVOCKVTML1COMPABILITY_P_H

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QSet>

#include <KLocalizedString>





// internal types, indented are subtypes

#define QM_VERB           "v"    // go
// #define   QM_VERB_IRR     "ir"
// #define   QM_VERB_REG     "re"
#define QM_NOUN           "n"    // table, coffee
// #define   QM_NOUN_F       "f"
// #define   QM_NOUN_M       "m"
// #define   QM_NOUN_S       "s"
#define QM_NAME           "nm"
// #define QM_ART            "ar"   // article
// #define   QM_ART_DEF      "def"  // definite    a/an
// #define   QM_ART_IND      "ind"  // indefinite  the
#define QM_ADJ            "aj"   // adjective   expensive, good
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

#define QM_USER_TYPE  "#"   // designates number of user type
#define QM_TYPE_DIV   ":"   // divide main from subtype

// usage delimiters (also declared in UsageManager.h)

#define UL_USER_USAGE  "#"   // designates number of user type








/**
  * @file contains defines and constants necessary for reading kvtml files prior to KDE4. kvtml version 1.
  */
class KEduVocKvtmlCompability {

public:
    /**
     * Constructor, initialize the map.
     */
    KEduVocKvtmlCompability();

    /**
     * In old kvtml documents usages could be added. When parsing the doc,
     * we need those too. They map "#1" to something meaningful.
     * Add them in order!
     * @param usage the old user defined usage.
     */
    void addUserdefinedUsage( const QString& usage );

    /**
     * This "translates" the old usage string found in the files to the
     * real word/meaning. It will also consider the user defined usages.
     * Use this method for usages.
     * @param oldUsage string e.g. "biol:anat:#1"
     * @return full set e.g. {"biology", "anatomy", "user defined 1"}
     */
    QSet<QString> usageFromKvtml1( const QString& oldUsage ) const;

    /**
     * Return set of all available usages. Since the doc also needs them.
     * @return usage set
     */
    QSet<QString> documentUsages() const;

    ///// TYPES
    /**
     * Get the type from an old type definition
     * @param typeSubtypeString the old string containing everything
     * @return new main type
     */
    QString mainTypeFromOldFormat(const QString& typeSubtypeString) const;

    /**
     * Get the subtype from an old type definition
     * @param typeSubtypeString the old string containing everything
     * @return new sub type
     */
    QString subTypeFromOldFormat(const QString& typeSubtypeString) const;

    /**
     * To write old docs: convert a nice new type to the ugly old style.
     * @param mainType type
     * @param subType subtype
     * @return old type string
     */
    QString oldType(const QString& mainType, const QString& subType) const;


private:
    /**
     * This gives a map of old abbreviations used in the files and their meaning.
     * [biol] == biology and so on...
     * @return the map
     */
    static QMap< QString, QString > usageMap();

    /// Map to store usages. Initialized with preset values by the constructor. Later user defined usages can be added.
    QMap< QString, QString > m_usages;

    /// # defines user defined usage
    static const QString KVTML_1_USAGE_USER_DEFINED;
    /// : seperates usages
    static const QString KVTML_1_USAGE_SEPERATOR;

    /// only order was decisive, we have to keep count.
    int m_userdefinedUsageCounter;


//////////// TYPES /////////////////
    void initOldTypeLists();


    /// user defined types of old documents
//     QStringList m_userTypeDescriptions;



    QMap<QString, QString> m_oldMainTypeNames;
    QMap<QString, QString> m_oldSubTypeNames;

    static const QString KVTML_1_TYPE_USER;
    static const QString KVTML_1_TYPE_DIV;

};



#endif // KEDUVOCKVTML1COMPABILITY_P_H
