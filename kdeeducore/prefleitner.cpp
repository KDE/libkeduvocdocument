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

class PrefLeitner::PrefLeitnerPrivate
{
public:
	PrefLeitnerPrivate( PrefLeitner* qq, LeitnerSystem* system )
	  : q( qq ), m_leitnerSystemView( 0 ), m_selectedSystem( system ),
	    m_selectedBox( 0 )
	{
		init();
	}

	PrefLeitner* q;

	LeitnerSystemView* m_leitnerSystemView;	//the LeitnerSystemView which shows the selected system
	LeitnerSystem* m_selectedSystem;	//the currently selected system to be changed
	LeitnerBox* m_selectedBox;		//the currently selected box

	Ui::PrefLeitnerBase m_ui;

	void init();

	void refreshSystemView();		//refresh the LeitnerSystemView
	void newSystem();

	// slots
	// catches the signal from the view if user clicks on a box
	void slotBoxClicked( int );
};


PrefLeitner::PrefLeitner( QWidget* parent )
  : QDialog( parent ), d( new PrefLeitnerPrivate( this, 0 ) )
{
}

PrefLeitner::PrefLeitner( LeitnerSystem* system, QWidget* parent )
  : QDialog( parent ), d( new PrefLeitnerPrivate( this, system ) )
{
}

PrefLeitner::~PrefLeitner()
{
	delete d;
}

void PrefLeitner::PrefLeitnerPrivate::init()
{
	m_ui.setupUi( q );

	QScrollArea *helperSV = new QScrollArea( q );

	m_leitnerSystemView = new LeitnerSystemView( helperSV->viewport() );
        m_leitnerSystemView->setObjectName( "LeitnerSystemView" );

        helperSV->setWidget( m_leitnerSystemView );

	connect( m_leitnerSystemView, SIGNAL( boxClicked( int ) ), q, SLOT( slotBoxClicked( int ) ) );

	if ( m_selectedSystem )
	{
		//insert the list of box' names in the comboboxes
		m_ui.cmbWrong->addItems( m_selectedSystem->getBoxNameList() );
		m_ui.cmbCorrect->addItems( m_selectedSystem->getBoxNameList() );

		//show leitnersystem
		m_leitnerSystemView->setSystem( m_selectedSystem );
	}
}

void PrefLeitner::slotCorrectWord( const QString& newBox )
{
	if( d->m_selectedBox == 0 )
		return;

	//when the correct word box was changed in the combobox
	d->m_selectedBox->setCorrectWordBox( d->m_selectedSystem->boxWithName( newBox ) );
	d->refreshSystemView();
}

void PrefLeitner::slotWrongWord( const QString& newBox )
{
	if( d->m_selectedBox == 0 )
		return;

	//when the wrong word box was changed in the combobox
	d->m_selectedBox->setWrongWordBox( d->m_selectedSystem->boxWithName( newBox ) );
	d->refreshSystemView();
}

void PrefLeitner::slotBoxName( const QString& newName )
{
	if( d->m_selectedBox == 0 )
		return;

	//when the boxes name was changed
	d->m_selectedSystem->setBoxName( d->m_selectedBox, newName );
}

void PrefLeitner::PrefLeitnerPrivate::newSystem()
{
	m_ui.cmbCorrect->addItems( m_selectedSystem->getBoxNameList() );
	m_ui.cmbWrong->addItems( m_selectedSystem->getBoxNameList() );

	refreshSystemView();
}

void PrefLeitner::PrefLeitnerPrivate::refreshSystemView()
{
	m_leitnerSystemView->setSystem( m_selectedSystem );
}

void PrefLeitner::PrefLeitnerPrivate::slotBoxClicked( int box )
{
	m_selectedBox = m_selectedSystem->boxWithNumber( box );

	m_ui.cmbCorrect->setCurrentIndex( m_selectedSystem->correctBoxNumber( box ) );
	m_ui.cmbWrong->setCurrentIndex( m_selectedSystem->wrongBoxNumber( box ) );
	m_ui.lndBoxName->setText( m_selectedBox->boxName() );
}

void PrefLeitner::slotAddBox()
{
	d->m_selectedSystem->insertBox( "New Box", 1, 1 );
	d->refreshSystemView();
}

void PrefLeitner::slotDeleteBox()
{
	d->m_selectedSystem->deleteBox( d->m_selectedBox );
	d->m_selectedBox = 0;

	d->refreshSystemView();
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
	return d->m_selectedSystem;
}

#include "prefleitner.moc"
