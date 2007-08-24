/***************************************************************************
              manage lessons
    -----------------------------------------------------------------------

    begin        : August 11, 2007

    copyright    : (C) 2007 Jeremy Whiting <jeremywhiting@scitools.com>

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

#ifndef KEDUVOCLESSON_H
#define KEDUVOCLESSON_H

#include "libkeduvocdocument_export.h"

#include <QtCore/QList>
#include <QtCore/QString>

/** class to store information about a lesson */
class KEDUVOCDOCUMENT_EXPORT KEduVocLesson
{
	public:
		/** default constructor */
		explicit KEduVocLesson();
		
		/** copy constructor for d-pointer safe copying */
		KEduVocLesson(const KEduVocLesson &other);
		
		/** destructor */
		~KEduVocLesson();
		
    /** assignment operator */
    KEduVocLesson& operator=(const KEduVocLesson&);
    
		/** set the lesson description
		 * @param description text to set for the description 
		 */
		void setDescription(const QString &description);
		
		/** get the lesson description */
		QString description();
		
		/** get a list of all entries in the lesson */
		QList<int> entries();
		
		/** add an entry to the lesson
		 * @param entryid id of the entry to add
		 */
		void addEntry(int entryid);
		
		/** remove an entry from the lesson
		 * @param entryid id of the entry to remove
		 */
		void removeEntry(int entryid);
		
	private:
		class Private;
		Private * const d;
};

#endif
