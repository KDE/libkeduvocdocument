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

const QString KEduVocKvtmlCompability::USAGE_USER_DEFINED = QString("#");
const QString KEduVocKvtmlCompability::USAGE_SEPERATOR = QString(":");


KEduVocKvtmlCompability::KEduVocKvtmlCompability()
{
    m_usages = usageMap();
    m_userdefinedUsageCounter = 0;
}


QSet<QString> KEduVocKvtmlCompability::usageFromKvtml1(const QString & oldUsage) const
{
    QSet<QString> usages;
    foreach ( QString usage , oldUsage.split(USAGE_SEPERATOR, QString::SkipEmptyParts) ) {
        usages.insert( m_usages[usage] );
    }
    return usages;
}


QMap< QString, QString > KEduVocKvtmlCompability::usageMap()
{
    QMap< QString, QString > usages;

    usages["Am"    ] = i18n("Americanism");
    usages["abbr"  ] = i18n("abbreviation");
    usages["anat"  ] = i18n("anatomy");
    usages["astr"  ] = i18n("astronomy");
    usages["biol"  ] = i18n("biology");
    usages["bs"    ] = i18n("bad sense");
    usages["contp" ] = i18n("contemptuously");
    usages["eccl"  ] = i18n("ecclesiastical");
    usages["fig"   ] = i18n("figuratively");
    usages["geol"  ] = i18n("geology");
    usages["hist"  ] = i18n("historical");
    usages["icht"  ] = i18n("ichthyology");
    usages["ifm"   ] = i18n("informal");
    usages["iro"   ] = i18n("ironic");
    usages["irr"   ] = i18n("irregular");
    usages["lit"   ] = i18n("literary");
    usages["metal" ] = i18n("metallurgy");
    usages["meteo" ] = i18n("meteorology");
    usages["miner" ] = i18n("mineralogy");
    usages["mot"   ] = i18n("motoring");
    usages["mount" ] = i18n("mountaineering");
    usages["myth"  ] = i18n("mythology");
    usages["npr"   ] = i18n("proper name");
    usages["opt"   ] = i18n("optics");
    usages["orn"   ] = i18n("ornithology");
    usages["os"    ] = i18n("oneself");
    usages["p"     ] = i18n("person");
    usages["parl"  ] = i18n("parliamentary");
    usages["pharm" ] = i18n("pharmacy");
    usages["phls"  ] = i18n("philosophy");
    usages["phot"  ] = i18n("photography");
    usages["phys"  ] = i18n("physics");
    usages["physio"] = i18n("physiology");
    usages["pl"    ] = i18n("plural");
    usages["poet"  ] = i18n("poetry");
    usages["pol"   ] = i18n("politics");
    usages["prov"  ] = i18n("provincialism");
    usages["psych" ] = i18n("psychology");
    usages["rhet"  ] = i18n("rhetoric");
    usages["surv"  ] = i18n("surveying");
    usages["telg"  ] = i18n("telegraphy");
    usages["telph" ] = i18n("telephony");
    usages["thea"  ] = i18n("theater");
    usages["typo"  ] = i18n("typography");
    usages["univ"  ] = i18n("university");
    usages["vet"   ] = i18n("veterinary medicine");
    usages["zoo"   ] = i18n("zoology");

    return usages;
}

void KEduVocKvtmlCompability::addUserdefinedUsage(const QString & usage)
{
    // start counting at 1 !!!
    m_userdefinedUsageCounter++;
    m_usages[USAGE_USER_DEFINED + QString::number(m_userdefinedUsageCounter)] = usage;
}

QSet< QString > KEduVocKvtmlCompability::documentUsages() const
{
    return QSet<QString>::fromList(m_usages.values());
}


