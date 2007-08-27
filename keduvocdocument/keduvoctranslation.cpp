/***************************************************************************
                        Vocabulary Expression Translation for KDE Edu
    -----------------------------------------------------------------------
    copyright      :(C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "keduvoctranslation.h"
#include <KDebug>


class KEduVocTranslation::KEduVocTranslationPrivate
{
public:

    KEduVocTranslationPrivate () // ( KEduVocTranslation* qq )
            //: q ( qq )
    {
        init();
    }

    void init();

    KEduVocTranslationPrivate& operator=(const KEduVocTranslationPrivate &translation);
    bool operator== ( const KEduVocTranslationPrivate &p ) const;

    //KEduVocTranslation* q;

    // This is the word itself. The vocabulary. This is what it is all about.
    QString m_translation;
    /// noun:male etc (language dependent)
    QString m_type;
    QString m_subType;
    QString m_usageLabel;
    QString m_comment;
    QString m_paraphrase;
    QString m_synonym;
    QString m_example;
    QString m_antonym;
    QString m_pronunciation;

    KEduVocConjugation m_conjugation;

    KEduVocComparison m_comparison;

    KEduVocMultipleChoice m_multipleChoice;

    // Here come all int indexFrom grades. (If you want, imagine the TO grades as int indexFrom of the other translation. That is where they belong. )
    // User is asked to give THIS here as answer, than the grades go here.
    // User answers, this is the source, grades go to the other translation.
    // Grades go to the translation the user has to type/choose/whatever.
    // not all have to be supplied
    QMap<int, KEduVocGrade> m_grades;

    /// One false friend string per other language
    QMap<int, QString> m_falseFriends;
};



void KEduVocTranslation::KEduVocTranslationPrivate::init()
{
    m_translation = QString();
}


bool KEduVocTranslation::KEduVocTranslationPrivate::operator== ( const KEduVocTranslation::KEduVocTranslationPrivate &other ) const
{
    return 	m_translation == other.m_translation &&
		m_type == other.m_type &&
        m_subType == other.m_subType &&
		m_usageLabel == other.m_usageLabel &&
		m_comment == other.m_comment &&
		m_paraphrase == other.m_paraphrase &&
		m_synonym == other.m_synonym &&
		m_example == other.m_example &&
		m_antonym == other.m_antonym &&
		m_pronunciation == other.m_pronunciation &&
		m_conjugation == other.m_conjugation &&
		m_comparison == other.m_comparison &&
		m_multipleChoice == other.m_multipleChoice &&
		m_grades.keys() == other.m_grades.keys() &&
		// uncomment this when grade class has an operator== defined
		//m_grades.values() == other.m_grades.values() &&
		m_falseFriends.keys() == other.m_falseFriends.keys() &&
		m_falseFriends.values() == other.m_falseFriends.values();
}

KEduVocTranslation::KEduVocTranslation() : d ( new KEduVocTranslationPrivate )
{
}


KEduVocTranslation::KEduVocTranslation( const QString &translation ) : d ( new KEduVocTranslationPrivate )
{
    d->m_translation = translation.simplified();
}

KEduVocTranslation::KEduVocTranslation( const KEduVocTranslation &other) : d ( new KEduVocTranslationPrivate )
{
	d->m_translation = other.d->m_translation;
    d->m_type = other.d->m_type;
    d->m_subType = other.d->m_subType;
    d->m_usageLabel = other.d->m_usageLabel;
    d->m_comment = other.d->m_comment;
    d->m_paraphrase = other.d->m_paraphrase;
    d->m_synonym = other.d->m_synonym;
    d->m_example = other.d->m_example;
    d->m_antonym = other.d->m_antonym;
    d->m_pronunciation = other.d->m_pronunciation;
	d->m_conjugation = other.d->m_conjugation;
    d->m_comparison = other.d->m_comparison;
    d->m_multipleChoice = other.d->m_multipleChoice;
    d->m_grades = other.d->m_grades;
	d->m_falseFriends = other.d->m_falseFriends;
}

KEduVocTranslation::~KEduVocTranslation()
{
    delete d;
}

QString KEduVocTranslation::translation ( ) const
{
    return d->m_translation;
}


void KEduVocTranslation::setTranslation ( const QString & expr )
{
    d->m_translation = expr.simplified();
}


/**
    @todo CHANGE ALL [index] to .value(index) in get methods! otherwise the element is automatically created!!!
*/


QString KEduVocTranslation::comment ( ) const
{
    return d->m_comment;
}


void KEduVocTranslation::setComment ( const QString & expr )
{
    d->m_comment = expr.simplified();
}


void KEduVocTranslation::setFalseFriend ( int indexFrom, const QString & expr )
{
    d->m_falseFriends[indexFrom] = expr.simplified();
}


