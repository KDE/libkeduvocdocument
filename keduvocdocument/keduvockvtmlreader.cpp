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

#include "keduvockvtmlreader.h"

#include <QtCore/QTextStream>
#include <QtCore/QList>
#include <QtCore/QIODevice>

#include <kdebug.h>
#include <klocale.h>

#include "keduvocdocument.h"
#include "keduvoclesson.h"
#include "keduvocwordtype.h"
#include "kvtmldefs.h"
#include "keduvoccommon_p.h"

KEduVocKvtmlReader::KEduVocKvtmlReader( QIODevice *file )
{
    // the file must be already open
    m_inputFile = file;
    m_errorMessage = "";
    kDebug() << "KEduVocKvtmlReader for kvtml version 1 files started.";
}


bool KEduVocKvtmlReader::readDoc( KEduVocDocument *doc )
{
    m_doc = doc;
    m_cols = 0;
    m_lines = 0;

    QDomDocument domDoc( "KEduVocDocument" );

    if ( !domDoc.setContent( m_inputFile, &m_errorMessage ) )
        return false;

    QDomElement domElementKvtml = domDoc.documentElement();
    if ( domElementKvtml.tagName() != KV_DOCTYPE ) {
        m_errorMessage = i18n( "This is not a KDE Vocabulary document." );
        return false;
    }

    //-------------------------------------------------------------------------
    // Attributes
    //-------------------------------------------------------------------------

    QDomAttr documentAttribute;
    documentAttribute = domElementKvtml.attributeNode( KV_ENCODING );
    if ( !documentAttribute.isNull() ) {
        // TODO handle old encodings
        // Qt DOM API autodetects encoding, so is there anything to do ?
    }

    documentAttribute = domElementKvtml.attributeNode( KV_TITLE );
    if ( !documentAttribute.isNull() )
        m_doc->setTitle( documentAttribute.value() );

    documentAttribute = domElementKvtml.attributeNode( KV_AUTHOR );
    if ( !documentAttribute.isNull() )
        m_doc->setAuthor( documentAttribute.value() );

    documentAttribute = domElementKvtml.attributeNode( KV_LICENSE );
    if ( !documentAttribute.isNull() )
        m_doc->setLicense( documentAttribute.value() );

    documentAttribute = domElementKvtml.attributeNode( KV_DOC_REM );
    if ( !documentAttribute.isNull() )
        m_doc->setDocumentComment( documentAttribute.value() );

    documentAttribute = domElementKvtml.attributeNode( KV_GENERATOR );
    if ( !documentAttribute.isNull() ) {
        m_doc->setGenerator( documentAttribute.value() );
        int pos = m_doc->generator().lastIndexOf( KVD_VERS_PREFIX );
        if ( pos >= 0 )
            m_doc->setVersion( m_doc->generator().remove( 0, pos + 2 ) );
    }

    documentAttribute = domElementKvtml.attributeNode( KV_COLS );
    if ( !documentAttribute.isNull() )
        m_cols = documentAttribute.value().toInt(); ///currently not used anywhere

    documentAttribute = domElementKvtml.attributeNode( KV_LINES );
    if ( !documentAttribute.isNull() )
        m_lines = documentAttribute.value().toInt();

    //-------------------------------------------------------------------------
    // Children
    //-------------------------------------------------------------------------

    bool result = readBody( domElementKvtml ); // read vocabulary

    return result;
}


bool KEduVocKvtmlReader::readBody( QDomElement &domElementParent )
{
    bool result = false;

    QDomElement currentElement;

    currentElement = domElementParent.firstChildElement( KV_LESS_GRP );
    if ( !currentElement.isNull() ) {
        result = readLesson( currentElement );
        if ( !result )
            return false;
    }

    currentElement = domElementParent.firstChildElement( KV_ARTICLE_GRP );
    if ( !currentElement.isNull() ) {
        result = readArticle( currentElement );
        if ( !result )
            return false;
    }

    currentElement = domElementParent.firstChildElement( KV_CONJUG_GRP );
    if ( !currentElement.isNull() ) {
        int count = 0;

        QDomElement domElementConjugChild = currentElement.firstChildElement(KV_CON_ENTRY);
        while ( !domElementConjugChild.isNull() ) {
            QString lang;
            QDomAttr domAttrLang = domElementConjugChild.attributeNode( KV_LANG ); // "l"
            // make sure, the identifier is there
            if (!addLanguage(count, domAttrLang.value())) {
                return false;
            }

            KEduVocPersonalPronoun pronouns;
            if (! readPersonalPronouns( domElementConjugChild, pronouns ) ) {
                return false;
            }
            m_doc->identifier(count).setPersonalPronouns( pronouns );

            count ++;

            domElementConjugChild = domElementConjugChild.nextSiblingElement( KV_CON_ENTRY );
        }
    }

    // initialize the list of predefined types
    m_doc->wordTypes().createDefaultWordTypes();
    currentElement = domElementParent.firstChildElement( KV_TYPE_GRP );
    if ( !currentElement.isNull() ) {
        result = readType( currentElement );
        if ( !result )
            return false;
    }

    currentElement = domElementParent.firstChildElement( KV_TENSE_GRP );
    if ( !currentElement.isNull() ) {
        result = readTense( currentElement );
        if ( !result )
            return false;
    }

    currentElement = domElementParent.firstChildElement( KV_USAGE_GRP );
    if ( !currentElement.isNull() ) {
        result = readUsage( currentElement );
        if ( !result )
            return false;
    }

    QDomNodeList entryList = domElementParent.elementsByTagName( KV_EXPR );
    if ( entryList.length() <= 0 )
        return false;

    for ( int i = 0; i < entryList.count(); ++i ) {
        currentElement = entryList.item( i ).toElement();
        if ( currentElement.parentNode() == domElementParent ) {
            result = readExpression( currentElement );
            if ( !result )
                return false;
        }
    }

    m_doc->setTenseDescriptions(m_compability.documentTenses());

    return true;
}


