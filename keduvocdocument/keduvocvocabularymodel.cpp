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
#include "keduvocvocabularymodel.h"

#include <keduvoclesson.h>
#include <keduvocwordtype.h>

#include "keduvocvocabularymimedata.h"
#include "keduvocdocument.h"

#include <keduvocexpression.h>
#include <KLocalizedString>
#include <QDebug>
#include <QPixmap>
#include <QTextStream>
#include <QFont>
#include <QAbstractTableModel>

class KEduVocVocabularyModel::Private
{
public:
    Private();

    KEduVocContainer *m_container;
    KEduVocLesson *m_lesson;

    KEduVocDocument *m_document;
    KEduVocContainer::EnumEntriesRecursive m_recursive;

    // stores the font-type for each language as specified by the application
    QVector<QFont> m_fontList;
};

KEduVocVocabularyModel::Private::Private()
{
    m_container = 0;
    m_document = 0;
    m_lesson = 0;
}

KEduVocVocabularyModel::KEduVocVocabularyModel(bool show, QObject *parent)
    : QAbstractTableModel(parent),
      d( new Private )
{
    setRecursive(show);

    qRegisterMetaType<KEduVocTranslation*>("KEduVocTranslationPointer");
}

KEduVocVocabularyModel::~KEduVocVocabularyModel()
{
    delete d;
}

void KEduVocVocabularyModel::setDocument(KEduVocDocument * doc)
{
    beginResetModel();

    if (d->m_document) {
        showContainer(d->m_document->lesson());
    } else {
        showContainer(0);
    }

    endResetModel();
}


void KEduVocVocabularyModel::showContainer(KEduVocContainer * container)
{
    // use remove and insert rows. using reset resets all table headers too.
    if (rowCount(QModelIndex()) > 0) {
        beginRemoveRows(QModelIndex(), 0, rowCount(QModelIndex()) - 1);
        d->m_container = 0;
        endRemoveRows();
    }
    if (container) {
        if (container->entryCount(d->m_recursive) > 0) {
            beginInsertRows(QModelIndex(), 0, container->entryCount(d->m_recursive) - 1);
            d->m_container = container;
            endInsertRows();
        } else {
            d->m_container = container;
        }
    }
}

void KEduVocVocabularyModel::setLesson(KEduVocLesson * lessonContainer)
{
    d->m_lesson = lessonContainer;
}

KEduVocLesson * KEduVocVocabularyModel::lesson()
{
    return d->m_lesson;
}

KEduVocContainer::EnumEntriesRecursive KEduVocVocabularyModel::recursive()
{
    return d->m_recursive;
}

int KEduVocVocabularyModel::rowCount(const QModelIndex &index) const
{
    // no lesson set - zarro rows
    if (!d->m_container) {
        return 0;
    }
    // only the root index has children because we have no hierarchical model.
    if (index == QModelIndex()) {
        return d->m_container->entryCount(d->m_recursive);
    }
    return 0;
}

int KEduVocVocabularyModel::columnCount(const QModelIndex &) const
{
    if (!d->m_document) {
        return 0;
    }
    return d->m_document->identifierCount() * EntryColumnsMAX;
}

QFont KEduVocVocabularyModel::font(int translation) const
{
    return d->m_fontList[translation];
}

void KEduVocVocabularyModel::setFont(QFont & font, int translation)
{
        d->m_fontList[translation] = font;
        emit dataChanged(createIndex(translation, translation), createIndex(translation, translation));
}

