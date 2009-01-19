/***************************************************************************
                        Vocabulary Expression for KDE Edu
    -----------------------------------------------------------------------
    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                     (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "keduvocexpression.h"

#include <KDebug>


class KEduVocExpression::KEduVocExpressionPrivate
{
public:
    KEduVocExpressionPrivate()
    {
        m_active = true;
        m_lesson = 0;
    }
    ~KEduVocExpressionPrivate();

    KEduVocExpressionPrivate(const KEduVocExpressionPrivate &other);
    KEduVocExpressionPrivate& operator= (const KEduVocExpressionPrivate &other);

    bool operator== ( const KEduVocExpressionPrivate &p ) const;

    KEduVocLesson* m_lesson;
    bool m_active;

    QMap <int, KEduVocTranslation*> m_translations;
};

KEduVocExpression::KEduVocExpressionPrivate::~KEduVocExpressionPrivate()
{
    QMap <int, KEduVocTranslation*> translations = m_translations;
    // empty the translations map, otherwise removal from word type will try to access them again when they don't exist any more
    m_translations.clear();
    qDeleteAll(translations);
}

KEduVocExpression::KEduVocExpressionPrivate::KEduVocExpressionPrivate(const KEduVocExpressionPrivate & other)
{
    m_active = other.m_active;
    m_lesson = 0;
}

KEduVocExpression::KEduVocExpressionPrivate & KEduVocExpression::KEduVocExpressionPrivate::operator =(const KEduVocExpressionPrivate & other)
{
    m_active = other.m_active;
    m_lesson = 0;

    return *this;
}

bool KEduVocExpression::KEduVocExpressionPrivate::operator== ( const KEduVocExpression::KEduVocExpressionPrivate &p ) const
{
    return
        m_translations == p.m_translations &&
        m_lesson == p.m_lesson &&
        m_active == p.m_active;
}


KEduVocExpression::KEduVocExpression()
        : d( new KEduVocExpressionPrivate )
{}

KEduVocExpression::KEduVocExpression( const QString & expression )
        : d( new KEduVocExpressionPrivate )
{
    setTranslation( 0, expression.simplified() );
}

KEduVocExpression::KEduVocExpression( const QStringList & translations)
        : d( new KEduVocExpressionPrivate )
{
    foreach ( const QString &translation, translations ) {
        setTranslation(d->m_translations.count(), translation);
    }
}


KEduVocExpression::KEduVocExpression(const KEduVocExpression & other)
    : d(new KEduVocExpressionPrivate(*other.d))
{
    foreach (int key, other.d->m_translations.keys()) {
        d->m_translations[key] = new KEduVocTranslation(*other.d->m_translations.value(key));
        d->m_translations[key]->setEntry(this);
    }
}

KEduVocExpression& KEduVocExpression::operator= ( const KEduVocExpression &other )
{
    *d = *other.d;
    foreach (int key, other.d->m_translations.keys()) {
        d->m_translations[key] = new KEduVocTranslation(*other.d->m_translations.value(key));
        d->m_translations[key]->setEntry(this);
    }
    return *this;
}

KEduVocExpression::~KEduVocExpression()
{
    setLesson(0);
    delete d;
}

void KEduVocExpression::removeTranslation( int index )
{
    int count = d->m_translations.count();

    // remove the index we delete
    delete d->m_translations.take(index);

    // shift all other indexes, +1 for the deleted
    for (int j = index; j <  count-1; j++) {
        d->m_translations[j] = d->m_translations.take(j+1);
    }
}


void KEduVocExpression::setTranslation( int index, const QString & expr )
{
    if ( index < 0 ) {
        return;
    }

    if (!d->m_translations.contains(index)) {
        d->m_translations[index] = new KEduVocTranslation(this);
    }
    d->m_translations[index]->setText(expr.simplified());
}


KEduVocLesson* KEduVocExpression::lesson() const
{
    return d->m_lesson;
}


bool KEduVocExpression::isActive() const
{
    return d->m_active;
}


void KEduVocExpression::setActive( bool flag )
{
    d->m_active = flag;
}


void KEduVocExpression::resetGrades( int index )
{
    if ( index == -1 ) { // clear grades for all languages
        foreach( KEduVocTranslation* trans, d->m_translations ) {
            trans->resetGrades();
        }
        return;
    }

    // only language index
    if ( d->m_translations.contains( index ) ) {
        d->m_translations[index]->resetGrades();
    }
}

bool KEduVocExpression::operator== ( const KEduVocExpression &expression ) const
{
    return ( *d == *expression.d );
}

KEduVocTranslation* KEduVocExpression::translation( int index )
{
    if(translationIndices().contains(index)) {
        return d->m_translations[index];
    }
    d->m_translations[index] = new KEduVocTranslation(this);
    return d->m_translations[index];
}

KEduVocTranslation * KEduVocExpression::translation(int index) const
{
    if(d->m_translations.contains(index)) {
        return 0;
    }
    return d->m_translations[index];
}

QList< int > KEduVocExpression::translationIndices() const
{
    return d->m_translations.keys();
}

void KEduVocExpression::setLesson(KEduVocLesson * l)
{
    if (d->m_lesson) {
        d->m_lesson->removeEntry(this);
    }
    d->m_lesson = l;
}


