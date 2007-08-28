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
    static const QString USAGE_USER_DEFINED;
    /// : seperates usages
    static const QString USAGE_SEPERATOR;

    /// only order was decisive, we have to keep count.
    int m_userdefinedUsageCounter;
};



#endif // KEDUVOCKVTML1COMPABILITY_P_H
