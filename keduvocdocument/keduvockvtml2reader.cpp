/***************************************************************************
                     read a KEduVocDocument from a KVTML file
    -----------------------------------------------------------------------
    copyright           : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                          (C) 2005 Eric Pignet <eric at erixpage.com>
                          (C) 2007 Peter Hedlund <peter.hedlund@kdemail.net>
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

#include "keduvockvtml2reader.h"

#include <QtCore/QTextStream>
#include <QtCore/QList>
#include <QtCore/QIODevice>

#include <klocale.h>

#include "keduvocdocument.h"
#include "keduvoclesson.h"
#include "keduvocwordtype.h"
#include "kvtml2defs.h"
#include "keduvockvtmlreader.h"
#include "keduvoccommon_p.h"

KEduVocKvtml2Reader::KEduVocKvtml2Reader( QIODevice *file )
        : m_inputFile( file )
{
    // the file must be already open
    if ( !m_inputFile->isOpen() ) {
        m_errorMessage = i18n( "file must be opened first" );
    }
}


bool KEduVocKvtml2Reader::readDoc( KEduVocDocument *doc )
{
    m_doc = doc;

    QDomDocument domDoc( "KEduVocDocument" );

    if ( !domDoc.setContent( m_inputFile, &m_errorMessage ) )
        return false;

    QDomElement domElementKvtml = domDoc.documentElement();
    if ( domElementKvtml.tagName() != KVTML_TAG ) {
        m_errorMessage = i18n( "This is not a KDE Vocabulary document." );
        return false;
    }

    if ( domElementKvtml.attribute( KVTML_VERSION ).toFloat() < 2.0 ) {
        // read the file with the old format

        // first reset the file to the beginning
        m_inputFile->seek( 0 );
        KEduVocKvtmlReader oldFormat( m_inputFile );

        // get the return value
        bool retval = oldFormat.readDoc( doc );

        // pass the errormessage up
        m_errorMessage = oldFormat.errorMessage();
        return retval;
    }

    //-------------------------------------------------------------------------
    // Information
    //-------------------------------------------------------------------------

    QDomElement info = domElementKvtml.firstChildElement( KVTML_INFORMATION );
    if ( !info.isNull() ) {
        if ( !readInformation( info ) )
            return false;
    }

    bool result = readGroups( domElementKvtml ); // read sub-groups

    return result;
}

bool KEduVocKvtml2Reader::readInformation( QDomElement &informationElement )
{
    // read the generator
    QDomElement currentElement = informationElement.firstChildElement( KVTML_GENERATOR );
    if ( !currentElement.isNull() ) {
        m_doc->setGenerator( currentElement.text() );
        // add the version if it's there
        int pos = m_doc->generator().lastIndexOf( KVD_VERS_PREFIX );
        if ( pos >= 0 ) {
            m_doc->setVersion( m_doc->generator().remove( 0, pos + 2 ) );
        }
    }

    // read the title
    currentElement = informationElement.firstChildElement( KVTML_TITLE );
    if ( !currentElement.isNull() ) {
        m_doc->setTitle( currentElement.text() );
    }

    // read the author
    currentElement = informationElement.firstChildElement( KVTML_AUTHOR );
    if ( !currentElement.isNull() ) {
        m_doc->setAuthor( currentElement.text() );
    }

    // read the license
    currentElement = informationElement.firstChildElement( KVTML_LICENSE );
    if ( !currentElement.isNull() ) {
        m_doc->setLicense( currentElement.text() );
    }

    // read the comment
    currentElement = informationElement.firstChildElement( KVTML_COMMENT );
    if ( !currentElement.isNull() ) {
        m_doc->setDocumentComment( currentElement.text() );
    }

    // read the category
    currentElement = informationElement.firstChildElement( KVTML_CATEGORY );
    if ( !currentElement.isNull() ) {
        m_doc->setCategory( currentElement.text() );
    }

    return true;
}

bool KEduVocKvtml2Reader::readGroups( QDomElement &domElementParent )
{
    bool result = false;

    QDomElement currentElement;

    QDomElement groupElement = domElementParent.firstChildElement( KVTML_IDENTIFIERS );
    if ( !groupElement.isNull() ) {
        QDomNodeList entryList = groupElement.elementsByTagName( KVTML_IDENTIFIER );
        if ( entryList.length() <= 0 ) {
            m_errorMessage = i18n( "missing identifier elements from identifiers tag" );
            return false;
        }

        for ( int i = 0; i < entryList.count(); ++i ) {
            currentElement = entryList.item( i ).toElement();
            if ( currentElement.parentNode() == groupElement ) {
                result = readIdentifier( currentElement );
                if ( !result )
                    return false;
            }
        }
    }

    groupElement = domElementParent.firstChildElement( KVTML_WORDTYPEDEFINITIONS );
    if ( !groupElement.isNull() ) {
        readTypes( groupElement );
    }

    groupElement = domElementParent.firstChildElement( KVTML_TENSES );
    if ( !groupElement.isNull() ) {
        readTenses( groupElement );
    }

    groupElement = domElementParent.firstChildElement( KVTML_USAGES );
    if ( !groupElement.isNull() ) {
        readUsages( groupElement );
    }

    groupElement = domElementParent.firstChildElement( KVTML_ENTRIES );
    if ( !groupElement.isNull() ) {
        QDomNodeList entryList = groupElement.elementsByTagName( KVTML_ENTRY );
        for ( int i = 0; i < entryList.count(); ++i ) {
            currentElement = entryList.item( i ).toElement();
            if ( currentElement.parentNode() == groupElement ) {
                result = readEntry( currentElement );
                if ( !result )
                    return false;
            }
        }
    }

    groupElement = domElementParent.firstChildElement( KVTML_LESSONS );
    if ( !groupElement.isNull() ) {
        QDomNodeList entryList = groupElement.elementsByTagName( KVTML_LESSON );
        if ( entryList.length() <= 0 ) {
            m_errorMessage = i18n( "no lessons found in 'lessons' tag" );
            return false; // at least one entry is required
        }

        for ( int i = 0; i < entryList.count(); ++i ) {
            currentElement = entryList.item( i ).toElement();
            if ( currentElement.parentNode() == groupElement ) {
                result = readLesson( currentElement );
                if ( !result )
                    return false;
            }
        }
    }

    return true;
}

bool KEduVocKvtml2Reader::readIdentifier( QDomElement &identifierElement )
{
    bool result = true;
    int id = identifierElement.attribute( KVTML_ID ).toInt( &result );
    if ( !result ) {
        m_errorMessage = i18n( "identifier missing id" );
        return false;
    }

    // generate empty identifiers in the doc
    for ( int i = m_doc->identifierCount(); i <= id; i++ ) {
        m_doc->appendIdentifier( KEduVocIdentifier() );
    }

    // the first element, create the identifier, even if empty
    QDomElement currentElement = identifierElement.firstChildElement( KVTML_NAME );
    m_doc->identifier(id).setName( currentElement.text() );

    currentElement = identifierElement.firstChildElement( KVTML_LOCALE );
    m_doc->identifier(id).setLocale( currentElement.text() );

    currentElement = identifierElement.firstChildElement( KVTML_IDENTIFIERTYPE );
    if ( !currentElement.isNull() ) {
        // TODO: do something with the type
    }

    currentElement = identifierElement.firstChildElement( KVTML_SIZEHINT );
    if ( !currentElement.isNull() ) {
        // TODO: do something with the sizehint
    }

    // read sub-parts
    currentElement = identifierElement.firstChildElement( KVTML_ARTICLE );
    if ( !currentElement.isNull() ) {
        readArticle( currentElement, id );
    }

    currentElement = identifierElement.firstChildElement( KVTML_PERSONALPRONOUNS );
    if ( !currentElement.isNull() ) {
        KEduVocPersonalPronoun personalPronoun;
        readPersonalPronoun( currentElement, personalPronoun );
        m_doc->identifier(id).setPersonalPronouns( personalPronoun );
    }
    return result;
}

bool KEduVocKvtml2Reader::readEntry( QDomElement &entryElement )
{
    KEduVocExpression expr;
    QDomElement currentElement;
    bool result = true;

    // get entry id
    int id = entryElement.attribute( KVTML_ID ).toInt( &result );
    if ( !result ) {
        m_errorMessage = i18n( "entry missing id" );
        return false;
    }

    // read info tags: inactive, inquery, and sizehint
    currentElement = entryElement.firstChildElement( KVTML_INACTIVE );
    if ( !currentElement.isNull() ) {
        // set the active state of the expression
        if ( currentElement.text() == KVTML_TRUE ) {
            expr.setActive( false );
        } else {
            expr.setActive( true );
        }
    }

//     currentElement = entryElement.firstChildElement( KVTML_INQUERY );
//     if ( !currentElement.isNull() ) {
//         // set the inquery information
//         if ( currentElement.text() == KVTML_TRUE ) {
//             expr.setInQuery( true );
//         } else {
//             expr.setInQuery( false );
//         }
//     }

    currentElement = entryElement.firstChildElement( KVTML_SIZEHINT );
    if ( !currentElement.isNull() ) {
        // set the sizehint
        expr.setSizeHint( currentElement.text().toInt() );
    }

    // read translation children
    QDomNodeList translationList = entryElement.elementsByTagName( KVTML_TRANSLATION );
    if ( translationList.length() <= 0 ) {
        m_errorMessage = i18n( "no translations found" );
        return false; // at least one translation is required
    }

    for ( int i = 0; i < translationList.count(); ++i ) {
        currentElement = translationList.item( i ).toElement();
        if ( currentElement.parentNode() == entryElement ) {
            result = readTranslation( currentElement, expr, i );
            if ( !result )
                return false;
        }
    }

    // TODO: probably should insert at id position with a check to see if it exists
    // may be useful for detecting corrupt documents
    m_doc->insertEntry( &expr, id );
    return result;
}

bool KEduVocKvtml2Reader::readTranslation( QDomElement &translationElement,
        KEduVocExpression &expr, int index )
{
    QDomElement currentElement = translationElement.firstChildElement( KVTML_TEXT );
    if ( !currentElement.isNull() ) {
        expr.translation( index ).setText( currentElement.text() );
    }

    currentElement = translationElement.firstChildElement( KVTML_COMMENT );
    if ( !currentElement.isNull() ) {
        expr.translation( index ).setComment( currentElement.text() );
    }

    currentElement = translationElement.firstChildElement( KVTML_WORDTYPE );
    if ( !currentElement.isNull() ) {
        QDomElement typeElement = currentElement.firstChildElement( KVTML_TYPENAME );
        expr.translation( index ).setType( typeElement.text() );
        // read subtype if the type is not empty
        typeElement = currentElement.firstChildElement( KVTML_SUBTYPENAME );
        if ( !typeElement.isNull() ) {
            expr.translation( index ).setSubType( typeElement.text() );
        }
    }

    //<pronunciation></pronunciation>
    currentElement = translationElement.firstChildElement( KVTML_PRONUNCIATION );
    if ( !currentElement.isNull() ) {
        expr.translation( index ).setPronunciation( currentElement.text() );
    }

    //<falsefriend fromid="1"></falsefriend>
    currentElement = translationElement.firstChildElement( KVTML_FALSEFRIEND );
    if ( !currentElement.isNull() ) {
        int fromid = currentElement.attribute( KVTML_FROMID ).toInt();
        expr.translation( index ).setFalseFriend( fromid, currentElement.text() );
    }

    //<antonym></antonym>
    currentElement = translationElement.firstChildElement( KVTML_ANTONYM );
    if ( !currentElement.isNull() ) {
        expr.translation( index ).setAntonym( currentElement.text() );
    }

    //<synonym></synonym>
    currentElement = translationElement.firstChildElement( KVTML_SYNONYM );
    if ( !currentElement.isNull() ) {
        expr.translation( index ).setSynonym( currentElement.text() );
    }

    //<example></example>
    currentElement = translationElement.firstChildElement( KVTML_EXAMPLE );
    if ( !currentElement.isNull() ) {
        expr.translation( index ).setExample( currentElement.text() );
    }

    //<usage></usage> can be as often as there are usage labels
    currentElement = translationElement.firstChildElement( KVTML_USAGE );
    while ( !currentElement.isNull() ) {
        expr.translation( index ).usages().insert( currentElement.text() );
        currentElement = currentElement.nextSiblingElement( KVTML_USAGE );
    }

    //<paraphrase></paraphrase>
    currentElement = translationElement.firstChildElement( KVTML_PARAPHRASE );
    if ( !currentElement.isNull() ) {
        expr.translation( index ).setParaphrase( currentElement.text() );
    }

    // conjugations
    currentElement = translationElement.firstChildElement( KVTML_CONJUGATION );
    while ( !currentElement.isNull() ) {
        // read any conjugations (NOTE: this will overwrite any conjugations of the same type for this
        // translation, as the type is used as the key
        QDomElement tenseElement = currentElement.firstChildElement( KVTML_TENSE );
        QString tense = tenseElement.text();

        readConjugation( currentElement, expr.translation(index).conjugation(tense) );
        currentElement = currentElement.nextSiblingElement( KVTML_CONJUGATION );
    }

    // grade elements
    currentElement = translationElement.firstChildElement( KVTML_GRADE );
    while ( !currentElement.isNull() ) {
        // TODO: read grade
        readGrade( currentElement, expr, index );
        currentElement = currentElement.nextSiblingElement( KVTML_GRADE );
    }

    // comparisons
    currentElement = translationElement.firstChildElement( KVTML_COMPARISON );
    if ( !currentElement.isNull() ) {
        KEduVocComparison comparison;
        readComparison( currentElement, comparison );
        expr.translation( index ).setComparison( comparison );
    }

    // multiple choice
    currentElement = translationElement.firstChildElement( KVTML_MULTIPLECHOICE );
    if ( !currentElement.isNull() ) {
        KEduVocMultipleChoice mc;
        readMultipleChoice( currentElement, mc );
        expr.translation( index ).setMultipleChoice( mc );
    }

    // image
    currentElement = translationElement.firstChildElement( KVTML_IMAGE );
    if ( !currentElement.isNull() ) {
        expr.translation( index ).setImageUrl( KUrl( m_doc->url(), currentElement.text() ) );
    }

    // sound
    currentElement = translationElement.firstChildElement( KVTML_SOUND );
    if ( !currentElement.isNull() ) {
        expr.translation( index ).setSoundUrl( KUrl( m_doc->url(), currentElement.text() ) );
    }

    return true;
}

bool KEduVocKvtml2Reader::readLesson( QDomElement &lessonElement )
{
    // NOTE: currently this puts an identifier into the last lesson it is in, once support for multiple lessons
    // is in the entry class, all lessons that include an entry will be in there
    int lessonId;
    //<name>Lesson name</name>
    QDomElement currentElement = lessonElement.firstChildElement( KVTML_NAME );
    if ( !currentElement.isNull() ) {
        lessonId = m_doc->appendLesson( currentElement.text() );
    } else {
        m_errorMessage = i18n( "each lesson must have a name" );
        return false;
    }

    //<query>true</query>
    currentElement = lessonElement.firstChildElement( KVTML_QUERY );
    m_doc->lesson(lessonId).setInQuery(currentElement.text() == KVTML_TRUE);

    //<current>true</current>
    currentElement = lessonElement.firstChildElement( KVTML_CURRENT );
    if ( !currentElement.isNull() ) {
        if ( currentElement.text() == KVTML_TRUE ) {
            m_doc->setCurrentLesson( lessonId );
        }
    }

    //<entryid>0</entryid>
    currentElement = lessonElement.firstChildElement( KVTML_ENTRYID );
    while ( !currentElement.isNull() ) {
        int entryId = currentElement.text().toInt();
        // TODO: once we have a lesson class, add each of these entryids to the lesson
        // set this lesson for the given enty
        m_doc->entry( entryId )->setLesson( lessonId );
        m_doc->lesson( lessonId ).addEntry( entryId );
        currentElement = currentElement.nextSiblingElement( KVTML_ENTRYID );
    }

    return true;
}


bool KEduVocKvtml2Reader::readArticle( QDomElement &articleElement, int identifierNum )
/*
 <article>
  <definite>
    <male>der</male>
    <female>die</female>
    <neutral>das</neutral>
  </definite>
  <indefinite>
    <male>ein</male>
    <female>eine</female>
    <neutral>ein</neutral>
  </indefinite>
 </article>
*/
{
    QString fem_def = "";
    QString mal_def = "";
    QString nat_def = "";
    QString fem_indef = "";
    QString mal_indef = "";
    QString nat_indef = "";

    QDomElement currentElement = articleElement.firstChildElement( KVTML_SINGULAR ).firstChildElement( KVTML_DEFINITE );
    if ( !currentElement.isNull() )
    {
        QDomElement subElement = currentElement.firstChildElement( KVTML_MALE );
        if ( !subElement.isNull() ) {
            mal_def = subElement.text();
        }

        subElement = currentElement.firstChildElement( KVTML_FEMALE );
        if ( !subElement.isNull() ) {
            fem_def = subElement.text();
        }

        subElement = currentElement.firstChildElement( KVTML_NEUTRAL );
        if ( !subElement.isNull() ) {
            nat_def = subElement.text();
        }
    }

    currentElement = articleElement.firstChildElement( KVTML_SINGULAR ).firstChildElement( KVTML_INDEFINITE );
    if ( !currentElement.isNull() )
    {
        QDomElement subElement = currentElement.firstChildElement( KVTML_MALE );
        if ( !subElement.isNull() ) {
            mal_indef = subElement.text();
        }

        subElement = currentElement.firstChildElement( KVTML_FEMALE );
        if ( !subElement.isNull() ) {
            fem_indef = subElement.text();
        }

        subElement = currentElement.firstChildElement( KVTML_NEUTRAL );
        if ( !subElement.isNull() ) {
            nat_indef = subElement.text();
        }
    }
    m_doc->identifier(identifierNum).setArticle( KEduVocArticle( fem_def, fem_indef, mal_def, mal_indef, nat_def, nat_indef ) );

    return true;
}


