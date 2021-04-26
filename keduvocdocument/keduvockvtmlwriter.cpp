/*
 * export a KEduVocDocument to a KVTML file
 * SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
 * SPDX-FileCopyrightText: 2005 Eric Pignet <eric at erixpage.com>
 * SPDX-FileCopyrightText: 2007 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "keduvockvtmlwriter.h"

#include <QLatin1String>
#include <QTextStream>

#include <QDebug>

#include "keduvocdocument.h"
#include "keduvocgrade.h"
#include "keduvoclesson.h"
#include "keduvocexpression.h"
#include "kvtmldefs.h"
#include "keduvoccommon_p.h"

KEduVocKvtmlWriter::KEduVocKvtmlWriter( QFile *file )
{
    // the file must be already open
    m_outputFile = file;
}


bool KEduVocKvtmlWriter::writeDoc( KEduVocDocument *doc, const QString &generator )
{
    bool first_expr = true;

    m_doc = doc;

    m_domDoc = QDomDocument( "kvtml SYSTEM \"kvoctrain.dtd\"" );
    m_domDoc.appendChild( m_domDoc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"UTF-8\"" ) );
    QDomElement domElementKvtml = m_domDoc.createElement( "kvtml" );
    m_domDoc.appendChild( domElementKvtml );

    domElementKvtml.setAttribute( KV_ENCODING, ( QString ) "UTF-8" );
    domElementKvtml.setAttribute( KV_GENERATOR, generator );
    domElementKvtml.setAttribute( KV_COLS, m_doc->identifierCount() );
    domElementKvtml.setAttribute( KV_LINES, m_doc->entryCount() );

    if ( !m_doc->title().isEmpty() )
        domElementKvtml.setAttribute( KV_TITLE, m_doc->title() );

    if ( !m_doc->author().isEmpty() )
        domElementKvtml.setAttribute( KV_AUTHOR, m_doc->author() );

    if ( !m_doc->license().isEmpty() )
        domElementKvtml.setAttribute( KV_LICENSE, m_doc->license() );

    if ( !m_doc->documentComment().isEmpty() )
        domElementKvtml.setAttribute( KV_DOC_REM, m_doc->documentComment() );

    if ( !writeLesson( domElementKvtml ) )
        return false;

    if ( !writeArticle( domElementKvtml ) )
        return false;

    QList<KEduVocPersonalPronoun> pronouns;
    for ( int i = 0; i < m_doc->identifierCount(); i++ )
        pronouns.append( m_doc->identifier(i).personalPronouns() );
    if ( pronouns.count() > 0 ) {
        if ( !writePersonalPronouns(domElementKvtml, pronouns ) )
            return false;
    }

    if ( !writeType( domElementKvtml ) )
        return false;

    if ( !writeTense( domElementKvtml ) )
        return false;

    if ( !writeUsage( domElementKvtml ) )
        return false;

    QString q_org, q_trans;
    m_doc->queryIdentifier( q_org, q_trans );

    int entryCount = m_doc->entryCount();

    for ( int i = 0; i < entryCount; i++ ) {
        KEduVocExpression *entry = m_doc->entry( i );
        QDomElement domElementExpression = m_domDoc.createElement( KV_EXPR );

        if ( entry->lesson() != 0 ) {
            int lm = entry->lesson();
            if ( lm > m_doc->lessonCount() ) {
                // should not be
                qCritical() << "index of lesson member too high: " << lm << endl;
                lm = 0;
            }
            domElementExpression.setAttribute( KV_LESS_MEMBER, lm );
        }

// Sorry, but we don't support this any more.
//         if ( entry->isInQuery() )
//             domElementExpression.setAttribute( KV_SELECTED, 1 );

        if ( !entry->isActive() )
            domElementExpression.setAttribute( KV_INACTIVE, 1 );

        QDomElement domElementOriginal = m_domDoc.createElement( KV_ORG );
        if ( first_expr ) {
            // save space, only tell language in first entry
            QString s;
            domElementOriginal.setAttribute( KV_SIZEHINT, m_doc->sizeHint( 0 ) );

            s = m_doc->identifier( 0 ).name().simplified();
            if ( s.isEmpty() )
                s = "original";
            domElementOriginal.setAttribute( KV_LANG, s );
            if ( s == q_org )
                domElementOriginal.setAttribute( KV_QUERY, ( QString ) KV_O );
            else if ( s == q_trans )
                domElementOriginal.setAttribute( KV_QUERY, ( QString ) KV_T );
        }

        if ( !entry->translation( 0 ).comment().isEmpty() )
            domElementOriginal.setAttribute( KV_REMARK, entry->translation( 0 ).comment() );

        if ( !entry->translation( 0 ).synonym().isEmpty() )
            domElementOriginal.setAttribute( KV_SYNONYM, entry->translation( 0 ).synonym() );

        if ( !entry->translation( 0 ).example().isEmpty() )
            domElementOriginal.setAttribute( KV_EXAMPLE, entry->translation( 0 ).example() );
///@todo enable writing of usages in old format
        /*
            if (!entry->translation(0).usageLabel().isEmpty() )
              domElementOriginal.setAttribute(KV_USAGE, entry->translation(0).usageLabel());
        */
        if ( !entry->translation( 0 ).paraphrase().isEmpty() )
            domElementOriginal.setAttribute( KV_PARAPHRASE, entry->translation( 0 ).paraphrase() );

        if ( !entry->translation( 0 ).antonym().isEmpty() )
            domElementOriginal.setAttribute( KV_ANTONYM, entry->translation( 0 ).antonym() );

        if ( !entry->translation( 0 ).pronunciation().isEmpty() )
            domElementOriginal.setAttribute( KV_PRONUNCE, entry->translation( 0 ).pronunciation() );

        if ( !entry->translation( 0 ).type().isEmpty() )
            domElementOriginal.setAttribute( KV_EXPRTYPE, m_compability.oldType( entry->translation( 0 ).type(), entry->translation( 0 ).subType() ) );

        if ( !writeMultipleChoice(domElementOriginal, entry->translation( 0 ).multipleChoice() ) )
            return false;

        QString s;
        QString entype = s = entry->translation( 0 ).type();
        int pos = s.indexOf( QM_TYPE_DIV );
        if ( pos >= 0 )
            entype = s.left( pos );
        else
            entype = s;

        if ( entype == QM_VERB ) {
                // conjugation
            if ( !writeConjugEntry(domElementOriginal, entry->translation( 0 )) ) {
                    return false;
            }


        } else if ( entype == QM_ADJ && !entry->translation( 0 ).comparison().isEmpty() ) {
            KEduVocComparison comp = entry->translation( 0 ).comparison();
            if ( !writeComparison(domElementOriginal, comp ) )
                return false;
        }

        QDomText domTextOriginal = m_domDoc.createTextNode( entry->translation( 0 ).text() );
        domElementOriginal.appendChild( domTextOriginal );
        domElementExpression.appendChild( domElementOriginal );

        int trans = 1;
        while ( trans < m_doc->identifierCount() ) {
            QDomElement domElementTranslation = m_domDoc.createElement( KV_TRANS );
            if ( first_expr ) {
                // save space, only tell language in first entry
                QString s;
                domElementTranslation.setAttribute( KV_SIZEHINT, m_doc->sizeHint( trans ) );

                s = m_doc->identifier( trans ).name().simplified();
                if ( s.isEmpty() ) {
                    s.setNum( trans );
                    s.prepend( QLatin1String("translation ") );
                }
                domElementTranslation.setAttribute( KV_LANG, s );
                if ( s == q_org )
                    domElementTranslation.setAttribute( KV_QUERY, ( QString ) KV_O );
                else if ( s == q_trans )
                    domElementTranslation.setAttribute( KV_QUERY, ( QString ) KV_T );
            }

            if ( entry->translation( trans ).gradeFrom( 0 ).grade() != 0 || entry->translation( 0 ).gradeFrom( trans ).grade() != 0 )
                domElementTranslation.setAttribute( KV_GRADE, QString::number( entry->translation( trans ).gradeFrom( 0 ).grade() ) + ';' + QString::number( entry->translation( 0 ).gradeFrom( trans ).grade() ) );

            if ( entry->translation( trans ).gradeFrom( 0 ).practiceCount() != 0 || entry->translation( 0 ).gradeFrom( trans ).practiceCount() != 0 )
                domElementTranslation.setAttribute( KV_COUNT, QString::number( entry->translation( trans ).gradeFrom( 0 ).practiceCount() ) + ';' + QString::number( entry->translation( 0 ).gradeFrom( trans ).practiceCount() ) );

            if ( entry->translation( trans ).gradeFrom( 0 ).badCount() != 0 || entry->translation( 0 ).gradeFrom( trans ).badCount() != 0 )
                domElementTranslation.setAttribute( KV_BAD, QString::number( entry->translation( trans ).gradeFrom( 0 ).badCount() ) + ';' + QString::number( entry->translation( 0 ).gradeFrom( trans ).badCount() ) );

            if ( entry->translation( trans ).gradeFrom( 0 ).practiceDate().toTime_t() != 0 || entry->translation( 0 ).gradeFrom( trans ).practiceDate().toTime_t() != 0 )
                domElementTranslation.setAttribute( KV_DATE, QString::number( entry->translation( trans ).gradeFrom( 0 ).practiceDate().toTime_t() ) + ';' + QString::number( entry->translation( 0 ).gradeFrom( trans ).practiceDate().toTime_t() ) );

            if ( !entry->translation( trans ).comment().isEmpty() )
                domElementTranslation.setAttribute( KV_REMARK, entry->translation( trans ).comment() );

            if ( !entry->translation( trans ).falseFriend( 0 ).isEmpty() )
                domElementTranslation.setAttribute( KV_FAUX_AMI_F, entry->translation( trans ).falseFriend( 0 ) );

            if ( !entry->translation( 0 ).falseFriend( trans ).isEmpty() )
                domElementTranslation.setAttribute( KV_FAUX_AMI_T, entry->translation( 0 ).falseFriend( trans ) );

            if ( !entry->translation( trans ).synonym().isEmpty() )
                domElementTranslation.setAttribute( KV_SYNONYM, entry->translation( trans ).synonym() );

            if ( !entry->translation( trans ).example().isEmpty() )
                domElementTranslation.setAttribute( KV_EXAMPLE, entry->translation( trans ).example() );

///@todo enable writing of usages in old format
            /*
                  if (!entry->translation(trans).usageLabel().isEmpty() )
                    domElementTranslation.setAttribute(KV_USAGE, entry->translation(trans).usageLabel());
            */
            if ( !entry->translation( trans ).paraphrase().isEmpty() )
                domElementTranslation.setAttribute( KV_PARAPHRASE, entry->translation( trans ).paraphrase() );

            if ( !entry->translation( trans ).antonym().isEmpty() )
                domElementTranslation.setAttribute( KV_ANTONYM, entry->translation( trans ).antonym() );

            if ( !entry->translation( trans ).pronunciation().isEmpty() )
                domElementTranslation.setAttribute( KV_PRONUNCE, entry->translation( trans ).pronunciation() );

            if ( !entry->translation( trans ).type().isEmpty() )
                domElementTranslation.setAttribute( KV_EXPRTYPE, m_compability.oldType( entry->translation( trans ).type(), entry->translation( trans ).subType() ) );

            if ( !writeMultipleChoice(domElementTranslation, entry->translation( trans ).multipleChoice() ) )
                return false;

            QString s;
            QString entype = s = entry->translation( 0 ).type();
            int pos = s.indexOf( QM_TYPE_DIV );
            if ( pos >= 0 )
                entype = s.left( pos );
            else
                entype = s;

            if ( entype == QM_VERB ) {
                if ( !writeConjugEntry(domElementTranslation, entry->translation( trans ) ) ) {
                    return false;
                }
            }

            if ( entype == QM_ADJ && !entry->translation( trans ).comparison().isEmpty() ) {
                KEduVocComparison comp = entry->translation( trans ).comparison();
                if ( !writeComparison(domElementTranslation, comp ) )
                    return false;
            }

            QDomText domTextTranslation = m_domDoc.createTextNode( entry->translation( trans ).text() );
            domElementTranslation.appendChild( domTextTranslation );
            domElementExpression.appendChild( domElementTranslation );

            trans++;
        }

        domElementKvtml.appendChild( domElementExpression );

        first_expr = false;
    }

    m_domDoc.appendChild( domElementKvtml );

    QTextStream ts( m_outputFile );
    m_domDoc.save( ts, 2 );

    return true;
}


