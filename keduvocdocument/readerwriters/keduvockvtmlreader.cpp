/*
 * read a KEduVocDocument from a KVTML file
 * SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
 * SPDX-FileCopyrightText: 2005 Eric Pignet <eric at erixpage.com>
 * SPDX-FileCopyrightText: 2007 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "keduvockvtmlreader.h"

#include <KLocalizedString>
#include <QTextStream>
#include <QIODevice>

#include <QDebug>
#include <QLocale>

#include "keduvoclesson.h"
#include "keduvocwordtype.h"
#include "kvtmldefs.h"
#include "keduvoccommon_p.h"

KEduVocKvtmlReader::KEduVocKvtmlReader(QIODevice & file)
    : m_inputFile( &file )
{
    m_errorMessage = QLatin1String("");
    qDebug() << "KEduVocKvtmlReader for kvtml version 1 files started.";
}

bool KEduVocKvtmlReader::isParsable()
{
    QTextStream ts( m_inputFile );
    QString line1( ts.readLine() );
    QString line2( ts.readLine() );

    m_inputFile->seek( 0 );
    return  ( ( line1.startsWith(QLatin1String("<?xml")) )
              && ( line2.indexOf( KV_DOCTYPE, 0 ) >  0 ) );
}

KEduVocDocument::ErrorCode KEduVocKvtmlReader::read(KEduVocDocument &doc)
{
    m_doc = &doc;
    m_cols = 0;
    m_lines = 0;

    QDomDocument domDoc( QStringLiteral("KEduVocDocument") );

    if ( !domDoc.setContent( m_inputFile, &m_errorMessage ) )
        return KEduVocDocument::InvalidXml;

    QDomElement domElementKvtml = domDoc.documentElement();
    if ( domElementKvtml.tagName() != KV_DOCTYPE ) {
        m_errorMessage = i18n( "This is not a KDE Vocabulary document." );
        return KEduVocDocument::FileTypeUnknown;
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

    return result ? KEduVocDocument::NoError : KEduVocDocument::FileReaderFailed;
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
    m_compability.setupWordTypes(m_doc->wordTypeContainer());

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

    for(int i = 0; i < m_doc->identifierCount(); i++) {
        m_doc->identifier(i).setTenseList(m_compability.documentTenses());
    }

    return true;
}


bool KEduVocKvtmlReader::readLesson( QDomElement &domElementParent )
{
    QString s;
    QDomAttr attribute;
    QDomElement currentElement;


    //-------------------------------------------------------------------------
    // Children
    //-------------------------------------------------------------------------

    QDomNodeList entryList = domElementParent.elementsByTagName( KV_LESS_DESC );
    if ( entryList.length() <= 0 )
        return false;

    for ( int i = 0; i < entryList.count(); ++i ) {
        currentElement = entryList.item( i ).toElement();
        if ( currentElement.parentNode() == domElementParent ) {
            int no = -1;

            attribute = currentElement.attributeNode( KV_LESS_NO );
            if ( !attribute.isNull() ) {
                no = attribute.value().toInt();
            }

            bool inQuery = false;
            attribute = currentElement.attributeNode( KV_LESS_QUERY );
            if ( !attribute.isNull() ) {
                inQuery = (attribute.value().toInt() != 0);
            }

            s = currentElement.text();
            KEduVocLesson* lesson = new KEduVocLesson(s, m_doc->lesson());
            lesson->setInPractice(inQuery);
            m_doc->lesson()->appendChildContainer( lesson );
            if ( m_doc->lesson()->childContainerCount() != no-1 ) {
                qDebug() << "Warning! Lesson order may be confused. Are all lessons in order in the file?";
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

//qDebug() << "KEduVocKvtmlReader::readArticle() read " << entryList.count() << " articles. ";
        currentElement = entryList.item( i ).toElement();
        if ( currentElement.parentNode() == domElementParent ) {
            QString lang;
            attribute = currentElement.attributeNode( KV_LANG );

            if (!addLanguage(i, attribute.value())) {
                return false;
            }

            //---------
            // Children

            QString fem_def = QLatin1String("");
            QString mal_def = QLatin1String("");
            QString nat_def = QLatin1String("");
            QString fem_indef = QLatin1String("");
            QString mal_indef = QLatin1String("");
            QString nat_indef = QLatin1String("");

            article = currentElement.firstChildElement( KV_ART_FD );
            if ( !article.isNull() ) {
                fem_def = article.text();
                if ( fem_def.isNull() )
                    fem_def = QLatin1String("");
            }

            article = currentElement.firstChildElement( KV_ART_FI );
            if ( !article.isNull() ) {
                fem_indef = article.text();
                if ( fem_indef.isNull() )
                    fem_indef = QLatin1String("");
            }

            article = currentElement.firstChildElement( KV_ART_MD );
            if ( !article.isNull() ) {
                mal_def = article.text();
                if ( mal_def.isNull() )
                    mal_def = QLatin1String("");
            }

            article = currentElement.firstChildElement( KV_ART_MI );
            if ( !article.isNull() ) {
                mal_indef = article.text();
                if ( mal_indef.isNull() )
                    mal_indef = QLatin1String("");
            }

            article = currentElement.firstChildElement( KV_ART_ND );
            if ( !article.isNull() ) {
                nat_def = article.text();
                if ( nat_def.isNull() )
                    nat_def = QLatin1String("");
            }

            article = currentElement.firstChildElement( KV_ART_NI );
            if ( !article.isNull() ) {
                nat_indef = article.text();
                if ( nat_indef.isNull() )
                    nat_indef = QLatin1String("");
            }

            KEduVocArticle article;
            article.setArticle(
                mal_def,
                KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Masculine
            );
            article.setArticle(
                fem_def,
                KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Feminine
            );
            article.setArticle(
                nat_def,
                KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Neuter
            );
            article.setArticle(
                mal_indef,
                KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Masculine
            );
            article.setArticle(
                fem_indef,
                KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Feminine
            );
            article.setArticle(
                nat_indef,
                KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Neuter
            );
            m_doc->identifier(i).setArticle(article);
        }
    }

    return true;
}


bool KEduVocKvtmlReader::readTranslationConjugations( QDomElement &domElementParent, KEduVocTranslation* translation )
{
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
        translation->setConjugation(tense, conjugation);

        domElementConjugChild = domElementConjugChild.nextSiblingElement( KV_CON_TYPE );
    } // while -> next tense, count++
    return true;
}

bool KEduVocKvtmlReader::readConjugation( QDomElement &domElementParent, KEduVocConjugation& conjugation )
/*
 <conjugation>        used in header for definition of "prefix"
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

    const KEduVocWordFlags numS = KEduVocWordFlag::Singular;
    const KEduVocWordFlags numP = KEduVocWordFlag::Plural;

    conjugation.setConjugation( pers1_sing, KEduVocWordFlag::First | numS);
    conjugation.setConjugation( pers2_sing, KEduVocWordFlag::Second | numS);
    conjugation.setConjugation( pers1_plur, KEduVocWordFlag::First | numP);
    conjugation.setConjugation( pers2_plur, KEduVocWordFlag::Second | numP);

    if ( s3_common ) {
        conjugation.setConjugation( pers3_f_sing, KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Singular );
    } else  {
        conjugation.setConjugation( pers3_m_sing,
            KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | KEduVocWordFlag::Singular );
        conjugation.setConjugation( pers3_f_sing,
            KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Singular );
        conjugation.setConjugation( pers3_n_sing,
            KEduVocWordFlag::Third | KEduVocWordFlag::Neuter |  KEduVocWordFlag::Singular );
    }

    if ( p3_common ) {
        conjugation.setConjugation( pers3_f_plur, KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Plural );
    } else  {
        conjugation.setConjugation( pers3_m_plur,
            KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | KEduVocWordFlag::Plural );
        conjugation.setConjugation( pers3_f_plur,
            KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Plural );
        conjugation.setConjugation( pers3_n_plur,
            KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Plural );
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

    KEduVocWordFlags numS = KEduVocWordFlag::Singular;
    pronouns.setMaleFemaleDifferent(false);
    pronouns.setPersonalPronoun( pers1_sing, KEduVocWordFlag::First | numS );
    pronouns.setPersonalPronoun( pers2_sing, KEduVocWordFlag::Second | numS );

    // used to have common in female
    if ( s3_common ) {
        pronouns.setPersonalPronoun( pers3_f_sing, KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | numS );
    } else  {
        pronouns.setPersonalPronoun( pers3_m_sing,
            KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | numS );
        pronouns.setPersonalPronoun( pers3_f_sing,
            KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | numS );
        pronouns.setPersonalPronoun( pers3_n_sing,
            KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | numS );
        pronouns.setMaleFemaleDifferent(true);
    }

    KEduVocWordFlags numP = KEduVocWordFlag::Plural;

    pronouns.setPersonalPronoun( pers1_plur, KEduVocWordFlag::First | numP );
    pronouns.setPersonalPronoun( pers2_plur, KEduVocWordFlag::Second | numP );
    if ( p3_common ) {
        pronouns.setPersonalPronoun( pers3_f_plur, KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | numP );
    } else  {
        pronouns.setPersonalPronoun( pers3_m_plur,
            KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | numP );
        pronouns.setPersonalPronoun( pers3_f_plur,
            KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | numP );
        pronouns.setPersonalPronoun( pers3_n_plur,
            KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | numP );
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

            qDebug() << "Adding old self defined type: " << currentElement.text();
            // add the type to the list of available types
            KEduVocWordType* type = new KEduVocWordType(currentElement.text(), m_doc->wordTypeContainer());
            m_doc->wordTypeContainer()->appendChildContainer( type );

            // from this the #1 are transformed to something sensible again
            m_oldSelfDefinedTypes.append( currentElement.text() );
        }
    }

    return true;
}


bool KEduVocKvtmlReader::readTense( QDomElement &domElementParent )
{
    QDomElement currentElement;

    currentElement = domElementParent.firstChildElement( KV_TENSE_DESC );
    while ( !currentElement.isNull() ) {
        qDebug() << "Reading user defined tense description: " << currentElement.text();
        m_compability.addUserdefinedTense( currentElement.text() );
        currentElement = currentElement.nextSiblingElement( KV_TENSE_DESC );
    }
    return true;
}


bool KEduVocKvtmlReader::readComparison( QDomElement &domElementParent, KEduVocTranslation * translation )
/*
 <comparison>
   <l1>good</l1> --- this one is dead as it always has to be the word itself
   <l2>better</l2>
   <l3>best</l3>
 </comparison>
*/
{
    QDomElement currentElement;

    currentElement = domElementParent.firstChildElement( KV_COMP_L2 );
    translation->setComparativeForm(currentElement.text());

    currentElement = domElementParent.firstChildElement( KV_COMP_L3 );
    translation->setSuperlativeForm(currentElement.text());

    return true;
}


