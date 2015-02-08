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
#ifndef KEDUVOCVOCABULARYMODEL_H
#define KEDUVOCVOCABULARYMODEL_H

#include <keduvocexpression.h>
#include <keduvoctranslation.h>
#include <QAbstractTableModel>

class KEduVocDocument;
class KEduVocLesson;


/**
    @author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class KEduVocVocabularyModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum entryColumns {
        Translation = 0,
        Pronunciation,
        WordClass,
        Synonym,
        Antonym,
        Example,
        Comment,
        Paraphrase,
        EntryColumnsMAX

    };

    enum roles {
        TranslationRole = Qt::UserRole,
        EntryRole,
        LocaleRole,
        AudioRole,
        ImageRole
    };

    KEduVocVocabularyModel(bool show, QObject *parent = 0);

    ~KEduVocVocabularyModel();


    int rowCount(const QModelIndex&) const;
    int columnCount(const QModelIndex&) const;
    QVariant data(const QModelIndex&, int) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    /**
    * Returns the name of the entryColumns column
    */
    static QString columnTitle(KEduVocDocument *document, int translation, int column);

    /**
     * Returns which translation this column matches. It starts from 0 and increases every
     * EntryColumnMax columns
     */
    static int translation(int column);

    /**
     * Returns the type of the column specified. Translation types are multiples of
     * EntryColumnsMAX
     */
    static int columnType(int column);

    QModelIndex appendEntry(KEduVocExpression *expression = 0);

    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());

    QStringList mimeTypes() const;
    QMimeData * mimeData(const QModelIndexList &indexes) const;

    void resetLanguages();

public slots:
    void setDocument(KEduVocDocument *doc);

    /**
     * Whatever the contents, the model will now display it.
     * @param container
     */
    void showContainer(KEduVocContainer *container);

    /**
     *
     * @param lessonContainer
     */
    void setLesson(KEduVocLesson *lessonContainer);

    KEduVocLesson * lesson();

    KEduVocContainer::EnumEntriesRecursive recursive();

    /**
     * Show the entries of child lessons in selected lessons
     * @param show
     */
    void setRecursive(bool show);

private:
    class Private;
    Private * const d;
};

Q_DECLARE_METATYPE(KEduVocExpression*)

#endif