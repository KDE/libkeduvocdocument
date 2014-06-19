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
#include <QDebug>
#include <QUrl>

#include <k4aboutdata.h>

int main( int argc, char ** argv )
{
    K4AboutData about( "kvtml-converter", 0, ki18n( "Kvtml-Converter" ), "0.1", ki18n( "kvtml file converter" ), K4AboutData::License_GPL, ki18n( "© 2007 Jeremy Whiting" ) );
    KCmdLineOptions options;
    options.add( "f <format>", ki18n("file format to write out (kvtml1, kvtml2, or csv)"));
    options.add( "+infile", ki18n( "file to read in" ) );
    options.add( "+outfile", ki18n( "file to write to" ) );

    KCmdLineArgs::init( argc, argv, &about );
    KCmdLineArgs::addCmdLineOptions( options );
    QCoreApplication app( KCmdLineArgs::qtArgc(), KCmdLineArgs::qtArgv() );

    KCmdLineArgs * arguments = KCmdLineArgs::parsedArgs();
    if ( arguments != NULL && arguments->count() > 0 ) {
        QUrl infile = arguments->url( 0 );
        if ( arguments->count() > 1 ) {
            QUrl outfile = arguments->url( 1 );

            KEduVocDocument document;
            qDebug() << "Reading " << infile;
            document.open( infile );
            qDebug() << "Writing to " << outfile;
            if (arguments->getOption("f") == "kvtml1")
            {
                document.saveAs( outfile, KEduVocDocument::Kvtml1, "converter" );
            }
            else if (arguments->getOption("f") == "csv")
            {
                document.saveAs( outfile, KEduVocDocument::Csv, "converter" );
            }
            else
            {
                document.saveAs( outfile, KEduVocDocument::Kvtml, "converter" );
            }
        }
    }
    else
    {
        arguments->usage();
    }

    arguments->clear();

    return 0;
}
