/***************************************************************************
                     scan a group of KVTML documents to get information from them
    -----------------------------------------------------------------------
    copyright      : (C) 2007 Jeremy Whiting <jeremy@scitools.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SHAREDKVTMLFILES_H
#define SHAREDKVTMLFILES_H

#include "libkdeedu_core_export.h"

#include <QtXml/QDomDocument>
#include <QMap>
#include <QStringList>

#include <kglobal.h>

#include "kvtmldefs.h"

class QIODevice;
class SharedKvtmlFilesPrivate;

/**
 *namespace and singleton class to scan groups of kvtml files
 *from shared kvtml location, and give information
 *about files found there
 *@author Jeremy Whiting
 */
class KDEEDUCORE_EXPORT SharedKvtmlFiles
{
  friend class SharedKvtmlFilesPrivate;
  public:
    /** singleton accessor */
    static SharedKvtmlFiles *self();

    /** get list of all languages found in any files */
    QStringList languages();
	
	/** get list of filenames found of given language 
	 *@param language language requested QString() for all languages
	 *@return a list of filenames with words in language
	 */
    QStringList fileNames(const QString &language = QString());

    /** get the list of document titles found of a given language
	  *@param language requested language QString() for all titles
	  *@return a list of document titles with words in language
	  */
	QStringList titles(const QString &language = QString());
	
	/** get the list of document remarts found of a given language
	  *@param language requested language QString() for all comments
	  *@return a list of document remarks with words in language
	  */
	QStringList comments(const QString &language = QString());

	/** rescan the shared kvtml locations */
	void rescan();

  private:
    /** default contstuctor */
    SharedKvtmlFiles();
	
	/** default destructor */
    ~SharedKvtmlFiles();
};

#endif

