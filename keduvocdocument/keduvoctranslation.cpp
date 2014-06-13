/***************************************************************************
                        Vocabulary Expression Translation for KDE Edu
    -----------------------------------------------------------------------

    Copyright 2007-2010 Frederik Gladhorn <gladhorn@kde.org>
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

#include "keduvocdeclension.h"
#include "keduvocwordtype.h"
#include "keduvocleitnerbox.h"
#include "kvtml2defs.h"
#include "keduvockvtml2writer.h"

#include <QtCore/QMap>

class KEduVocTranslation::KEduVocTranslationPrivate
{
public:
    KEduVocTranslationPrivate(KEduVocExpression* parent);

    ~KEduVocTranslationPrivate();

    KEduVocExpression* m_entry;

    /// Type of a word noun, verb, adjective etc
    KEduVocWordType* m_wordType;

    /// Leitner box of the translation.
    KEduVocLeitnerBox* m_leitnerBox;

    /// A comment giving additional information.
    QString m_comment;
    /// A hint, to make guessing the word easier.
    QString m_hint;
    /// Paraphrase
    QString m_paraphrase;
    /// An example
    QString m_example;
    /// Pronunciation
    QString m_pronunciation;
    /// Image url
    QUrl m_imageUrl;
    /// Sound url
    QUrl m_soundUrl;

    /// When creating multiple choice tests, these are possible answers. (otherwise other words are added randomly)
    QStringList m_multipleChoice;

    /// Conjugations of a word (I go, you go, he goes... boring in english)
    QMap <QString, KEduVocConjugation> m_conjugations;

    /// The comparison forms of adjectives and adverbs: (fast), faster, fastest
    KEduVocText* m_comparative;
    KEduVocText* m_superlative;

    /// The grade of an article. The text part should not be used.
    KEduVocText* m_articleGrade;

    KEduVocDeclension* m_declension;

    // connections to other translations
    /// Synonyms for a word: sick and ill, student and pupil
    QList< KEduVocTranslation* > m_synonyms;
    /// An antonym - the opposite: hot - cold
    QList< KEduVocTranslation* > m_antonyms;
    /// List of false friends
    QList< KEduVocTranslation* > m_falseFriends;
};

KEduVocTranslation::KEduVocTranslationPrivate::KEduVocTranslationPrivate(KEduVocExpression* parent)
{
    m_entry = parent;
    m_wordType = 0;
    m_leitnerBox = 0;
    m_declension = 0;

    m_comparative = 0;
    m_superlative = 0;
    m_articleGrade = 0;
}


KEduVocTranslation::KEduVocTranslationPrivate::~KEduVocTranslationPrivate()
{
    delete m_declension;
}

KEduVocTranslation::KEduVocTranslation(KEduVocExpression* entry) : d( new KEduVocTranslationPrivate(entry) )
{
}


KEduVocTranslation::KEduVocTranslation(KEduVocExpression* entry, const QString &translation ) : d( new KEduVocTranslationPrivate(entry) )
{
    setText(translation.simplified());
}

KEduVocTranslation::KEduVocTranslation( const KEduVocTranslation &other )
    : KEduVocText(other),
    // set the entry to 0, the translation will be put into a copied entry by the expression copy constructor
    d( new KEduVocTranslationPrivate(0) )
{
    // beter no word type copy as this is pointer copying
    // will not work as this is not added to the word type container!
//  d->m_wordType = other.d->m_wordType;
//  d->m_leitnerBox = translation.d->m_leitnerBox;
    d->m_comment = other.d->m_comment;
    d->m_paraphrase = other.d->m_paraphrase;
    d->m_example = other.d->m_example;
    d->m_pronunciation = other.d->m_pronunciation;
    d->m_conjugations = other.d->m_conjugations;
    d->m_comparative = other.d->m_comparative;
    d->m_superlative = other.d->m_superlative;
    d->m_multipleChoice = other.d->m_multipleChoice;
    d->m_imageUrl = other.d->m_imageUrl;
    d->m_soundUrl = other.d->m_soundUrl;
//  no copies of the following for now. we don't know enough to also add this as synonym/etc
//  d->m_synonyms = other.d->m_synonyms;
//  d->m_antonyms = other.d->m_antonyms;
//  d->m_falseFriends = other.d->m_falseFriends;
    if (other.d->m_declension) {
        d->m_declension = new KEduVocDeclension(*other.d->m_declension);
    }
}

KEduVocTranslation::~KEduVocTranslation()
{
    setWordType(0);
    setLeitnerBox(0);
    foreach (KEduVocTranslation *synonym, d->m_synonyms) {
        synonym->removeSynonym(this);
    }
    foreach (KEduVocTranslation *antonym, d->m_antonyms) {
        antonym->removeAntonym(this);
    }
    foreach (KEduVocTranslation *falseFriend, d->m_falseFriends) {
        falseFriend->removeFalseFriend(this);
    }
    delete d;
}

bool KEduVocTranslation::operator == ( const KEduVocTranslation & translation ) const
{
    return KEduVocText::operator==(translation) &&
        d->m_wordType == translation.d->m_wordType &&
        d->m_leitnerBox == translation.d->m_leitnerBox &&
        d->m_comment == translation.d->m_comment &&
        d->m_paraphrase == translation.d->m_paraphrase &&
        d->m_example == translation.d->m_example &&
        d->m_pronunciation == translation.d->m_pronunciation &&
        d->m_imageUrl == translation.d->m_imageUrl &&
        d->m_soundUrl == translation.d->m_soundUrl &&
        d->m_comparative == translation.d->m_comparative &&
        d->m_superlative == translation.d->m_superlative &&
        d->m_multipleChoice == translation.d->m_multipleChoice &&
        d->m_synonyms == translation.d->m_synonyms &&
        d->m_antonyms == translation.d->m_antonyms &&
        d->m_falseFriends == translation.d->m_falseFriends &&
        d->m_conjugations == translation.d->m_conjugations;
           /// @todo check and include declensions d->m_declension == translation.d->m_declension;
}

KEduVocTranslation & KEduVocTranslation::operator = ( const KEduVocTranslation & translation )
{
    KEduVocText::operator=(translation);
    d->m_entry = translation.d->m_entry;
//     d->m_wordType = translation.d->m_wordType;
//     d->m_leitnerBox = translation.d->m_leitnerBox;
    d->m_comment = translation.d->m_comment;
    d->m_paraphrase = translation.d->m_paraphrase;
    d->m_example = translation.d->m_example;
    d->m_pronunciation = translation.d->m_pronunciation;
    d->m_imageUrl = translation.d->m_imageUrl;
    d->m_soundUrl = translation.d->m_soundUrl;
    d->m_comparative = translation.d->m_comparative;
    d->m_superlative = translation.d->m_superlative;
    d->m_multipleChoice = translation.d->m_multipleChoice;
    d->m_falseFriends = translation.d->m_falseFriends;
    d->m_synonyms = translation.d->m_synonyms;
    d->m_antonyms = translation.d->m_antonyms;
    d->m_conjugations = translation.d->m_conjugations;
    if (translation.d->m_declension) {
        d->m_declension = new KEduVocDeclension(*translation.d->m_declension);
    }

    return *this;
}


QString KEduVocTranslation::comment() const
{
    return d->m_comment;
}


void KEduVocTranslation::setComment( const QString & expr )
{
    d->m_comment = expr.simplified();
}


void KEduVocTranslation::addFalseFriend( KEduVocTranslation* falseFriend )
{
    d->m_falseFriends.append(falseFriend);
}

void KEduVocTranslation::removeFalseFriend(KEduVocTranslation * falseFriend)
{
    d->m_falseFriends.removeAt(d->m_falseFriends.indexOf(falseFriend));
}

QList< KEduVocTranslation* > KEduVocTranslation::falseFriends() const
{
    return d->m_falseFriends;
}


void KEduVocTranslation::addSynonym( KEduVocTranslation* synonym )
{
    d->m_synonyms.append(synonym);
}

void KEduVocTranslation::removeSynonym(KEduVocTranslation * synonym)
{
    d->m_synonyms.removeAt(d->m_synonyms.indexOf(synonym));
}

QList<KEduVocTranslation*> KEduVocTranslation::synonyms() const
{
    return d->m_synonyms;
}

void KEduVocTranslation::addAntonym( KEduVocTranslation* antonym )
{
    d->m_antonyms.append(antonym);
}

QList<KEduVocTranslation*> KEduVocTranslation::antonyms() const
{
    return d->m_antonyms;
}

void KEduVocTranslation::removeAntonym(KEduVocTranslation * antonym)
{
    d->m_antonyms.removeAt(d->m_antonyms.indexOf(antonym));
}

void KEduVocTranslation::setExample( const QString & expr )
{
    d->m_example = expr.simplified();
}


QString KEduVocTranslation::example() const
{
    return d->m_example;
}


void KEduVocTranslation::setParaphrase( const QString & expr )
{
    d->m_paraphrase = expr.simplified();
}


QString KEduVocTranslation::paraphrase() const
{
    return d->m_paraphrase;
}


void KEduVocTranslation::setConjugation( const QString& tense, const KEduVocConjugation& con )
{
    d->m_conjugations[tense] = con;
}


KEduVocConjugation& KEduVocTranslation::conjugation( const QString& tense )
{
    return d->m_conjugations[tense];
}


QStringList & KEduVocTranslation::multipleChoice()
{
    return d->m_multipleChoice;
}


QString KEduVocTranslation::pronunciation() const
{
    return d->m_pronunciation;
}


void KEduVocTranslation::setPronunciation( const QString & expr )
{
    d->m_pronunciation = expr.simplified();
}

QStringList KEduVocTranslation::conjugationTenses() const
{
    return d->m_conjugations.keys();
}

QMap< QString, KEduVocConjugation > KEduVocTranslation::conjugations() const
{
    return d->m_conjugations;
}

void KEduVocTranslation::setConjugations(const QMap< QString, KEduVocConjugation > & conjugations)
{
    d->m_conjugations = conjugations;
}

/** get the sound url for this translation if it exists */
QUrl KEduVocTranslation::soundUrl()
{
    return d->m_soundUrl;
}

