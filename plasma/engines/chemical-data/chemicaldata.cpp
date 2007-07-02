/*
 *   Copyright (C) 2007 Aaron Seigo <aseigo@kde.org>
 *   Copyright (C) 2007 Sebastian Kuegler <sebas@kde.org>
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

#include "chemicaldata.h"
#include "element.h"
#include "elementparser.h"

//solid specific includes
#include <KDebug>
#include <KLocale>
#include <kstandarddirs.h>

#include "plasma/datasource.h"

#include <QFile>

ChemicalDataEngine::ChemicalDataEngine(QObject* parent, const QStringList& args)
        : Plasma::DataEngine(parent)
{
    Q_UNUSED(args)
 
    ElementSaxParser * parser = new ElementSaxParser();
    QFile xmlFile( KStandardDirs::locate( "data", "libkdeedu/data/elements.xml" ) );
    QXmlInputSource source(&xmlFile);
    QXmlSimpleReader reader;

    reader.setContentHandler(parser);
    reader.parse(source);

    m_elements = parser->getElements();
}

ChemicalDataEngine::~ChemicalDataEngine()
{}

void ChemicalDataEngine::init()
{}

bool ChemicalDataEngine::sourceRequested(const QString &name)
{
    kDebug() << "ChemicalDataEngine::sourceRequested()" << endl;
    int num = name.toInt(); num -= 1;
    QString e_name   = m_elements.at(num)->dataAsString( ChemicalDataObject::name );
    QString e_symbol = m_elements.at(num)->dataAsString( ChemicalDataObject::symbol );
    QString e_weight = m_elements.at(num)->dataAsString( ChemicalDataObject::mass );
    kDebug() << "    name, symbol and weight: " << e_name << " " << e_symbol << " " << e_weight << endl;
    setData( name, I18N_NOOP("Name"), e_name);
    setData( name, I18N_NOOP("Symbol"), e_symbol );
    setData( name, I18N_NOOP("Mass"), e_weight );

    return true;
}

#include "chemicaldata.moc"
