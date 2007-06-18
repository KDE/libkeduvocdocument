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

/*
class KEduVocTranslationPrivate
{
public:

    KEduVocTranslationPrivate () // ( KEduVocTranslation* qq )
            //: q ( qq )
    {
        kDebug() << "KEduVocTranslationPrivate()" <<endl;
        init();
    }

    void init();

    KEduVocTranslationPrivate& operator=(const KEduVocTranslationPrivate &translation);
    bool operator== ( const KEduVocTranslationPrivate &p ) const;

    //KEduVocTranslation* q;

};
*/

/*
void KEduVocTranslation::KEduVocTranslationPrivate::init()
{
kDebug() << "KEduVocTranslation::KEduVocTranslationPrivate::init()" << endl;
    m_translation = QString();
}
*/
/*
bool KEduVocTranslation::KEduVocTranslationPrivate::operator== ( const KEduVocTranslation::KEduVocTranslationPrivate &p ) const
{
    /// @todo when it's all more defined...
    return m_translation == p.m_translation;

/*
    return m_original == p.m_original &&
           m_expressionTypes == p.m_expressionTypes &&
           m_translations == p.m_translations &&
           m_remarks == p.m_remarks &&
           m_usageLabels == p.m_usageLabels &&
           m_paraphrases == p.m_paraphrases &&
           m_fauxAmi == p.m_fauxAmi &&
           m_reverseFauxAmi == p.m_reverseFauxAmi &&
           m_synonym == p.m_synonym &&
           m_example == p.m_example &&
           m_antonym == p.m_antonym &&
           m_pronunciations == p.m_pronunciations &&
           m_grades == p.m_grades &&
           m_reverseGrades == p.m_reverseGrades &&
           m_queryCounts == p.m_queryCounts &&
           m_reverseQueryCounts == p.m_reverseQueryCounts &&
           m_badCounts == p.m_badCounts &&
           m_reverseBadCounts == p.m_reverseBadCounts &&
           m_queryDates == p.m_queryDates &&
           m_reverseQueryDates == p.m_reverseQueryDates &&
           m_conjugations == p.m_conjugations &&
           m_comparisons == p.m_comparisons &&
           m_multipleChoices == p.m_multipleChoices &&
           m_leitnerBox == p.m_leitnerBox &&
           m_sortIndex == p.m_sortIndex &&
           m_inQuery == p.m_inQuery &&
           m_active == p.m_active;
           */
/*}
*/
/*
void KEduVocTranslation::KEduVocTranslationPrivate::operator= ( const KEduVocTranslation::KEduVocTranslationPrivate &p )
{
    kDebug() << "KEduVocTranslation::KEduVocTranslationPrivate::operator=" << endl;
}
*/

KEduVocTranslation::KEduVocTranslation() //: d ( new KEduVocTranslationPrivate )
{
}


KEduVocTranslation::KEduVocTranslation( const QString &translation ) //: d ( new KEduVocTranslationPrivate )
{
    m_translation = translation.simplified();
}


KEduVocTranslation::~KEduVocTranslation()
{
    //delete d;
}

QString KEduVocTranslation::translation ( ) const
{
    return m_translation;
}


void KEduVocTranslation::setTranslation ( const QString & expr )
{
    m_translation = expr.simplified();
}


/**
    @todo CHANGE ALL [index] to .value(index) in get methods! otherwise the element is automatically created!!!
*/


QString KEduVocTranslation::comment ( ) const
{
    return m_comment;
}


void KEduVocTranslation::setComment ( const QString & expr )
{
    m_comment = expr.simplified();
}


void KEduVocTranslation::setFalseFriend ( int indexFrom, const QString & expr )
{
//    m_falseFriends[indexFrom] = expr.simplified();
}


QString KEduVocTranslation::falseFriend ( int indexFrom ) const
{
//    return m_falseFriends.value(indexFrom);
return QString();
}


