/***************************************************************************
 *   Copyright (C) 2007 by Jeremy Whiting <jpwhiting@kde.org>              *
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
 * @author Jeremy Whiting <jpwhiting@kde.org>
 */

#include "keduvocdocument.h"
#include "liblocalization.h"

#include <QApplication>
#include <QDebug>
#include <QUrl>
#include <QCommandLineParser>

int main( int argc, char ** argv )
{
    QCoreApplication::setApplicationName("kvtml-converter");
    QCoreApplication::setApplicationVersion("0.2");
    QCoreApplication::setOrganizationDomain("kde.org");
    QCoreApplication app( argc, argv );

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption formatOption(QStringList() << "f" << "format",
            i18nc("main", "file format to write out (kvtml1, kvtml2, or csv)."));
    parser.addOption(formatOption);
    parser.addPositionalArgument("infile", i18nc("main", "File to read in."));
    parser.addPositionalArgument("outfile", i18nc("main", "File to write to."));

    parser.process(app);

    QStringList files = parser.positionalArguments();

    if ( files.count() > 0 ) {
        QUrl infile = files.at( 0 );
        if ( files.count() > 1 ) {
            QUrl outfile = files.at( 1 );

            KEduVocDocument document;
            qDebug() << "Reading " << infile;
            document.open( infile );
            qDebug() << "Writing to " << outfile;
            if (parser.value("f") == "kvtml1")
            {
                document.saveAs( outfile, KEduVocDocument::Kvtml1 );
            }
            else if (parser.value("f") == "csv")
            {
                document.saveAs( outfile, KEduVocDocument::Csv );
            }
            else
            {
                document.saveAs( outfile, KEduVocDocument::Kvtml );
            }
        }
    }
    else
    {
        qDebug() << parser.helpText();
    }

    return 0;
}
