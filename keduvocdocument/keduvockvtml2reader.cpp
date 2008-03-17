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

#include <KDebug>

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

    groupElement = domElementParent.firstChildElement( KVTML_TENSES );
    if ( !groupElement.isNull() ) {
        readTenses( groupElement );
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

    groupElement = domElementParent.firstChildElement( KVTML_WORDTYPES );
    if ( !groupElement.isNull() ) {
        readChildWordTypes( m_doc->wordTypeContainer(), groupElement );
    }

    groupElement = domElementParent.firstChildElement( KVTML_LESSONS );
    if ( !groupElement.isNull() ) {
        readChildLessons(m_doc->lesson(), groupElement);
    }

    // Additional cleanup: Put orphaned entries without a lesson into a default lesson.
    KEduVocLesson *defaultLesson = new KEduVocLesson(i18n("Default Lesson"), m_doc->lesson());

    // now make sure we don't have any orphan entries
    foreach (KEduVocExpression * entry, m_allEntries.values()) {
        if (entry->lessons().count() == 0)
        {
            defaultLesson->appendEntry(entry);
        }
    }

    if (defaultLesson->entryCount() > 0)
    {
        m_doc->lesson()->appendChildContainer(defaultLesson);
    } else {
        delete defaultLesson;
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
    QDomElement currentElement;
    bool result = true;

    // get entry id
    int id = entryElement.attribute( KVTML_ID ).toInt( &result );
    if ( !result ) {
        m_errorMessage = i18n( "entry missing id" );
        return false;
    }

    KEduVocExpression *expr = new KEduVocExpression;

    // read info tags: inactive, inquery, and sizehint
    currentElement = entryElement.firstChildElement( KVTML_DEACTIVATED );
    if ( !currentElement.isNull() ) {
        // set the active state of the expression
        if ( currentElement.text() == KVTML_TRUE ) {
            expr->setActive( false );
        } else {
            expr->setActive( true );
        }
    }


    // read translation children
    QDomNodeList translationList = entryElement.elementsByTagName( KVTML_TRANSLATION );
    if ( translationList.length() <= 0 ) {
        delete expr;
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
    m_allEntries[id] = expr;
    return result;
}

bool KEduVocKvtml2Reader::readTranslation( QDomElement &translationElement,
        KEduVocExpression *expr, int index )
{
    // read the text, grade, declension and conjugation
    expr->translation(index)->fromKVTML2(translationElement);

    //<falsefriend fromid="1"></falsefriend>
    QDomElement currentElement = translationElement.firstChildElement( KVTML_FALSEFRIEND );
    if ( !currentElement.isNull() ) {
        int fromid = currentElement.attribute( KVTML_FROMID ).toInt();
        expr->translation(index)->setFalseFriend( fromid, currentElement.text() );
    }

    // comparisons
    currentElement = translationElement.firstChildElement( KVTML_COMPARISON );
    if ( !currentElement.isNull() ) {
        readComparison( currentElement, expr->translation(index) );
    }

    // multiple choice
    currentElement = translationElement.firstChildElement( KVTML_MULTIPLECHOICE );
    if ( !currentElement.isNull() ) {
        readMultipleChoice( currentElement, expr->translation(index) );
    }

    // image
    currentElement = translationElement.firstChildElement( KVTML_IMAGE );
    if ( !currentElement.isNull() ) {
        expr->translation(index)->setImageUrl( KUrl( m_doc->url(), currentElement.text() ) );
    }

    // sound
    currentElement = translationElement.firstChildElement( KVTML_SOUND );
    if ( !currentElement.isNull() ) {
        expr->translation(index)->setSoundUrl( KUrl( m_doc->url(), currentElement.text() ) );
    }

    return true;
}

bool KEduVocKvtml2Reader::readChildLessons( KEduVocLesson* parentLesson, QDomElement &lessonElement )
{
    QDomElement currentElement = lessonElement.firstChildElement( KVTML_CONTAINER );
    while ( !currentElement.isNull() ) {
        readLesson(parentLesson, currentElement);
        currentElement = currentElement.nextSiblingElement( KVTML_CONTAINER );
    }
    return true;
}


bool KEduVocKvtml2Reader::readLesson( KEduVocLesson* parentLesson, QDomElement &lessonElement )
{
    //<name>Lesson name</name>
    QDomElement currentElement = lessonElement.firstChildElement( KVTML_NAME );
    KEduVocLesson * lesson = new KEduVocLesson(currentElement.text(), parentLesson);
    parentLesson->appendChildContainer( lesson );

    readChildLessons( lesson, lessonElement );

    //<query>true</query>
    currentElement = lessonElement.firstChildElement( KVTML_INPRACTICE );
    lesson->setInPractice(currentElement.text() == KVTML_TRUE);

    //<entry id="123"/>
    currentElement = lessonElement.firstChildElement( KVTML_ENTRY );
    while ( !currentElement.isNull() ) {
        bool result = false;
        int entryId = currentElement.attribute( KVTML_ID ).toInt( &result );
        if(result) {
            lesson->appendEntry( m_allEntries[entryId] );
        }
        currentElement = currentElement.nextSiblingElement( KVTML_ENTRY );
    }
    return true;
}


bool KEduVocKvtml2Reader::readArticle( QDomElement &articleElement, int identifierNum )
/*
 <article>
  <singlular>
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
  </singular>
  <dual>
  </dual>
 </article>
*/
{
    // singular
    for ( KEduVocArticle::ArticleNumber num = KEduVocArticle::Singular; num <= KEduVocArticle::Plural; num = KEduVocArticle::ArticleNumber(num+1) ) {
        QDomElement numberElement = articleElement.firstChildElement( KVTML_GRAMMATICAL_NUMBER[num] );
        if (!numberElement.isNull()) {
            // definite
            for ( KEduVocArticle::ArticleDefiniteness def = KEduVocArticle::Definite; def <= KEduVocArticle::Indefinite; def = KEduVocArticle::ArticleDefiniteness(def+1) ) {
                QDomElement defElement = numberElement.firstChildElement( KVTML_GRAMMATICAL_DEFINITENESS[def] );
                if (!defElement.isNull()) {
                    // male
                    for ( KEduVocArticle::ArticleGender gen = KEduVocArticle::Masculine; gen <= KEduVocArticle::Neutral; gen = KEduVocArticle::ArticleGender(gen+1) ) {
                        QDomElement genderElement = defElement.firstChildElement( KVTML_GRAMMATICAL_GENDER[gen] );
                        if (!genderElement.isNull()) {
                            m_doc->identifier(identifierNum).article().setArticle( genderElement.text(), KEduVocArticle::indexOf(num, def, gen) );
                        }
                    }
                }
            }
        }
    }

    return true;
}


bool KEduVocKvtml2Reader::readChildWordTypes(KEduVocWordType* parentContainer, QDomElement &lessonElement)
{
    QDomElement currentElement = lessonElement.firstChildElement( KVTML_CONTAINER );
    while ( !currentElement.isNull() ) {
        readWordType(parentContainer, currentElement);
        currentElement = currentElement.nextSiblingElement( KVTML_CONTAINER );
    }
    return true;
}

bool KEduVocKvtml2Reader::readWordType( KEduVocWordType* parentContainer, QDomElement &typeElement )
{
    // set type and specialtype
    QString typeName =
        typeElement.firstChildElement( KVTML_NAME ).text();

    KEduVocWordType * wordTypeContainer = new KEduVocWordType(typeName, parentContainer);
    parentContainer->appendChildContainer(wordTypeContainer);

    QString specialType = typeElement.firstChildElement( KVTML_SPECIALWORDTYPE ).text();
    if ( !specialType.isEmpty() ) {
        // get the localized version
        if ( specialType == KVTML_SPECIALWORDTYPE_NOUN ) {
            wordTypeContainer->setWordType(KEduVocWordType::Noun);
        }
        if ( specialType == KVTML_SPECIALWORDTYPE_VERB ) {
            wordTypeContainer->setWordType(KEduVocWordType::Verb);
        }
        if ( specialType == KVTML_SPECIALWORDTYPE_ADVERB ) {
            wordTypeContainer->setWordType(KEduVocWordType::Adverb);
        }
        if ( specialType == KVTML_SPECIALWORDTYPE_ADJECTIVE ) {
            wordTypeContainer->setWordType(KEduVocWordType::Adjective);
        }
        if ( specialType == KVTML_SPECIALWORDTYPE_NOUN_MALE ) {
            wordTypeContainer->setWordType(KEduVocWordType::NounMale);
        }
        if ( specialType == KVTML_SPECIALWORDTYPE_NOUN_FEMALE ) {
            wordTypeContainer->setWordType(KEduVocWordType::NounFemale);
        }
        if ( specialType == KVTML_SPECIALWORDTYPE_NOUN_NEUTRAL ) {
            wordTypeContainer->setWordType(KEduVocWordType::NounNeutral);
        }
    } // special type


    // read entries
    QDomElement entryElement = typeElement.firstChildElement( KVTML_ENTRY );
    while ( !entryElement.isNull() ) {
        // read <entry id="123"></entryid>
        int entryId = entryElement.attribute( KVTML_ID ).toInt();
        QDomElement translationElement = entryElement.firstChildElement( KVTML_TRANSLATION );
        while( !translationElement.isNull() ) {
            // <translation id="234"/>
            int translationId = translationElement.attribute( KVTML_ID ).toInt();
            m_allEntries.value(entryId)->translation(translationId)->setWordType(wordTypeContainer);
            translationElement = translationElement.nextSiblingElement( KVTML_TRANSLATION );
        }
        entryElement = entryElement.nextSiblingElement( KVTML_ENTRY );
    }

    readChildWordTypes(wordTypeContainer, typeElement);

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

/*
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
}*/


bool KEduVocKvtml2Reader::readComparison( QDomElement &domElementParent, KEduVocTranslation* translation )
/*
 <comparison>
   <comparative>better</comparative>
   <superlative>best</superlative>
 </comparison>
*/
{
    QDomElement currentElement;

    currentElement = domElementParent.firstChildElement( KVTML_COMPARATIVE );
    if ( !currentElement.isNull() )
    {
        translation->setComparative( currentElement.text() );
    }

    currentElement = domElementParent.firstChildElement( KVTML_SUPERLATIVE );
    if ( !currentElement.isNull() )
    {
        translation->setSuperlative( currentElement.text() );
    }
    return true;
}


bool KEduVocKvtml2Reader::readMultipleChoice( QDomElement &multipleChoiceElement, KEduVocTranslation* translation )
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
    QDomNodeList choiceNodes = multipleChoiceElement.elementsByTagName( KVTML_CHOICE );
    for ( int i = 0; i < choiceNodes.count(); ++i )
    {
        currentElement = choiceNodes.item( i ).toElement();
        if ( currentElement.parentNode() == multipleChoiceElement ) {
            translation->multipleChoice().append( currentElement.text() );
        }
    }
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

    QDomElement personElement = pronounElement.firstChildElement( KVTML_GRAMMATICAL_NUMBER[0] );
    if ( !personElement.isNull() ) {
        readPersonalPronounChild( personElement, pronoun, KEduVocConjugation::Singular );
    }

    personElement = pronounElement.firstChildElement( KVTML_GRAMMATICAL_NUMBER[1] );
    if ( !personElement.isNull() ) {
        readPersonalPronounChild( personElement, pronoun, KEduVocConjugation::Dual );
    }

    personElement = pronounElement.firstChildElement( KVTML_GRAMMATICAL_NUMBER[2] );
    if ( !personElement.isNull() ) {
        readPersonalPronounChild( personElement, pronoun, KEduVocConjugation::Plural );
    }
    return true;
}


bool KEduVocKvtml2Reader::readPersonalPronounChild(QDomElement & personElement, KEduVocPersonalPronoun & pronoun, KEduVocConjugation::ConjugationNumber number)
{
    for (int person = KEduVocConjugation::First; person <= KEduVocConjugation::ThirdNeutralCommon; person++) {
        QDomElement currentElement = personElement.firstChildElement( KVTML_GRAMMATICAL_PERSON[person] );
        pronoun.setPersonalPronoun( currentElement.text(), KEduVocConjugation::ConjugationPerson(person), number );
    }

    return true;
}


#include "keduvockvtml2reader.moc"
