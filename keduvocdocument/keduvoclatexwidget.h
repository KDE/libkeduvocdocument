/***************************************************************************
    Copyright 2010 Daniel Laidig <laidig@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef KEDUVOCLATEXWIDGET_H
#define KEDUVOCLATEXWIDGET_H

#include <QItemDelegate>
#include <QItemSelection>
#include <QCheckBox>
#include <QWidget>

#include <keduvocdocument_export.h>

class QDataWidgetMapper;
class KEduVocDocument;
class KEduVocExpression;
class KEduVocTranslation;

namespace Practice
{
class KEduVocLatexRenderer;
}

class KEduVocVocabularyFilter;

namespace Editor
{

class KEDUVOCDOCUMENT_EXPORT KEduVocLatexWidget : public QWidget
{
    Q_OBJECT

public:
    KEduVocLatexWidget( KEduVocVocabularyFilter *model, KEduVocDocument *doc, QWidget *parent = 0 );
    ~KEduVocLatexWidget();

public slots:
    /**
    * Sets the selected word (KEduVocExpression) from the vocabularyView
    */
    void setTranslation( KEduVocExpression *entry, int translation );

    /**
    * Called when a KEduVocDocument change happened
    */
    void slotDocumentChanged( KEduVocDocument *doc );

    /**
    * Called when the selection changed in the vocabulary view
    */
    void slotSelectionChanged( const QItemSelection &, const QItemSelection & );

    void checkBoxToggled();

    void updateLatex();

private:
    class Private;
    Private * const d;
};

class KEDUVOCDOCUMENT_EXPORT KEduVocLatexDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    KEduVocLatexDelegate( QObject *parent = 0 );
    ~KEduVocLatexDelegate();

    void setEditorData( QWidget *editor, const QModelIndex &index ) const;
    void setModelData( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
    void setMathModeCheckBox( QCheckBox *checkBox );

private:
    class Private;
    Private * const d;
};
}

#endif //LATEXWIDGET_H
