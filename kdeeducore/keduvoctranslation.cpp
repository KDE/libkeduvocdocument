//
// C++ Implementation: KEduVocTranslationtranslation
//
// Description:
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@kdemail.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

//#include "keduvocexpression.h"

#include "keduvoctranslation.h"
#include <KDebug>

class KEduVocTranslationPrivate
{
public:
/*
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
*/
    QString m_translation; // This is the word itself. The vocabulary. This is what it is all about.

//     QString m_types; // noun:male etc (language dependend)
//     QString m_usageLabel;
//     QString m_comment;
//     QString m_paraphrase;
//     QString m_synonym;
//     QString m_example;
//     QString m_antonym;
//     QString m_pronunciation;
//
//     //QMap<int, KEduVocConjugation> m_conjugations; /// Conjugations are numbered 0..n with respect to the tenses they are for.
//
//     KEduVocComparison m_comparison;
//
//     KEduVocMultipleChoice m_multipleChoice;

        // Here come all int indexFrom grades. (If you want, imagine the TO grades as int indexFrom of the other translation. That is where they belong. )
    // User is asked to give THIS here as answer, than the grades go here.
    // User answers, this is the source, grades go to the other translation.
    // Grades go to the translation the user has to supply.
    //not all have to be supplied

    //QMap<int, TranslationGrade> m_grades;

    // one false friend per other language is allowed
    //QMap<int, QString> m_falseFriends;

};

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


