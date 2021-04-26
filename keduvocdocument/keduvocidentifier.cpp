/*
 * SPDX-FileCopyrightText: 2007-2008 Frederik Gladhorn <gladhorn@kde.org>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "keduvocidentifier.h"

class KEduVocIdentifier::Private
{
public:
    /// the name: English, Anatomy, Fruit salad
    QString m_name;
    /// the locale: en, de, es, ...
    QString m_locale;

    /**
     * Let the user provide some additional information about the language.
     * This could be Traditional/Simplified for Chinese to differentiate between them.
     */
    QString m_comment;

    /** not sure yet: language|other|??? */
    QString m_type;

    /** I, you, he, she, it... */
    KEduVocPersonalPronoun m_personalPronouns;

    /** the for english ;)
       der, die, das ... in german */
    KEduVocArticle m_articles;

    /** Future, present and past... and many more */
    QStringList m_tenses;
};

KEduVocIdentifier::KEduVocIdentifier()
: d( new Private )
{
    ///@todo maybe the user locale would be more appropriate
    d->m_locale = QStringLiteral("en");
}

KEduVocIdentifier::~KEduVocIdentifier()
{
    delete d;
}

KEduVocIdentifier::KEduVocIdentifier( const KEduVocIdentifier &other )
: d( new Private( *other.d ) )
{
#if 0
    d->m_locale = other.d->m_locale;
    d->m_name = other.d->m_name;
    d->m_articles = other.d->m_articles;
    d->m_personalPronouns = other.d->m_personalPronouns;
    d->m_comment = other.d->m_comment;
    d->m_tenses = other.d->m_tenses;
    d->m_type = other.d->m_type;
#endif
}

KEduVocIdentifier& KEduVocIdentifier::operator= ( const KEduVocIdentifier &other )
{
    d->m_locale = other.d->m_locale;
    d->m_name = other.d->m_name;
    d->m_articles = other.d->m_articles;
    d->m_personalPronouns = other.d->m_personalPronouns;
    d->m_comment = other.d->m_comment;
    d->m_tenses = other.d->m_tenses;
    d->m_type = other.d->m_type;
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

void KEduVocIdentifier::setArticle( const KEduVocArticle& articles )
{
    d->m_articles = articles;
}

KEduVocArticle& KEduVocIdentifier::article() const
{
    return d->m_articles;
}

KEduVocPersonalPronoun& KEduVocIdentifier::personalPronouns() const
{
    return d->m_personalPronouns;
}

void KEduVocIdentifier::setPersonalPronouns( const KEduVocPersonalPronoun & pronouns )
{
    d->m_personalPronouns = pronouns;
}

QString KEduVocIdentifier::tense(int tenseIndex) const
{
    Q_ASSERT(d->m_tenses.size() > tenseIndex);
    return d->m_tenses.value(tenseIndex);
}

void KEduVocIdentifier::setTense(int tenseIndex, const QString& tense)
{
    Q_ASSERT(d->m_tenses.size() >= tenseIndex);
    if (tenseIndex == d->m_tenses.size()) {
        d->m_tenses.append(tense);
    } else {
        d->m_tenses[tenseIndex] = tense;
    }
}

QStringList KEduVocIdentifier::tenseList() const
{
    return d->m_tenses;
}

void KEduVocIdentifier::setTenseList(const QStringList& tenses)
{
    d->m_tenses = tenses;
}