QString KEduVocTranslation::falseFriend ( int indexFrom ) const
{
    return d->m_falseFriends.value(indexFrom);
}


void KEduVocTranslation::setSynonym (  const QString & expr )
{
    d->m_synonym = expr.simplified();
}


QString KEduVocTranslation::synonym ( ) const
{
    return d->m_synonym;
}


void KEduVocTranslation::setExample (  const QString & expr )
{
   d->m_example = expr.simplified();
}


QString KEduVocTranslation::example ( ) const
{
    return d->m_example;
}


void KEduVocTranslation::setUsageLabel (  const QString & usage )
{
    d->m_usageLabel = usage;
}


QString KEduVocTranslation::usageLabel () const
{
    return d->m_usageLabel;
}


void KEduVocTranslation::setParaphrase (  const QString & expr )
{
    d->m_paraphrase = expr.simplified();
}


QString KEduVocTranslation::paraphrase ( ) const
{
    return d->m_paraphrase;
}


void KEduVocTranslation::setAntonym (  const QString & expr )
{
    d->m_antonym = expr.simplified();
}


QString KEduVocTranslation::antonym ( ) const
{
    return d->m_antonym;
}


void KEduVocTranslation::setConjugation ( const KEduVocConjugation &con )
{
    d->m_conjugation = con;
}


KEduVocConjugation KEduVocTranslation::conjugation ( ) const
{
    return d->m_conjugation;
}


void KEduVocTranslation::setComparison (  const KEduVocComparison &con )
{
    d->m_comparison = con;
}


KEduVocComparison & KEduVocTranslation::comparison ( )
{
    return d->m_comparison;
}


void KEduVocTranslation::setMultipleChoice (  const KEduVocMultipleChoice &mc )
{
    d->m_multipleChoice = mc;
}


KEduVocMultipleChoice & KEduVocTranslation::multipleChoice ( )
{
    return d->m_multipleChoice;
}


QString KEduVocTranslation::pronunciation ( ) const
{
    return d->m_pronunciation;
}


void KEduVocTranslation::setPronunciation (  const QString & expr )
{
    d->m_pronunciation = expr.simplified();
}


QString KEduVocTranslation::type() const
{
    return d->m_type;
}


void KEduVocTranslation::setType (  const QString &type )
{
    d->m_type = type;
}

QString KEduVocTranslation::subType() const
{
    return d->m_subType;
}


void KEduVocTranslation::setSubType (  const QString &type )
{
    d->m_subType = type;
}

void KEduVocTranslation::resetGrades(){
    d->m_grades.clear();
}

/*
QList< int > KEduVocTranslation::conjugationTenses() const
{
    return m_conjugations.keys();
}*/

bool KEduVocTranslation::operator ==(const KEduVocTranslation & translation) const
{
    return d->m_translation == translation.d->m_translation &&
        d->m_type == translation.d->m_type &&
        d->m_subType == translation.d->m_subType &&
        d->m_usageLabel == translation.d->m_usageLabel &&
        d->m_comment == translation.d->m_comment &&
        d->m_paraphrase == translation.d->m_paraphrase &&
        d->m_synonym == translation.d->m_synonym &&
        d->m_example == translation.d->m_example &&
        d->m_antonym == translation.d->m_antonym &&
        d->m_pronunciation == translation.d->m_pronunciation &&
        d->m_comparison == translation.d->m_comparison &&
        d->m_multipleChoice == translation.d->m_multipleChoice
        /// @todo m_falseFriends == translation.m_falseFriends &&
        /// @todo m_grades == translation.m_grades &&
        /// @todo m_conjugations == translation.m_conjugations
        ;
}


KEduVocTranslation & KEduVocTranslation::operator =(const KEduVocTranslation & translation)
{
    d->m_translation = translation.d->m_translation;
    d->m_type = translation.d->m_type;
    d->m_subType = translation.d->m_subType;
    d->m_usageLabel = translation.d->m_usageLabel;
    d->m_comment = translation.d->m_comment;
    d->m_paraphrase = translation.d->m_paraphrase;
    d->m_synonym = translation.d->m_synonym;
    d->m_example = translation.d->m_example;
    d->m_antonym = translation.d->m_antonym;
    d->m_pronunciation = translation.d->m_pronunciation;
    d->m_comparison = translation.d->m_comparison;
    d->m_multipleChoice = translation.d->m_multipleChoice;
    /// @todo m_falseFriends == translation.m_falseFriends;
    /// @todo m_grades == translation.m_grades;
    /// @todo m_conjugations == translation.m_conjugations;
    return *this;
}

KEduVocGrade & KEduVocTranslation::gradeFrom(int indexFrom)
{
    return d->m_grades[indexFrom];
}

