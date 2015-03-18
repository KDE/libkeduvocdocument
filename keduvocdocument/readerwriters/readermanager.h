/*
 Copyright 2014 Andreas Xavier <andxav at zoho dot com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 USA
*/

#ifndef READERMANAGER_H
#define READERMANAGER_H

#include "keduvocdocument.h"

#include <QSharedPointer>
#include <QIODevice>
class ReaderBase;

/**
 * @brief manages the choice of readers.
 * @details When ReaderManager goes out of scope it frees its reader.
 * */
class ReaderManager
{
public:
    /** @brief  a QSharedPointer to the ReaderBase class.*/
    typedef QSharedPointer<ReaderBase> ReaderPtr;

    /** @brief returns a reader that can read this device.
        @details The device must be open for read and seekable.
        The manager checks all the readers that are known.  If none of
        them can parse this file/device it returns a BadReader.
        @param device an open readable, non-sequential device.
        @return a QSharedPointer<ReaderBase> for the device. */
    static ReaderPtr reader(QIODevice & device);

};



#endif // READERMANAGER_H
