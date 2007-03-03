#ifndef LEITNERSYSTEM_H
#define LEITNERSYSTEM_H

//
// C++ Interface: leitnersystem
//
// Description:
//
//
// Author: Martin Pfeiffer <martin-pfeiffer-bensheim@web.de>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <libkdeedu_core_export.h>

#include <QString>
#include <QStringList>

#include "leitnerbox.h"

/**
@author Martin Pfeiffer
*/

class KDEEDUCORE_EXPORT LeitnerSystem
{
public:
	/**Constructor without arguments*/
	explicit LeitnerSystem();

	/**Constructor with arguments
	 * @param boxes reference to a QList of LeitnerBox
	 * @param name the new LeitnerSystems name
	 */
	LeitnerSystem( const QList<LeitnerBox> &boxes, const QString &name );

	/**Destructor*/
	~LeitnerSystem();

	/**Returns the number of LeitnerBoxes contained by this LeitnerSystem
	 * @return the number of LeitnerBoxes as integer
	 */
	int getNumberOfBoxes() const;

	/**Returns a QStringList with all the LeitnerBoxes' names
	 * @return a QStringList with all names
	 */
	QStringList getBoxNameList();

	/**Returns the LeitnerSystem's name
	 * @return the LeitnerSystem's name as reference to QString
	 */
	QString systemName() const;

	/**Sets the LeitnerSystem's name
   *
	 * @param name   the new name as QString
	 */
	void setSystemName( const QString& name );

	/**Returns a LeitnerBox by number
	 * @param number the number of the LeitnerBox to be returned
	 * @return a pointer to the LeitnerBox with the number
	 */
	LeitnerBox* boxWithNumber( int number );

	const LeitnerBox* boxWithNumber( int number ) const;

	/**Returns a LeitnerBox by name
	 * @param name the name of the LeitnerBox to be returned
	 * @return a pointer to the LeitnerBox with the name,
	 *         or 0 if no such box exists.
	 */
	LeitnerBox* boxWithName( const QString& name );

	const LeitnerBox* boxWithName( const QString& name ) const;

	/**Returns the number of the given LeitnerBox
	 * @param box a pointer to the LeitnerBox
	 * @return the number of the given LeitnerBox
	 */
	int number( const LeitnerBox* box ) const;

	/**Returns the name of the LeitnerBox with number @p i
	 * @param i the LeitnerBox's number
	 * @return the name of the LeitnerBox with number @p i
	 */
	QString box( int i ) const;

	/**Returns the LeitnerBox following @p previousBox
	 * @param previousBox the name of the LeitnerBox
	 * @return the name of the LeitnerBox following previousBox
	 */
	QString nextBox( const QString& previousBox );

	/**Returns the correct word box of "int box"
	 */
	QString correctBox( int box ) const;
	/**Returns the wrong word box of "int box"
	 */
	QString wrongBox( int box ) const;
	QString correctBox( const QString& box ) const;
	QString wrongBox( const QString& box ) const;

	int wrongBoxNumber( int box ) const;
	int correctBoxNumber( int box ) const;

	void setCorrectBox( const QString& box, const QString& correctWordBox );
	void setWrongBox( const QString& box, const QString& wrongWordBox );

	bool setBoxName( int box, const QString& name );
	bool setBoxName( LeitnerBox* box, const QString& name );

	void setBoxVocabCount( QString& box, int vocabCount );
	int boxVocabCount( QString& box );

	void incrementBoxVocabCount( const QString& box );
 	void decrementBoxVocabCount( const QString& box );

	//inserts a box with number, name, correct and wrong word box
	bool insertBox( const QString& name, int correctWordBox, int wrongWordBox );
	bool insertBox( const QString& name );
	void deleteBox( int box );
	void deleteBox( LeitnerBox* box );

private:
	QString m_systemName;				//the systems name

	QList<LeitnerBox> m_boxes;
};

#endif
