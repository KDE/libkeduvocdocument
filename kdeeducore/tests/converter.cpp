/***************************************************************************
 *   Copyright (C) 2007 by Jeremy Whiting <jeremy@scitools.com>            *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

/** @file
 * \brief kvtml document reader/writer
 * partly to test the document class, partly to convert
 * old kvtml files to the new format
 * @author Jeremy Whiting <jeremy@scitools.com>
 */

#include "keduvocdocument.h"

#include <QApplication>

#include <KAboutData>
#include <KCmdLineArgs>
#include <KCmdLineOptions>
#include <KDebug>
#include <KUrl>

int main (int argc, char ** argv)
{
	KAboutData about("converter", 0, ki18n("Converter"), "0.1", ki18n("kvtml file converter"), KAboutData::License_GPL, ki18n("© 2007 Jeremy Whiting"));
  KCmdLineOptions options;
  options.add("+infile");
  options.add("+outfile");

  KCmdLineArgs::init(argc, argv, &about);
  KCmdLineArgs::addCmdLineOptions(options);
  QCoreApplication app(KCmdLineArgs::qtArgc(), KCmdLineArgs::qtArgv());

  KCmdLineArgs * arguments = KCmdLineArgs::parsedArgs();
  if (arguments != NULL && arguments->count() > 0)
  {
    KUrl infile(arguments->url(0));
    if (arguments->count() > 1)
    {
      KUrl outfile(arguments->url(1));
      
      KEduVocDocument document;
      kDebug() << "Reading " << infile << endl;
      document.open(infile);
      kDebug() << "Writing to " << outfile << endl;
      document.saveAs(outfile, KEduVocDocument::kvtml, "converter");
    }
  }
  
  arguments->clear();
  
  return 0;
}
