/***************************************************************************

    C++ Implementation: keduvocidentifier

    -----------------------------------------------------------------------

    begin         : Mi Aug 29 2007

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "keduvocidentifier.h"

#include <KLocalizedString>

class KEduVocIdentifier::Private
{
public:
    /// the name: English, Anatomy, Fruit salad
    QString m_name;
    /// the locale: en, de, es, ...
    QString m_locale;

    /**
     * Let the user provide some additional informatioin about the language.
     * This could be Traditional/Simplified for chinese to differentiate between them.
     */
    QString m_comment;

    /** not sure yet: language|other|??? */
    QString m_type;

    /** I, you, he, she, it... */
    KEduVocConjugation  m_personalPronouns;

    /** the for english ;)
       der, die, das ... in german */
    KEduVocArticle      m_articles;

    /** Future, present and past... and many more */
    QSet < QString >    m_tenses;

    /** Size hint for the width of this column - has to go somewere. Here at least we have the headers... */
    int m_sizeHint;
};


KEduVocIdentifier::KEduVocIdentifier()
: d( new Private )
{
    d->m_name = i18nc("A default title for a single column of vocabulary. Will hardly be used. Should be very generic.", "Title");
    ///@todo maybe the user locale would be more appropriate
    d->m_locale = "en";
}


KEduVocIdentifier::~KEduVocIdentifier()
{
    delete d;
}


KEduVocIdentifier::KEduVocIdentifier( const KEduVocIdentifier &other )
: d( new Private )
{
///@todo
    d->m_locale = other.d->m_locale;
    d->m_name = other.d->m_name;

}


KEduVocIdentifier& KEduVocIdentifier::operator= ( const KEduVocIdentifier &other )
{
    d->m_locale = other.d->m_locale;
    d->m_name = other.d->m_name;
    return *this;
}


QString KEduVocIdentifier::name() const
{
    return d->m_name;
}

void KEduVocIdentifier::setName(const QString & name)
{
    d->m_name = name;
}

QString KEduVocIdentifier::locale() const
{
    return d->m_locale;
}

void KEduVocIdentifier::setLocale(const QString & locale)
{
    d->m_locale = locale;
}

