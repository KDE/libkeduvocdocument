/*
 * C++ Interface: keduvockvtml1compability_p
 * SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef KEDUVOCKVTMLCOMPABILITY_H
#define KEDUVOCKVTMLCOMPABILITY_H

#include <QString>
#include <QMap>
#include <QSet>


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
// #define QM_CON            "con"  // conjunction  and, but
// #define QM_PREP           "pre"  // preposition behind, between
// #define QM_QUEST          "qu"   // question    who, what

// type delimiters

#define QM_USER_TYPE  "#"   // designates number of user type
#define QM_TYPE_DIV   ":"   // divide main from subtype

// usage delimiters (also declared in UsageManager.h)

#define UL_USER_USAGE  "#"   // designates number of user type

class KEduVocWordType;

/**
  * @file keduvockvtmlcompability.h
  * contains defines and constants necessary for reading kvtml files prior to KDE4. kvtml version 1.
  */
class KEduVocKvtmlCompability
{

public:
    /**
     * Constructor, initialize the map.
     */
    KEduVocKvtmlCompability();

    ///// TYPES
    /**
     * Get the type from an old type definition
     * @param parent the parent word type
     * @param typeSubtypeString the old string containing everything
     * @return type
     */
    KEduVocWordType* typeFromOldFormat(KEduVocWordType* parent, const QString & typeSubtypeString ) const;

    /**
     * To write old docs: convert a nice new type to the ugly old style.
     * @param mainType type
     * @param subType subtype
     * @return old type string
     */
    QString oldType( const QString& mainType, const QString& subType ) const;


    void addUserdefinedTense( const QString& tense );
    QString tenseFromKvtml1( const QString & oldTense );
    QStringList documentTenses() const;
    QString oldTense( const QString& tense );

    void setupWordTypes(KEduVocWordType* parent);

private:

//////////// TYPES /////////////////
    void initOldTypeLists();

    QMap<QString, QString> m_oldMainTypeNames;
    QMap<QString, QString> m_oldSubTypeNames;
    int m_userdefinedTypeCounter;

///////////TENSES/CONJUGATIONS///////
    void initOldTenses();

    QMap<QString, QString> m_oldTenses;
    int m_userdefinedTenseCounter;
    QSet<QString> m_tenses;

    /// # defines user defined usage/word type/tense
    static const QString KVTML_1_USER_DEFINED;
    /// : separates usages/word types
    static const QString KVTML_1_SEPERATOR;
};

#endif // KEDUVOCKVTMLCOMPABILITY_H