bool KEduVocKvtmlWriter::writeLesson( QDomElement &domElementParent )
{
    if ( m_doc->lessonCount() == 0 )
        return true;

    QDomElement domElementLesson = m_domDoc.createElement( KV_LESS_GRP );
    domElementLesson.setAttribute( KV_SIZEHINT, m_doc->sizeHint( -1 ) );

    for ( int i = 0; i < m_doc->lessonCount(); ++i ) {
        QDomElement domElementDesc = m_domDoc.createElement( KV_LESS_DESC );
        QDomText domTextDesc = m_domDoc.createTextNode( m_doc->lesson(i).name() );

        domElementDesc.setAttribute( KV_LESS_NO, i );
        if ( m_doc->currentLesson() == i ) {
            domElementDesc.setAttribute( KV_LESS_CURR, 1 );
        }
        if ( m_doc->lesson(i).inPractice() ) {
            domElementDesc.setAttribute( KV_LESS_QUERY, 1 );
        }
        domElementDesc.appendChild( domTextDesc );
        domElementLesson.appendChild( domElementDesc );
    }

    domElementParent.appendChild( domElementLesson );
    return true;
}


bool KEduVocKvtmlWriter::writeArticle( QDomElement &domElementParent )
/*
 <article>
  <e l="de">    lang determines also lang order in entries !!
   <fi>eine</fi>  which must NOT differ
   <fd>die</fd>
   <mi>ein</mi>
   <md>der</md>
   <ni>ein</ni>
   <nd>das</nd>
  </e>
 </article>
*/
{
    QDomElement domElementArticle = m_domDoc.createElement( KV_ARTICLE_GRP );
    QString def;
    QString indef;
    QString s;

    for ( int i = 0; i < m_doc->identifierCount(); i++ )
    {
        QDomElement domElementEntry = m_domDoc.createElement( KV_ART_ENTRY );
        s = m_doc->identifier(i).name().simplified();
        domElementEntry.setAttribute( KV_LANG, s );

        QString articleString;
        // female
        articleString = m_doc->identifier(i).article().article( KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Feminine );
        if ( !articleString.isEmpty() ) {
            QDomElement domElementFD = m_domDoc.createElement( KV_ART_FD );
            QDomText domTextFD = m_domDoc.createTextNode( articleString );

            domElementFD.appendChild( domTextFD );
            domElementEntry.appendChild( domElementFD );
        }
        articleString = m_doc->identifier(i).article().article( KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Feminine );
        if ( !articleString.isEmpty() ) {
            QDomElement domElementFI = m_domDoc.createElement( KV_ART_FI );
            QDomText domTextFI = m_domDoc.createTextNode( articleString );

            domElementFI.appendChild( domTextFI );
            domElementEntry.appendChild( domElementFI );
        }


        // male
        articleString = m_doc->identifier(i).article().article( KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Masculine );
        if ( !articleString.isEmpty() ) {
            QDomElement domElementMD = m_domDoc.createElement( KV_ART_MD );
            QDomText domTextMD = m_domDoc.createTextNode( articleString );

            domElementMD.appendChild( domTextMD );
            domElementEntry.appendChild( domElementMD );
        }
        articleString = m_doc->identifier(i).article().article( KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Masculine );
        if ( !articleString.isEmpty() ) {
            QDomElement domElementMI = m_domDoc.createElement( KV_ART_MI );
            QDomText domTextMI = m_domDoc.createTextNode( articleString );

            domElementMI.appendChild( domTextMI );
            domElementEntry.appendChild( domElementMI );
        }

        // neutral
        articleString = m_doc->identifier(i).article().article( KEduVocWordFlag::Singular | KEduVocWordFlag::Definite | KEduVocWordFlag::Neutral );
        if ( !articleString.isEmpty() ) {
            QDomElement domElementND = m_domDoc.createElement( KV_ART_ND );
            QDomText domTextND = m_domDoc.createTextNode( articleString );

            domElementND.appendChild( domTextND );
            domElementEntry.appendChild( domElementND );
        }
        articleString = m_doc->identifier(i).article().article( KEduVocWordFlag::Singular | KEduVocWordFlag::Indefinite | KEduVocWordFlag::Neutral );
        if ( !articleString.isEmpty() ) {
            QDomElement domElementNI = m_domDoc.createElement( KV_ART_NI );
            QDomText domTextNI = m_domDoc.createTextNode( articleString );

            domElementNI.appendChild( domTextNI );
            domElementEntry.appendChild( domElementNI );
        }

        domElementArticle.appendChild( domElementEntry );
    }

    domElementParent.appendChild( domElementArticle );
    return true;
}