/** set the sound url for this translation
 * @param url               url of the sound file */
void KEduVocTranslation::setSoundUrl(const QUrl &url)
{
    d->m_soundUrl = url;
}

/** get the image url for this translation if it exists */
QUrl KEduVocTranslation::imageUrl()
{
    return d->m_imageUrl;
}

/** set the image url for this translation
 * @param url               url of the image
 */
void KEduVocTranslation::setImageUrl(const QUrl &url)
{
    d->m_imageUrl = url;
}

KEduVocWordType * KEduVocTranslation::wordType() const
{
    if (d) {
        return d->m_wordType;
    } else {
        return 0;
    }
}

void KEduVocTranslation::setWordType(KEduVocWordType * wordType)
{
    if ( d->m_wordType ) {
        d->m_wordType->removeTranslation(this);
    }
    if ( wordType ) {
        wordType->addTranslation(this);
    }
    d->m_wordType = wordType;
}

KEduVocLeitnerBox * KEduVocTranslation::leitnerBox() const
{
    return d->m_leitnerBox;
}

void KEduVocTranslation::setLeitnerBox(KEduVocLeitnerBox * leitnerBox)
{
    if ( d->m_leitnerBox ) {
        d->m_leitnerBox->removeTranslation(this);
    }
    if ( leitnerBox ) {
        leitnerBox->addTranslation(this);
    }
    d->m_leitnerBox = leitnerBox;
}

