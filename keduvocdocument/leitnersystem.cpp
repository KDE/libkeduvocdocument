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

#include "leitnersystem.h"

#include <kdebug.h>

LeitnerSystem::LeitnerSystem(const QList<LeitnerBox> &boxes, const QString &name)
{
	if( !boxes.empty() )
		m_boxes = boxes;

	if( name.isNull() )
		m_systemName = name;
}

LeitnerSystem::LeitnerSystem()
{
}

LeitnerSystem::~LeitnerSystem()
{
}

QStringList LeitnerSystem::getBoxNameList()
{
	QStringList boxNameList;

	foreach( const LeitnerBox &box, m_boxes )
		boxNameList.append( box.boxName() );
	
	return boxNameList;
}

int LeitnerSystem::getNumberOfBoxes() const
{
	return m_boxes.count();
}

LeitnerBox* LeitnerSystem::boxWithNumber( int number )
{
	return &m_boxes[ number ];
}

const LeitnerBox* LeitnerSystem::boxWithNumber( int number ) const
{
	return &m_boxes.at( number );
}

LeitnerBox* LeitnerSystem::boxWithName( const QString& name )
{
	QList<LeitnerBox>::iterator it, it2;
	it = m_boxes.begin();
	it2 = m_boxes.end();
	for ( ; it != it2; ++it)
	{
		if ( (*it).boxName() == name )
			return &(*it);
	}

	return 0;
}

const LeitnerBox* LeitnerSystem::boxWithName( const QString& name ) const
{
	QList<LeitnerBox>::const_iterator it, it2;
	it = m_boxes.begin();
	it2 = m_boxes.end();
	for ( ; it != it2; ++it)
	{
		if ( (*it).boxName() == name )
			return &(*it);
	}

	return 0;
}

QString LeitnerSystem::systemName() const
{
	return m_systemName;
}

QString LeitnerSystem::nextBox( const QString& previousBox )
{
	Q_UNUSED( previousBox )
	for( int i = 0; i < m_boxes.count(); i++ )
	{
		if( m_boxes[i].vocabCount() > 0 )
			return box( i );
	}

	return QString();
}

QString LeitnerSystem::correctBox( int box ) const
{
	return m_boxes[ box ].correctWordBox()->boxName();
}

QString LeitnerSystem::wrongBox( int box ) const
{
	return m_boxes[ box ].wrongWordBox()->boxName();
}

QString LeitnerSystem::correctBox( const QString& box ) const
{
	return boxWithName( box )->correctWordBox()->boxName();
}

QString LeitnerSystem::wrongBox( const QString& box ) const
{
	return boxWithName( box )->wrongWordBox()->boxName();
}

int LeitnerSystem::wrongBoxNumber( int box ) const
{
	return number( m_boxes[ box ].wrongWordBox() );
}

int LeitnerSystem::correctBoxNumber( int box ) const
{
	return number( m_boxes[ box ].correctWordBox() );
}

void LeitnerSystem::deleteBox( int box )
{
	m_boxes.removeAt( box );
}

void LeitnerSystem::deleteBox( LeitnerBox* box )
{
	for( int i = 0; i < m_boxes.count(); i++ )
	{
		if( m_boxes[i].correctWordBox() == box )
			m_boxes[i].setCorrectWordBox( 0 );

		if( m_boxes[i].wrongWordBox() == box )
			m_boxes[i].setWrongWordBox( 0 );
	}

	m_boxes.removeAll( *box );
}

bool LeitnerSystem::insertBox( const QString& name, int correctWordBox, int wrongWordBox )
{
	if( getBoxNameList().contains( name ) != 0 )
		return false;

	LeitnerBox tmpBox;
	tmpBox.setBoxName( name );
	tmpBox.setCorrectWordBox( boxWithNumber( correctWordBox ) );
	tmpBox.setWrongWordBox( boxWithNumber( wrongWordBox ) );

	m_boxes.append( tmpBox );
	return true;
}

void LeitnerSystem::setSystemName( const QString& name )
{
	m_systemName = name;
}

int LeitnerSystem::number( const LeitnerBox* box ) const
{
	if( box == 0 )
		return -1;

	return m_boxes.indexOf( *box );
}

bool LeitnerSystem::setBoxName( int box, const QString& name )
{
	if( boxWithName( name ) == 0 || boxWithName( name ) == boxWithNumber( box ) )
	{
		boxWithNumber( box )->setBoxName( name );

		return true;
	}
	else
		return false;
}

bool LeitnerSystem::setBoxName( LeitnerBox* box, const QString& name )
{
	if( boxWithName( name ) == 0 || boxWithName( name ) == box )
	{
		box->setBoxName( name );

		return true;
	}
	else
		return false;
}

bool LeitnerSystem::insertBox( const QString& name )
{
	if( getBoxNameList().contains( name ) != 0 )
		return false;

	LeitnerBox tmpBox;
	tmpBox.setBoxName( name );

	m_boxes.append( tmpBox );
	return true;
}

void LeitnerSystem::setCorrectBox( const QString& box, const QString& correctWordBox )
{
	boxWithName( box )->setCorrectWordBox( boxWithName( correctWordBox ) );
}

void LeitnerSystem::setWrongBox( const QString& box, const QString& wrongWordBox )
{
	boxWithName( box )->setWrongWordBox( boxWithName( wrongWordBox ) );
}

QString LeitnerSystem::box( int i ) const
{
	return m_boxes.at( i ).boxName();
}

void LeitnerSystem::setBoxVocabCount( QString& box, int vocabCount )
{
	boxWithName( box )->setVocabCount( vocabCount );
}

int LeitnerSystem::boxVocabCount( QString& box )
{
	return boxWithName( box )->vocabCount();
}

void LeitnerSystem::incrementBoxVocabCount( const QString& box )
{
	int tmp = boxWithName( box )->vocabCount();
	boxWithName( box )->setVocabCount( tmp++ );
}

void LeitnerSystem::decrementBoxVocabCount( const QString& box )
{
	int tmp = boxWithName( box )->vocabCount();
	boxWithName( box )->setVocabCount( tmp-- );
}