bool KEduVocKvtmlWriter::writeType( QDomElement &domElementParent )
{
    /** @todo port to new type handling */
    /*
      if (m_doc->typeDescriptions().isEmpty())
        return true;

      QDomElement domElementType = m_domDoc.createElement(KV_TYPE_GRP);
      int count = 1;

      foreach(const QString &type, m_doc->typeDescriptions())
      {
        if (!(type.isNull()) )
        {
          QDomElement domElementDesc = m_domDoc.createElement(KV_TYPE_DESC);
          QDomText domTextDesc = m_domDoc.createTextNode(type);

          domElementDesc.setAttribute(KV_TYPE_NO, count);
          domElementDesc.appendChild(domTextDesc);
          domElementType.appendChild(domElementDesc);
          count++;
        }
      }

      domElementParent.appendChild(domElementType);
    */
    return true;
}

/*
bool KEduVocKvtmlWriter::writeTense( QDomElement &domElementParent )
{
    if ( m_doc->tenseDescriptions().isEmpty() )
        return true;

    QDomElement domElementTense = m_domDoc.createElement( KV_TENSE_GRP );
    int count = 1;

    foreach( const QString &tense, m_doc->tenseDescriptions() ) {
        if ( !( tense.isNull() ) ) {
            QDomElement domElementDesc = m_domDoc.createElement( KV_TENSE_DESC );
            QDomText domTextDesc = m_domDoc.createTextNode( tense );

            domElementDesc.setAttribute( KV_TENSE_NO, count );
            domElementDesc.appendChild( domTextDesc );
            domElementTense.appendChild( domElementDesc );
            count++;
        }
    }

    domElementParent.appendChild( domElementTense );
    return true;
}
*/

