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
#include "keduvocarticle.h"
#include "keduvocconjugation.h"
#include "keduvoctext.h"
#include <KDE/KUrl>
#include <QtCore/QString>

class KEduVocExpression;
class KEduVocString;
class KEduVocWordType;
class KEduVocLeitnerBox;
class KEduVocDeclension;

/**
 @author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class KEDUVOCDOCUMENT_EXPORT KEduVocTranslation
    :public KEduVocText
{
public:
    enum Related {
        Synonym,
        Antonym,
        FalseFriend
    };

    /**
     * Default constructor for an empty translation.
     */
    KEduVocTranslation(KEduVocExpression* entry);

    /**
        * Constructor
        * @param translation is used as translation
        */
    KEduVocTranslation(KEduVocExpression* entry, const QString &translation );

    /** copy constructor for d-pointer safety */
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

    /** returns the word type of this expression, you will get a 0 pointer
    *   if wordtype isn't set for the translation
    *
    * @return                 type or "" if no type available
    */
    KEduVocWordType* wordType() const;

    /** sets the word type of this expression
    * @param type             type of this expression ("" = none)
    */
    void setWordType( KEduVocWordType* wordType );

    /** returns the leitner box of this translation
     * @return the box
     */
    KEduVocLeitnerBox* leitnerBox() const;

    /** sets the leitner box of this translation
     * @param leitnerBox the box
     */
    void setLeitnerBox( KEduVocLeitnerBox* leitnerBox );

    /**
     * returns a conjugation if available
     * @param tense tense of the requested conjugation
     * @return the conjugation
     */
    KEduVocConjugation& conjugation( const QString& tense );

    /** adds conjugations or replaces them, if they exist.
    * @param conjugation      conjugation
    */
    void setConjugation( const QString& tense, const KEduVocConjugation & conjugation );

    /**
     * Returns a pointer to the declension object of this translation.
     * Returns 0 if no declension object exists!
     * @return the declension
     */
    KEduVocDeclension* declension();

    /**
     * Set a new declension for a translation
     * @param declension
     */
    void setDeclension(KEduVocDeclension* declension);

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
     * add a false friend
     * @param falseFriend false friend of this index
     */
    void addFalseFriend( KEduVocTranslation* falseFriend );

    /**
     * remove a false friend
     * @param falseFriend false friend of this index
     */
    void removeFalseFriend( KEduVocTranslation* falseFriend );

    /**
     * returns false friends of this expression
     * @return list of false friends
     */
    QList<KEduVocTranslation*> falseFriends() const;

    /**
     * add a synonym
     * @param synonym
     */
    void addSynonym( KEduVocTranslation* synonym );

    /**
     * remove a synonym
     * @param synonym
     */
    void removeSynonym( KEduVocTranslation* synonym );

    /**
     * returns synonyms of this expression
     * @return synonyms
     */
    QList<KEduVocTranslation*> synonyms() const;

    /**
     * add a antonym
     * @param antonym
         */
    void addAntonym( KEduVocTranslation* antonym );

    /**
     * remove a antonym
     * @param antonym
     */
    void removeAntonym( KEduVocTranslation* antonym );

    /**
     * returns antonyms of this expression
     * @return antonyms
     */
    QList<KEduVocTranslation*> antonyms() const;

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

    void fromKVTML2(QDomElement& parent);
    void toKVTML2(QDomElement& parent);

private:
    class KEduVocTranslationPrivate;
    KEduVocTranslationPrivate* const d;

    // for the copy constructor
    void setEntry(KEduVocExpression* entry);
    friend class KEduVocExpression;
};

#endif