bool KEduVocKvtmlReader::readLesson( QDomElement &domElementParent )
{
    QString s;
    QDomAttr attribute;
    QDomElement currentElement;

    //-------------------------------------------------------------------------
    // Attributes
    //-------------------------------------------------------------------------

    attribute = domElementParent.attributeNode( KV_SIZEHINT );
    if ( !attribute.isNull() )
        m_doc->setSizeHint( -1, attribute.value().toInt() );

    //-------------------------------------------------------------------------
    // Children
    //-------------------------------------------------------------------------

    QDomNodeList entryList = domElementParent.elementsByTagName( KV_LESS_DESC );
    if ( entryList.length() <= 0 )
        return false;

    for ( int i = 0; i < entryList.count(); ++i ) {
        currentElement = entryList.item( i ).toElement();
        if ( currentElement.parentNode() == domElementParent ) {
            int no;

            attribute = currentElement.attributeNode( KV_LESS_NO );
            if ( !attribute.isNull() ) {
                no = attribute.value().toInt();
            }

            attribute = currentElement.attributeNode( KV_LESS_CURR );
            if ( !attribute.isNull() ) {
                if ( attribute.value().toInt() != 0 ) {
                    m_doc->setCurrentLesson( no );
                }
            }

            bool inQuery = false;
            attribute = currentElement.attributeNode( KV_LESS_QUERY );
            if ( !attribute.isNull() ) {
                inQuery = (attribute.value().toInt() != 0);
            }

            s = currentElement.text();
            int index = m_doc->appendLesson( s, inQuery );
            if ( index != no-1 ) {
                kDebug() << "Warning! Lesson order may be confused. Are all lessons in order in the file?";
            }
        }
    }

    return true;
}


bool KEduVocKvtmlReader::readArticle( QDomElement &domElementParent )
/*
 <article>
  <e l="de">        lang determines also lang order in entries !!
   <fi>eine</fi>    which must NOT differ
   <fd>die</fd>
   <mi>ein</mi>
   <md>der</md>
   <ni>ein</ni>
   <nd>das</nd>
  </e>
 </article>
*/
{

    QString s;
    QDomAttr attribute;
    QDomElement currentElement;
    QDomElement article;

    QDomNodeList entryList = domElementParent.elementsByTagName( KV_ART_ENTRY );
    if ( entryList.length() <= 0 )
        return false;

    for ( int i = 0; i < entryList.count(); ++i ) {

//kDebug() << "KEduVocKvtmlReader::readArticle() read " << entryList.count() << " articles. ";
        currentElement = entryList.item( i ).toElement();
        if ( currentElement.parentNode() == domElementParent ) {
            QString lang;
            attribute = currentElement.attributeNode( KV_LANG );

            if (!addLanguage(i, attribute.value())) {
                return false;
            }

            //---------
            // Children

            QString fem_def = "";
            QString mal_def = "";
            QString nat_def = "";
            QString fem_indef = "";
            QString mal_indef = "";
            QString nat_indef = "";

            article = currentElement.firstChildElement( KV_ART_FD );
            if ( !article.isNull() ) {
                fem_def = article.text();
                if ( fem_def.isNull() )
                    fem_def = "";
            }

            article = currentElement.firstChildElement( KV_ART_FI );
            if ( !article.isNull() ) {
                fem_indef = article.text();
                if ( fem_indef.isNull() )
                    fem_indef = "";
            }

            article = currentElement.firstChildElement( KV_ART_MD );
            if ( !article.isNull() ) {
                mal_def = article.text();
                if ( mal_def.isNull() )
                    mal_def = "";
            }

            article = currentElement.firstChildElement( KV_ART_MI );
            if ( !article.isNull() ) {
                mal_indef = article.text();
                if ( mal_indef.isNull() )
                    mal_indef = "";
            }

            article = currentElement.firstChildElement( KV_ART_ND );
            if ( !article.isNull() ) {
                nat_def = article.text();
                if ( nat_def.isNull() )
                    nat_def = "";
            }

            article = currentElement.firstChildElement( KV_ART_NI );
            if ( !article.isNull() ) {
                nat_indef = article.text();
                if ( nat_indef.isNull() )
                    nat_indef = "";
            }

            m_doc->identifier(i).setArticle( KEduVocArticle( fem_def, fem_indef, mal_def, mal_indef, nat_def, nat_indef ) );
        }
    }

    return true;
}


