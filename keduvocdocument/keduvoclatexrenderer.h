/***************************************************************************
    Copyright 2010 Daniel Laidig <laidig@kde.org>
    Copyright 2009 Alexander Rieder <alexanderrieder@gmail.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// based on Cantor's LaTeX rendering code

#ifndef KEDUVOCLATEXRENDERER_H
#define KEDUVOCLATEXRENDERER_H

#include <QObject>

#include <keduvocdocument_export.h>

class QLabel;

namespace Practice
{

class KEDUVOCDOCUMENT_EXPORT KEduVocLatexRenderer : public QObject
{
    Q_OBJECT

public:
    KEduVocLatexRenderer( QObject* parent = 0 );
    ~KEduVocLatexRenderer();

    void setResultLabel( QLabel *label );
    void renderLatex(QString tex);

    static bool isLatex( const QString& tex );

private slots:
    void convertToPs();
    void convertToImage();
    void latexRendered();

private:
    class Private;
    Private * const d;
};

}

#endif // PRACTICE_LATEXRENDERER_H
