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
        writeConjugation( personalpronouns, m_doc->identifier(i).personalPronouns(), QString() );
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
    QDomElement definite = m_domDoc.createElement( KVTML_DEFINITE );
    QDomElement indefinite = m_domDoc.createElement( KVTML_INDEFINITE );
    QString def;
    QString indef;

    // male
    m_doc->identifier(article).article().getMale( &def, &indef );
    if ( !def.isEmpty() ) {
        definite.appendChild( newTextElement( KVTML_MALE, def ) );
    }
    if ( !indef.isEmpty() ) {
        indefinite.appendChild( newTextElement( KVTML_MALE, indef ) );
    }

    // female
    m_doc->identifier(article).article().getFemale( &def, &indef );
    if ( !def.isEmpty() ) {
        definite.appendChild( newTextElement( KVTML_FEMALE, def ) );
    }
    if ( !indef.isEmpty() ) {
        indefinite.appendChild( newTextElement( KVTML_FEMALE, indef ) );
    }

    // neutral
    m_doc->identifier(article).article().getNeutral( &def, &indef );
    if ( !def.isEmpty() ) {
        definite.appendChild( newTextElement( KVTML_NEUTRAL, def ) );
    }
    if ( !indef.isEmpty() ) {
        indefinite.appendChild( newTextElement( KVTML_NEUTRAL, indef ) );
    }

    if ( definite.hasChildNodes() ) {
        articleElement.appendChild( definite );
    }

    if ( indefinite.hasChildNodes() ) {
        articleElement.appendChild( indefinite );
    }
    return true;
}

bool KEduVocKvtml2Writer::writeTypes( QDomElement &typesElement )
{
    KEduVocWordType* wt = m_doc->wordTypes();
    foreach( QString mainTypeName, wt->typeNameList() ) {
        kDebug() << "Writing type: " << mainTypeName;
        QDomElement typeDefinitionElement = m_domDoc.createElement( KVTML_WORDTYPEDEFINITION );
        typeDefinitionElement.appendChild( newTextElement( KVTML_TYPENAME, mainTypeName ) );
        if ( !wt->specialType( mainTypeName ).isEmpty() ) {
            typeDefinitionElement.appendChild( newTextElement( KVTML_SPECIALWORDTYPE, wt->specialType( mainTypeName ) ) );
        }

        // subtypes
        foreach( QString subTypeName, wt->subTypeNameList( mainTypeName ) ) {
            QDomElement subTypeDefinitionElement = m_domDoc.createElement( KVTML_SUBWORDTYPEDEFINITION );
            subTypeDefinitionElement.appendChild( newTextElement( KVTML_SUBTYPENAME, subTypeName ) );
            if ( !wt->specialSubType( mainTypeName, subTypeName ).isEmpty() ) {
                subTypeDefinitionElement.appendChild( newTextElement( KVTML_SPECIALWORDTYPE, wt->specialSubType( mainTypeName, subTypeName ) ) );
            }
            typeDefinitionElement.appendChild( subTypeDefinitionElement );
        }
        typesElement.appendChild( typeDefinitionElement );
    }





    /*
      foreach(QString type, m_doc->typeDescriptions())
      {
        if (!(type.isNull()) )
        {
          typesElement.appendChild(newTextElement(KVTML_WORDTYPE, type));
        }
      }*/

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
            gradeElement.appendChild( newTextElement( KVTML_DATE, QString::number( thisGrade.queryDate().toTime_t() ) ) );

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
        translationElement.appendChild( newTextElement( KVTML_IMAGE, translation.imageUrl() ) );
    }

    // sound
    if ( !translation.soundUrl().isEmpty() ) {
        translationElement.appendChild( newTextElement( KVTML_SOUND, translation.soundUrl() ) );
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

    // first singular conjugations
    QString first = conjugation.pers1Singular();
    QString second = conjugation.pers2Singular();
    bool third_common = conjugation.pers3SingularCommon();
    QString third_male = conjugation.pers3MaleSingular();
    QString third_female = conjugation.pers3FemaleSingular();
    QString third_neutral = conjugation.pers3NaturalSingular();

    if ( !first.isEmpty() || !second.isEmpty() || !third_female.isEmpty() ||
            !third_male.isEmpty() || !third_neutral.isEmpty() ) {
        QDomElement singular = m_domDoc.createElement( KVTML_SINGULAR );

        singular.appendChild( newTextElement( KVTML_1STPERSON, first ) );
        singular.appendChild( newTextElement( KVTML_2NDPERSON, second ) );

        QDomElement thirdPerson = m_domDoc.createElement( KVTML_3RDPERSON );
        singular.appendChild( thirdPerson );

        if ( third_common ) {
            thirdPerson.appendChild( newTextElement( KVTML_COMMON, third_female ) );
        } else {
            thirdPerson.appendChild( newTextElement( KVTML_MALE, third_male ) );
            thirdPerson.appendChild( newTextElement( KVTML_FEMALE, third_female ) );
            thirdPerson.appendChild( newTextElement( KVTML_NEUTRAL, third_neutral ) );
        }
        conjugationElement.appendChild( singular );
    }

    // now for plurals
    first = conjugation.pers1Plural();
    second = conjugation.pers2Plural();
    third_common = conjugation.pers3PluralCommon();
    third_male = conjugation.pers3MalePlural();
    third_female = conjugation.pers3FemalePlural();
    third_neutral = conjugation.pers3NaturalPlural();

    if ( !first.isEmpty() || !second.isEmpty() || !third_female.isEmpty() ||
            !third_male.isEmpty() || !third_neutral.isEmpty() ) {
        QDomElement plural = m_domDoc.createElement( KVTML_PLURAL );

        plural.appendChild( newTextElement( KVTML_1STPERSON, first ) );
        plural.appendChild( newTextElement( KVTML_2NDPERSON, second ) );

        QDomElement thirdPerson = m_domDoc.createElement( KVTML_3RDPERSON );
        plural.appendChild( thirdPerson );

        if ( third_common ) {
            thirdPerson.appendChild( newTextElement( KVTML_COMMON, third_female ) );
        } else {
            thirdPerson.appendChild( newTextElement( KVTML_MALE, third_male ) );
            thirdPerson.appendChild( newTextElement( KVTML_FEMALE, third_female ) );
            thirdPerson.appendChild( newTextElement( KVTML_NEUTRAL, third_neutral ) );
        }
        conjugationElement.appendChild( plural );
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