bool KEduVocKvtmlReader::readTranslationConjugations( QDomElement &domElementParent, KEduVocTranslation &translation ) {

    QString tense;

    QDomElement domElementConjugChild = domElementParent.firstChildElement(KV_CON_TYPE);
    while ( !domElementConjugChild.isNull() )
    {
        // "n" == is the type is the tense
        QDomAttr domAttrLang = domElementConjugChild.attributeNode( KV_CON_NAME );
        QString oldShortTense = domAttrLang.value();

        tense = m_compability.tenseFromKvtml1( oldShortTense );
        KEduVocConjugation conjugation;
        readConjugation(domElementConjugChild, conjugation);
        translation.setConjugation(tense, conjugation);

        domElementConjugChild = domElementConjugChild.nextSiblingElement( KV_CON_TYPE );
    } // while -> next tense, count++
    return true;
}

bool KEduVocKvtmlReader::readConjugation( QDomElement &domElementParent, KEduVocConjugation& conjugation )
/*
 <conjugation>        used in header for definiton of "prefix"
  <e l="de">          lang determines also lang order in entries !!
   <s1>I</s1>         which must NOT differ
   <s2>you<2>
   <s3f>he</s3f>
   <s3m>she</s3m>
   <s3n>it</s3n>
   <p1>we</p1>
   <p2>you</p2>
   <p3f>they</p3f>
   <p3m>they</p3m>
   <p3n>they</p3n>
  </e>
 </conjugation>

 <conjugation>        and in entry for definition of tenses of (irreg.) verbs
  <t n="sipa">
   <s1>go</s1>
   <s2>go</s2>
   <s3f>goes</s3f>
   <s3m>goes</s3m>
   <s3n>goes</s3n>
   <p1>go</p1>
   <p2>go</p2>
   <p3f>go</p3f>
   <p3m>go</p3m>
   <p3n>go</p3n>
  </t>
 </conjugation>
*/
{
//     QString s;
    bool p3_common;
    bool s3_common;
    QString pers1_sing;
    QString pers2_sing;
    QString pers3_m_sing;
    QString pers3_f_sing;
    QString pers3_n_sing;
    QString pers1_plur;
    QString pers2_plur;
    QString pers3_m_plur;
    QString pers3_f_plur;
    QString pers3_n_plur;

    p3_common = false;
    s3_common = false;

    // get the individual entries for persons...
    QDomElement domElementConjugGrandChild = domElementParent.firstChild().toElement();
    while ( !domElementConjugGrandChild.isNull() ) {
        if ( domElementConjugGrandChild.tagName() == KV_CON_P1S ) {
            pers1_sing = domElementConjugGrandChild.text();
        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P2S ) {
            pers2_sing = domElementConjugGrandChild.text();
        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3SF ) {
            QDomAttr domAttrCommon = domElementConjugGrandChild.attributeNode( KV_CONJ_COMMON );
            if ( !domAttrCommon.isNull() )
                s3_common = domAttrCommon.value().toInt();  // returns 0 if the conversion fails
            pers3_f_sing = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3SM ) {
            pers3_m_sing = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3SN ) {
            pers3_n_sing = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P1P ) {
            pers1_plur = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P2P ) {
            pers2_plur = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3PF ) {
            QDomAttr domAttrCommon = domElementConjugGrandChild.attributeNode( KV_CONJ_COMMON );
            if ( !domAttrCommon.isNull() )
                p3_common = domAttrCommon.value().toInt();  // returns 0 if the conversion fails

            pers3_f_plur = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3PM ) {
            pers3_m_plur = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3PN ) {
            pers3_n_plur = domElementConjugGrandChild.text();

        } else {
            return false;
        }

        domElementConjugGrandChild = domElementConjugGrandChild.nextSibling().toElement();
    } // while - probably to be sure, because the persons could be in any order.
    // I guess this goes over only one set, such as:
    // <s1>traigo</s1><s2>traes</s2><s3fcommon="1">trae</s3f>
    // <p1>traemos</p1><p2>traÃÂ©is</p2><p3f common="1">traen</p3f>
    // until no elements are left in that soup.

    // now set the data: [count] - number of conjug?
    // type - the tense?
    // finally the person

    const KEduVocConjugation::ConjugationNumber numS = KEduVocConjugation::Singular;
    const KEduVocConjugation::ConjugationNumber numP = KEduVocConjugation::Plural;

    conjugation.setConjugation( pers1_sing, KEduVocConjugation::First, numS);
    conjugation.setConjugation( pers2_sing, KEduVocConjugation::Second, numS);
    conjugation.setConjugation( pers1_plur, KEduVocConjugation::First, numP);
    conjugation.setConjugation( pers2_plur, KEduVocConjugation::Second, numP);

    if ( s3_common ) {
        conjugation.setConjugation( pers3_f_sing, KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Singular );
    } else  {
        conjugation.setConjugation( pers3_m_sing,
            KEduVocConjugation::ThirdMale, KEduVocConjugation::Singular );
        conjugation.setConjugation( pers3_f_sing,
            KEduVocConjugation::ThirdFemale, KEduVocConjugation::Singular );
        conjugation.setConjugation( pers3_n_sing,
            KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Singular );
    }

    if ( p3_common ) {
        conjugation.setConjugation( pers3_f_plur, KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Plural );
    } else  {
        conjugation.setConjugation( pers3_m_plur,
            KEduVocConjugation::ThirdMale, KEduVocConjugation::Plural );
        conjugation.setConjugation( pers3_f_plur,
            KEduVocConjugation::ThirdFemale, KEduVocConjugation::Plural );
        conjugation.setConjugation( pers3_n_plur,
            KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Plural );
    }

    return true;
}




