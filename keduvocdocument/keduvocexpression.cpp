/***************************************************************************
                        Vocabulary Expression for KDE Edu
    -----------------------------------------------------------------------
    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                     (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
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
    KEduVocExpressionPrivate( KEduVocExpression* qq )
            : q( qq )
    {
        init();
    }

    void init();

    bool operator== ( const KEduVocExpressionPrivate &p ) const;

    KEduVocExpression* q;

    int m_sortIndex;
    int m_lesson;
    bool m_active;
    int m_sizeHint;

    QMap <int, KEduVocTranslation> m_translations;
};


void KEduVocExpression::KEduVocExpressionPrivate::init()
{
    m_translations.clear();
    m_active = true;
    m_lesson = -1;
    m_sortIndex = 0;
    m_sizeHint = 0;
}


bool KEduVocExpression::KEduVocExpressionPrivate::operator== ( const KEduVocExpression::KEduVocExpressionPrivate &p ) const
{
    return
        m_translations == p.m_translations &&
        m_lesson == p.m_lesson &&
        m_sortIndex == p.m_sortIndex &&
        m_active == p.m_active;
}


KEduVocExpression::KEduVocExpression()
        : d( new KEduVocExpressionPrivate( this ) )
{}

KEduVocExpression::KEduVocExpression( const QString & expression, int lesson )
        : d( new KEduVocExpressionPrivate( this ) )
{
    d->m_lesson = lesson;
    setTranslation( 0, expression.simplified() );
}

KEduVocExpression::KEduVocExpression( const QStringList & translations, int lesson )
        : d( new KEduVocExpressionPrivate( this ) )
{
    d->m_lesson = lesson;
    foreach ( QString translation, translations ) {
        setTranslation(d->m_translations.count(), translation);
    }
}

KEduVocExpression::KEduVocExpression( const KEduVocExpression &expression )
        : d( new KEduVocExpressionPrivate( *expression.d ) )
{}

KEduVocExpression::~KEduVocExpression()
{
    delete d;
}


void KEduVocExpression::removeTranslation( int index )
{
    d->m_translations.remove( index );
}


void KEduVocExpression::setTranslation( int index, const QString & expr )
{
    if ( index < 0 ) {
        return;
    }

    d->m_translations[index] = expr.simplified();
}


int KEduVocExpression::lesson() const
{
    return d->m_lesson;
}


void KEduVocExpression::setLesson( int l )
{
    d->m_lesson = l;
}


bool KEduVocExpression::isActive() const
{
    return d->m_active;
}


void KEduVocExpression::setActive( bool flag )
{
    d->m_active = flag;
}

int KEduVocExpression::sizeHint() const
{
    return d->m_sizeHint;
}

void KEduVocExpression::setSizeHint( int sizeHint )
{
    d->m_sizeHint = sizeHint;
}

void KEduVocExpression::resetGrades( int index )
{
    if ( index == -1 ) { // clear grades for all languages
        foreach( KEduVocTranslation trans, d->m_translations ) {
            trans.resetGrades();
        }
        return;
    }

    // only language index
    if ( d->m_translations.contains( index ) ) {
        d->m_translations[index].resetGrades();
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

KEduVocTranslation& KEduVocExpression::translation( int index ) const
{
    return d->m_translations[index];
}

QList< int > KEduVocExpression::translationIndices() const
{
    return d->m_translations.keys();
}

