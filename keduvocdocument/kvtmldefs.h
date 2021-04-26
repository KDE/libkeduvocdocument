/*
 * Vocabulary Document for KDE Edu
 * SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
 * SPDX-FileCopyrightText: 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
 * SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KVTMLDEFS_H
#define KVTMLDEFS_H



/** XML tags and attribute names */

static const QLatin1String KV_DOCTYPE( "kvtml" ); // doctype
#define KV_TITLE       "title"     // doc title
#define KV_AUTHOR      "author"    // doc author
#define KV_LICENSE     "license"   // doc license
#define KV_DOC_REM     "remark"    // doc remark
#define KV_LINES       "lines"     // entries
#define KV_GENERATOR   "generator" // who generated the doc
#define KV_COLS        "cols"      // columns
#define KV_ENCODING    "encoding"  // document encoding  (obsolete!)

#define KV_EXPR        "e"         // entry for one expression
static const QLatin1String KV_ORG( "o" );       // original expression in specified language
static const QLatin1String KV_TRANS( "t" );     // translated expression in specified language
#define KV_LANG        "l"         // language: en, de, it, fr ...
#define KV_QUERY       "q"         // query: org or translation
#define KV_O           "o"         // org
#define KV_T           "t"         // translation
#define KV_GRADE       "g"         // grade of knowledge: 0=well known, x=not known for x times
#define KV_LESS_MEMBER "m"         // member of lesson 1 .. x
#define KV_COUNT       "c"         // number of times queried
#define KV_SIZEHINT    "width"     // recommended column width
#define KV_CHARSET     "charset"   // recommended charset (obsolete!)
#define KV_BAD         "b"         // number of times failed
#define KV_DATE        "d"         // last query date
#define KV_DATE2       "w"         // last query date, compressed format, deprecated, currently ignored
#define KV_REMARK      "r"         // remark for this entry
#define KV_FAUX_AMI_F  "ff"        // false friend of this entry from org
#define KV_FAUX_AMI_T  "tf"        // false friend of this entry to org
#define KV_SYNONYM     "y"         // synonym (same meaning) of expr
#define KV_ANTONYM     "a"         // antonym (opposite) of expr
#define KV_PRONUNCE    "p"         // how to pronounce this expression
#define KV_SELECTED    "s"         // entry selected for queries
#define KV_INACTIVE    "i"         // entry inactive (for queries)
#define KV_EXPRTYPE    "t"         // type of expression
#define KV_EXAMPLE     "x"         // example string with word
#define KV_USAGE       "u"         // usage label
#define KV_PARAPHRASE  "h"         // paraphrase for expression

/*
 <type>
  <desc no="1">My type 1</desc>
  <desc no="2">My type 2</desc>
 </type>
*/

#define KV_TYPE_GRP    "type"      // type descriptor group
#define KV_TYPE_DESC   "desc"      // type descriptor
#define KV_TYPE_NO     "no"        // type descriptor number

/*
 <usage>
  <desc no="1">My usage 1</desc>
  <desc no="2">My usage 2</desc>
 </type>
*/

#define KV_USAGE_GRP    "usage"     // usage descriptor group
#define KV_USAGE_DESC   "desc"      // usage descriptor
#define KV_USAGE_NO     "no"        // usage descriptor number

/*
 <lesson width="138">
  <desc no="1">Lesson #1</desc>
  <desc no="2" query="1">Lesson #2</desc>
 </lesson>
*/

#define KV_LESS_GRP    "lesson"    // lesson descriptor group
#define KV_LESS_CURR   "current"   // is current lesson
#define KV_LESS_DESC   "desc"      // lesson descriptor
#define KV_LESS_QUERY  "query"     // lesson contained in query
#define KV_LESS_NO     "no"        // lesson descriptor number

/*
 <tense>
  <desc no="1">user tense #1</desc>
  <desc no="2">user tense #2</desc>
 </tense>
*/

#define KV_TENSE_GRP    "tense"     // tense descriptor group
#define KV_TENSE_DESC   "desc"      // tense descriptor
#define KV_TENSE_NO     "no"        // tense descriptor number

/*
 <options>
  <sort on="1"/>
 </options>
*/

#define KV_OPTION_GRP  "options"   // internal options group
#define KV_OPT_SORT    "sort"      // allow sorting
#define KV_BOOL_FLAG   "on"        // general boolean flag

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

#define KV_ARTICLE_GRP "article"   // article descriptor group
static const QLatin1String KV_ART_ENTRY( "e" ); // article entry
#define KV_ART_FD      "fd"        // female definite
#define KV_ART_MD      "md"        // male definite
#define KV_ART_ND      "nd"        // natural definite
#define KV_ART_FI      "fi"        // female indefinite
#define KV_ART_MI      "mi"        // male indefinite
#define KV_ART_NI      "ni"        // natural indefinite

/*
 <comparison>
   <l1>good</l1>
   <l2>better</l2>
   <l3>best</l3>
 </comparison>
*/

#define KV_COMPARISON_GRP "comparison"   // comparison descriptor group
#define KV_COMP_L1        "l1"           // base form
#define KV_COMP_L2        "l2"           // next form
#define KV_COMP_L3        "l3"           // last form

/*
 <multiplechoice>
   <mc1>good</mc1>
   <mc2>better</mc2>
   <mc3>best</mc3>
   <mc4>best 2</mc4>
   <mc5>best 3</mc5>
 </multiplechoice>
*/

#define KV_MULTIPLECHOICE_GRP "multiplechoice"   // multiple choice descriptor group
#define KV_MC_1        "mc1"           // choice 1
#define KV_MC_2        "mc2"           // choice 2
#define KV_MC_3        "mc3"           // choice 3
#define KV_MC_4        "mc4"           // choice 4
#define KV_MC_5        "mc5"           // choice 5

/*
 <conjugation>        used in header for definition of "prefix"
  <e l="de">          lang determines also lang order in entries !!
   <s1>I</s1>         which must NOT differ in subsequent <e>-tags
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

#define KV_CONJUG_GRP  "conjugation" // conjugation descriptor group
#define KV_CON_ENTRY   "e"           // conjugation entry (header)
#define KV_CON_TYPE    "t"           // conjugation type  (voc entries)
#define KV_CON_NAME    "n"           // conjugation type name (voc entries)
#define KV_CON_P1S     "s1"          // 1. person singular
#define KV_CON_P2S     "s2"          // 2. person singular
#define KV_CON_P3SF    "s3f"         // 3. person singular female
#define KV_CON_P3SM    "s3m"         // 3. person singular male
#define KV_CON_P3SN    "s3n"         // 3. person singular natural
#define KV_CON_P1P     "p1"          // 1. person plural
#define KV_CON_P2P     "p2"          // 2. person plural
#define KV_CON_P3PF    "p3f"         // 3. person plural female
#define KV_CON_P3PM    "p3m"         // 3. person plural male
#define KV_CON_P3PN    "p3n"         // 3. person plural natural
#define KV_CONJ_COMMON "common"      // female contains common for all three


#endif // KVTMLDEFS_H