bool KEduVocKvtmlReader::readMultipleChoice( QDomElement &domElementParent, KEduVocTranslation* translation )
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
    QDomElement currentElement;
    QStringList choices = translation->getMultipleChoice();

    currentElement = domElementParent.firstChildElement( KV_MC_1 );
    if ( !currentElement.isNull() ) {
        choices.append(currentElement.text());
    }

    currentElement = domElementParent.firstChildElement( KV_MC_2 );
    if ( !currentElement.isNull() ) {
        choices.append(currentElement.text());
    }

    currentElement = domElementParent.firstChildElement( KV_MC_3 );
    if ( !currentElement.isNull() ) {
        choices.append(currentElement.text());
    }

    currentElement = domElementParent.firstChildElement( KV_MC_4 );
    if ( !currentElement.isNull() ) {
        choices.append(currentElement.text());
    }

    currentElement = domElementParent.firstChildElement( KV_MC_5 );
    if ( !currentElement.isNull() ) {
        choices.append(currentElement.text());
    }

    translation->setMultipleChoice(choices);

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
        QString &faux_ami_f,
        QString &faux_ami_t,
        QString &synonym,
        QString &example,
        QString &antonym,
        QSet<QString> &usages,
        QString &paraphrase )
{
    Q_UNUSED(usages)
    int pos;
    QDomAttr attribute;

    lang = QLatin1String("");
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
            grade = s.leftRef( pos ).toInt();
            rev_grade = s.midRef( pos + 1, s.length() ).toInt();
        } else
            grade = s.toInt();
    }

    count = 0;
    rev_count = 0;
    attribute = domElementExpressionChild.attributeNode( KV_COUNT );
    if ( !attribute.isNull() ) {
        QString s = attribute.value();
        if (( pos = s.indexOf( ';' ) ) >= 1 ) {
            count = s.leftRef( pos ).toInt();
            rev_count = s.midRef( pos + 1, s.length() ).toInt();
        } else
            count = s.toInt();
    }

    bcount = 0;
    rev_bcount = 0;
    attribute = domElementExpressionChild.attributeNode( KV_BAD );
    if ( !attribute.isNull() ) {
        QString s = attribute.value();
        if (( pos = s.indexOf( ';' ) ) >= 1 ) {
            bcount = s.leftRef( pos ).toInt();
            rev_bcount = s.midRef( pos + 1, s.length() ).toInt();
        } else
            bcount = s.toInt();
    }

    date.setTime_t( 0 );
    rev_date.setTime_t( 0 );
    attribute = domElementExpressionChild.attributeNode( KV_DATE );
    if ( !attribute.isNull() ) {
        QString s = attribute.value();
        if (( pos = s.indexOf( ';' ) ) >= 1 ) {
            date.setTime_t( s.leftRef( pos ).toInt() );
            rev_date.setTime_t( s.midRef( pos + 1, s.length() ).toInt() );
        } else
            date.setTime_t( s.toInt() );
    }

    attribute = domElementExpressionChild.attributeNode( KV_DATE2 );
    if ( !attribute.isNull() ) {
        //this format is deprecated and ignored.
    }

    remark = QLatin1String("");
    attribute = domElementExpressionChild.attributeNode( KV_REMARK );
    if ( !attribute.isNull() )
        remark = attribute.value();

    faux_ami_f = QLatin1String("");
    attribute = domElementExpressionChild.attributeNode( KV_FAUX_AMI_F );
    if ( !attribute.isNull() )
        faux_ami_f = attribute.value();

    faux_ami_t = QLatin1String("");
    attribute = domElementExpressionChild.attributeNode( KV_FAUX_AMI_T );
    if ( !attribute.isNull() )
        faux_ami_t = attribute.value();

    synonym = QLatin1String("");
    attribute = domElementExpressionChild.attributeNode( KV_SYNONYM );
    if ( !attribute.isNull() )
        synonym = attribute.value();

    example = QLatin1String("");
    attribute = domElementExpressionChild.attributeNode( KV_EXAMPLE );
    if ( !attribute.isNull() )
        example = attribute.value();

    paraphrase = QLatin1String("");
    attribute = domElementExpressionChild.attributeNode( KV_PARAPHRASE );
    if ( !attribute.isNull() )
        paraphrase = attribute.value();

    antonym = QLatin1String("");
    attribute = domElementExpressionChild.attributeNode( KV_ANTONYM );
    if ( !attribute.isNull() )
        antonym = attribute.value();

    // this is all done by reference - so we have to care about "type" :(
    attribute = domElementExpressionChild.attributeNode( KV_EXPRTYPE );
    if ( !attribute.isNull() ) {
        type = attribute.value();
    }

    pronunciation = QLatin1String("");
    attribute = domElementExpressionChild.attributeNode( KV_PRONUNCE );
    if ( !attribute.isNull() )
        pronunciation = attribute.value();

    query_id = QLatin1String("");
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
    // bool                      inquery;
    bool                      active;
    QString                   lang;
    QString                   textstr;
    QString                   q_org;
    QString                   q_trans;
    QString                   query_id;
    int                       width;
    QString                   type;
    QString                   faux_ami_f;
    QString                   faux_ami_t;
    QString                   synonym;
    QString                   example;
    QString                   antonym;
    QSet<QString>             usage;
    QString                   paraphrase;

    QDomAttr                  attribute;
    QDomElement               currentElement;
    QDomElement               currentChild;

    int lessonNumber = -1;

    //-------------------------------------------------------------------------
    // Attributes
    //-------------------------------------------------------------------------

    attribute = domElementParent.attributeNode( KV_LESS_MEMBER );
    if ( !attribute.isNull() ) {
        // we start counting from 0 in new documents
        lessonNumber = attribute.value().toInt() - 1;
        if ( lessonNumber > m_doc->lesson()->childContainerCount() ) {
            ///@todo can this happen? does it need a while loop?
            // it's from a lesson that hasn't been added yet
            // so make sure this lesson is in the document
            qDebug() << "Warning: lesson > m_doc->lessonCount() in readExpression.";

            KEduVocLesson* lesson = new KEduVocLesson(i18nc("A generic name for a new lesson and its number.", "Lesson %1", lessonNumber ), m_doc->lesson());
            m_doc->lesson()->appendChildContainer(lesson);
        }
    }

    attribute = domElementParent.attributeNode( KV_SELECTED );
    // if ( !attribute.isNull() )
    //     inquery = attribute.value() == "1" ? true : false;
    // else
    //     inquery = false;

    attribute = domElementParent.attributeNode( KV_INACTIVE );
    if ( !attribute.isNull() )
        active = attribute.value() == QLatin1Char('1') ? false : true;
    else
        active = true;

    // this is all done by reference - so we have to care about "type" :(
    attribute = domElementParent.attributeNode( KV_EXPRTYPE );
    if ( !attribute.isNull() ) {
        type = attribute.value();
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

    KEduVocExpression* entry = 0;

    while ( !currentElement.isNull() ) {

        //-----------
        // Attributes
        //-----------

        // read attributes - the order of the query grades is interchanged!
        if ( i == 0 && !readExpressionChildAttributes( currentElement, lang, grade, r_grade, qcount, r_qcount, qdate, r_qdate, remark, bcount, r_bcount, query_id,
                pronunciation, width, type, faux_ami_t, faux_ami_f, synonym, example, antonym, usage, paraphrase ) ) {
            return false;
        }

        if ( i != 0 && !readExpressionChildAttributes( currentElement, lang, grade, r_grade, qcount, r_qcount, qdate, r_qdate, remark, bcount, r_bcount, query_id,
                pronunciation, width, type, faux_ami_f, faux_ami_t, synonym, example, antonym, usage, paraphrase ) ) {
            return false;
        }


        //---------
        // Children

        textstr = currentElement.lastChild().toText().data();

        if ( i == 0 ) {
            entry = new KEduVocExpression( textstr );
            entry->setActive( active );
            if ( lessonNumber != -1 ) {
                static_cast<KEduVocLesson*>(m_doc->lesson()->childContainer(lessonNumber))->appendEntry(entry);
            } else {
                m_doc->lesson()->appendEntry(entry);
            }
        } else {
            entry->setTranslation( i, textstr );
        }

        if ( m_doc->lesson()->entries(KEduVocLesson::Recursive).count() == 1 ) { // this is because in kvtml the languages are saved in the FIRST ENTRY ONLY.
            // new translation
            if (!addLanguage(i, lang)) {
                return false;
            }
        }

        // better make sure, translation(i) already exists...
        currentChild = currentElement.firstChildElement( KV_CONJUG_GRP );
        if ( !currentChild.isNull() ) {
            if ( !readTranslationConjugations( currentChild, entry->translation(i) ) ) {
                return false;
            }
        }

        currentChild = currentElement.firstChildElement( KV_MULTIPLECHOICE_GRP );
        if ( !currentChild.isNull() ) {
            if ( !readMultipleChoice( currentChild, entry->translation(i) ) ) {
                return false;
            }
        }

        currentChild = currentElement.firstChildElement( KV_COMPARISON_GRP );
        if ( !currentChild.isNull() ) {
            if ( !readComparison( currentChild, entry->translation(i) ) ) {
                return false;
            }
        }

        if ( !type.isEmpty() ) {
            KEduVocWordType* wordType = m_compability.typeFromOldFormat(m_doc->wordTypeContainer(), type);
            entry->translation(i)->setWordType(wordType);
        }

        if ( !remark.isEmpty() )
            entry->translation( i )->setComment( remark );
        if ( !pronunciation.isEmpty() )
            entry->translation( i )->setPronunciation( pronunciation );

        ///@todo include false friends from kvtml-1 again?
//         if ( !faux_ami_f.isEmpty() )
//             entry->translation( i )->setFalseFriend( 0, faux_ami_f );
//         if ( !faux_ami_t.isEmpty() )
//             entry->translation( 0 )->setFalseFriend( i, faux_ami_t );
        ///@todo include synonyms from kvtml-1 again?
//         if ( !synonym.isEmpty() )
//             entry->translation( i )->setSynonym( synonym );
//         if ( !antonym.isEmpty() )
//             entry->translation( i )->setAntonym( antonym );

        if ( !example.isEmpty() )
            entry->translation( i )->setExample( example );
        if ( !paraphrase.isEmpty() )
            entry->translation( i )->setParaphrase( paraphrase );

        if ( i != 0 ) {
            entry->translation( i )->setGrade( grade );
            entry->translation( 0 )->setGrade( r_grade );
            entry->translation( i )->setPracticeCount( qcount );
            entry->translation( 0 )->setPracticeCount( r_qcount );
            entry->translation( i )->setBadCount( bcount );
            entry->translation( 0 )->setBadCount( r_bcount );
            entry->translation( i )->setPracticeDate( qdate );
            entry->translation( 0 )->setPracticeDate( r_qdate );
        }

        // Next translation
        currentElement = currentElement.nextSiblingElement( KV_TRANS );
        i++;
    }

    return true;
}


bool KEduVocKvtmlReader::addLanguage( int languageId, const QString& locale)
{
    if ( m_doc->identifierCount() <= languageId ) {
        m_doc->appendIdentifier();
        // first entry
        if ( !locale.isEmpty() ) {      // no definition in first entry
            m_doc->identifier(languageId).setLocale(locale);

            QString languageName;
            // when using from qt-only apps this would crash (converter)
            languageName = QLocale::languageToString(QLocale(locale).language());
            if ( languageName.isEmpty() ) {
                languageName = locale;
            }

            m_doc->identifier(languageId).setName(languageName);
            qDebug() << "addLanguage( " << languageId << ", " << locale << "): " << languageName;

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
