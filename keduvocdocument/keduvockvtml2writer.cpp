/***************************************************************************
                   export a KEduVocDocument to a KVTML file
    -----------------------------------------------------------------------
    copyright           : (C) 2007 Jeremy Whiting <jeremy@scitools.com>
                          (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvockvtml2writer.h"

#include <QtCore/QTextStream>
#include <QtCore/QFile>

#include <KDebug>

#include "keduvocdocument.h"
#include "keduvocexpression.h"
#include "keduvocgrade.h"
#include "keduvoclesson.h"
#include "keduvocwordtype.h"
#include "kvtml2defs.h"

KEduVocKvtml2Writer::KEduVocKvtml2Writer( QFile *file )
{
    // the file must be already open
    m_outputFile = file;
}


bool KEduVocKvtml2Writer::writeDoc( KEduVocDocument *doc, const QString &generator )
{
    m_doc = doc;

    m_domDoc = QDomDocument( "kvtml PUBLIC \"kvtml2.dtd\" \"http://edu.kde.org/kanagram/kvtml2.dtd\"" );
    m_domDoc.appendChild( m_domDoc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"UTF-8\"" ) );
    QDomElement domElementKvtml = m_domDoc.createElement( "kvtml" );
    m_domDoc.appendChild( domElementKvtml );

    domElementKvtml.setAttribute( KVTML_VERSION, ( QString ) "2.0" );

    // information group
    QDomElement currentElement = m_domDoc.createElement( KVTML_INFORMATION );
    writeInformation( currentElement, generator );
    domElementKvtml.appendChild( currentElement );

    // identifiers
    currentElement = m_domDoc.createElement( KVTML_IDENTIFIERS );
    writeIdentifiers( currentElement );
    domElementKvtml.appendChild( currentElement );

    // types
    currentElement = m_domDoc.createElement( KVTML_WORDTYPEDEFINITIONS );
    writeTypes( currentElement );
    if ( currentElement.hasChildNodes() ) {
        domElementKvtml.appendChild( currentElement );
    }

    // tenses
    currentElement = m_domDoc.createElement( KVTML_TENSES );
    writeTenses( currentElement );
    if ( currentElement.hasChildNodes() ) {
        domElementKvtml.appendChild( currentElement );
    }

    // usages
    currentElement = m_domDoc.createElement( KVTML_USAGES );
    writeUsages( currentElement );
    if ( currentElement.hasChildNodes() ) {
        domElementKvtml.appendChild( currentElement );
    }

    // entries
    currentElement = m_domDoc.createElement( KVTML_ENTRIES );
    if ( !writeEntries( currentElement ) ) {
        // at least one entry is required!
        return false;
    }
    domElementKvtml.appendChild( currentElement );

    // lessons
    currentElement = m_domDoc.createElement( KVTML_LESSONS );
    writeLessons( currentElement );
    if ( currentElement.hasChildNodes() ) {
        domElementKvtml.appendChild( currentElement );
    }

    m_domDoc.appendChild( domElementKvtml );

    QTextStream ts( m_outputFile );
    m_domDoc.save( ts, 2 );

    return true;
}

bool KEduVocKvtml2Writer::writeInformation( QDomElement &informationElement, const QString &generator )
{
    QDomElement currentElement;
    QDomText textNode;

    // generator
    informationElement.appendChild( newTextElement( KVTML_GENERATOR, generator ) );

    // title
    if ( !m_doc->title().isEmpty() ) {
        informationElement.appendChild( newTextElement( KVTML_TITLE, m_doc->title() ) );
    }

    // author
    if ( !m_doc->author().isEmpty() ) {
        informationElement.appendChild( newTextElement( KVTML_AUTHOR, m_doc->author() ) );
    }

    // license
    if ( !m_doc->license().isEmpty() ) {
        informationElement.appendChild( newTextElement( KVTML_LICENSE, m_doc->license() ) );
    }

    // comment
    if ( !m_doc->documentComment().isEmpty() ) {
        informationElement.appendChild( newTextElement( KVTML_COMMENT, m_doc->documentComment() ) );
    }

    // category
    if ( !m_doc->category().isEmpty() ) {
        informationElement.appendChild( newTextElement( KVTML_CATEGORY, m_doc->category() ) );
    }

    return true;
}


bool KEduVocKvtml2Writer::writeIdentifiers( QDomElement &identifiersElement )
{
    for ( int i = 0; i < m_doc->identifierCount(); ++i ) {
        // create the node
        QDomElement identifier = m_domDoc.createElement( KVTML_IDENTIFIER );

        // set the id
        identifier.setAttribute( KVTML_ID, QString::number( i ) );

        // record the identifier as the locale for now
        // TODO: when support for more parts of the identifier is in the document class (name, type, etc.) store those here as well
        identifier.appendChild( newTextElement( KVTML_NAME, m_doc->identifier( i ).name() ) );

        identifier.appendChild( newTextElement( KVTML_LOCALE, m_doc->identifier( i ).locale() ) );

        // record articles
        QDomElement article = m_domDoc.createElement( KVTML_ARTICLE );
        writeArticle( article, i );
        if ( article.hasChildNodes() ) {
            identifier.appendChild( article );
        }

        // record personalpronouns
        QDomElement personalpronouns = m_domDoc.createElement( KVTML_PERSONALPRONOUNS );
        writePersonalPronoun( personalpronouns, m_doc->identifier(i).personalPronouns() );
        if ( personalpronouns.hasChildNodes() ) {
            identifier.appendChild( personalpronouns );
        }

        // add this identifier to the group
        identifiersElement.appendChild( identifier );
    }
    return true;
}

bool KEduVocKvtml2Writer::writeLessons( QDomElement &lessonsElement )
{
    for( int lessonId = 0; lessonId < m_doc->lessonCount(); lessonId++ ) {
        // make lesson element
        QDomElement thisLessonElement = m_domDoc.createElement( KVTML_LESSON );

        // add a name
        thisLessonElement.appendChild( newTextElement( KVTML_NAME, m_doc->lesson(lessonId).name() ) );

        // add a inquery tag
        if ( m_doc->lesson(lessonId).inQuery() ) {
            thisLessonElement.appendChild( newTextElement( KVTML_QUERY, KVTML_TRUE ) );
        }

        // add a current tag
        if ( lessonId == m_doc->currentLesson() ) {
            thisLessonElement.appendChild( newTextElement( KVTML_CURRENT, KVTML_TRUE ) );
        }

        // TODO: add the entryids...
        for ( int i = 0; i < m_doc->entryCount(); ++i ) {
            if ( m_doc->entry( i )->lesson() == lessonId ) {
                thisLessonElement.appendChild( newTextElement( KVTML_ENTRYID, QString::number( i ) ) );
            }
        }

        lessonsElement.appendChild( thisLessonElement );
    }

    return true;
}


bool KEduVocKvtml2Writer::writeArticle( QDomElement &articleElement, int article )
{
    ///@todo: this is ugly as hell and writes only singular

    QDomElement singular = m_domDoc.createElement( KVTML_SINGULAR );

    QDomElement definite = m_domDoc.createElement( KVTML_DEFINITE );
    QDomElement indefinite = m_domDoc.createElement( KVTML_INDEFINITE );
    QString def;
    QString indef;

    // male
    QString articleString;
    articleString = m_doc->identifier(article).article().article( KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Masculine );
    if ( !articleString.isEmpty() ) {
        definite.appendChild( newTextElement( KVTML_MALE, articleString ) );
    }
    articleString = m_doc->identifier(article).article().article( KEduVocArticle::Singular, KEduVocArticle::Indefinite, KEduVocArticle::Masculine );
    if ( !articleString.isEmpty() ) {
        indefinite.appendChild( newTextElement( KVTML_MALE, articleString ) );
    }

    // female
    articleString = m_doc->identifier(article).article().article( KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Feminine );
    if ( !articleString.isEmpty() ) {
        definite.appendChild( newTextElement( KVTML_FEMALE, articleString ) );
    }
    articleString = m_doc->identifier(article).article().article( KEduVocArticle::Singular, KEduVocArticle::Indefinite, KEduVocArticle::Feminine );
    if ( !articleString.isEmpty() ) {
        indefinite.appendChild( newTextElement( KVTML_FEMALE, articleString ) );
    }

    // neutral
    articleString = m_doc->identifier(article).article().article( KEduVocArticle::Singular, KEduVocArticle::Definite, KEduVocArticle::Neutral );
    if ( !articleString.isEmpty() ) {
        definite.appendChild( newTextElement( KVTML_NEUTRAL, articleString ) );
    }
    articleString = m_doc->identifier(article).article().article( KEduVocArticle::Singular, KEduVocArticle::Indefinite, KEduVocArticle::Neutral );
    if ( !articleString.isEmpty() ) {
        indefinite.appendChild( newTextElement( KVTML_NEUTRAL, articleString ) );
    }




    if ( definite.hasChildNodes() ) {
        singular.appendChild( definite );
    }

    if ( indefinite.hasChildNodes() ) {
        singular.appendChild( indefinite );
    }

    if ( singular.hasChildNodes() ) {
        articleElement.appendChild( singular );
    }
    return true;
}

bool KEduVocKvtml2Writer::writeTypes( QDomElement &typesElement )
{
    KEduVocWordType wt = m_doc->wordTypes();
    foreach( QString mainTypeName, wt.typeNameList() ) {
        kDebug() << "Writing type: " << mainTypeName;
        QDomElement typeDefinitionElement = m_domDoc.createElement( KVTML_WORDTYPEDEFINITION );
        typeDefinitionElement.appendChild( newTextElement( KVTML_TYPENAME, mainTypeName ) );

        QString specialType = wt.specialType( mainTypeName );
        if ( !specialType.isEmpty() ) {
            // get the NOT localized version for the doc
            if ( specialType == m_doc->wordTypes().specialTypeNoun() ) {
                specialType = KVTML_SPECIALWORDTYPE_NOUN;
            }
            if ( specialType == m_doc->wordTypes().specialTypeVerb()) {
                specialType =  KVTML_SPECIALWORDTYPE_VERB;
            }
            if ( specialType == m_doc->wordTypes().specialTypeAdverb()) {
                specialType = KVTML_SPECIALWORDTYPE_ADVERB;
            }
            if ( specialType ==  m_doc->wordTypes().specialTypeAdjective()) {
                specialType = KVTML_SPECIALWORDTYPE_ADJECTIVE;
            }
            typeDefinitionElement.appendChild( newTextElement( KVTML_SPECIALWORDTYPE, specialType ) );
        }

        // subtypes
        foreach( QString subTypeName, wt.subTypeNameList( mainTypeName ) ) {
            QDomElement subTypeDefinitionElement = m_domDoc.createElement( KVTML_SUBWORDTYPEDEFINITION );
            subTypeDefinitionElement.appendChild( newTextElement( KVTML_SUBTYPENAME, subTypeName ) );
            QString specialSubType = wt.specialSubType( mainTypeName, subTypeName );
            if ( !specialSubType.isEmpty() ) {
                if ( specialSubType == m_doc->wordTypes().specialTypeNounMale() ) {
                    specialSubType = KVTML_SPECIALWORDTYPE_NOUN_MALE;
                }
                if ( specialSubType == m_doc->wordTypes().specialTypeNounFemale() ) {
                    specialSubType = KVTML_SPECIALWORDTYPE_NOUN_FEMALE;
                }
                if ( specialSubType == m_doc->wordTypes().specialTypeNounNeutral() ) {
                    specialSubType = KVTML_SPECIALWORDTYPE_NOUN_NEUTRAL;
                }
                subTypeDefinitionElement.appendChild( newTextElement( KVTML_SPECIALWORDTYPE, specialSubType ) );
            }
            typeDefinitionElement.appendChild( subTypeDefinitionElement );
        }
        typesElement.appendChild( typeDefinitionElement );
    }
    return true;
}


bool KEduVocKvtml2Writer::writeTenses( QDomElement &tensesElement )
{
    foreach( QString tense, m_doc->tenseDescriptions() ) {
        if ( !( tense.isNull() ) ) {
            tensesElement.appendChild( newTextElement( KVTML_TENSE, tense ) );
        }
    }

    return true;
}


bool KEduVocKvtml2Writer::writeUsages( QDomElement &usagesElement )
{
    foreach( QString usage, m_doc->usages() ) {
        usagesElement.appendChild( newTextElement( KVTML_USAGE, usage ) );
    }

    return true;
}

bool KEduVocKvtml2Writer::writeEntries( QDomElement &entriesElement )
{
    // loop through entries
    for ( int i = 0; i < m_doc->entryCount(); ++i ) {
        KEduVocExpression *thisEntry = m_doc->entry( i );

        // write entry tag
        QDomElement entryElement = m_domDoc.createElement( KVTML_ENTRY );

        // add id
        entryElement.setAttribute( KVTML_ID, QString::number( i ) );

        // write inactive
        entryElement.appendChild( newTextElement( KVTML_INACTIVE, thisEntry->isActive() ? KVTML_FALSE : KVTML_TRUE ) );

// kvtml 1 relic no longer used
//         // write inquery
//         entryElement.appendChild( newTextElement( KVTML_INQUERY, thisEntry->isInQuery() ? KVTML_TRUE : KVTML_FALSE ) );

        // write sizehint
        if ( thisEntry->sizeHint() > 0 ) {
            entryElement.appendChild( newTextElement( KVTML_SIZEHINT, QString::number( thisEntry->sizeHint() ) ) );
        }

        // loop through translations
        foreach( int trans, thisEntry->translationIndices() ) {
            // write translations
            QDomElement translation = m_domDoc.createElement( KVTML_TRANSLATION );
            translation.setAttribute( KVTML_ID, QString::number( trans ) );
            writeTranslation( translation, thisEntry->translation( trans ) );
            entryElement.appendChild( translation );
        }
        // add this entry to the entriesElement
        entriesElement.appendChild( entryElement );
    }
    return true;
}

bool KEduVocKvtml2Writer::writeTranslation( QDomElement &translationElement, KEduVocTranslation &translation )
{
    // <text>Kniebeugen</text>
    translationElement.appendChild( newTextElement( KVTML_TEXT, translation.text() ) );

    // <wordtype></wordtype>
    if ( !translation.type().isEmpty() ) {
        QDomElement wordTypeElement = m_domDoc.createElement( KVTML_WORDTYPE );
        translationElement.appendChild( wordTypeElement );
        //<typename>noun</typename>
        wordTypeElement.appendChild( newTextElement( KVTML_TYPENAME, translation.type() ) );
        // <subwordtype>male</subwordtype>
        if ( !translation.subType().isEmpty() ) {
            wordTypeElement.appendChild( newTextElement( KVTML_SUBTYPENAME, translation.subType() ) );
        }
    }

    // <comment></comment>
    translationElement.appendChild( newTextElement( KVTML_COMMENT, translation.comment() ) );

    // <pronunciation></pronunciation>
    if ( !translation.pronunciation().isEmpty() ) {
        translationElement.appendChild( newTextElement( KVTML_PRONUNCIATION, translation.pronunciation() ) );
    }

    // <falsefriend fromid="0"></falsefriend>
    // loop through the identifiers
    for ( int i = 0; i < m_doc->identifierCount(); ++i ) {
        // see if this identifier has a falsefriend in this translation
        QString thisFriend = translation.falseFriend( i );
        if ( !thisFriend.isEmpty() ) {
            // if so, create it, and set the fromid to i
            QDomElement thisFriendElement = newTextElement( KVTML_FALSEFRIEND, thisFriend );
            thisFriendElement.setAttribute( KVTML_FROMID, QString::number( i ) );
            translationElement.appendChild( thisFriendElement );
        }
    }

    // <antonym></antonym>
    if ( !translation.antonym().isEmpty() ) {
        translationElement.appendChild( newTextElement( KVTML_ANTONYM, translation.antonym() ) );
    }

    // <synonym></synonym>
    if ( !translation.synonym().isEmpty() ) {
        translationElement.appendChild( newTextElement( KVTML_SYNONYM, translation.synonym() ) );
    }

    // <example></example>
    if ( !translation.example().isEmpty() ) {
        translationElement.appendChild( newTextElement( KVTML_EXAMPLE, translation.example() ) );
    }

    // <usage></usage>
    foreach( QString usage, translation.usages() ) {
        translationElement.appendChild( newTextElement( KVTML_USAGE, usage ) );
    }

    // <paraphrase></paraphrase>
    if ( !translation.paraphrase().isEmpty() ) {
        translationElement.appendChild( newTextElement( KVTML_PARAPHRASE, translation.paraphrase() ) );
    }

    // grades
    for ( int i = 0; i < m_doc->identifierCount(); ++i ) {
        KEduVocGrade thisGrade = translation.gradeFrom( i );
        if ( thisGrade.queryCount() > 0 ) {
            QDomElement gradeElement = m_domDoc.createElement( KVTML_GRADE );
            gradeElement.setAttribute( KVTML_FROMID, QString::number( i ) );
            //<currentgrade>2</currentgrade>
            gradeElement.appendChild( newTextElement( KVTML_CURRENTGRADE, QString::number( thisGrade.grade() ) ) );

            //<count>6</count>
            gradeElement.appendChild( newTextElement( KVTML_COUNT, QString::number( thisGrade.queryCount() ) ) );

            //<errorcount>1</errorcount>
            gradeElement.appendChild( newTextElement( KVTML_ERRORCOUNT, QString::number( thisGrade.badCount() ) ) );

            //<date>949757271</date>
            gradeElement.appendChild( newTextElement( KVTML_DATE,  thisGrade.queryDate().toString( Qt::ISODate ) ) );

            translationElement.appendChild( gradeElement );
        }
    }

    // conjugation
    foreach ( QString tense, translation.conjugationTenses() ) {
        QDomElement thisElement = m_domDoc.createElement( KVTML_CONJUGATION );
        writeConjugation( thisElement, translation.conjugation(tense), tense );
        translationElement.appendChild( thisElement );
    }

    // comparison
    if ( !translation.comparison().isEmpty() ) {
        QDomElement comparisonElement = m_domDoc.createElement( KVTML_COMPARISON );
        writeComparison( comparisonElement, translation.comparison() );
        translationElement.appendChild( comparisonElement );
    }

    // multiplechoice
    if ( !translation.multipleChoice().isEmpty() ) {
        QDomElement multipleChoiceElement = m_domDoc.createElement( KVTML_MULTIPLECHOICE );
        writeMultipleChoice( multipleChoiceElement, translation.multipleChoice() );
        translationElement.appendChild( multipleChoiceElement );
    }

    // image
    if ( !translation.imageUrl().isEmpty() ) {
        QString urlString;
        if ( translation.imageUrl().url().startsWith(m_doc->url().upUrl().url()) ) {
            // try to save as relative url
            urlString = KUrl::relativeUrl( m_doc->url() , translation.imageUrl() );
        } else {
            urlString =  translation.imageUrl().url();
        }
        translationElement.appendChild( newTextElement( KVTML_IMAGE, urlString ) );
    }

    // sound
    if ( !translation.soundUrl().isEmpty() ) {
        QString urlString;
        if ( translation.soundUrl().url().startsWith(m_doc->url().upUrl().url()) ) {
            // try to save as relative url
            urlString = KUrl::relativeUrl( m_doc->url() , translation.soundUrl() );
        } else {
            urlString =  translation.soundUrl().url();
        }
        translationElement.appendChild( newTextElement( KVTML_SOUND, urlString ) );
    }

    return true;
}

bool KEduVocKvtml2Writer::writeComparison( QDomElement &comparisonElement, const KEduVocComparison &comparison )
/*
 <comparison>
   <absolute>good</absolute>
   <comparative>better</comparative>
   <superlative>best</superlative>
 </comparison>
*/
{
    comparisonElement.appendChild( newTextElement( KVTML_ABSOLUTE, comparison.l1() ) );
    comparisonElement.appendChild( newTextElement( KVTML_COMPARATIVE, comparison.l2() ) );
    comparisonElement.appendChild( newTextElement( KVTML_SUPERLATIVE, comparison.l3() ) );

    return true;
}