bool KEduVocKvtmlReader::readPersonalPronouns( QDomElement &domElementParent, KEduVocPersonalPronoun& pronouns )
{
//     QString s;
    bool p3_common;
    bool s3_common;
    QString pers1_sing;
    QString pers2_sing;
    QString pers3_m_sing;
    QString pers3_f_sing;
    QString pers3_n_sing;
    QString pers1_plur;
    QString pers2_plur;
    QString pers3_m_plur;
    QString pers3_f_plur;
    QString pers3_n_plur;

    p3_common = false;
    s3_common = false;

    // get the individual entries for persons...
    QDomElement domElementConjugGrandChild = domElementParent.firstChild().toElement();
    while ( !domElementConjugGrandChild.isNull() ) {
        if ( domElementConjugGrandChild.tagName() == KV_CON_P1S ) {
            pers1_sing = domElementConjugGrandChild.text();
        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P2S ) {
            pers2_sing = domElementConjugGrandChild.text();
        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3SF ) {
            QDomAttr domAttrCommon = domElementConjugGrandChild.attributeNode( KV_CONJ_COMMON );
            if ( !domAttrCommon.isNull() )
                s3_common = domAttrCommon.value().toInt();  // returns 0 if the conversion fails
            pers3_f_sing = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3SM ) {
            pers3_m_sing = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3SN ) {
            pers3_n_sing = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P1P ) {
            pers1_plur = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P2P ) {
            pers2_plur = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3PF ) {
            QDomAttr domAttrCommon = domElementConjugGrandChild.attributeNode( KV_CONJ_COMMON );
            if ( !domAttrCommon.isNull() )
                p3_common = domAttrCommon.value().toInt();  // returns 0 if the conversion fails

            pers3_f_plur = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3PM ) {
            pers3_m_plur = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3PN ) {
            pers3_n_plur = domElementConjugGrandChild.text();

        } else {
            return false;
        }

        domElementConjugGrandChild = domElementConjugGrandChild.nextSibling().toElement();
    } // while - probably to be sure, because the persons could be in any order.
    // I guess this goes over only one set, such as:
    // <s1>traigo</s1><s2>traes</s2><s3fcommon="1">trae</s3f>
    // <p1>traemos</p1><p2>traÃÂ©is</p2><p3f common="1">traen</p3f>
    // until no elements are left in that soup.

    // now set the data: [count] - number of conjug?
    // type - the tense?
    // finally the person

    const KEduVocConjugation::ConjugationNumber numS = KEduVocConjugation::Singular;
    pronouns.setMaleFemaleDifferent(false);
    pronouns.setPersonalPronoun( pers1_sing, KEduVocConjugation::First, numS );
    pronouns.setPersonalPronoun( pers2_sing, KEduVocConjugation::Second, numS );

    // used to have common in female
    if ( s3_common ) {
        pronouns.setPersonalPronoun( pers3_f_sing, KEduVocConjugation::ThirdNeuterCommon, numS );
    } else  {
        pronouns.setPersonalPronoun( pers3_m_sing,
            KEduVocConjugation::ThirdMale, numS );
        pronouns.setPersonalPronoun( pers3_f_sing,
            KEduVocConjugation::ThirdFemale, numS );
        pronouns.setPersonalPronoun( pers3_n_sing,
            KEduVocConjugation::ThirdNeuterCommon, numS );
        pronouns.setMaleFemaleDifferent(true);
    }

    const KEduVocConjugation::ConjugationNumber numP = KEduVocConjugation::Plural;

    pronouns.setPersonalPronoun( pers1_sing, KEduVocConjugation::First, numP );
    pronouns.setPersonalPronoun( pers2_sing, KEduVocConjugation::Second, numP );
    if ( p3_common ) {
        pronouns.setPersonalPronoun( pers3_f_plur, KEduVocConjugation::ThirdNeuterCommon, numP );
    } else  {
        pronouns.setPersonalPronoun( pers3_m_plur,
            KEduVocConjugation::ThirdMale, numP );
        pronouns.setPersonalPronoun( pers3_f_plur,
            KEduVocConjugation::ThirdFemale, numP );
        pronouns.setPersonalPronoun( pers3_n_plur,
            KEduVocConjugation::ThirdNeuterCommon, numP );
        pronouns.setMaleFemaleDifferent(true);
    }

    return true;
}


