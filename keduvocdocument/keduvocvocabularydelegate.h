/***************************************************************************

    Copyright 2006, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
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

#ifndef KEDUVOCVOCABULARYDELEGATE_H
#define KEDUVOCVOCABULARYDELEGATE_H


#include <QItemDelegate>
#include <QModelIndex>

#include <keduvocdocument_export.h>
#include <keduvocreadonlycontainermodel.h>
#include <keduvoctranslator.h>
#include <keduvocdocument.h>

class KEduVocDocument;

namespace Editor
{

class KEDUVOCDOCUMENT_EXPORT KEduVocVocabularyDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    enum entryColumns {
        Translation = 0,
        Pronunciation,
        WordType,
        Synonym,
        Antonym,
        Example,
        Comment,
        Paraphrase,
//         Audio,
//         Image,
        EntryColumnsMAX
    };

    /**
     * @param automaticTranslation Pass the value of Prefs::automaticTranslation() for this
    */
    KEduVocVocabularyDelegate( bool automaticTranslation, QObject *parent = 0 );

    ~KEduVocVocabularyDelegate();

    /**
     * @param keyboardLayout Represents LanguageSettings::keyboardLayout
     *      Example: In Parley, obtain the keyboard-layout value in this manner
     *          QString locale = index.model()->data(index, KEduVocVocabularyModel::LocaleRole).toString();
                if (!locale.isEmpty()) {                       *
                    LanguageSettings settings(locale);
                    settings.load();
                    QString layout = settings.keyboardLayout();
                }
     */
    QWidget * createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index, QString keyboardLayout ) const;

    void setEditorData( QWidget *editor, const QModelIndex &index ) const;
    void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
    void setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const;
    void setTranslator( KEduVocTranslator * translator );

    /** Returns the translations of the word of the given index */
    QSet<QString> getTranslations( const QModelIndex & index ) const;

    /** Set m_automaticTranslation */
    void setAutomaticTranslation( bool v );

    /** Return current value of m_automaticTranslation */
    bool automaticTranslation();

    /** Set m_keyboardLayout */
    void setKeyboardLayout( QString keyboardLayout );

    /** Return current value of m_keyboardLayout */
    QString keyboardLayout();

    static int columnType( int column );

    bool hasAudio( const QModelIndex &index ) const;
    bool hasImage( const QModelIndex &index ) const;
    QString audioUrl( const QModelIndex &index ) const;
    QString imageUrl( const QModelIndex &index ) const;
    QPolygon audioPolygon( const QStyleOptionViewItem &option ) const;
    QPolygon imagePolygon( const QStyleOptionViewItem &option ) const;

public slots:
    void setDocument( KEduVocDocument *doc );
    bool helpEvent( QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index );

private:
    class Private;
    Private * const d;
};

}

#endif