void KEduVocTranslation::setSynonym (  const QString & expr )
{
    m_synonym = expr.simplified();
}


QString KEduVocTranslation::synonym ( ) const
{
    return m_synonym;
}


void KEduVocTranslation::setExample (  const QString & expr )
{
   m_example = expr.simplified();
}


QString KEduVocTranslation::example ( ) const
{
    return m_example;
}


void KEduVocTranslation::setUsageLabel (  const QString & usage )
{
    m_usageLabel = usage;
}


QString KEduVocTranslation::usageLabel () const
{
    return m_usageLabel;
}


void KEduVocTranslation::setParaphrase (  const QString & expr )
{
    m_paraphrase = expr.simplified();
}


QString KEduVocTranslation::paraphrase ( ) const
{
    return m_paraphrase;
}


void KEduVocTranslation::setAntonym (  const QString & expr )
{
    m_antonym = expr.simplified();
}


QString KEduVocTranslation::antonym ( ) const
{
    return m_antonym;
}


void KEduVocTranslation::setConjugation ( const KEduVocConjugation &con )
{
    m_conjugation = con;
}


KEduVocConjugation KEduVocTranslation::conjugation ( ) const
{
    return m_conjugation;
}


void KEduVocTranslation::setComparison (  const KEduVocComparison &con )
{
    m_comparison = con;
}


KEduVocComparison KEduVocTranslation::comparison ( ) const
{
    return m_comparison;
}


void KEduVocTranslation::setMultipleChoice (  const KEduVocMultipleChoice &mc )
{
    m_multipleChoice = mc;
}


KEduVocMultipleChoice KEduVocTranslation::multipleChoice ( ) const
{
    return m_multipleChoice;
}


QString KEduVocTranslation::pronunciation ( ) const
{
    return m_pronunciation;
}


void KEduVocTranslation::setPronunciation (  const QString & expr )
{
    m_pronunciation = expr.simplified();
}


QString KEduVocTranslation::type() const
{
    return m_types;
}


void KEduVocTranslation::setType (  const QString &type )
{
    m_types = type;
}


void KEduVocTranslation::resetGrades(){
    m_grades.clear();
}

/*
QList< int > KEduVocTranslation::conjugationTenses() const
{
    return m_conjugations.keys();
}*/

bool KEduVocTranslation::operator ==(const KEduVocTranslation & translation) const
{
    return m_translation == translation.m_translation &&
        m_types == translation.m_types &&
        m_usageLabel == translation.m_usageLabel &&
        m_comment == translation.m_comment &&
        m_paraphrase == translation.m_paraphrase &&
        m_synonym == translation.m_synonym &&
        m_example == translation.m_example &&
        m_antonym == translation.m_antonym &&
        m_pronunciation == translation.m_pronunciation &&
        m_comparison == translation.m_comparison &&
        m_multipleChoice == translation.m_multipleChoice
        /// @todo m_falseFriends == translation.m_falseFriends &&
        /// @todo m_grades == translation.m_grades &&
        /// @todo m_conjugations == translation.m_conjugations
        ;
}


KEduVocTranslation & KEduVocTranslation::operator =(const KEduVocTranslation & translation)
{
    m_translation = translation.m_translation;
    m_types = translation.m_types;
    m_usageLabel = translation.m_usageLabel;
    m_comment = translation.m_comment;
    m_paraphrase = translation.m_paraphrase;
    m_synonym = translation.m_synonym;
    m_example = translation.m_example;
    m_antonym = translation.m_antonym;
    m_pronunciation = translation.m_pronunciation;
    m_comparison = translation.m_comparison;
    m_multipleChoice = translation.m_multipleChoice;
    /// @todo m_falseFriends == translation.m_falseFriends;
    /// @todo m_grades == translation.m_grades;
    /// @todo m_conjugations == translation.m_conjugations;
    return *this;
}

KEduVocGrade & KEduVocTranslation::gradeFrom(int indexFrom)
{
    return m_grades[indexFrom];
}