bool KEduVocKvtmlReader::readType( QDomElement &domElementParent )
{
    QString s;
    QDomElement currentElement;

    QDomNodeList entryList = domElementParent.elementsByTagName( KV_TYPE_DESC );
    if ( entryList.length() <= 0 )
        return false;

    for ( int i = 0; i < entryList.count(); ++i ) {
        currentElement = entryList.item( i ).toElement();
        if ( currentElement.parentNode() == domElementParent ) {
            // We need to even add empty elements since the old system relied on
            // the order. So "type1" "" "type2" should be just like that.

            kDebug() << "Adding old self defined type: " << currentElement.text();
            // add the type to the list of available types
            m_doc->wordTypes().addType( currentElement.text() );

            // from this the #1 are transformed to something sensible again
            m_oldSelfDefinedTypes.append( currentElement.text() );
        }
    }

    return true;
}


bool KEduVocKvtmlReader::readTense( QDomElement &domElementParent )
{
    QDomElement currentElement;

    QDomNodeList entryList = domElementParent.elementsByTagName( KV_TENSE_DESC );
    if ( entryList.length() <= 0 )
        return false;

    for ( int i = 0; i < entryList.count(); ++i ) {
        currentElement = entryList.item( i ).toElement();
        if ( currentElement.parentNode() == domElementParent ) {
            m_compability.addUserdefinedTense( currentElement.text() );
        }
    }

    return true;
}


bool KEduVocKvtmlReader::readUsage( QDomElement &domElementParent )
{
    // get user defined usages

    QDomElement currentElement;

    QDomNodeList entryList = domElementParent.elementsByTagName( KV_USAGE_DESC );
    if ( entryList.length() <= 0 ) {
        return false;
    }


    for ( int i = 0; i < entryList.count(); ++i ) {
        currentElement = entryList.item( i ).toElement();
        if ( currentElement.parentNode() == domElementParent ) {
            m_compability.addUserdefinedUsage( currentElement.text() );
        }
    }

    foreach( QString usage, m_compability.documentUsages() ) {
        m_doc->addUsage( usage );
    }

    return true;
}


bool KEduVocKvtmlReader::readComparison( QDomElement &domElementParent, KEduVocComparison &comp )
/*
 <comparison>
   <l1>good</l1>
   <l2>better</l2>
   <l3>best</l3>
 </comparison>
*/
{
    QString s;
    comp.clear();

    QDomElement currentElement;

    currentElement = domElementParent.firstChildElement( KV_COMP_L1 );
    if ( !currentElement.isNull() ) {
        s = currentElement.text();
        if ( s.isNull() )
            s = "";
        comp.setL1( s );
    }

    currentElement = domElementParent.firstChildElement( KV_COMP_L2 );
    if ( !currentElement.isNull() ) {
        s = currentElement.text();
        if ( s.isNull() )
            s = "";
        comp.setL2( s );
    }

    currentElement = domElementParent.firstChildElement( KV_COMP_L3 );
    if ( !currentElement.isNull() ) {
        s = currentElement.text();
        if ( s.isNull() )
            s = "";
        comp.setL3( s );
    }
    return true;
}


bool KEduVocKvtmlReader::readMultipleChoice( QDomElement &domElementParent, KEduVocMultipleChoice &mc )
/*
 <multiplechoice>
   <mc1>good</mc1>
   <mc2>better</mc2>
   <mc3>best</mc3>
   <mc4>best 2</mc4>
   <mc5>best 3</mc5>
 </multiplechoice>
*/

{
    QString s;
    mc.clear();

    QDomElement currentElement;

    currentElement = domElementParent.firstChildElement( KV_MC_1 );
    if ( !currentElement.isNull() ) {
        s = currentElement.text();
        mc.appendChoice( s );
    }

    currentElement = domElementParent.firstChildElement( KV_MC_2 );
    if ( !currentElement.isNull() ) {
        s = currentElement.text();
        mc.appendChoice( s );
    }

    currentElement = domElementParent.firstChildElement( KV_MC_3 );
    if ( !currentElement.isNull() ) {
        s = currentElement.text();
        mc.appendChoice( s );
    }

    currentElement = domElementParent.firstChildElement( KV_MC_4 );
    if ( !currentElement.isNull() ) {
        s = currentElement.text();
        mc.appendChoice( s );
    }

    currentElement = domElementParent.firstChildElement( KV_MC_5 );
    if ( !currentElement.isNull() ) {
        s = currentElement.text();
        mc.appendChoice( s );
    }

    return true;
}