bool KEduVocKvtml2Writer::writeMultipleChoice( QDomElement &multipleChoiceElement, const KEduVocMultipleChoice &mc )
/*
 <multiplechoice>
   <choice>good</choice>
   <choice>better</choice>
   <choice>best</choice>
   <choice>best 2</choice>
   <choice>best 3</choice>
 </multiplechoice>
*/
{
    QStringList choices = mc.choices();
    for ( int i = 0; i < choices.size(); ++i ) {
        multipleChoiceElement.appendChild( newTextElement( KVTML_CHOICE, choices[i] ) );
    }

    return true;
}

bool KEduVocKvtml2Writer::writeConjugation( QDomElement &conjugationElement,
        const KEduVocConjugation &conjugation, const QString &tense )
{
    // write the tense tag
    conjugationElement.appendChild( newTextElement(KVTML_TENSE, tense) );

    for ( KEduVocConjugation::ConjugationNumber num = KEduVocConjugation::Singular; num < KEduVocConjugation::NumberMAX; num = KEduVocConjugation::ConjugationNumber(num +1) ) {
        QString first = conjugation.conjugation(
            KEduVocConjugation::First, num );
        QString second = conjugation.conjugation(
            KEduVocConjugation::Second, num );
        QString third_male = conjugation.conjugation(
            KEduVocConjugation::ThirdMale, num );
        QString third_female = conjugation.conjugation(
            KEduVocConjugation::ThirdFemale, num );
        QString third_neutral = conjugation.conjugation(
            KEduVocConjugation::ThirdNeutralCommon, num );

        if ( !first.isEmpty() || !second.isEmpty() || !third_female.isEmpty() ||
                !third_male.isEmpty() || !third_neutral.isEmpty() ) {
            QDomElement number;
            switch (num) {
            case KEduVocConjugation::Singular:
                number = m_domDoc.createElement( KVTML_SINGULAR );
                break;
            case KEduVocConjugation::Dual:
                number = m_domDoc.createElement( KVTML_DUAL );
                break;
            case KEduVocConjugation::Plural:
                number = m_domDoc.createElement( KVTML_PLURAL );
                break;
            }

            number.appendChild( newTextElement( KVTML_1STPERSON, first ) );
            number.appendChild( newTextElement( KVTML_2NDPERSON, second ) );
            number.appendChild( newTextElement( KVTML_THIRD_MALE, third_male ) );
            number.appendChild( newTextElement( KVTML_THIRD_FEMALE, third_female ) );
            number.appendChild( newTextElement( KVTML_THIRD_NEUTRAL_COMMON, third_neutral ) );

            conjugationElement.appendChild( number );
        }
    }

    return true;
}

