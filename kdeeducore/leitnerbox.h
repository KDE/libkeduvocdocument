#ifndef LEITNERBOX_H
#define LEITNERBOX_H

/***************************************************************************
 *   Copyright (C) 2005      by Martin Pfeiffer <hubipete@gmx.net>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <kdeedu_core_export.h>

#include <QString>

/**
 * A box in a LeitnerSystem - a LeitnerBox
 *
 * This box has a name and a certain number of vocabulars in it. This number changes continuosly
 * as the vocab cards increase or decrease in box position. There is a determined order of LeitnerBoxes
 * in a LeitnerSystem, therefore each LeitnerBox helds pointers to the LeitnerBox for wrong and correct
 * cards of this LeitnerBox
 * @author Martin Pfeiffer <hubipete@gmx.net>
 */
class EDUCORE_EXPORT LeitnerBox
{
public:
	/**
	 * The constructor
	 */
	LeitnerBox();

	/**
	 * The destructor
	 */
	~LeitnerBox();

	/**
	 * Sets the LeitnerBox's pointer to the correct word LeitnerBox
	 * @param correctWordBox a pointer to the LeitnerBox to be set
	 */
	void setCorrectWordBox( LeitnerBox* correctWordBox );

	/**
	 * Sets the LeitnerBox's pointer to the wrong word LeitnerBox
	 * @param wrongWordBox a pointer to the LeitnerBox to be set
	 */
	void setWrongWordBox( LeitnerBox* wrongWordBox );
	

	/**
	 * Sets the LeitnerBox's name to name. The name is the unique identifier of a LeitnerBox.
	 * @param name a QString with the LeitnerBox's new name
	 */	
	void setBoxName( const QString& name );

	/**
	 * Returns the current name of the LeitnerBox
	 * @return a const QString with the name of the LeitnerBox
	 */
	const QString& boxName() const;

	/**
	 * Sets the LeitnerBox's vocabCount to count
	 * @param count the new vocabCount to be set
	 */
	void setVocabCount( int count );

	/**
	 * Returns the current vocabCount
	 * @return the number of cards/ vocabs that are currently assigned to the LeitnerBox.
	 */
	int vocabCount();

	/**
	 * Returns a pointer to the LeitnerBox correct cards of this LeitnerBox here are assigned to
	 * @return a pointer to a LeitnerBox
	 */
	LeitnerBox* correctWordBox();

	/**
	 * Returns a pointer to the LeitnerBox wrong cards of this LeitnerBox here are assigned to
	 * @return a pointer to a LeitnerBox
	 */
	LeitnerBox* wrongWordBox();
	
	bool operator == ( const LeitnerBox& a ) const;

private:
	QString m_name;
	int m_vocabCount;
	LeitnerBox* m_wrongWordBox;
	LeitnerBox* m_correctWordBox;
};

#endif