// QString KEduVocTranslation::comment ( ) const
// {
//     return d->m_comment;
// }
//
//
// void KEduVocTranslation::setComment ( const QString & expr )
// {
//     d->m_comment = expr.simplified();
// }
//
//
// void KEduVocTranslation::setFalseFriend ( int indexFrom, const QString & expr )
// {
// //    d->m_falseFriends[indexFrom] = expr.simplified();
// }
//
//
// QString KEduVocTranslation::falseFriend ( int indexFrom ) const
// {
// //    return d->m_falseFriends.value(indexFrom);
// return QString();
// }
//
//
// void KEduVocTranslation::setSynonym (  const QString & expr )
// {
//     d->m_synonym = expr.simplified();
// }
//
//
// QString KEduVocTranslation::synonym ( ) const
// {
//     return d->m_synonym;
// }
//
//
// void KEduVocTranslation::setExample (  const QString & expr )
// {
//    d->m_example = expr.simplified();
// }
//
//
// QString KEduVocTranslation::example ( ) const
// {
//     return d->m_example;
// }
//
//
// void KEduVocTranslation::setUsageLabel (  const QString & usage )
// {
//     d->m_usageLabel = usage;
// }
//
//
// QString KEduVocTranslation::usageLabel () const
// {
//     return d->m_usageLabel;
// }
//
//
// void KEduVocTranslation::setParaphrase (  const QString & expr )
// {
//     d->m_paraphrase = expr.simplified();
// }
//
//
// QString KEduVocTranslation::paraphrase ( ) const
// {
//     return d->m_paraphrase;
// }
//
//
// void KEduVocTranslation::setAntonym (  const QString & expr )
// {
//     d->m_antonym = expr.simplified();
// }
//
//
// QString KEduVocTranslation::antonym ( ) const
// {
//     return d->m_antonym;
// }
//
//
// void KEduVocTranslation::setConjugation ( int tense, const KEduVocConjugation &con )
// {
// //    d->m_conjugations[tense] = con;
// }
//
//
// KEduVocConjugation KEduVocTranslation::conjugation ( int tense ) const
// {
// //    return d->m_conjugations.value(tense);
// }
//
//
// void KEduVocTranslation::setComparison (  const KEduVocComparison &con )
// {
//     d->m_comparison = con;
// }
//
//
// KEduVocComparison KEduVocTranslation::comparison ( ) const
// {
//     return d->m_comparison;
// }
//
//
// void KEduVocTranslation::setMultipleChoice (  const KEduVocMultipleChoice &mc )
// {
//     d->m_multipleChoice = mc;
// }
//
//
// KEduVocMultipleChoice KEduVocTranslation::multipleChoice ( ) const
// {
//     return d->m_multipleChoice;
// }
//
//
// QString KEduVocTranslation::pronunciation ( ) const
// {
//     return d->m_pronunciation;
// }
//
//
// void KEduVocTranslation::setPronunciation (  const QString & expr )
// {
//     d->m_pronunciation = expr.simplified();
// }
//
//
// grade_t KEduVocTranslation::grade (  int indexFrom ) const
// {
// //    return d->m_grades.value(indexFrom).m_grade;
// }
//
//
// void KEduVocTranslation::setGrade (  int indexFrom, grade_t grade )
// {
//     if ( grade > KV_MAX_GRADE )
//         grade = KV_MAX_GRADE;
//     if ( grade < KV_MIN_GRADE )
//         grade = KV_MIN_GRADE;
// //    d->m_grades[indexFrom].m_grade = grade;
// }
//
//
// void KEduVocTranslation::incGrade (  int indexFrom )
// {
//     setGrade ( indexFrom, grade( indexFrom) + 1 );
// }
//
//
// void KEduVocTranslation::decGrade (  int indexFrom )
// {
//     setGrade ( indexFrom, grade( indexFrom) - 1 );
// }
//
//
// count_t KEduVocTranslation::queryCount (  int indexFrom )  const
// {
// //    return d->m_grades.value(indexFrom).m_queryCount;
// }
//
//
// void KEduVocTranslation::setQueryCount (  int indexFrom, count_t count )
// {
// //    d->m_grades[indexFrom].m_queryCount = count;
// }
//
//
// count_t KEduVocTranslation::badCount (  int indexFrom ) const
// {
// //    return d->m_grades.value(indexFrom).m_badCount;
// }
//
//
// void KEduVocTranslation::setBadCount (  int indexFrom, count_t count )
// {
// //    d->m_grades[indexFrom].m_badCount = count;
// }
//
//
// QDateTime KEduVocTranslation::queryDate (  int indexFrom ) const
// {
// //    return d->m_grades.value(indexFrom).m_queryDate;
// }
//
//
// void KEduVocTranslation::setQueryDate (  int indexFrom, const QDateTime & date )
// {
// //    d->m_grades[indexFrom].m_queryDate = date;
// }
//
//
// bool KEduVocTranslation::uniqueType() const
// { ///@todo what is this??? I have no clue... help!
// /*
//     bool unique = true;
//     QString type0 = type ( 0 );
//     for ( int i = 1; i < translationCount(); i++ )
//         if ( type0 != type ( i ) )
//             unique = false;
//     return unique; */
//     return false;
// }
//
//
// QString KEduVocTranslation::type() const
// {
//     return d->m_types;
// }
//
//
// void KEduVocTranslation::setType (  const QString &type )
// {
//     d->m_types = type;
// }
//
//
// void KEduVocTranslation::incQueryCount (  int indexFrom )
// {
//     setQueryCount ( indexFrom, queryCount ( indexFrom ) + 1 );
// }
//
//
// void KEduVocTranslation::incBadCount (  int indexFrom )
// {
//     setBadCount ( indexFrom, badCount ( indexFrom ) + 1 );
// }
//
//
// void KEduVocTranslation::resetGrades(){
// //    d->m_grades.clear();
// }
//
//
// QList< int > KEduVocTranslation::conjugationTenses() const
// {
// //    return d->m_conjugations.keys();
// }

bool KEduVocTranslation::operator ==(const KEduVocTranslation & translation) const
{
    return m_translation == translation.m_translation;
    //return ( *d == *translation.d );
}


KEduVocTranslation & KEduVocTranslation::operator =(const KEduVocTranslation & translation)
{
    // Check for self-assignment!
    if (this == &translation) {      // Same object?
      return *this;        // Yes, so skip assignment, and just return *this.
      kDebug() << "Warning - self assignment" << endl;
    }
//kDebug() << "KEduVocTranslation & KEduVocTranslation::operator =(const KEduVocTranslation & translation)" << translation.d->m_translation << " d: " << d << endl;

    m_translation = translation.m_translation;

//    *d = *translation.d;

//    d->m_translation = translation.d->m_translation;
kDebug() << "KEduVocTranslation & KEduVocTranslation::operator =(const KEduVocTranslation & translation)" << translation.m_translation << "This: " << this << endl;
    return *this;
}