QVariant KEduVocVocabularyModel::data(const QModelIndex & index, int role) const
{
    if (!d->m_document || !d->m_container) {
        return QVariant();
    }

    int translationId = translation(index.column());
    int entryColumn = columnType(index.column());

    switch (role) {
    case Qt::DisplayRole:
        switch (entryColumn) {
        case Translation:
            return QVariant(d->m_container->entry(index.row(), d->m_recursive)->translation(translationId)->text());
        case Pronunciation:
            return QVariant(d->m_container->entry(index.row(), d->m_recursive)->translation(translationId)->pronunciation());
        case WordClass:
            // if no word type is set, we get a null pointer
            if (d->m_container->entry(index.row(), d->m_recursive)->translation(translationId)->wordType()) {
                return QVariant(d->m_container->entry(index.row(), d->m_recursive)->translation(translationId)->wordType()->name());
            }
            return QVariant(QString());
        case Synonym: {
            QStringList displayElements;
            foreach(KEduVocTranslation * synonym,  d->m_container->entry(index.row(), d->m_recursive)->translation(translationId)->synonyms()) {
                displayElements.append(synonym->text());
            }
            return QVariant(displayElements.join("; "));
        }
        case Antonym: {
            QStringList displayElements;
            foreach(KEduVocTranslation * antonym,  d->m_container->entry(index.row(), d->m_recursive)->translation(translationId)->antonyms()) {
                displayElements.append(antonym->text());
            }
            return QVariant(displayElements.join("; "));
        }
        case Example: {
            QString example = d->m_container->entry(index.row(), d->m_recursive)->translation(translationId)->example();
            return QVariant(example);
        }
        case Comment:
            return QVariant(d->m_container->entry(index.row(), d->m_recursive)->translation(translationId)->comment());
        case Paraphrase:
            return QVariant(d->m_container->entry(index.row(), d->m_recursive)->translation(translationId)->paraphrase());
        default:
            return QVariant();
        }
        break;
    case Qt::FontRole:
        if (entryColumn == Translation) {
            if( &(d->m_fontList.at( translationId )) )
                return QVariant( d->m_fontList[translationId] );
            return QVariant();
        }
        return QVariant();
    case LocaleRole:
        return QVariant(d->m_document->identifier(translationId).locale());
    case AudioRole:
        return QVariant(d->m_container->entry(index.row(), d->m_recursive)->translation(translationId)->soundUrl());
    case ImageRole:
        return QVariant(d->m_container->entry(index.row(), d->m_recursive)->translation(translationId)->imageUrl());
    case EntryRole: {
        QVariant v;
        v.setValue(d->m_container->entry(index.row(), d->m_recursive));
        return v;
    }
    case Qt::ToolTipRole: {
        ///@todo more tooltips?
        switch (entryColumn) {
        case WordClass:
            return i18n("You can drag and drop words onto their word type.");
        case Synonym:
            return i18n("Enable the synonym view to edit synonyms.");
        case Antonym:
            return i18n("Enable the antonym view to edit antonyms.");
        }
    }
    }
    return QVariant();
}


bool KEduVocVocabularyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ((!index.isValid()) || (role != Qt::EditRole)) {
        return false;
    }

    int translationId = translation(index.column());
    int column = columnType(index.column());

    switch (column) {
    case Translation:
        d->m_container->entry(index.row(), d->m_recursive)->translation(translationId)->setText(value.toString());
        break;
    case Pronunciation:
        d->m_container->entry(index.row(), d->m_recursive)->translation(translationId)->setPronunciation(value.toString());
        break;
    case WordClass:
        break;
    case Example:
        d->m_container->entry(index.row(), d->m_recursive)->translation(translationId)->setExample(value.toString());
        break;
    case Comment:
        d->m_container->entry(index.row(), d->m_recursive)->translation(translationId)->setComment(value.toString());
        break;
    case Paraphrase:
        d->m_container->entry(index.row(), d->m_recursive)->translation(translationId)->setParaphrase(value.toString());
        break;
    default:
        return false;
    }

    emit(dataChanged(index, index));
    ///@todo trust dirty bit
    d->m_document->setModified();
    return true;
}

Qt::ItemFlags KEduVocVocabularyModel::flags(const QModelIndex & index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsDropEnabled;
    }

    switch (columnType(index.column())) {
    case Translation:
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    case WordClass:
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    case Synonym:
    case Antonym:
        return QAbstractItemModel::flags(index) | Qt::ItemIsDropEnabled;
    default:
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsDropEnabled;
    }
}

