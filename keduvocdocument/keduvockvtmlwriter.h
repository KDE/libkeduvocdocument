/***************************************************************************
                     export a KEduVocDocument to a KVTML file
    -----------------------------------------------------------------------
    copyright       : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                      (C) 2005 Eric Pignet <eric at erixpage.com>
                      (C) 2007 Peter Hedlund <peter.hedlund@kdemail.net>
                      (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCKVTMLWRITER_H
#define KEDUVOCKVTMLWRITER_H

#include <QtXml/QDomDocument>
#include <QtCore/QList>
#include <QtCore/QFile>

#include "keduvocarticle.h"
#include "keduvocpersonalpronoun.h"
#include "keduvocconjugation.h"
#include "keduvocmultiplechoice.h"
#include "keduvockvtmlcompability.h"
#include "keduvoctranslation.h"

class KEduVocDocument;

/**
@author Eric Pignet
*/
class KEduVocKvtmlWriter
{
public:
    KEduVocKvtmlWriter( QFile *file );

    bool writeDoc( KEduVocDocument *doc, const QString &generator );

private:
    bool writeLesson( QDomElement &domElementParent );
    bool writeType( QDomElement &domElementParent );
    bool writeTense( QDomElement &domElementParent );
    bool writeUsage( QDomElement &domElementParent );
    bool writeOption( QDomElement &domElementParent );
    bool writeArticle( QDomElement &domElementParent );

    bool writePersonalPronouns( QDomElement &domElementParent, QList<KEduVocPersonalPronoun> &curr_conjug );
    bool writePersonalPronounChild( QDomElement &domElementParent, const KEduVocPersonalPronoun &curr_conjug );

    bool writeConjugHeader( QDomElement &domElementParent, QList<KEduVocConjugation> &curr_conjug );
    bool writeConjug( QDomElement &domElementParent, const KEduVocConjugation &curr_conjug, const QString &type );
    bool writeConjugEntry( QDomElement &domElementParent, KEduVocTranslation &translation  );
    bool writeComparison( QDomElement &domElementParent, KEduVocTranslation* translation );
    bool writeMultipleChoice( QDomElement &domElementParent, const KEduVocMultipleChoice &mc );

    QDomElement newTextElement( const QString &elementName, const QString &text );

    QFile *m_outputFile;
    KEduVocDocument *m_doc;

    QDomDocument m_domDoc;

    KEduVocKvtmlCompability m_compability;
};

#endif