bool KEduVocKvtmlReader::readExpressionChildAttributes( QDomElement &domElementExpressionChild,
        QString &lang,
        grade_t &grade, grade_t &rev_grade,
        int &count, int &rev_count,
        QDateTime &date, QDateTime &rev_date,
        QString &remark,
        int &bcount, int &rev_bcount,
        QString &query_id,
        QString &pronunciation,
        int &width,
        QString &type,
        QString &subType,
        QString &faux_ami_f,
        QString &faux_ami_t,
        QString &synonym,
        QString &example,
        QString &antonym,
        QSet<QString> &usages,
        QString &paraphrase )
{
    int pos;
    QDomAttr attribute;

    lang = "";
    attribute = domElementExpressionChild.attributeNode( KV_LANG );
    if ( !attribute.isNull() )
        lang = attribute.value();

    width = -1;
    attribute = domElementExpressionChild.attributeNode( KV_SIZEHINT );
    if ( !attribute.isNull() )
        width = attribute.value().toInt();

    grade = KV_NORM_GRADE;
    rev_grade = KV_NORM_GRADE;
    attribute = domElementExpressionChild.attributeNode( KV_GRADE );
    if ( !attribute.isNull() ) {
        QString s = attribute.value();
        if (( pos = s.indexOf( ';' ) ) >= 1 ) {
            grade = s.left( pos ).toInt();
            rev_grade = s.mid( pos + 1, s.length() ).toInt();
        } else
            grade = s.toInt();
    }

    count = 0;
    rev_count = 0;
    attribute = domElementExpressionChild.attributeNode( KV_COUNT );
    if ( !attribute.isNull() ) {
        QString s = attribute.value();
        if (( pos = s.indexOf( ';' ) ) >= 1 ) {
            count = s.left( pos ).toInt();
            rev_count = s.mid( pos + 1, s.length() ).toInt();
        } else
            count = s.toInt();
    }

    bcount = 0;
    rev_bcount = 0;
    attribute = domElementExpressionChild.attributeNode( KV_BAD );
    if ( !attribute.isNull() ) {
        QString s = attribute.value();
        if (( pos = s.indexOf( ';' ) ) >= 1 ) {
            bcount = s.left( pos ).toInt();
            rev_bcount = s.mid( pos + 1, s.length() ).toInt();
        } else
            bcount = s.toInt();
    }

    date.setTime_t( 0 );
    rev_date.setTime_t( 0 );
    attribute = domElementExpressionChild.attributeNode( KV_DATE );
    if ( !attribute.isNull() ) {
        QString s = attribute.value();
        if (( pos = s.indexOf( ';' ) ) >= 1 ) {
            date.setTime_t( s.left( pos ).toInt() );
            rev_date.setTime_t( s.mid( pos + 1, s.length() ).toInt() );
        } else
            date.setTime_t( s.toInt() );
    }

    attribute = domElementExpressionChild.attributeNode( KV_DATE2 );
    if ( !attribute.isNull() ) {
        //this format is deprecated and ignored.
    }

    remark = "";
    attribute = domElementExpressionChild.attributeNode( KV_REMARK );
    if ( !attribute.isNull() )
        remark = attribute.value();

    faux_ami_f = "";
    attribute = domElementExpressionChild.attributeNode( KV_FAUX_AMI_F );
    if ( !attribute.isNull() )
        faux_ami_f = attribute.value();

    faux_ami_t = "";
    attribute = domElementExpressionChild.attributeNode( KV_FAUX_AMI_T );
    if ( !attribute.isNull() )
        faux_ami_t = attribute.value();

    synonym = "";
    attribute = domElementExpressionChild.attributeNode( KV_SYNONYM );
    if ( !attribute.isNull() )
        synonym = attribute.value();

    example = "";
    attribute = domElementExpressionChild.attributeNode( KV_EXAMPLE );
    if ( !attribute.isNull() )
        example = attribute.value();

///@todo usages

    attribute = domElementExpressionChild.attributeNode( KV_USAGE );
    if ( !attribute.isNull() ) {
        kDebug() << "Read usages: " << attribute.value();
        usages = m_compability.usageFromKvtml1( attribute.value() );

        /*
        usage = attribute.value();

        if (usage.length() != 0 && usage.left(1) == UL_USER_USAGE)
        {
          int num = qMin(usage.mid (1, 40).toInt(), 1000); // paranioa check
          if (num > m_doc->usageDescriptions().count())
          {
            // description missing ?
            QStringList sl = m_doc->usageDescriptions();
            QString s;
            for (int i = m_doc->usageDescriptions().count(); i < num; i++)
            {
              s.setNum(i + 1);
              s.prepend("#");  // invent descr according to number
              sl.append(s);
            }
            m_doc->setUsageDescriptions(sl);
          }
        }*/
    }

    paraphrase = "";
    attribute = domElementExpressionChild.attributeNode( KV_PARAPHRASE );
    if ( !attribute.isNull() )
        paraphrase = attribute.value();

    antonym = "";
    attribute = domElementExpressionChild.attributeNode( KV_ANTONYM );
    if ( !attribute.isNull() )
        antonym = attribute.value();

    // this is all done by reference - so we have to care about "type" :(
    attribute = domElementExpressionChild.attributeNode( KV_EXPRTYPE );
    if ( !attribute.isNull() ) {
        QString oldType = attribute.value();
        if ( oldType.length() >= 2 && type.left( 1 ) == QM_USER_TYPE ) {
            // they started counting at 1
            int selfDefinedTypeIndex = oldType.right( type.count()-1 ).toInt() -1;
            // append invented types (do we not trust our own writer?)
            if ( selfDefinedTypeIndex >= m_oldSelfDefinedTypes.count() ) {
                while ( selfDefinedTypeIndex >= m_oldSelfDefinedTypes.count() ) {
                    m_oldSelfDefinedTypes.append( i18n( "User defined word type %1", m_oldSelfDefinedTypes.count() - 1 ) );
                }
            }
            type = m_oldSelfDefinedTypes.value( selfDefinedTypeIndex );
        } else {
            type = m_compability.mainTypeFromOldFormat( oldType );
            subType = m_compability.subTypeFromOldFormat( oldType );
        } // not user defined - preset types
    }

    pronunciation = "";
    attribute = domElementExpressionChild.attributeNode( KV_PRONUNCE );
    if ( !attribute.isNull() )
        pronunciation = attribute.value();

    query_id = "";
    attribute = domElementExpressionChild.attributeNode( KV_QUERY );
    if ( !attribute.isNull() )
        query_id = attribute.value();

    return true;
}


