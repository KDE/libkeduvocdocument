/* This file is part of the KDE Edu Library
   Copyright (C) 2002 Scott Wheeler <wheeler@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include "kedudata.h"

#include <qfile.h>

#include <kdebug.h>

////////////////////////////////////////////////////////////////////////////////
// class KEduDataItem
////////////////////////////////////////////////////////////////////////////////

// public methods

KEduDataItem::KEduDataItem()
{

}

KEduDataItem::KEduDataItem(QDomElement &entry)
{
    domElement = entry;
}

KEduDataItem::~KEduDataItem()
{

}

QString KEduDataItem::originalText()
{
    return getText("o");
}

QString KEduDataItem::translatedText()
{
    return getText("t");
}

// protected methods

QString KEduDataItem::getText(const QString tagName)
{
    if(!domElement.isNull()) {

        QDomNodeList list = domElement.elementsByTagName(tagName);

        if(list.count() > 0) {

            QDomElement element = list.item(0).toElement();

            if(!element.isNull()) {
                return element.text();
            }
            else
                return QString::null;
        }
        else
            return QString::null;
    }
    else
        return QString::null;
}

////////////////////////////////////////////////////////////////////////////////
// class KEduData
////////////////////////////////////////////////////////////////////////////////

// public static methods

KEduDataItemList KEduData::parse(const QString fileName)
{
    KEduDataItemList list;

    QDomDocument document;
    QFile file(fileName);
    document.setContent(&file);

    QDomNodeList entries = document.elementsByTagName("e");

    // loop through the "e" (entry) tags looking for data
    for(uint i = 0 ; i < entries.count() ; i++) {

        // get an entry to operate on
        QDomElement entry = entries.item(i).toElement();

        // if the "node" is in fact an element -- i.e. not null
        if(!entry.isNull()) {
            KEduDataItem item(entry);
            list.append(item);
        }
    }

    return list;
}