bool KEduVocKvtmlWriter::writeUsage( QDomElement &domElementParent )
{

///@todo usages
    /*
      if (m_doc->usageDescriptions().isEmpty())
        return true;

      QDomElement domElementUsage = m_domDoc.createElement(KV_USAGE_GRP);
      int count = 1;

      foreach(const QString &usage, m_doc->usageDescriptions())
      {
        if (!(usage.isNull()))
        {
          QDomElement domElementDesc = m_domDoc.createElement(KV_USAGE_DESC);
          QDomText domTextDesc = m_domDoc.createTextNode(usage);

          domElementDesc.setAttribute(KV_USAGE_NO, count);
          domElementDesc.appendChild(domTextDesc);
          domElementUsage.appendChild(domElementDesc);
          count++;
        }
      }

      domElementParent.appendChild(domElementUsage);
    */
    return true;
}


bool KEduVocKvtmlWriter::writeComparison( QDomElement &domElementParent, const KEduVocTranslation* translation )
/*
 <comparison>
   <l1>good</l1>
   <l2>better</l2>
   <l3>best</l3>
 </comparison>
*/
{
    if ( translation.comparison().isEmpty() && translation.superlative().isEmpty() ) {
        return true;
    }

    QDomElement domElementComparison = m_domDoc.createElement( KV_COMPARISON_GRP );

    if ( !comp.l1().isEmpty() ) {
        QDomElement domElementL1 = m_domDoc.createElement( KV_COMP_L1 );
        QDomText domTextL1 = m_domDoc.createTextNode( comp.l1() );

        domElementL1.appendChild( domTextL1 );
        domElementComparison.appendChild( domElementL1 );
    }

    if ( !comp.l2().isEmpty() ) {
        QDomElement domElementL2 = m_domDoc.createElement( KV_COMP_L2 );
        QDomText domTextL2 = m_domDoc.createTextNode( comp.l2() );

        domElementL2.appendChild( domTextL2 );
        domElementComparison.appendChild( domElementL2 );
    }

    if ( !comp.l3().isEmpty() ) {
        QDomElement domElementL3 = m_domDoc.createElement( KV_COMP_L3 );
        QDomText domTextL3 = m_domDoc.createTextNode( comp.l3() );

        domElementL3.appendChild( domTextL3 );
        domElementComparison.appendChild( domElementL3 );
    }

    domElementParent.appendChild( domElementComparison );
    return true;
}


