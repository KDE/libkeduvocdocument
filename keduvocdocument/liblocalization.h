/***************************************************************************
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
 ***************************************************************************/

/** @file liblocalization.h
    @brief defines a separate library localization domain for liblexikon

    Include the file at the beginning of every cpp or non-exported header
    that requires translation.  If an exported header file requires translation then
    include this file at the beginning of the header and liblocalizationheaderclose.h
    at the end of the header file to clearly separate the liblexi and application
    translation domains.

    @see http://api.kde.org/frameworks-api/frameworks5-apidocs/ki18n/html/prg_guide.html#link_lib
 */

#include "libname.h"
#define TRANSLATION_DOMAIN LIBLEXIKON_NAME
#include <klocalizedstring.h>

/*
 *
otherwise it needs to be bookended with
#undef TRANSLATION_DOMAIN
#include <klocalizedstring.h>

 * */
