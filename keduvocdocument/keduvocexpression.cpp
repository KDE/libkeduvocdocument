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
    }
    ~KEduVocExpressionPrivate();

    KEduVocExpressionPrivate(const KEduVocExpressionPrivate &other);

    bool operator== ( const KEduVocExpressionPrivate &p ) const;

    QList<KEduVocLesson*> m_lessons;
    bool m_active;

    QMap <int, KEduVocTranslation*> m_translations;
};


KEduVocExpression::KEduVocExpressionPrivate::~KEduVocExpressionPrivate()
{
    qDeleteAll(m_translations);
}


KEduVocExpression::KEduVocExpressionPrivate::KEduVocExpressionPrivate(const KEduVocExpressionPrivate & other)
{
    m_active = other.m_active;
    m_lessons = other.m_lessons;

    foreach (int key, other.m_translations.keys()) {
        m_translations[key] = new KEduVocTranslation(*other.m_translations.value(key));
    }
}


bool KEduVocExpression::KEduVocExpressionPrivate::operator== ( const KEduVocExpression::KEduVocExpressionPrivate &p ) const
{
    return
        m_translations == p.m_translations &&
        m_lessons == p.m_lessons &&
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
{}


KEduVocExpression::~KEduVocExpression()
{
    foreach(KEduVocLesson * lesson, d->m_lessons) {
        lesson->removeEntry(this);
    }
    delete d;
}


void KEduVocExpression::removeTranslation( int index )
{
    delete d->m_translations.take(index);

    for ( int j = index; j < d->m_translations.count(); j++ ) {
        d->m_translations[j] = d->m_translations.value(j+1);
    }
    d->m_translations.remove(d->m_translations.count() - 1);
}


void KEduVocExpression::setTranslation( int index, const QString & expr )
{
    if ( index < 0 ) {
        return;
    }

    d->m_translations[index] = new KEduVocTranslation(this, expr.simplified());
}


QList<KEduVocLesson*> KEduVocExpression::lessons() const
{
    return d->m_lessons;
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


KEduVocExpression& KEduVocExpression::operator= ( const KEduVocExpression &expression )
{
    *d = *expression.d;
    return *this;
}


bool KEduVocExpression::operator== ( const KEduVocExpression &expression ) const
{
    return ( *d == *expression.d );
}

KEduVocTranslation* KEduVocExpression::translation( int index )
{
    if(d->m_translations.contains(index)) {
        return d->m_translations[index];
    }
    d->m_translations[index] = new KEduVocTranslation(this);
    return d->m_translations[index];
}

QList< int > KEduVocExpression::translationIndices() const
{
    return d->m_translations.keys();
}

void KEduVocExpression::addLesson(KEduVocLesson * l)
{
    d->m_lessons.append(l);
}

void KEduVocExpression::removeLesson(KEduVocLesson * l)
{
    d->m_lessons.removeAt(d->m_lessons.indexOf(l));
}