QDomElement KEduVocKvtml2Writer::newTextElement( const QString &elementName, const QString &text )
{
    QDomElement retval = m_domDoc.createElement( elementName );
    QDomText textNode = m_domDoc.createTextNode( text );
    retval.appendChild( textNode );
    return retval;
}

bool KEduVocKvtml2Writer::writePersonalPronoun(QDomElement & pronounElement, const KEduVocPersonalPronoun & pronoun)
{
    // general pronoun properties
    if ( pronoun.maleFemaleDifferent() ) {
        pronounElement.appendChild( m_domDoc.createElement( KVTML_THIRD_PERSON_MALE_FEMALE_DIFFERENT ) );
    }
    if ( pronoun.neutralExists() ) {
        pronounElement.appendChild( m_domDoc.createElement( KVTML_THIRD_PERSON_NEUTRAL_EXISTS ) );
    }
    if ( pronoun.dualExists() ) {
        pronounElement.appendChild( m_domDoc.createElement( KVTML_DUAL_EXISTS ) );
    }

    for ( KEduVocConjugation::ConjugationNumber num = KEduVocConjugation::Singular; num < KEduVocConjugation::NumberMAX; num = KEduVocConjugation::ConjugationNumber(num +1) ) {
        QString first = pronoun.personalPronoun(
            KEduVocConjugation::First, num );
        QString second = pronoun.personalPronoun(
            KEduVocConjugation::Second, num );
        QString third_male = pronoun.personalPronoun(
            KEduVocConjugation::ThirdMale, num );
        QString third_female = pronoun.personalPronoun(
            KEduVocConjugation::ThirdFemale, num );
        QString third_neutral = pronoun.personalPronoun(
            KEduVocConjugation::ThirdNeutralCommon, num );

        if ( !first.isEmpty() || !second.isEmpty() || !third_female.isEmpty() ||
                !third_male.isEmpty() || !third_neutral.isEmpty() ) {
            QDomElement number;
            switch (num) {
            case KEduVocConjugation::Singular:
                number = m_domDoc.createElement( KVTML_SINGULAR );
                break;
            case KEduVocConjugation::Dual:
                number = m_domDoc.createElement( KVTML_DUAL );
                break;
            case KEduVocConjugation::Plural:
                number = m_domDoc.createElement( KVTML_PLURAL );
                break;
            }

            number.appendChild( newTextElement( KVTML_1STPERSON, first ) );
            number.appendChild( newTextElement( KVTML_2NDPERSON, second ) );
            number.appendChild( newTextElement( KVTML_THIRD_MALE, third_male ) );
            number.appendChild( newTextElement( KVTML_THIRD_FEMALE, third_female ) );
            number.appendChild( newTextElement( KVTML_THIRD_NEUTRAL_COMMON, third_neutral ) );

            if ( pronoun.maleFemaleDifferent() ) {
                number.appendChild( m_domDoc.createElement( KVTML_THIRD_PERSON_MALE_FEMALE_DIFFERENT ) );
            }
            if ( pronoun.neutralExists() ) {
                number.appendChild( m_domDoc.createElement( KVTML_THIRD_PERSON_NEUTRAL_EXISTS ) );
            }
            pronounElement.appendChild( number );
        }
    }
    return true;
}
