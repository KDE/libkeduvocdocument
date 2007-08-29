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

#include "leitnerbox.h"

LeitnerBox::LeitnerBox()
{}


LeitnerBox::~LeitnerBox()
{}

int LeitnerBox::vocabCount() const
{
    return m_vocabCount;
}

void LeitnerBox::setVocabCount( int count )
{
    m_vocabCount = count;
}

const QString& LeitnerBox::boxName() const
{
    return m_name;
}

LeitnerBox* LeitnerBox::correctWordBox()
{
    return m_correctWordBox;
}

const LeitnerBox* LeitnerBox::correctWordBox() const
{
    return m_correctWordBox;
}

LeitnerBox* LeitnerBox::wrongWordBox()
{
    return m_wrongWordBox;
}

const LeitnerBox* LeitnerBox::wrongWordBox() const
{
    return m_wrongWordBox;
}

void LeitnerBox::setBoxName( const QString& name )
{
    m_name = name;
}

void LeitnerBox::setCorrectWordBox( LeitnerBox* correctWordBox )
{
    m_correctWordBox = correctWordBox;
}

void LeitnerBox::setWrongWordBox( LeitnerBox* wrongWordBox )
{
    m_wrongWordBox = wrongWordBox;
}

bool LeitnerBox::operator == ( const LeitnerBox& a ) const
{
    if ( a.boxName() == boxName() )
        return true;
    else
        return false;
}
