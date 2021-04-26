/*
 * export a KEduVocDocument to a KVTML file
 * SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
 * SPDX-FileCopyrightText: 2005 Eric Pignet <eric at erixpage.com>
 * SPDX-FileCopyrightText: 2007 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KEDUVOCKVTMLWRITER_H
#define KEDUVOCKVTMLWRITER_H

#include <QDomDocument>
#include <QList>
#include <QFile>

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
    explicit KEduVocKvtmlWriter( QFile *file );

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