bool KEduVocKvtmlWriter::writeMultipleChoice( QDomElement &domElementParent, const KEduVocMultipleChoice &mc )
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
    if ( mc.isEmpty() )
        return true;

    QDomElement domElementMC = m_domDoc.createElement( KV_MULTIPLECHOICE_GRP );

    if ( !mc.choice( 1 ).isEmpty() ) {
        QDomElement domElementMC1 = m_domDoc.createElement( KV_MC_1 );
        QDomText domTextMC1 = m_domDoc.createTextNode( mc.choice( 1 ) );

        domElementMC1.appendChild( domTextMC1 );
        domElementMC.appendChild( domElementMC1 );
    }

    if ( !mc.choice( 2 ).isEmpty() ) {
        QDomElement domElementMC2 = m_domDoc.createElement( KV_MC_2 );
        QDomText domTextMC2 = m_domDoc.createTextNode( mc.choice( 2 ) );

        domElementMC2.appendChild( domTextMC2 );
        domElementMC.appendChild( domElementMC2 );
    }

    if ( !mc.choice( 3 ).isEmpty() ) {
        QDomElement domElementMC3 = m_domDoc.createElement( KV_MC_3 );
        QDomText domTextMC3 = m_domDoc.createTextNode( mc.choice( 3 ) );

        domElementMC3.appendChild( domTextMC3 );
        domElementMC.appendChild( domElementMC3 );
    }

    if ( !mc.choice( 4 ).isEmpty() ) {
        QDomElement domElementMC4 = m_domDoc.createElement( KV_MC_4 );
        QDomText domTextMC4 = m_domDoc.createTextNode( mc.choice( 4 ) );

        domElementMC4.appendChild( domTextMC4 );
        domElementMC.appendChild( domElementMC4 );
    }

    if ( !mc.choice( 5 ).isEmpty() ) {
        QDomElement domElementMC5 = m_domDoc.createElement( KV_MC_5 );
        QDomText domTextMC5 = m_domDoc.createTextNode( mc.choice( 5 ) );

        domElementMC5.appendChild( domTextMC5 );
        domElementMC.appendChild( domElementMC5 );
    }

    domElementParent.appendChild( domElementMC );
    return true;
}


