/*
 * Vocabulary Expression Translation for KDE Edu
 * SPDX-FileCopyrightText: 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KEDUVOCTRANSLATION_H
#define KEDUVOCTRANSLATION_H

#include "keduvocdocument_export.h"
#include "keduvocconjugation.h"
#include "keduvoctext.h"
#include <QUrl>
#include <QString>
#include <QMap>
class KEduVocExpression;
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
        * @param entry
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

    /** Sets the pronunciation of this expression
    * @param expression       pronunciation of this index
    */
    void setPronunciation( const QString & expression );

    /** Returns the pronunciation of this expression
    * @return                 pronunciation or "" if none available
    */
    QString pronunciation() const;

    /** Returns comments of this expression
    * @return                 comment or "" if no comment available
    */
    QString comment() const;

    /** Sets comment of this expression
    * @param expr             comment of this index
    */
    void setComment( const QString& expr );

    /** Sets example this expression
    * @param expression       example of this index
    */
    void setExample( const QString & expression );

    /** Returns example of this expression
    * @return                 example or "" if no string available
    */
    QString example() const;

    /** Sets paraphrase of this expression
    * @param expression       paraphrase of this index
    */
    void setParaphrase( const QString & expression );

    /** Returns paraphrase of this expression
    * @return                 paraphrase or "" if no string available
    */
    QString paraphrase() const;

    /** Sets antonym this expression
    * @param expression       antonym of this index
    */
    void setAntonym( const QString & expression );

    /** Returns antonym of this expression
    * @return                 antonym or "" if no string available
    */
    QString antonym() const;

    /**
     * Sets an irregular plural form.
     * @param plural plural of the word
     */
    void setIrregularPlural( const QString& plural );

    /**
     * Gets an irregular plural form.
     * @return plural of the word
     */
    QString irregularPlural() const;

    /** Returns the word type of this expression, you will get a 0 pointer
    *   if wordtype is not set for the translation
    *
    * @return                 type or "" if no type available
    */
    KEduVocWordType* wordType() const;

    /** Sets the word type of this expression
    * @param wordType             type of this expression ("" = none)
    */
    void setWordType( KEduVocWordType* wordType );

    /** Returns the leitner box of this translation
     * @return the box
     */
    KEduVocLeitnerBox* leitnerBox() const;

    /** Sets the leitner box of this translation
     * @param leitnerBox the box
     */
    void setLeitnerBox( KEduVocLeitnerBox* leitnerBox );

    /**
     * This method is deprecated, please use @see getConjugation() @see setConjugation()
     * Returns a conjugation if available
     * @param tense tense of the requested conjugation
     * @return the conjugation
     */
    KEDUVOCDOCUMENT_DEPRECATED KEduVocConjugation& conjugation(const QString &tense);

    /**
     * Returns a conjugation if available
     * @param tense tense of the requested conjugation
     * @return the conjugation
     */
    KEduVocConjugation getConjugation(const QString &tense) const;

    /** adds conjugations or replaces them, if they exist.
    * @param tense            tense
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
     * @param conjugations
     */
    void setConjugations( const QMap<QString, KEduVocConjugation>& conjugations );

    QStringList conjugationTenses() const;

    /**
     * Bad, only compatibility. Deprecated.
     * @return
     */
    KEDUVOCDOCUMENT_DEPRECATED QMap <QString, KEduVocConjugation> conjugations() const;

    /**
     * Comparison forms of adjectives/adverbs.
     */
    KEDUVOCDOCUMENT_DEPRECATED QString comparative() const;
    KEDUVOCDOCUMENT_DEPRECATED void setComparative(const QString& comparative);
    // TODO rename to comparative and remove the deprecated function
    KEduVocText comparativeForm() const;
    void setComparativeForm(const KEduVocText& comparative);

    KEDUVOCDOCUMENT_DEPRECATED QString superlative() const;
    KEDUVOCDOCUMENT_DEPRECATED void setSuperlative(const QString& superlative);
    KEduVocText superlativeForm() const;
    void setSuperlativeForm(const KEduVocText& superlative);

    KEduVocText article() const;
    void setArticle(const KEduVocText& article);

    /**
     * This method is deprecated, please use @see getMultipleChoice() @see setMultipleChoice()
     * Returns multiple choice if available
     */
    KEDUVOCDOCUMENT_DEPRECATED QStringList& multipleChoice();

    /**
     * Returns multiple choice if available
     * @return list of multiple choice answers
     */
    QStringList getMultipleChoice() const;

    /** Sets multiple choice
     * @param choices               multiple choice block
     */
    void setMultipleChoice(const QStringList &choices);

    /** Get the sound url for this translation if it exists */
    QUrl soundUrl();

    /** Set the sound url for this translation
     * @param url               url of the sound file */
    void setSoundUrl(const QUrl &url);

    /** Get the image url for this translation if it exists */
    QUrl imageUrl();

    /** Set the image url for this translation
     * @param url               url of the image
     */
    void setImageUrl(const QUrl &url);

    /**
     * Add a false friend
     * @param falseFriend false friend of this index
     */
    void addFalseFriend( KEduVocTranslation* falseFriend );

    /**
     * Remove a false friend
     * @param falseFriend false friend of this index
     */
    void removeFalseFriend( KEduVocTranslation* falseFriend );

    /**
     * Returns false friends of this expression
     * @return list of false friends
     */
    QList<KEduVocTranslation*> falseFriends() const;

    /**
     * Add a synonym
     * @param synonym
     */
    void addSynonym( KEduVocTranslation* synonym );

    /**
     * Remove a synonym
     * @param synonym
     */
    void removeSynonym( KEduVocTranslation* synonym );

    /**
     * Returns synonyms of this expression
     * @return synonyms
     */
    QList<KEduVocTranslation*> synonyms() const;

    /**
     * Add a antonym
     * @param antonym
         */
    void addAntonym( KEduVocTranslation* antonym );

    /**
     * Remove a antonym
     * @param antonym
     */
    void removeAntonym( KEduVocTranslation* antonym );

    /**
     * Returns antonyms of this expression
     * @return antonyms
     */
    QList<KEduVocTranslation*> antonyms() const;

    /**
     * Equal operator to assign a translation to another one.
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
