#ifndef CHEMICALDATA_H
#define CHEMICALDATA_H
/*
 *   Copyright (C) 2007 Carsten Niehaus <cniehaus@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License version 2 as
 *   published by the Free Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#include "plasma/dataengine.h"
#include "element.h"
#include "chemicaldataobject.h"

#include <QtXml/qxml.h>
#include <QList>

/**
 * This class provides runtime information about the battery and AC status
 * for use in a simple batter monitor application.
 */
class ChemicalDataEngine : public Plasma::DataEngine
{
    Q_OBJECT

public:
    ChemicalDataEngine( QObject* parent, const QStringList& args );
    ~ChemicalDataEngine();

protected:
    bool sourceRequested(const QString &name);
    void init();

private:
    /**
     * Set the Element
     * @param number The number of the Element (1 == H and so on)
     */
    void setElement( int number );

    QList<Element*> m_elements; 
};


K_EXPORT_PLASMA_DATAENGINE(chemicaldata, ChemicalDataEngine)

#endif // CHEMICALDATA_H