bool KEduVocKvtml2Reader::readTypes( QDomElement &typesElement )
{
    QString mainTypeName;

    QDomElement currentTypeElement =    typesElement.firstChildElement( KVTML_WORDTYPEDEFINITION );
    // go over <wordtypedefinition> elements
    while ( !currentTypeElement.isNull() ) {
        // set type and specialtype
        mainTypeName =
            currentTypeElement.firstChildElement( KVTML_TYPENAME ).text();

        QString specialType = currentTypeElement.firstChildElement( KVTML_SPECIALWORDTYPE ).text();
        if ( !specialType.isEmpty() ) {
            // get the localized version
            if ( specialType == KVTML_SPECIALWORDTYPE_NOUN ) {
                specialType = m_doc->wordTypes().specialTypeNoun();
            }
            if ( specialType == KVTML_SPECIALWORDTYPE_VERB ) {
                specialType = m_doc->wordTypes().specialTypeVerb();
            }
            if ( specialType == KVTML_SPECIALWORDTYPE_ADVERB ) {
                specialType = m_doc->wordTypes().specialTypeAdverb();
            }
            if ( specialType == KVTML_SPECIALWORDTYPE_ADJECTIVE ) {
                specialType = m_doc->wordTypes().specialTypeAdjective();
            }
        }
        m_doc->wordTypes().addType( mainTypeName, specialType );

        // iterate sub type elements <subwordtypedefinition>
        QDomElement currentSubTypeElement = currentTypeElement.firstChildElement( KVTML_SUBWORDTYPEDEFINITION );
        while ( !currentSubTypeElement.isNull() ) {
            QString specialSubType = currentSubTypeElement.firstChildElement( KVTML_SPECIALWORDTYPE ).text();
            if ( !specialSubType.isEmpty() ) {
                // get the localized version
                if ( specialSubType == KVTML_SPECIALWORDTYPE_NOUN_MALE ) {
                    specialSubType = m_doc->wordTypes().specialTypeNounMale();
                }
                if ( specialSubType == KVTML_SPECIALWORDTYPE_NOUN_FEMALE ) {
                    specialSubType = m_doc->wordTypes().specialTypeNounFemale();
                }
                if ( specialSubType == KVTML_SPECIALWORDTYPE_NOUN_NEUTRAL ) {
                    specialSubType = m_doc->wordTypes().specialTypeNounNeutral();
                }
            }
            // set type and specialtype
            m_doc->wordTypes().addSubType( mainTypeName,
                                            currentSubTypeElement.firstChildElement( KVTML_SUBTYPENAME ).text(),
                                            specialSubType );
            currentSubTypeElement = currentSubTypeElement.nextSiblingElement( KVTML_SUBWORDTYPEDEFINITION );
        }
        currentTypeElement = currentTypeElement.nextSiblingElement( KVTML_WORDTYPEDEFINITION );
    }
    return true;
}