bool KEduVocKvtmlWriter::writeConjugHeader( QDomElement &domElementParent, QList<KEduVocConjugation> &curr_conjug )
{
    /*
     <conjugation>    used in header for definition of "prefix"
      <e l="de">      lang determines also lang order in entries !!
       <s1>I</s1>     which must NOT differ
       <s2>you<2>
       <s3f common="0">he</s3f>
       <s3m>she</s3m>
       <s3n>it</s3n>
       <p1>we</p1>
       <p2>you</p2>
       <p3f common="1">they</p3f>
       <p3m>they</p3m>
       <p3n>they</p3n>
      </e>
     </conjugation>

    */
    if ( curr_conjug.size() == 0 )
        return true;

    QDomElement domElementConjug = m_domDoc.createElement( KV_CONJUG_GRP );
    QString s;

    for ( int ent = 0; ent < qMin( curr_conjug.count(), m_doc->identifierCount() ); ent++ ) {
        QDomElement domElementEntry = m_domDoc.createElement( KV_CON_ENTRY );

        s = m_doc->identifier( ent ).name().simplified();
        if ( s.isEmpty() ) {
            s.setNum( ent );
            s.prepend( QLatin1String("translation ") );
        }

        domElementEntry.setAttribute( KV_LANG, s );
        if ( !writeConjug(domElementEntry, curr_conjug[ent], "--" ) )
            return false;

        domElementConjug.appendChild( domElementEntry );
    }

    domElementParent.appendChild( domElementConjug );
    return true;
}


bool KEduVocKvtmlWriter::writeConjugEntry( QDomElement &domElementParent, KEduVocTranslation &translation )
/*
 <conjugation>    in entry for definition of tenses of (irreg.) verbs
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

//     curr_conjug.cleanUp();
    QDomElement domElementConjug = m_domDoc.createElement( KV_CONJUG_GRP );

    foreach ( const QString &tense, translation.conjugationTenses() ) {
        QDomElement domElementType = m_domDoc.createElement( KV_CON_TYPE );

        domElementType.setAttribute( KV_CON_NAME, m_compability.oldTense(tense) );

        if ( !writeConjug(domElementType, translation.conjugation(tense), m_compability.oldTense(tense) ) )
            return false;

        domElementConjug.appendChild( domElementType );
    }

    domElementParent.appendChild( domElementConjug );
    return true;
}


bool KEduVocKvtmlWriter::writeConjug( QDomElement &domElementParent, const KEduVocConjugation &curr_conjug, const QString &type )
{
///@todo if we are to enable the writer: port this!
/*
    if ( !curr_conjug.pers1Singular().isEmpty() ) {
        QDomElement domElementP1s = m_domDoc.createElement( KV_CON_P1S );
        QDomText domTextP1s = m_domDoc.createTextNode( curr_conjug.pers1Singular() );

        domElementP1s.appendChild( domTextP1s );
        domElementParent.appendChild( domElementP1s );
    }

    if ( !curr_conjug.pers2Singular().isEmpty() ) {
        QDomElement domElementP2s = m_domDoc.createElement( KV_CON_P2S );
        QDomText domTextP2s = m_domDoc.createTextNode( curr_conjug.pers2Singular() );

        domElementP2s.appendChild( domTextP2s );
        domElementParent.appendChild( domElementP2s );
    }

    if ( !curr_conjug.pers3FemaleSingular().isEmpty() || curr_conjug.pers3SingularCommon() ) {
        QDomElement domElementP3sf = m_domDoc.createElement( KV_CON_P3SF );
        QDomText domTextP3sf = m_domDoc.createTextNode( curr_conjug.pers3FemaleSingular() );

        if ( curr_conjug.pers3SingularCommon() )
            domElementP3sf.setAttribute( KV_CONJ_COMMON, 1 );

        domElementP3sf.appendChild( domTextP3sf );
        domElementParent.appendChild( domElementP3sf );
    }

    if ( !curr_conjug.pers3MaleSingular().isEmpty() ) {
        QDomElement domElementP3sm = m_domDoc.createElement( KV_CON_P3SM );
        QDomText domTextP3sm = m_domDoc.createTextNode( curr_conjug.pers3MaleSingular() );

        domElementP3sm.appendChild( domTextP3sm );
        domElementParent.appendChild( domElementP3sm );
    }

    if ( !curr_conjug.pers3NaturalSingular().isEmpty() ) {
        QDomElement domElementP3sn = m_domDoc.createElement( KV_CON_P3SN );
        QDomText domTextP3sn = m_domDoc.createTextNode( curr_conjug.pers3NaturalSingular() );

        domElementP3sn.appendChild( domTextP3sn );
        domElementParent.appendChild( domElementP3sn );
    }

    if ( !curr_conjug.pers1Plural().isEmpty() ) {
        QDomElement domElementP1p = m_domDoc.createElement( KV_CON_P1P );
        QDomText domTextP1p = m_domDoc.createTextNode( curr_conjug.pers1Plural() );

        domElementP1p.appendChild( domTextP1p );
        domElementParent.appendChild( domElementP1p );
    }

    if ( !curr_conjug.pers2Plural().isEmpty() ) {
        QDomElement domElementP2p = m_domDoc.createElement( KV_CON_P2P );
        QDomText domTextP2p = m_domDoc.createTextNode( curr_conjug.pers2Plural() );

        domElementP2p.appendChild( domTextP2p );
        domElementParent.appendChild( domElementP2p );
    }

    if ( !curr_conjug.pers3FemalePlural().isEmpty() || curr_conjug.pers3PluralCommon() ) {
        QDomElement domElementP3pf = m_domDoc.createElement( KV_CON_P3PF );
        QDomText domTextP3pf = m_domDoc.createTextNode( curr_conjug.pers3FemalePlural() );

        if ( curr_conjug.pers3PluralCommon() )
            domElementP3pf.setAttribute( KV_CONJ_COMMON, 1 );

        domElementP3pf.appendChild( domTextP3pf );
        domElementParent.appendChild( domElementP3pf );
    }

    if ( !curr_conjug.pers3MalePlural().isEmpty() ) {
        QDomElement domElementP3pm = m_domDoc.createElement( KV_CON_P3PM );
        QDomText domTextP3pm = m_domDoc.createTextNode( curr_conjug.pers3MalePlural() );

        domElementP3pm.appendChild( domTextP3pm );
        domElementParent.appendChild( domElementP3pm );
    }

    if ( !curr_conjug.pers3NaturalPlural().isEmpty() ) {
        QDomElement domElementP3pn = m_domDoc.createElement( KV_CON_P3PN );
        QDomText domTextP3pn = m_domDoc.createTextNode( curr_conjug.pers3NaturalPlural() );

        domElementP3pn.appendChild( domTextP3pn );
        domElementParent.appendChild( domElementP3pn );
    }

    return true; */
    qDebug() << "Implement me!";
    return false;
}

