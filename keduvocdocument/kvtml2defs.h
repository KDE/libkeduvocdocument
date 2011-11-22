/***************************************************************************
                   Vocabulary Document for KDE Edu KVTML version 2
    -----------------------------------------------------------------------
    copyright     : (C) 2007 Jeremy Whiting <jpwhiting@kde.org>
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
#define KVTML_AUTHORCONTACT     "contact"
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

// conjugation, declension and personal pronouns
#define KVTML_CONJUGATION       "conjugation"
#define KVTML_CONJUGATIONTYPE   "conjugationtype"
#define KVTML_PERSONALPRONOUNS  "personalpronouns"
#define KVTML_DECLENSION       "declension"

static const QString KVTML_DECLENSION_CASE[] = {
    "nominative",
    "genitive",
    "dative",
    "accusative",
    "ablative",
    "locative",
    "vocative"
};

static const QString KVTML_GRAMMATICAL_NUMBER[] = {
    "singular",
    "dual", // dual is the case where there's a special form for exactly two
    "plural"
};

static const QString KVTML_GRAMMATICAL_GENDER[] = {
    "male",
    "female",
    "neutral"
};

static const QString KVTML_GRAMMATICAL_PERSON[] = {
    "firstperson",
    "secondperson",
    "thirdpersonmale",
    "thirdpersonfemale",
    "thirdpersonneutralcommon"
};

static const QString KVTML_GRAMMATICAL_DEFINITENESS[] = {
    "definite",
    "indefinite"
};


// for the personal pronuns:
// if this tag exists, in a conjugation male/female are different
#define KVTML_THIRD_PERSON_MALE_FEMALE_DIFFERENT "malefemaledifferent"
// if this tag exists conjugations even have a neutral form for the third person
#define KVTML_THIRD_PERSON_NEUTRAL_EXISTS "neutralexists"
#define KVTML_DUAL_EXISTS        "dualexists"

// word types, lessons and leitner boxes are vocabulary containers.
// each is allowed to have as many child containers as desired.
// for KDE 4.0 Parley will only allow a depth of wordtype cildren of two,
// leitner is not implemented at all.
// it is assumed that one container only has children of the same type.
// containers may have an image associated to represent them visually.
#define KVTML_WORDTYPES         "wordtypes"
#define KVTML_LESSONS           "lessons"
#define KVTML_LEITNERBOXES      "leitnerboxes"
#define KVTML_CONTAINER         "container"
#define KVTML_INPRACTICE        "inpractice"

// these are necessary to enable practices based on word types. users can give types arbitrary names, but these few are hardcoded. special container types
#define KVTML_SPECIALWORDTYPE       "specialwordtype"
#define KVTML_SPECIALWORDTYPE_NOUN        "noun"
#define KVTML_SPECIALWORDTYPE_NOUN_MALE   "noun/male"
#define KVTML_SPECIALWORDTYPE_NOUN_FEMALE "noun/female"
#define KVTML_SPECIALWORDTYPE_NOUN_NEUTRAL "noun/neutral"
#define KVTML_SPECIALWORDTYPE_VERB        "verb"
#define KVTML_SPECIALWORDTYPE_ADJECTIVE   "adjective"
#define KVTML_SPECIALWORDTYPE_ADVERB      "adverb"
#define KVTML_SPECIALWORDTYPE_CONJUNCTION "conjunction"


#define KVTML_TENSES            "tenses"
#define KVTML_TENSE             "tense"

#define KVTML_USAGES            "usages"
#define KVTML_USAGE             "usage"

#define KVTML_CURRENT           "current"

#define KVTML_ENTRIES           "entries"
#define KVTML_ENTRY             "entry"
#define KVTML_DEACTIVATED       "deactivated"

#define KVTML_TRANSLATION       "translation"
#define KVTML_TEXT              "text"
#define KVTML_PRONUNCIATION     "pronunciation"

#define KVTML_PAIR              "pair"
#define KVTML_FALSEFRIEND       "falsefriend"
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