bool KEduVocKvtml2Reader::readTenses( QDomElement &tensesElement )
{
    QStringList tenses;

    QDomNodeList tenseNodes = tensesElement.elementsByTagName( KVTML_TENSE );
    for ( int i = 0; i < tenseNodes.count(); ++i ) {
        QDomElement currentElement = tenseNodes.item( i ).toElement();
        if ( currentElement.parentNode() == tensesElement ) {
            tenses.append( currentElement.text() );
        }
    }

    m_doc->setTenseDescriptions( tenses );
    return true;
}


bool KEduVocKvtml2Reader::readUsages( QDomElement &usagesElement )
{
    QStringList usages;

    QDomNodeList usageNodes = usagesElement.elementsByTagName( KVTML_USAGE );
    for ( int i = 0; i < usageNodes.count(); ++i ) {
        QDomElement currentElement = usageNodes.item( i ).toElement();
        if ( currentElement.parentNode() == usagesElement ) {
            m_doc->addUsage( currentElement.text() );
        }
    }

    return true;
}


bool KEduVocKvtml2Reader::readComparison( QDomElement &domElementParent, KEduVocComparison &comp )
/*
 <comparison>
   <absolute>good</absolute>
   <comparative>better</comparative>
   <superlative>best</superlative>
 </comparison>
*/
{
    QDomElement currentElement;

    currentElement = domElementParent.firstChildElement( KVTML_ABSOLUTE );
    if ( !currentElement.isNull() ) {
        comp.setL1( currentElement.text() );
    }

    currentElement = domElementParent.firstChildElement( KVTML_COMPARATIVE );
    if ( !currentElement.isNull() )
    {
        comp.setL2( currentElement.text() );
    }

    currentElement = domElementParent.firstChildElement( KVTML_SUPERLATIVE );
    if ( !currentElement.isNull() )
    {
        comp.setL3( currentElement.text() );
    }
    return true;
}


