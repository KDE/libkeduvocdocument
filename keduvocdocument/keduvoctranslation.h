/***************************************************************************
                        Vocabulary Expression Translation for KDE Edu
    -----------------------------------------------------------------------
    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef KEDUVOCTRANSLATION_H
#define KEDUVOCTRANSLATION_H

#include "libkeduvocdocument_export.h"
#include "keduvocgrammar.h"
#include "keduvocconjugation.h"
#include "keduvoctext.h"
#include <KDE/KUrl>
#include <QtCore/QString>

class KEduVocExpression;
class KEduVocString;
class KEduVocWordType;

/**
 @author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class KEDUVOCDOCUMENT_EXPORT KEduVocTranslation
    :public KEduVocText
{
public:
    /**
     * Default constructor for an empty translation.
     */
    KEduVocTranslation(KEduVocExpression* entry);

    /**
        * Constructor
        * @param translation is used as translation
        */
    KEduVocTranslation(KEduVocExpression* entry, const QString &translation );

    /** copy constructor for d-pointer safet */
    KEduVocTranslation( const KEduVocTranslation &other );

    /**
     * Destructor
     */
    ~KEduVocTranslation();

    KEduVocExpression* entry();

    /** sets the pronunciation of this expression
    * @param expression       pronunciation of this index
    */
    void setPronunciation( const QString & expression );

    /** returns the pronunciation of this expression
    * @return                 pronunciation or "" if none available
    */
    QString pronunciation() const;

    /** returns comments of this expression
    * @return                 comment or "" if no comment available
    */
    QString comment() const;

    /** sets comment of this expression
    * @param expr             comment of this index
    */
    void setComment( const QString & expr );

    /** sets false friend of this expression
    * @param indexFrom        index of original translation
    * @param expression       false friend of this index
    */
    void setFalseFriend( int indexFrom, const QString & expression );

    /**
    * returns false friend of this expression
    * @param indexFrom index of original translation
    * @return false friend or "" if no string available
    */
    QString falseFriend( int indexFrom ) const;

    /** sets synonym this expression
    * @param expression       synonym of this index
    */
    void setSynonym( const QString & expression );

    /** returns synonym of this expression
    * @return                 synonym or "" if no string available
    */
    QString synonym() const;

    /** sets example this expression
    * @param expression       example of this index
    */
    void setExample( const QString & expression );

    /** returns example of this expression
    * @return                 example or "" if no string available
    */
    QString example() const;

    /** sets paraphrase of this expression
    * @param expression       paraphrase of this index
    */
    void setParaphrase( const QString & expression );

    /** returns paraphrase of this expression
    * @return                 paraphrase or "" if no string available
    */
    QString paraphrase() const;

    /** sets antonym this expression
    * @param expression       antonym of this index
    */
    void setAntonym( const QString & expression );

    /** returns antonym of this expression
    * @return                 antonym or "" if no string available
    */
    QString antonym() const;

    /**
     * Set a irregular plural form.
     * @param plural plural of the word
     */
    void setIrregularPlural( const QString& plural );

    /**
     * Get a irregular plural form.
     * @return plural of the word
     */
    QString irregularPlural() const;

    /** returns the word type of this expression
    *
    * @return                 type or "" if no type available
    */
    KEduVocWordType* wordType() const;

    /** sets the word type of this expression
    * @param type             type of this expression ("" = none)
    */
    void setWordType( KEduVocWordType* wordType );

    /** returns a conjugation if available
    */
    KEduVocConjugation& conjugation( const QString& tense );

    /** adds conjugations or replaces them, if they exist.
    * @param conjugation      conjugation
    */
    void setConjugation( const QString& tense, const KEduVocConjugation & conjugation );

    /**
     * Bad, only used for tense entry page, will be deleted later. Deprecated.
     * @param conjugation
     */
    void setConjugations( const QMap<QString, KEduVocConjugation>& conjugations );

    QStringList conjugationTenses() const;

    /**
     * Bad, only compatibility. Deprecated.
     * @return
     */
    QMap <QString, KEduVocConjugation> conjugations() const;

    /**
     * Comparison forms of adjectives/adverbs.
     */
    QString comparative() const;
    void setComparative(const QString& comparative);
    QString superlative() const;
    void setSuperlative(const QString& superlative);

    /** returns multiple choice if available
      */
    QStringList & multipleChoice();

    /** sets multiple choice
     * @param mc               multiple choice block
      */
//     void setMultipleChoice( const QStringList &mc );

    /** get the sound url for this translation if it exists */
    KUrl soundUrl();

    /** set the sound url for this translation
     * @param url               url of the sound file */
    void setSoundUrl(const KUrl &url);

    /** get the image url for this translation if it exists */
    KUrl imageUrl();

    /** set the image url for this translation
     * @param url               url of the image
     */
    void setImageUrl(const KUrl &url);

    /**
     * Equal operator to assing a translation to another one.
     * @param translation translation to be copied
     * @return reference to the new translation
     */
    KEduVocTranslation& operator= ( const KEduVocTranslation &translation );

    /**
     * Compare two translations, including word type etc.
     * @param translation
     * @return true if equal
     */
    bool operator== ( const KEduVocTranslation &translation ) const;

private:
    class KEduVocTranslationPrivate;
    KEduVocTranslationPrivate* const d;
};

#endif