KEduVocExpression * KEduVocTranslation::entry()
{
    return d->m_entry;
}

QString KEduVocTranslation::comparative() const
{
    if (d->m_comparative) {
        return d->m_comparative->text();
    }
    return QString();
}

void KEduVocTranslation::setComparative(const QString & comparative)
{
    if (!d->m_comparative) {
        d->m_comparative = new KEduVocText(comparative);
    } else {
        d->m_comparative->setText(comparative);
    }
}

QString KEduVocTranslation::superlative() const
{
    if (d->m_superlative) {
        return d->m_superlative->text();
    }
    return QString();
}

void KEduVocTranslation::setSuperlative(const QString & superlative)
{
    if (!d->m_superlative) {
        d->m_superlative = new KEduVocText(superlative);
    } else {
        d->m_superlative->setText(superlative);
    }
}

KEduVocText KEduVocTranslation::comparativeForm() const
{
    if (!d->m_comparative) {
        return KEduVocText();
    }
    KEduVocText t(*(d->m_comparative));
    return t;
}

void KEduVocTranslation::setComparativeForm(const KEduVocText& comparative)
{
    if (!d->m_comparative) {
        d->m_comparative = new KEduVocText();
    }
    *(d->m_comparative) = comparative;
}

KEduVocText KEduVocTranslation::superlativeForm() const
{
    if (!d->m_superlative) {
        return KEduVocText();
    }
    KEduVocText t(*d->m_superlative);
    return t;
}

