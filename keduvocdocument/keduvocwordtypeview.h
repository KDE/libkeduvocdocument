/***************************************************************************

    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCWORDTYPEVIEW_H
#define KEDUVOCWORDTYPEVIEW_H

#include <keduvoccontainerview.h>
#include <keduvocdocument_export.h>

class QAction;
class KSelectAction;
class KEduVocWordType;

namespace Editor
{

/* FIXME Reimplement this after editor.h is added to keduvoc, and change the constructor accordingly, removing KActionCollection
 * class EditorWindow;
 */

class KEDUVOCDOCUMENT_EXPORT KEduVocWordTypeView : public KEduVocContainerView
{
    Q_OBJECT

public:
    KEduVocWordTypeView( QWidget *parent);
    ~KEduVocWordTypeView();

public slots:
    /** Append a word type to the model and automatically set an edit up so the user can change "New lesson" into something meaningfull.*/
    void slotCreateNewWordType();

    /** Remove a word type. */
    void slotDeleteWordType();

    void setTranslation( KEduVocExpression* entry, int translationId );

signals:
    void selectedWordTypeChanged( KEduVocWordType* wordType );

protected:
    void contextMenuEvent( QContextMenuEvent * event );

private slots:
    void setWordTypeNone();
    void setWordTypeNoun();
    void setWordTypeNounMale();
    void setWordTypeNounFemale();
    void setWordTypeNounNeutral();
    void setWordTypeAdjective();
    void setWordTypeAdverb();
    void setWordTypeVerb();
    void setWordTypeConjunction();

private:
    class Private;
    Private * const d;
};
}

#endif