bool KEduVocKvtmlWriter::writePersonalPronouns( QDomElement &domElementParent, QList<KEduVocPersonalPronoun> &curr_conjug )
{
    /*
     <conjugation>    used in header for definition of "prefix"
      <e l="de">      lang determines also lang order in entries !!
       <s1>I</s1>     which must NOT differ
       <s2>you<2>
       <s3f common="0">he</s3f>
       <s3m>she</s3m>
       <s3n>it</s3n>
       <p1>we</p1>
       <p2>you</p2>
       <p3f common="1">they</p3f>
       <p3m>they</p3m>
       <p3n>they</p3n>
      </e>
     </conjugation>
    */
    const QString type = QString::fromLatin1("--");

    if ( curr_conjug.size() == 0 )
        return true;

    QDomElement domElementConjug = m_domDoc.createElement( KV_CONJUG_GRP );
    QString s;

    for ( int ent = 0; ent < qMin( curr_conjug.count(), m_doc->identifierCount() ); ent++ ) {
        QDomElement domElementEntry = m_domDoc.createElement( KV_CON_ENTRY );

        s = m_doc->identifier( ent ).name().simplified();
        if ( s.isEmpty() ) {
            s.setNum( ent );
            s.prepend( QLatin1String("translation ") );
        }

        domElementEntry.setAttribute( KV_LANG, s );
        if ( !writePersonalPronounChild(domElementEntry, curr_conjug[ent] ) )
            return false;

        domElementConjug.appendChild( domElementEntry );
    }

    domElementParent.appendChild( domElementConjug );
    return true;
}


