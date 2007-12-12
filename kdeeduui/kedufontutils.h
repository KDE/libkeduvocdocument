/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef FONTUTILS_H
#define FONTUTILS_H

#include "libkdeedu_ui_export.h"

class QPainter;
class QString;

namespace fontUtils
{
	enum Flags
	{
		NoFlags = 1,
		DoNotAllowWordWrap = 2
	};

	int KDEEDUUI_EXPORT fontSize(QPainter &p, const QString &s1, int w, int h, Flags flags = NoFlags);
}

#endif

