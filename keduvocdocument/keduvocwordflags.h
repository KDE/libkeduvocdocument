/*
 * SPDX-FileCopyrightText: 2008 David Capel <wot.narg@gmail.com>
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KEDUVOCWORDFLAGS_H
#define KEDUVOCWORDFLAGS_H


class KEduVocWordFlag
{
public:
    enum Flags
    {
        // This is used for both empty flags and to denote no flags of the correct type were set.
        NoInformation = 0x0,

        // Gender
        Masculine = 0x1,
        Feminine = 0x2,
        Neuter = 0x4,

        // Plurality
        Singular = 0x10,
        Dual = 0x20,
        Plural = 0x40,

        // Part of Speech
        Verb = 0x100,
        Noun = 0x200,
        Pronoun = 0x400,
        Adjective = 0x800,
        Adverb = 0x1000,
        Article = 0x2000,
        Conjunction = 0x4000,

        // Person
        First = 0x10000,
        Second = 0x20000,
        Third = 0x40000,

        // Declension Case
        Nominative = 0x80000,
        Genitive = 0x100000,
        Dative = 0x200000,
        Accusative = 0x400000,
        Ablative = 0x800000,
        Locative = 0x1000000,
        Vocative = 0x2000000,

        // Other assorted flags
        Definite = 0x4000000, // The article is definite
        Indefinite = 0x8000000, // The article is indefinite
        Regular = 0x10000000,
        Irregular = 0x20000000
    };

    static const Flags genders = (Flags)(Masculine | Feminine | Neuter);
    static const Flags partsOfSpeech =(Flags)(Noun | Verb | Article | Pronoun | Adjective | Adverb | Conjunction);
    static const Flags numbers = (Flags)(Singular | Plural | Dual);
    static const Flags cases = (Flags)(Nominative|Genitive|Dative|Accusative|Ablative|Locative|Vocative);
    static const Flags persons = (Flags)(First | Second | Third);

};

Q_DECLARE_FLAGS(KEduVocWordFlags, KEduVocWordFlag::Flags)
Q_DECLARE_OPERATORS_FOR_FLAGS(KEduVocWordFlags)

#endif
