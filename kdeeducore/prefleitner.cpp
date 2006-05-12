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
#include "prefleitner.h"

#include "leitnersystemview.h"
#include "leitnersystem.h"
#include "ui_prefleitnerbase.h"

#include <QLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>

#include <kdebug.h>

PrefLeitner::PrefLeitner( QWidget* parent ) : QDialog( parent )
{
	init( 0L );
}

PrefLeitner::PrefLeitner( QWidget* parent, LeitnerSystem* system ) : QDialog( parent )
{
	init( system );
}

PrefLeitner::~PrefLeitner()
{
	delete m_base;
}

void PrefLeitner::init( LeitnerSystem* system )
{
	m_base = new Ui::PrefLeitnerBase();
	m_base->setupUi( this );
	
	m_selectedBox = 0;

	QScrollArea *helperSV = new QScrollArea( this );

	m_leitnerSystemView = new LeitnerSystemView( helperSV->viewport() );
        m_leitnerSystemView->setObjectName( "LeitnerSystemView" );
	
        helperSV->setWidget( m_leitnerSystemView );

	connect( m_leitnerSystemView, SIGNAL( boxClicked( int ) ), this, SLOT( slotBoxClicked( int ) ) );
	
	m_selectedSystem = system;

	if ( m_selectedSystem )
	{
		//insert the list of box' names in the comboboxes
		m_base->cmbWrong->addItems( m_selectedSystem->getBoxNameList() );
		m_base->cmbCorrect->addItems( m_selectedSystem->getBoxNameList() );

		//show leitnersystem
		m_leitnerSystemView->setSystem( m_selectedSystem );
	}
}

void PrefLeitner::slotCorrectWord( const QString& newBox )
{
	if( m_selectedBox == 0 )
		return;

	//when the correct word box was changed in the combobox
	m_selectedBox->setCorrectWordBox( m_selectedSystem->boxWithName( newBox ) );
	refreshSystemView();
}

void PrefLeitner::slotWrongWord( const QString& newBox )
{
	if( m_selectedBox == 0 )
		return;

	//when the wrong word box was changed in the combobox
	m_selectedBox->setWrongWordBox( m_selectedSystem->boxWithName( newBox ) );
	refreshSystemView();
}

void PrefLeitner::slotBoxName( const QString& newName )
{
	if( m_selectedBox == 0 )
		return;

	//when the boxes name was changed
	m_selectedSystem->setBoxName( m_selectedBox, newName );
}

void PrefLeitner::newSystem()
{
	m_base->cmbCorrect->addItems( m_selectedSystem->getBoxNameList() );
	m_base->cmbWrong->addItems( m_selectedSystem->getBoxNameList() );

	refreshSystemView();
}

void PrefLeitner::refreshSystemView()
{
	m_leitnerSystemView->setSystem( m_selectedSystem );
}

void PrefLeitner::slotBoxClicked( int box )
{
	m_selectedBox = m_selectedSystem->boxWithNumber( box );

	m_base->cmbCorrect->setCurrentIndex( m_selectedSystem->correctBoxNumber( box ) );
	m_base->cmbWrong->setCurrentIndex( m_selectedSystem->wrongBoxNumber( box ) );
	m_base->lndBoxName->setText( m_selectedBox->boxName() );
}

void PrefLeitner::slotAddBox()
{
	m_selectedSystem->insertBox( "New Box", 1, 1 );
	refreshSystemView();
}

void PrefLeitner::slotDeleteBox()
{
	m_selectedSystem->deleteBox( m_selectedBox );
	m_selectedBox = 0;

	refreshSystemView();
}

void PrefLeitner::slotApply()
{
	setResult( QDialog::Accepted );
	close();
}

void PrefLeitner::slotDiscard()
{
	setResult( QDialog::Rejected );
	close();
}

LeitnerSystem* PrefLeitner::system()
{
	return m_selectedSystem;
}

#include "prefleitner.moc"