bool KEduVocKvtml2Reader::readMultipleChoice( QDomElement &multipleChoiceElement, KEduVocMultipleChoice &mc )
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
    QDomElement currentElement;
    QString s;
    mc.clear();

    QDomNodeList choiceNodes = multipleChoiceElement.elementsByTagName( KVTML_CHOICE );
    for ( int i = 0; i < choiceNodes.count(); ++i )
    {
        currentElement = choiceNodes.item( i ).toElement();
        if ( currentElement.parentNode() == multipleChoiceElement ) {
            mc.appendChoice( currentElement.text() );
        }
    }
    return true;
}

bool KEduVocKvtml2Reader::readGrade( QDomElement &gradeElement, KEduVocExpression &expr, int index )
{
    bool result = true;
    int id = gradeElement.attribute( KVTML_FROMID ).toInt( &result );
    if ( !result ) {
        m_errorMessage = i18n( "identifier missing id" );
        return false;
    }

    QDomElement currentElement = gradeElement.firstChildElement( KVTML_CURRENTGRADE );
    if ( !currentElement.isNull() ) {
        int value = currentElement.text().toInt();
        expr.translation( index ).gradeFrom( id ).setGrade( value );
    }

    currentElement = gradeElement.firstChildElement( KVTML_COUNT );
    if ( !currentElement.isNull() ) {
        int value = currentElement.text().toInt();
        expr.translation( index ).gradeFrom( id ).setQueryCount( value );
    }

    currentElement = gradeElement.firstChildElement( KVTML_ERRORCOUNT );
    if ( !currentElement.isNull() ) {
        int value = currentElement.text().toInt();
        expr.translation( index ).gradeFrom( id ).setBadCount( value );
    }

    currentElement = gradeElement.firstChildElement( KVTML_DATE );
    if ( !currentElement.isNull() ) {
        QString dateString = currentElement.text();
        if ( !dateString.isEmpty() ) {
            QDateTime value = QDateTime::fromString( dateString, Qt::ISODate );
            expr.translation( index ).gradeFrom( id ).setQueryDate( value );
        }
    }

    return true;
}



