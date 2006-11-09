/*
    This file is part of libkdeedu.
    Copyright (c) 2006 Pino Toscano <toscano.pino@tiscali.it>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef KDEEDU_EXTDATE_EXPORT_H
#define KDEEDU_EXTDATE_EXPORT_H

/* needed for KDE_EXPORT macros */
#include <kdemacros.h>

#if defined Q_OS_WIN

#ifndef EXTDATE_EXPORT
# ifdef MAKE_EXTDATE_LIB
#  define EXTDATE_EXPORT KDE_EXPORT
# else
#  define EXTDATE_EXPORT KDE_IMPORT
# endif
#endif

#else
/* export statements for unix */
#define EXTDATE_EXPORT KDE_EXPORT
#endif

#endif