QVariant KEduVocVocabularyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!d->m_document) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        int translationId = section / EntryColumnsMAX;
        int entryColumn = section % EntryColumnsMAX;

        switch (role) {
        case Qt::DisplayRole:
            return KEduVocVocabularyModel::columnTitle(d->m_document, translationId, entryColumn);
            break;
        } // switch role
    } // if horizontal
    return QVariant();
}

QString KEduVocVocabularyModel::columnTitle(KEduVocDocument *document, int translation, int column)
{
    switch (column) {
    case Translation:
        if (document->identifierCount() - 1 < translation) {
            return QString();
        }
        return document->identifier(translation).name(); //returns "English", "German", etc
    case Pronunciation:
        return i18n("Pronunciation");
    case WordClass:
        return i18n("Word Type");
    case Synonym:
        return i18n("Synonym");
    case Antonym:
        return i18n("Antonym");
    case Example:
        return i18n("Example");
    case Comment:
        return i18n("Comment");
    case Paraphrase:
        return i18n("Paraphrase");
    }

    return QString();
}

int KEduVocVocabularyModel::translation(int column)
{
    return column / EntryColumnsMAX;
}

int KEduVocVocabularyModel::columnType(int column)
{
    return column % EntryColumnsMAX;
}

QModelIndex KEduVocVocabularyModel::appendEntry(KEduVocExpression *expression)
{
    if (d->m_document->identifierCount() == 0) {
        return QModelIndex();
    }

    if (!d->m_lesson || !d->m_lesson->parent()) {
        return QModelIndex();
    }

    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    if (!expression) {
        expression = new KEduVocExpression;
    }
    d->m_lesson->appendEntry(expression);
    endInsertRows();

    return index(rowCount(QModelIndex()) - 1, 0, QModelIndex());
}

bool KEduVocVocabularyModel::removeRows(int row, int count, const QModelIndex & parent)
{
    Q_UNUSED(parent);
    if (count < 1 || row < 0 || row + count > d->m_container->entryCount(d->m_recursive)) {
        return false;
    }

    int bottomRow = row + count - 1;
    beginRemoveRows(QModelIndex(), row, bottomRow);

    for (int i = bottomRow; i >= row; i--) {
        delete d->m_container->entry(i, d->m_recursive);
    }

    endRemoveRows();
    return true;
}

QStringList KEduVocVocabularyModel::mimeTypes() const
{
    return QStringList() << "text/plain";
}

QMimeData * KEduVocVocabularyModel::mimeData(const QModelIndexList & indexes) const
{
    KEduVocVocabularyMimeData *mimeData = new KEduVocVocabularyMimeData();
    QModelIndexList sortedIndexes = indexes;
    qSort(sortedIndexes);

    //qDebug() << "mimeData for " << indexes.count() << "indexes";

    QList<KEduVocTranslation*> translations;
    foreach(const QModelIndex & index, sortedIndexes) {
        // only add if it's a translation. other cells like word type are being ignored for now.
        if (columnType(index.column()) == Translation) {
            translations.append(d->m_container->entry(index.row(), d->m_recursive)->translation(translation(index.column())));
        }
    }

    mimeData->setTranslations(translations);
    return mimeData;
}

void KEduVocVocabularyModel::setRecursive(bool show)
{
    beginResetModel();
    if (show) {
        d->m_recursive = KEduVocContainer::Recursive;
    } else {
        d->m_recursive = KEduVocContainer::NotRecursive;
    }
    endResetModel();
}

void KEduVocVocabularyModel::showEntriesOfSubcontainers(bool show)
{
    beginResetModel();
    setRecursive(show);
    endResetModel();
}

void KEduVocVocabularyModel::resetLanguages()
{
    // play it save - this happens seldom enough to warrant a reload
    setDocument(d->m_document);
}