bool KEduVocKvtml2Reader::readConjugation( QDomElement &conjugElement, KEduVocConjugation &conjugation )
/*
 <conjugation>
  <tense>Futurepastperfekt:)</tense>
  <singular>
    <firstperson></firstperson>
    <secondperson></secondperson>
    <thirdperson>
      <male></male>
      <female></female>
      <neutral></neutral>
    </thirdperson>
  </singular>
  <plural>
    <firstperson></firstperson>
    <secondperson></secondperson>
    <thirdsperson>
      <common></common>
    </third person>
  </plural>
 </conjugation>
*/
{
    QDomElement personElement = conjugElement.firstChildElement( KVTML_SINGULAR );
    if ( !personElement.isNull() )
    {
        readConjugationPerson( personElement, conjugation, KEduVocConjugation::Singular );
    }

    personElement = conjugElement.firstChildElement( KVTML_DUAL );
    if ( !personElement.isNull() )
    {
        readConjugationPerson( personElement, conjugation, KEduVocConjugation::Dual );
    }

    personElement = conjugElement.firstChildElement( KVTML_PLURAL );
    if ( !personElement.isNull() )
    {
        readConjugationPerson( personElement, conjugation, KEduVocConjugation::Plural );
    }

    return true;
}


bool KEduVocKvtml2Reader::readConjugationPerson(QDomElement & personElement, KEduVocConjugation & conjugation, KEduVocConjugation::ConjugationNumber number)
{
    QDomElement currentElement = personElement.firstChildElement( KVTML_1STPERSON );
    conjugation.setConjugation( currentElement.text(),
        KEduVocConjugation::First, number );

    currentElement = personElement.firstChildElement( KVTML_2NDPERSON );
    conjugation.setConjugation( currentElement.text(),
        KEduVocConjugation::Second, number );

    currentElement = personElement.firstChildElement( KVTML_THIRD_MALE );
    conjugation.setConjugation( currentElement.text(),
        KEduVocConjugation::ThirdMale, number );

    currentElement = personElement.firstChildElement( KVTML_THIRD_FEMALE );
    conjugation.setConjugation( currentElement.text(),
        KEduVocConjugation::ThirdFemale, number );

    currentElement = personElement.firstChildElement( KVTML_THIRD_NEUTRAL_COMMON );
    conjugation.setConjugation( currentElement.text(),
        KEduVocConjugation::ThirdNeutralCommon, number );
    return true;
}


