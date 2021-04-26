/*
 * SPDX-FileCopyrightText: 2014 Andreas Xavier <andxav at zoho dot com>
 * SPDX-License-Identifier: GPL-2.0-or-later
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
