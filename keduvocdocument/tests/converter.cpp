/*
 * SPDX-FileCopyrightText: 2007 Jeremy Whiting <jpwhiting@kde.org>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

/** @file
 * \brief kvtml document reader/writer
 * partly to test the document class, partly to convert
 * old kvtml files to the new format
 * @author Jeremy Whiting <jpwhiting@kde.org>
 */

#include "keduvocdocument.h"

#include <KLocalizedString>
#include <QCoreApplication>
#include <QDebug>
#include <QUrl>
#include <QCommandLineParser>

int main( int argc, char ** argv )
{
    QCoreApplication::setApplicationName(QStringLiteral("kvtml-converter"));
    QCoreApplication::setApplicationVersion(QStringLiteral("0.2"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));
    QCoreApplication app( argc, argv );

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption formatOption(QStringList() << QStringLiteral("f") << QStringLiteral("format"),
            i18nc("main", "file format to write out (kvtml1, kvtml2, or csv)."));
    parser.addOption(formatOption);
    parser.addPositionalArgument(QStringLiteral("infile"), i18nc("main", "File to read in."));
    parser.addPositionalArgument(QStringLiteral("outfile"), i18nc("main", "File to write to."));

    parser.process(app);

    QStringList files = parser.positionalArguments();

    if ( files.count() > 0 ) {
        QUrl infile = QUrl::fromLocalFile(files.at( 0 ));
        if ( files.count() > 1 ) {
            QUrl outfile = QUrl::fromLocalFile(files.at( 1 ));

            KEduVocDocument document;
            qDebug() << "Reading " << infile;
            document.open( infile );
            qDebug() << "Writing to " << outfile;
            if (parser.value(QStringLiteral("f")) == QLatin1String("kvtml1"))
            {
                document.saveAs( outfile, KEduVocDocument::Kvtml1 );
            }
            else if (parser.value(QStringLiteral("f")) == QLatin1String("csv"))
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