bool KEduVocKvtml2Reader::readPersonalPronoun(QDomElement & pronounElement, KEduVocPersonalPronoun & pronoun)
{
    pronoun.setMaleFemaleDifferent(!pronounElement.firstChildElement(
        KVTML_THIRD_PERSON_MALE_FEMALE_DIFFERENT).isNull());
    pronoun.setNeutralExists( !pronounElement.firstChildElement(
        KVTML_THIRD_PERSON_NEUTRAL_EXISTS).isNull() );
    pronoun.setDualExists( !pronounElement.firstChildElement(
        KVTML_DUAL_EXISTS).isNull() );

    QDomElement personElement = pronounElement.firstChildElement( KVTML_SINGULAR );
    if ( !personElement.isNull() ) {
        readPersonalPronounChild( personElement, pronoun, KEduVocConjugation::Singular );
    }

    personElement = pronounElement.firstChildElement( KVTML_DUAL );
    if ( !personElement.isNull() ) {
        readPersonalPronounChild( personElement, pronoun, KEduVocConjugation::Dual );
    }

    personElement = pronounElement.firstChildElement( KVTML_PLURAL );
    if ( !personElement.isNull() ) {
        readPersonalPronounChild( personElement, pronoun, KEduVocConjugation::Plural );
    }
    return true;
}


bool KEduVocKvtml2Reader::readPersonalPronounChild(QDomElement & personElement, KEduVocPersonalPronoun & pronoun, KEduVocConjugation::ConjugationNumber number)
{
    QDomElement currentElement = personElement.firstChildElement( KVTML_1STPERSON );
    pronoun.setPersonalPronoun( currentElement.text(),
        KEduVocConjugation::First, number );

    currentElement = personElement.firstChildElement( KVTML_2NDPERSON );
    pronoun.setPersonalPronoun( currentElement.text(),
        KEduVocConjugation::Second, number );

    currentElement = personElement.firstChildElement( KVTML_THIRD_MALE );
    pronoun.setPersonalPronoun( currentElement.text(),
        KEduVocConjugation::ThirdMale, number );

    currentElement = personElement.firstChildElement( KVTML_THIRD_FEMALE );
    pronoun.setPersonalPronoun( currentElement.text(),
        KEduVocConjugation::ThirdFemale, number );

    currentElement = personElement.firstChildElement( KVTML_THIRD_NEUTRAL_COMMON );
    pronoun.setPersonalPronoun( currentElement.text(),
        KEduVocConjugation::ThirdNeutralCommon, number );
    return true;
}


#include "keduvockvtml2reader.moc"
