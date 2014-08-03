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

/** @file liblocalizationheaderclose.h
    @brief closes the separate library localization domain for liblexikon exported headers

    Include the file at the end of every exported header
    that requires translation.  If an exported header file requires translation then
    include liblocalization.h at the beginning of the header and liblocalizationheaderclose.h
    at the end of the header file to clearly separate the liblexi and application
    translation domains.

    @see http://api.kde.org/frameworks-api/frameworks5-apidocs/ki18n/html/prg_guide.html#link_lib
 */

#include "libname.h"
#undef TRANSLATION_DOMAIN
#include <klocalizedstring.h>
