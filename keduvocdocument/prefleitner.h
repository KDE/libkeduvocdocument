#ifndef PREFLEITNER_H
#define PREFLEITNER_H

/***************************************************************************
 *   Copyright (C) 2005 by Martin Pfeiffer                                 *
 *   hubipete@gmx.net                                                      *
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
 **************************************************************************/

#include "libkeduvocdocument_export.h"

#include <QtGui/QDialog>

class LeitnerSystem;

/**
 * This class is a dialog for configuring a LeitnerSystem
 * @author Martin Pfeiffer <hubipete@gmx.net>
 */
class KEDUVOCDOCUMENT_EXPORT PrefLeitner : public QDialog
{
    Q_OBJECT

public:
    /**
     * The public contructor
     * @param parent the widget's parent
     */
    explicit PrefLeitner( QWidget* parent = 0 );

    /**
     * The public contructur which sets also the LeitnerSystem
     * @param parent is the parent widget
     * @param system a pointer to the LeitnerSystem to configure
     */
    explicit PrefLeitner( LeitnerSystem* system, QWidget* parent = 0 );

    /**
     * The public destructor
     */
    ~PrefLeitner();

    /**
     * Sets the LeitnerSystem for the dialog
     * @param system a pointer to the LeitnerSystem to configure
     */
    void setLeitnerSystem( LeitnerSystem* system );

    /**
     * Get the LeitnerSystem that is set to the dialog
     * @return a pointer to the actual set LeitnerSystem
     */
    LeitnerSystem* system();

public Q_SLOTS:
    void slotCorrectWord( const QString& newBox );
    void slotWrongWord( const QString& newBox );
    void slotBoxName( const QString& newName );
    void slotAddBox();
    void slotDeleteBox();
    void slotDiscard();
    void slotApply();

private:
    class PrefLeitnerPrivate;
    PrefLeitnerPrivate* const d;

    Q_DISABLE_COPY( PrefLeitner )
    Q_PRIVATE_SLOT( d, void slotBoxClicked( int ) )
};

#endif //PREFLEITNER_H
