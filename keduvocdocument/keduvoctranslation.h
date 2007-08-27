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


#ifndef KEDUVOCTRANSLATION_H
#define KEDUVOCTRANSLATION_H

#include "libkeduvocdocument_export.h"

#include <QtCore/QString>

#include "keduvocgrammar.h"
#include "keduvocmultiplechoice.h"

class KEduVocGrade;

/**
	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class KEDUVOCDOCUMENT_EXPORT KEduVocTranslation
{
public:
    /**
     * Default constructor for an empty translation.
     */
    KEduVocTranslation( );

	/**
     * Constructor
     * @param translation is used as translation
     */
    KEduVocTranslation( const QString &translation );

	/** copy constructor for d-pointer safet */
	KEduVocTranslation( const KEduVocTranslation &other);

    /**
     * Destructor
     */
    ~KEduVocTranslation();

    /**
     * The translation as string (the word itself)
     * @return the translation
     */
    QString translation () const;
    /**
     * Sets the translation
     * @param expr
     */
    void setTranslation ( const QString & expr );


    /** sets the pronunciation of this expression
    * @param expression       pronunciation of this index
    */
    void setPronunciation( const QString & expression);

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
    void setComment( const QString & expr);

    /** sets false friend of this expression
    * @param indexint indexFrom        index of original translation
    * @param expression       false friend of this index
    */
    void setFalseFriend( int indexFrom, const QString & expression );

    /**
    * returns false friend of this expression
    * @param indexFrom index of original translation
    * @return false friend or "" if no string available
    */
    QString falseFriend( int indexFrom) const;

    /** sets synonym this expression
    * @param expression       synonym of this index
    */
    void setSynonym( const QString & expression);

    /** returns synonym of this expression
    * @return                 synonym or "" if no string available
    */
    QString synonym() const;

    /** sets example this expression
    * @param expression       example of this index
    */
    void setExample( const QString & expression);

    /** returns example of this expression
    * @return                 example or "" if no string available
    */
    QString example() const;

    /** sets usage label this expression
    * @param usage            usage label of this index
    */
    void setUsageLabel( const QString & usage);

    /** returns usage label of this expression
    * @return                 usage or "" if no string available
    */
    QString usageLabel() const;

    /** sets paraphrase of this expression
    * @param expression       paraphrase of this index
    */
    void setParaphrase( const QString & expression);

    /** returns paraphrase of this expression
    * @return                 paraphrase or "" if no string available
    */
    QString paraphrase() const;

    /** sets antonym this expression
    * @param expression       antonym of this index
    */
    void setAntonym( const QString & expression);

    /** returns antonym of this expression
    * @return                 antonym or "" if no string available
    */
    QString antonym() const;

    /** returns type of this expression
    *
    * @return                 type or "" if no type available
    */
    QString type() const;

    /** sets type of this expression
    * @param type             type of this expression ("" = none)
    */
    void setType( const QString &type);

    /** returns subtype of this expression
    *
    * @return                 type or "" if no type available
    */
    QString subType() const;

    /** sets subtype of this expression
    * @param type             type of this expression ("" = none)
    */
    void setSubType( const QString &type);

    /** reset the grades for this translation */
    void resetGrades();

    /** get the gradeobject from given identifier
    * @param indexFrom which identifier to get the grade from
    * @returns the grade object
    */
    KEduVocGrade & gradeFrom( int indexFrom );

    /** returns conjugations if available
    */
    KEduVocConjugation conjugation() const;

    /** sets conjugations
    * @param conjugation      conjugation block
    */
    void setConjugation(const KEduVocConjugation & conjugation);

  /** returns comparison if available
   */
  KEduVocComparison & comparison();

  /** sets comparison
   * @param comparison       comparison block
   */
  void setComparison( const KEduVocComparison & comparison);

  /** returns multiple choice if available
    */
  KEduVocMultipleChoice & multipleChoice();

  /** sets multiple choice
   * @param mc               multiple choice block
    */
  void setMultipleChoice( const KEduVocMultipleChoice &mc);


    /**
     * Equal operator to assing a translation to another one.
     * @param translation translation to be copied
     * @return reference to the new translation
     */
    KEduVocTranslation& operator=(const KEduVocTranslation &translation);
    /**
     * Compare two translations, including word type etc.
     * @param translation
     * @return true if equal
     */
    bool operator==(const KEduVocTranslation &translation) const;

private:
	class KEduVocTranslationPrivate;
    KEduVocTranslationPrivate* const d;
};

#endif
