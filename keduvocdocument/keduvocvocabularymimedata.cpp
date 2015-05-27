/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "keduvocvocabularymimedata.h"

#include <keduvoctranslation.h>
#include <keduvocwordtype.h>

class KEduVocVocabularyMimeData::Private
{
public:
    QList<KEduVocTranslation*> m_translations;
    QList <MimeExpression> m_expressions;
    QString m_text;
};

KEduVocVocabularyMimeData::KEduVocVocabularyMimeData()
    : d( new Private )
{
}

KEduVocVocabularyMimeData::~KEduVocVocabularyMimeData()
{
    delete d;
}

void KEduVocVocabularyMimeData::setTranslations(QList<KEduVocTranslation *> translations)
{
    // list of pointers for drag and drop - for example to assign word types
    d->m_translations = translations;

    // sort the translations into entries to make deep copies for real copy and paste
    // to only include each expression once
    QList<KEduVocExpression *> expressions;
    foreach(KEduVocTranslation * translation, d->m_translations) {
        if (!expressions.contains(translation->entry())) {
            expressions.append(translation->entry());
        }
    }

    foreach(KEduVocExpression * expression, expressions) {
        MimeExpression exp;
        // deep copy
        exp.expression = KEduVocExpression(*expression);

        // copy word types
        // this sucks but there is not really a better was. copying pointers is not a good idea because copy and paste can be done between different documents.
        foreach(int i, expression->translationIndices()) {
            // generate text string representation
            d->m_text.append(expression->translation(i)->text());
            d->m_text.append(" - ");

            // fill in word types independent of pointers
            KEduVocWordType *type = expression->translation(i)->wordType();

            if (type) { // check if it has a type != 0
                exp.wordTypes[i].grammarType = expression->translation(i)->wordType()->wordType();

                // this may seem weird, but the root element is "word types" so no need to copy that one.
                while (type->parent()) {
                    exp.wordTypes[i].wordType.prepend(type->name());
                    type = static_cast<KEduVocWordType*>(type->parent());
                }
            }
        }
        d->m_expressions.append(exp);
        d->m_text.append('\n');
    }
}

QList< KEduVocTranslation * > KEduVocVocabularyMimeData::translationList() const
{
    return d->m_translations;
}

QVariant KEduVocVocabularyMimeData::retrieveData(const QString & mimeType, QVariant::Type type) const
{
    Q_UNUSED(type)
    // only use the expression list.expressions
    // the translation list may be invalid (eg when cut it is no longer valid.
    // translations can only be used internally for drag and drop!!!

    if (mimeType == "text/plain") {
        return d->m_text;
    }
    return QVariant();
}

QStringList KEduVocVocabularyMimeData::formats() const
{
    return QStringList() << "text/plain";
}

QList< KEduVocVocabularyMimeData::MimeExpression > KEduVocVocabularyMimeData::expressionList() const
{
    return d->m_expressions;
}
