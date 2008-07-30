//
// C++ Interface: keduvocwordflags
//
// Description:
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef KEDUVOCWORDFLAGS_H
#define KEDUVOCWORDFLAGS_H

#include <QFlags>

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
        OtherPartOfSpeech = 0x8000,

        // Other assorted flags
        Regular = 0x10000, // Regular verb conjugation
        Irregular = 0x20000, // Irregular verb conjugation
        Phrase = 0x40000, // The 'word' is actually a phrase
        Definite = 0x80000, // The article is definite
        Indefinite = 0x100000, // The article is indefinite
    };

    static const Flags genders = (Flags)(Masculine | Feminine | Neuter);
    static const Flags partsOfSpeech =(Flags)(Noun | Verb | Article | Pronoun | Adjective | Adverb | Conjunction | OtherPartOfSpeech);
    static const Flags numbers = (Flags)(Singular | Plural | Dual);
};

Q_DECLARE_FLAGS(KEduVocWordFlags, KEduVocWordFlag::Flags);
Q_DECLARE_OPERATORS_FOR_FLAGS(KEduVocWordFlags);


#endif
