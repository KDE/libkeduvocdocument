/***************************************************************************
                   Vocabulary Document for KDE Edu KVTML version 2
    -----------------------------------------------------------------------
    copyright     : (C) 2007 Jeremy Whiting <jeremy@scitools.com>
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

#ifndef KVTML2DEFS_H
#define KVTML2DEFS_H

#define KVTML_TAG               "kvtml"
#define KVTML_VERSION           "version"
#define KVTML_ID                "id"

#define KVTML_INFORMATION       "information"

#define KVTML_GENERATOR         "generator"
#define KVTML_TITLE             "title"
#define KVTML_AUTHOR            "author"
#define KVTML_LICENSE           "license"
#define KVTML_COMMENT           "comment"
#define KVTML_CATEGORY          "category"

#define KVTML_IDENTIFIERS       "identifiers"
#define KVTML_IDENTIFIER        "identifier"
#define KVTML_IDENTIFIERTYPE    "identifiertype"
#define KVTML_LOCALE            "locale"
#define KVTML_NAME              "name"
#define KVTML_SIZEHINT          "sizehint"

// articles
#define KVTML_ARTICLE           "article"
#define KVTML_DEFINITE          "definite"
#define KVTML_INDEFINITE        "indefinite"

// conjugation and personal pronouns
#define KVTML_CONJUGATION       "conjugation"
#define KVTML_CONJUGATIONTYPE   "conjugationtype"
#define KVTML_PERSONALPRONOUNS  "personalpronouns"

#define KVTML_MALE              "male"
#define KVTML_FEMALE            "female"
#define KVTML_NEUTRAL           "neutral"
#define KVTML_SINGULAR          "singular"
// dual is the case where there's a special form for exactly two
#define KVTML_DUAL              "dual"
#define KVTML_PLURAL            "plural"
#define KVTML_1STPERSON         "firstperson"
#define KVTML_2NDPERSON         "secondperson"
#define KVTML_THIRD_MALE        "thirdpersonmale"
#define KVTML_THIRD_FEMALE      "thirdpersonfemale"
#define KVTML_THIRD_NEUTER_COMMON "thirdpersonneutercommon"

// for the personal pronuns:
// if this tag exists, in a conjugation male/female are different
#define KVTML_THIRD_PERSON_MALE_FEMALE_DIFFERENT "malefemaledifferent"
// if this tag exists conjugations even have a neuter form for the third person
#define KVTML_THIRD_PERSON_NEUTER_EXISTS "neuterexists"
#define KVTML_DUAL_EXISTS        "dualexists"

// word types
#define KVTML_WORDTYPEDEFINITIONS   "wordtypedefinitions"
#define KVTML_WORDTYPEDEFINITION    "wordtypedefinition"
#define KVTML_SUBWORDTYPEDEFINITION   "subwordtypedefinition"

#define KVTML_WORDTYPE              "wordtype"
#define KVTML_TYPENAME              "typename"
#define KVTML_SUBTYPENAME           "subtypename"
#define KVTML_SPECIALWORDTYPE       "specialwordtype"

#define KVTML_TENSES            "tenses"
#define KVTML_TENSE             "tense"

#define KVTML_USAGES            "usages"
#define KVTML_USAGE             "usage"

#define KVTML_LESSONS           "lessons"
#define KVTML_LESSON            "lesson"
#define KVTML_QUERY             "query"
#define KVTML_CURRENT           "current"
#define KVTML_ENTRYID           "entryid"

#define KVTML_ENTRIES           "entries"
#define KVTML_ENTRY             "entry"
#define KVTML_INACTIVE          "inactive"
#define KVTML_INQUERY           "inquery"

#define KVTML_TRANSLATION       "translation"
#define KVTML_TEXT              "text"
#define KVTML_PRONUNCIATION     "pronunciation"
#define KVTML_FALSEFRIEND       "falsefriend"
#define KVTML_FROMID            "fromid"
#define KVTML_ANTONYM           "antonym"
#define KVTML_SYNONYM           "synonym"
#define KVTML_EXAMPLE           "example"
#define KVTML_PARAPHRASE        "paraphrase"

#define KVTML_COMPARISON        "comparison"
#define KVTML_ABSOLUTE          "absolute"
#define KVTML_COMPARATIVE       "comparative"
#define KVTML_SUPERLATIVE       "superlative"

#define KVTML_MULTIPLECHOICE    "multiplechoice"
#define KVTML_CHOICE            "choice"

#define KVTML_IMAGE             "image"
#define KVTML_SOUND             "sound"

#define KVTML_GRADE             "grade"
#define KVTML_FROMID            "fromid"
#define KVTML_CURRENTGRADE      "currentgrade"
#define KVTML_COUNT             "count"
#define KVTML_ERRORCOUNT        "errorcount"
#define KVTML_DATE              "date"

#define KVTML_TRUE              "true"
#define KVTML_FALSE             "false"

#endif