void KEduVocTranslation::setSuperlativeForm(const KEduVocText& superlative)
{
    if (!d->m_superlative) {
        d->m_superlative = new KEduVocText();
    }
    *d->m_superlative = superlative;
}

KEduVocText KEduVocTranslation::article() const
{
    if (!d->m_articleGrade) {
        return KEduVocText();
    }
    KEduVocText t(*d->m_articleGrade);
    return t;
}

void KEduVocTranslation::setArticle(const KEduVocText& article)
{
    if (!d->m_articleGrade) {
        d->m_articleGrade = new KEduVocText();
    }
    *d->m_articleGrade = article;
}

KEduVocDeclension * KEduVocTranslation::declension()
{
    return d->m_declension;
}

void KEduVocTranslation::setDeclension(KEduVocDeclension * declension)
{
    // remove the old declension object
    delete d->m_declension;
    d->m_declension = declension;
}

void KEduVocTranslation::toKVTML2(QDomElement & parent)
{
    // text and grade
    KEduVocText::toKVTML2(parent);

    // declension
    if (d->m_declension) {
        d->m_declension->toKVTML2(parent);
    }

    // conjugation
    foreach ( const QString &tense, conjugationTenses() ) {
        QDomElement conjugationElement = parent.ownerDocument().createElement( KVTML_CONJUGATION );
        conjugation(tense).toKVTML2(conjugationElement, tense);
        if (conjugationElement.hasChildNodes()) {
            parent.appendChild( conjugationElement );
        }
    }

    // <comment>
    KEduVocKvtml2Writer::appendTextElement( parent, KVTML_COMMENT, comment() );

    // <pronunciation>
    KEduVocKvtml2Writer::appendTextElement( parent, KVTML_PRONUNCIATION, pronunciation() );

    // <example>
    KEduVocKvtml2Writer::appendTextElement( parent, KVTML_EXAMPLE, example() );

    // <paraphrase>
    KEduVocKvtml2Writer::appendTextElement( parent, KVTML_PARAPHRASE, paraphrase() );

    ///@todo synonyms, antonyms
    ///@todo false friends
}

void KEduVocTranslation::fromKVTML2(QDomElement & parent)
{
    KEduVocText::fromKVTML2(parent);

    setDeclension(KEduVocDeclension::fromKVTML2(parent));

    setComment( parent.firstChildElement( KVTML_COMMENT ).text() );

    setPronunciation( parent.firstChildElement( KVTML_PRONUNCIATION ).text() );

    //<example></example>
    setExample( parent.firstChildElement( KVTML_EXAMPLE ).text() );

    //<paraphrase></paraphrase>
    setParaphrase( parent.firstChildElement( KVTML_PARAPHRASE ).text() );

    // conjugations
    QDomElement conjugationElement = parent.firstChildElement( KVTML_CONJUGATION );
    while ( !conjugationElement.isNull() ) {
        QDomElement tenseElement = conjugationElement.firstChildElement( KVTML_TENSE );
        QString tense = tenseElement.text();
        KEduVocConjugation *conjugation = KEduVocConjugation::fromKVTML2(conjugationElement);
        setConjugation(tense, *conjugation);
        delete conjugation;
        conjugationElement = conjugationElement.nextSiblingElement( KVTML_CONJUGATION );
    }

    ///@todo synonyms, antonym
    ///@todo false friends
}

void KEduVocTranslation::setEntry(KEduVocExpression * entry)
{
    d->m_entry = entry;
}