bool KEduVocKvtmlWriter::writePersonalPronounChild( QDomElement &domElementParent, const KEduVocPersonalPronoun &curr_conjug )
{
    // the old stuff only has singular
    const KEduVocWordFlag::ConjugationNumber num = KEduVocWordFlag::Singular;

    if ( !curr_conjug.personalPronoun( KEduVocWordFlag::First, num ).isEmpty() ) {
        domElementParent.appendChild( newTextElement( KV_CON_P1S, curr_conjug.personalPronoun( KEduVocWordFlag::First, num ) ) );
    }

    if ( !curr_conjug.personalPronoun( KEduVocWordFlag::Second, num ).isEmpty() ) {
        domElementParent.appendChild( newTextElement( KV_CON_P2S, curr_conjug.personalPronoun( KEduVocWordFlag::Second, num ) ) );
    }

    ///@todo if the writer ever becomes enabled, write the other pronouns
/*
    if ( !curr_conjug.pers2Singular().isEmpty() ) {
        QDomElement domElementP2s = m_domDoc.createElement( KV_CON_P2S );
        QDomText domTextP2s = m_domDoc.createTextNode( curr_conjug.pers2Singular() );

        domElementP2s.appendChild( domTextP2s );
        domElementParent.appendChild( domElementP2s );
    }

    if ( !curr_conjug.pers3FemaleSingular().isEmpty() || curr_conjug.pers3SingularCommon() ) {
        QDomElement domElementP3sf = m_domDoc.createElement( KV_CON_P3SF );
        QDomText domTextP3sf = m_domDoc.createTextNode( curr_conjug.pers3FemaleSingular() );

        if ( curr_conjug.pers3SingularCommon() )
            domElementP3sf.setAttribute( KV_CONJ_COMMON, 1 );

        domElementP3sf.appendChild( domTextP3sf );
        domElementParent.appendChild( domElementP3sf );
    }

    if ( !curr_conjug.pers3MaleSingular().isEmpty() ) {
        QDomElement domElementP3sm = m_domDoc.createElement( KV_CON_P3SM );
        QDomText domTextP3sm = m_domDoc.createTextNode( curr_conjug.pers3MaleSingular() );

        domElementP3sm.appendChild( domTextP3sm );
        domElementParent.appendChild( domElementP3sm );
    }

    if ( !curr_conjug.pers3NaturalSingular().isEmpty() ) {
        QDomElement domElementP3sn = m_domDoc.createElement( KV_CON_P3SN );
        QDomText domTextP3sn = m_domDoc.createTextNode( curr_conjug.pers3NaturalSingular() );

        domElementP3sn.appendChild( domTextP3sn );
        domElementParent.appendChild( domElementP3sn );
    }

    if ( !curr_conjug.pers1Plural().isEmpty() ) {
        QDomElement domElementP1p = m_domDoc.createElement( KV_CON_P1P );
        QDomText domTextP1p = m_domDoc.createTextNode( curr_conjug.pers1Plural() );

        domElementP1p.appendChild( domTextP1p );
        domElementParent.appendChild( domElementP1p );
    }

    if ( !curr_conjug.pers2Plural().isEmpty() ) {
        QDomElement domElementP2p = m_domDoc.createElement( KV_CON_P2P );
        QDomText domTextP2p = m_domDoc.createTextNode( curr_conjug.pers2Plural() );

        domElementP2p.appendChild( domTextP2p );
        domElementParent.appendChild( domElementP2p );
    }

    if ( !curr_conjug.pers3FemalePlural().isEmpty() || curr_conjug.pers3PluralCommon() ) {
        QDomElement domElementP3pf = m_domDoc.createElement( KV_CON_P3PF );
        QDomText domTextP3pf = m_domDoc.createTextNode( curr_conjug.pers3FemalePlural() );

        if ( curr_conjug.pers3PluralCommon() )
            domElementP3pf.setAttribute( KV_CONJ_COMMON, 1 );

        domElementP3pf.appendChild( domTextP3pf );
        domElementParent.appendChild( domElementP3pf );
    }

    if ( !curr_conjug.pers3MalePlural().isEmpty() ) {
        QDomElement domElementP3pm = m_domDoc.createElement( KV_CON_P3PM );
        QDomText domTextP3pm = m_domDoc.createTextNode( curr_conjug.pers3MalePlural() );

        domElementP3pm.appendChild( domTextP3pm );
        domElementParent.appendChild( domElementP3pm );
    }

    if ( !curr_conjug.pers3NaturalPlural().isEmpty() ) {
        QDomElement domElementP3pn = m_domDoc.createElement( KV_CON_P3PN );
        QDomText domTextP3pn = m_domDoc.createTextNode( curr_conjug.pers3NaturalPlural() );

        domElementP3pn.appendChild( domTextP3pn );
        domElementParent.appendChild( domElementP3pn );
    }*/

    return true;
}

QDomElement KEduVocKvtmlWriter::newTextElement( const QString &elementName, const QString &text )
{
    QDomElement retval = m_domDoc.createElement( elementName );
    QDomText textNode = m_domDoc.createTextNode( text );
    retval.appendChild( textNode );
    return retval;
}