bool KEduVocKvtmlReader::readExpression( QDomElement &domElementParent )
{
    grade_t                   grade;
    grade_t                   r_grade;
    int                       qcount;
    int                       r_qcount;
    int                       bcount;
    int                       r_bcount;
    QString                   remark;
    QString                   pronunciation;
    QDateTime                 qdate;
    QDateTime                 r_qdate;
    bool                      inquery;
    bool                      active;
    QString                   lang;
    QString                   textstr;
    QString                   q_org;
    QString                   q_trans;
    QString                   query_id;
    int                       lesson = - 1;
    int                       width;
    QString                   type;
    QString                   subType;
    QString                   faux_ami_f;
    QString                   faux_ami_t;
    QString                   synonym;
    QString                   example;
    QString                   antonym;
    QSet<QString>             usage;
    QString                   paraphrase;

    KEduVocComparison         comparison;
    KEduVocMultipleChoice     mc;
    KEduVocExpression         expr;

    QDomAttr                  attribute;
    QDomElement               currentElement;
    QDomElement               currentChild;

    //-------------------------------------------------------------------------
    // Attributes
    //-------------------------------------------------------------------------

    attribute = domElementParent.attributeNode( KV_LESS_MEMBER );
    if ( !attribute.isNull() ) {
        // we start conting from 0 in new documents
        lesson = attribute.value().toInt() - 1;
        if ( lesson > m_doc->lessonCount() ) {
            ///@todo can this happen? does it need a while loop?
            // it's from a lesson that hasn't been added yet
            // so make sure this lesson is in the document
            kDebug() << "Warning: lesson > m_doc->lessonCount() in readExpression.";
            m_doc->appendLesson( i18nc("A generic name for a new lesson and its number.", "Lesson %1" ), lesson );
        }
    }

    attribute = domElementParent.attributeNode( KV_SELECTED );
    if ( !attribute.isNull() )
        inquery = attribute.value() == "1" ? true : false;
    else
        inquery = false;

    attribute = domElementParent.attributeNode( KV_INACTIVE );
    if ( !attribute.isNull() )
        active = attribute.value() == "1" ? false : true;
    else
        active = true;

    // this is all done by reference - so we have to care about "type" :(
    attribute = domElementParent.attributeNode( KV_EXPRTYPE );
    if ( !attribute.isNull() ) {
        QString oldType = attribute.value();

        if ( oldType.length() >= 2 && type.left( 1 ) == QM_USER_TYPE ) {
            // they started counting at 1
            int selfDefinedTypeIndex = oldType.right( type.count()-1 ).toInt() -1;
            // append invented types (do we not trust our own writer?)
            if ( selfDefinedTypeIndex >= m_oldSelfDefinedTypes.count() ) {
                while ( selfDefinedTypeIndex >= m_oldSelfDefinedTypes.count() ) {
                    m_oldSelfDefinedTypes.append( i18n( "User defined word type %1", m_oldSelfDefinedTypes.count() - 1 ) );
                }
            }
            type = m_oldSelfDefinedTypes.value( selfDefinedTypeIndex );
        } else {
            type = m_compability.mainTypeFromOldFormat( oldType );
            subType = m_compability.subTypeFromOldFormat( oldType );
        } // not user defined - preset types
    }


    //-------------------------------------------------------------------------
    // Children 'Translation'
    //-------------------------------------------------------------------------

    //QDomNodeList translationList = domElementParent.elementsByTagName(KV_TRANS);

    // count which translation we are on
    int i=0;

    // kvtml 1: we always have an original element (required)
    currentElement = domElementParent.firstChildElement( KV_ORG );
    if ( currentElement.isNull() ) { // sanity check
        m_errorMessage = i18n( "Data for original language missing" );
        return false;
    }

    while ( !currentElement.isNull() ) {

        //-----------
        // Attributes
        //-----------

        // read attributes - the order of the query grades is interchanged!
        if ( i == 0 && !readExpressionChildAttributes( currentElement, lang, grade, r_grade, qcount, r_qcount, qdate, r_qdate, remark, bcount, r_bcount, query_id,
                pronunciation, width, type, subType, faux_ami_t, faux_ami_f, synonym, example, antonym, usage, paraphrase ) )
            return false;

        if ( i != 0 && !readExpressionChildAttributes( currentElement, lang, grade, r_grade, qcount, r_qcount, qdate, r_qdate, remark, bcount, r_bcount, query_id,
                pronunciation, width, type, subType, faux_ami_f, faux_ami_t, synonym, example, antonym, usage, paraphrase ) )
            return false;

        if ( m_doc->entryCount() == 0 ) {
            // only accept in first entry
            if ( width >= 0 )
                m_doc->setSizeHint( i, width );

            if ( query_id == KV_O )
                q_org = lang;

            if ( query_id == KV_T )

                q_trans = lang;
        }

        if ( m_doc->entryCount() == 0 ) { // this is because in kvtml the languages are saved in the FIRST ENTRY ONLY.

            // new translation
            if (!addLanguage(i, lang)) {
                return false;
            }
        }
        //---------
        // Children

        currentChild = currentElement.firstChildElement( KV_COMPARISON_GRP );
        if ( !currentChild.isNull() ) {
            comparison.clear();
            if ( !readComparison( currentChild, comparison ) )
                return false;
        }

        currentChild = currentElement.firstChildElement( KV_MULTIPLECHOICE_GRP );
        if ( !currentChild.isNull() ) {
            mc.clear();
            if ( !readMultipleChoice( currentChild, mc ) )
                return false;
        }

        textstr = currentElement.lastChild().toText().data();

        if ( i == 0 ) {
            expr = KEduVocExpression( textstr, lesson );
            expr.setActive( active );
        } else {
            expr.setTranslation( i, textstr );
        }

        // better make sure, translation(i) already exists...
        currentChild = currentElement.firstChildElement( KV_CONJUG_GRP );
        if ( !currentChild.isNull() ) {
            if ( !readTranslationConjugations( currentChild, expr.translation(i) ) ) {
                return false;
            }
        }

        if ( !comparison.isEmpty() ) {
            expr.translation( i ).setComparison( comparison );
            comparison.clear();
        }
        if ( !mc.isEmpty() ) {
            expr.translation( i ).setMultipleChoice( mc );
            mc.clear();
        }

        if ( !type.isEmpty() ) {
            expr.translation( i ).setType( type );
            if ( !subType.isEmpty() ) {
                expr.translation( i ).setSubType( subType );
            }
        }

        if ( !remark.isEmpty() )
            expr.translation( i ).setComment( remark );
        if ( !pronunciation.isEmpty() )
            expr.translation( i ).setPronunciation( pronunciation );
        if ( !faux_ami_f.isEmpty() )
            expr.translation( i ).setFalseFriend( 0, faux_ami_f );
        if ( !faux_ami_t.isEmpty() )
            expr.translation( 0 ).setFalseFriend( i, faux_ami_t );
        if ( !synonym.isEmpty() )
            expr.translation( i ).setSynonym( synonym );
        if ( !example.isEmpty() )
            expr.translation( i ).setExample( example );
        if ( !usage.isEmpty() )
            expr.translation( i ).setUsages( usage );
        if ( !paraphrase.isEmpty() )
            expr.translation( i ).setParaphrase( paraphrase );
        if ( !antonym.isEmpty() )
            expr.translation( i ).setAntonym( antonym );

        if ( i != 0 ) {
            expr.translation( i ).gradeFrom( 0 ).setGrade( grade );
            expr.translation( 0 ).gradeFrom( i ).setGrade( r_grade );
            expr.translation( i ).gradeFrom( 0 ).setQueryCount( qcount );
            expr.translation( 0 ).gradeFrom( i ).setQueryCount( r_qcount );
            expr.translation( i ).gradeFrom( 0 ).setBadCount( bcount );
            expr.translation( 0 ).gradeFrom( i ).setBadCount( r_bcount );
            expr.translation( i ).gradeFrom( 0 ).setQueryDate( qdate );
            expr.translation( 0 ).gradeFrom( i ).setQueryDate( r_qdate );
        }

        // Next translation
        currentElement = currentElement.nextSiblingElement( KV_TRANS );
        i++;
    }

//     if ( m_doc->entryCount() == 0 ) {
//         m_doc->setQueryIdentifier( q_org, q_trans );
//     }
    m_doc->appendEntry( &expr );

    return true;
}


bool KEduVocKvtmlReader::addLanguage( int languageId, const QString& locale)
{
    if ( m_doc->identifierCount() <= languageId ) {
        m_doc->appendIdentifier();
        // first entry
        if ( !locale.isEmpty() ) {      // no definition in first entry
            m_doc->identifier(languageId).setLocale(locale);

            QString languageName = KGlobal::locale()->languageCodeToName(locale);
            if ( languageName.isEmpty() ) {
                languageName = locale;
            }

            m_doc->identifier(languageId).setName(languageName);
            kDebug() << "addLanguage( " << languageId << ", " << locale << "): " << languageName;

        }
    } else {
        if ( !locale.isEmpty() ) {
            if ( locale != m_doc->identifier(languageId).locale() ) {
                // different originals ?
                m_errorMessage = i18n( "Ambiguous definition of language code" );
                return false;
            }
        }
    }
    return true;
}




#include "keduvockvtmlreader.moc"
