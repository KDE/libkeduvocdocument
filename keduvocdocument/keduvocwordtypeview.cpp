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

#include "keduvocwordtypeview.h"

#include <QContextMenuEvent>
#include <QMenu>

#include <QAction>
#include <KLocalizedString>
#include <KMessageBox>
#include <KSelectAction>

#include <keduvocexpression.h>
#include <keduvocwordtype.h>
#include <keduvoccontainermodel.h>
#include <keduvoccontainerview.h>

//#include "editor/editor.h"

using namespace Editor;

class KEduVocWordTypeView::Private
{
public:
    Private();

    QAction *m_actionNewWordType;
    QAction *m_actionDeleteWordType;
    QAction *m_actionRenameWordType;
    KSelectAction *m_actionSpecialTypeMenu;

    QAction *m_noneAction;
    QAction *m_separator;
    QAction *m_nounAction;
    QAction *m_nounMaleAction;
    QAction *m_nounFemaleAction;
    QAction *m_nounNeutralAction;
    QAction *m_adjectiveAction;
    QAction *m_adverbAction;
    QAction *m_verbAction;
    QAction *m_conjunctionAction;
};

KEduVocWordTypeView::Private::Private()
{
}

KEduVocWordTypeView::KEduVocWordTypeView( QWidget* parent )
    : KEduVocContainerView( parent )
    , d( new Private )
{
    setContextMenuPolicy( Qt::DefaultContextMenu );

    d->m_actionNewWordType = new QAction( this );
    d->m_actionNewWordType->setText( i18n( "New" ) );
    d->m_actionNewWordType->setIcon( QIcon::fromTheme( "lesson-add" ) );
    d->m_actionNewWordType->setWhatsThis( i18n( "Add a new word type to your document" ) );
    d->m_actionNewWordType->setToolTip( d->m_actionNewWordType->whatsThis() );
    d->m_actionNewWordType->setStatusTip( d->m_actionNewWordType->whatsThis() );
    d->m_actionNewWordType->setStatusTip( d->m_actionNewWordType->whatsThis() );

    d->m_actionRenameWordType = new QAction( this );
    d->m_actionRenameWordType->setText( i18n( "Rename" ) );
    d->m_actionRenameWordType->setIcon( QIcon::fromTheme( "edit-rename" ) );
    d->m_actionRenameWordType->setWhatsThis( i18n( "Rename the selected word type" ) );
    d->m_actionRenameWordType->setToolTip( d->m_actionRenameWordType->whatsThis() );
    d->m_actionRenameWordType->setStatusTip( d->m_actionRenameWordType->whatsThis() );
    d->m_actionRenameWordType->setStatusTip( d->m_actionRenameWordType->whatsThis() );

    d->m_actionDeleteWordType = new QAction( this );
    d->m_actionDeleteWordType->setText( i18n( "Delete Word Type" ) );
    d->m_actionDeleteWordType->setIcon( QIcon::fromTheme( "lesson-remove" ) );
    d->m_actionDeleteWordType->setWhatsThis( i18n( "Delete the selected word type." ) );
    d->m_actionDeleteWordType->setToolTip( d->m_actionDeleteWordType->whatsThis() );
    d->m_actionDeleteWordType->setStatusTip( d->m_actionDeleteWordType->whatsThis() );
    d->m_actionDeleteWordType->setStatusTip( d->m_actionDeleteWordType->whatsThis() );

    d->m_actionSpecialTypeMenu = new KSelectAction( this );
    d->m_actionSpecialTypeMenu->setText( i18nc( "Let the user select what grammatical meaning is connected to a word type (nouns have gender, verbs conjugations etc)", "Grammar" ) );
    //actionSplitWordType->setIcon(QIcon::fromTheme(""));  /// @todo better icon
    d->m_actionSpecialTypeMenu->setWhatsThis( i18n( "To let Parley know the grammatical meaning of a word type." ) );
    d->m_actionSpecialTypeMenu->setToolTip( d->m_actionSpecialTypeMenu->whatsThis() );
    d->m_actionSpecialTypeMenu->setStatusTip( d->m_actionSpecialTypeMenu->whatsThis() );
    d->m_actionSpecialTypeMenu->setStatusTip( d->m_actionSpecialTypeMenu->whatsThis() );


    d->m_nounAction = new QAction( this );
    d->m_nounAction->setText( i18n( "Noun" ) );
    d->m_nounAction->setCheckable( true );
    d->m_nounAction->setWhatsThis( i18n( "This word type folder contains nouns." ) );
    d->m_nounAction->setToolTip( d->m_nounAction->whatsThis() );
    d->m_nounAction->setStatusTip( d->m_nounAction->whatsThis() );
    d->m_nounAction->setStatusTip( d->m_nounAction->whatsThis() );

    d->m_nounMaleAction = new QAction( this );
    d->m_nounMaleAction->setText( i18n( "Masculine Noun" ) );
    d->m_nounMaleAction->setCheckable( true );
    d->m_nounMaleAction->setWhatsThis( i18n( "This word type folder contains masculine nouns." ) );
    d->m_nounMaleAction->setToolTip( d->m_nounMaleAction->whatsThis() );
    d->m_nounMaleAction->setStatusTip( d->m_nounMaleAction->whatsThis() );
    d->m_nounMaleAction->setStatusTip( d->m_nounMaleAction->whatsThis() );

    d->m_nounFemaleAction = new QAction( this );
    d->m_nounFemaleAction->setText( i18n( "Feminine Noun" ) );
    d->m_nounFemaleAction->setCheckable( true );
    d->m_nounFemaleAction->setWhatsThis( i18n( "This word type folder contains feminine nouns." ) );
    d->m_nounFemaleAction->setToolTip( d->m_nounFemaleAction->whatsThis() );
    d->m_nounFemaleAction->setStatusTip( d->m_nounFemaleAction->whatsThis() );
    d->m_nounFemaleAction->setStatusTip( d->m_nounFemaleAction->whatsThis() );

    d->m_nounNeutralAction = new QAction( this );
    d->m_nounNeutralAction->setText( i18n( "Neuter Noun" ) );
    d->m_nounNeutralAction->setCheckable( true );
    d->m_nounNeutralAction->setWhatsThis( i18n( "This word type folder contains neuter nouns." ) );
    d->m_nounNeutralAction->setToolTip( d->m_nounNeutralAction->whatsThis() );
    d->m_nounNeutralAction->setStatusTip( d->m_nounNeutralAction->whatsThis() );
    d->m_nounNeutralAction->setStatusTip( d->m_nounNeutralAction->whatsThis() );

    d->m_adjectiveAction = new QAction( this );
    d->m_adjectiveAction->setText( i18n( "Adjective" ) );
    d->m_adjectiveAction->setCheckable( true );
    d->m_adjectiveAction->setWhatsThis( i18n( "This word type folder contains adjectives." ) );
    d->m_adjectiveAction->setToolTip( d->m_adjectiveAction->whatsThis() );
    d->m_adjectiveAction->setStatusTip( d->m_adjectiveAction->whatsThis() );
    d->m_adjectiveAction->setStatusTip( d->m_adjectiveAction->whatsThis() );

    d->m_adverbAction = new QAction( this );
    d->m_adverbAction->setText( i18n( "Adverb" ) );
    d->m_adverbAction->setCheckable( true );
    d->m_adverbAction->setWhatsThis( i18n( "This word type folder contains adverbs." ) );
    d->m_adverbAction->setToolTip( d->m_adverbAction->whatsThis() );
    d->m_adverbAction->setStatusTip( d->m_adverbAction->whatsThis() );
    d->m_adverbAction->setStatusTip( d->m_adverbAction->whatsThis() );


    d->m_verbAction = new QAction( this );
    d->m_verbAction->setText( i18n( "Verb" ) );
    d->m_verbAction->setCheckable( true );
    d->m_verbAction->setWhatsThis( i18n( "This word type folder contains verbs." ) );
    d->m_verbAction->setToolTip( d->m_verbAction->whatsThis() );
    d->m_verbAction->setStatusTip( d->m_verbAction->whatsThis() );
    d->m_verbAction->setStatusTip( d->m_verbAction->whatsThis() );

    d->m_conjunctionAction = new QAction( this );
    d->m_conjunctionAction->setText( i18n( "Conjunction" ) );
    d->m_conjunctionAction->setCheckable( true );
    d->m_conjunctionAction->setWhatsThis( i18n( "This word type folder contains conjunctions." ) );
    d->m_conjunctionAction->setToolTip( d->m_conjunctionAction->whatsThis() );
    d->m_conjunctionAction->setStatusTip( d->m_conjunctionAction->whatsThis() );
    d->m_conjunctionAction->setStatusTip( d->m_conjunctionAction->whatsThis() );

    d->m_noneAction = new QAction( this );
    d->m_noneAction->setText( i18n( "No Special Type" ) );
    d->m_noneAction->setCheckable( true );
    d->m_noneAction->setChecked( true );
    d->m_noneAction->setWhatsThis( i18n( "This word type folder contains no word type with special meaning." ) );
    d->m_noneAction->setToolTip( d->m_noneAction->whatsThis() );
    d->m_noneAction->setStatusTip( d->m_noneAction->whatsThis() );
    d->m_noneAction->setStatusTip( d->m_noneAction->whatsThis() );

    QAction* separator = new QAction( this );
    separator->setSeparator( true );

    d->m_actionSpecialTypeMenu->addAction( d->m_noneAction );
    d->m_actionSpecialTypeMenu->addAction( separator );
    d->m_actionSpecialTypeMenu->addAction( d->m_nounAction );
    d->m_actionSpecialTypeMenu->addAction( d->m_nounMaleAction );
    d->m_actionSpecialTypeMenu->addAction( d->m_nounFemaleAction );
    d->m_actionSpecialTypeMenu->addAction( d->m_nounNeutralAction );
    d->m_actionSpecialTypeMenu->addAction( d->m_adjectiveAction );
    d->m_actionSpecialTypeMenu->addAction( d->m_adverbAction );
    d->m_actionSpecialTypeMenu->addAction( d->m_verbAction );
    d->m_actionSpecialTypeMenu->addAction( d->m_conjunctionAction );

    connect( d->m_actionNewWordType, &QAction::triggered, this, &KEduVocWordTypeView::slotCreateNewWordType );
    connect( d->m_actionRenameWordType, &QAction::triggered, this, &KEduVocWordTypeView::slotRename );
    connect( d->m_actionDeleteWordType, &QAction::triggered, this, &KEduVocWordTypeView::slotDeleteWordType );

    // right cick menu for the WordType view:
    addAction( d->m_actionNewWordType );
    addAction( d->m_actionRenameWordType );
    addAction( d->m_actionDeleteWordType );
    separator = new QAction( this );
    separator->setSeparator( true );
    addAction( separator );
    separator = new QAction( this );
    separator->setSeparator( true );
    addAction( separator );
    addAction( d->m_actionSpecialTypeMenu );

    connect( d->m_noneAction, &QAction::triggered, this, &KEduVocWordTypeView::setWordTypeNone );
    connect( d->m_nounAction, &QAction::triggered, this, &KEduVocWordTypeView::setWordTypeNoun );
    connect( d->m_nounMaleAction, &QAction::triggered, this, &KEduVocWordTypeView::setWordTypeNounMale );
    connect( d->m_nounFemaleAction, &QAction::triggered, this, &KEduVocWordTypeView::setWordTypeNounFemale );
    connect( d->m_nounNeutralAction, &QAction::triggered, this, &KEduVocWordTypeView::setWordTypeNounNeutral );
    connect( d->m_adjectiveAction, &QAction::triggered, this, &KEduVocWordTypeView::setWordTypeAdjective );
    connect( d->m_adverbAction, &QAction::triggered, this, &KEduVocWordTypeView::setWordTypeAdverb );
    connect( d->m_verbAction, &QAction::triggered, this, &KEduVocWordTypeView::setWordTypeVerb );
    connect( d->m_conjunctionAction, &QAction::triggered, this, &KEduVocWordTypeView::setWordTypeConjunction);
}

KEduVocWordTypeView::~KEduVocWordTypeView()
{
    delete d;
}

void KEduVocWordTypeView::setTranslation( KEduVocExpression * entry, int translation )
{
    if (entry == 0) {
        return;
    }

    // attempt to find the container to select
    QModelIndex index( model()->index( entry->translation( translation )->wordType() ) );
    scrollTo( index );
    selectionModel()->select( index, QItemSelectionModel::ClearAndSelect );
}

void KEduVocWordTypeView::slotCreateNewWordType()
{
    QModelIndex selectedIndex = selectionModel()->currentIndex();

    if( !selectedIndex.isValid() ) {
        selectedIndex = model()->index( 0, 0, QModelIndex() );
    }

    QModelIndex modelIndex = model()->appendContainer( selectedIndex );

    scrollTo( modelIndex );
    selectionModel()->setCurrentIndex( modelIndex, QItemSelectionModel::ClearAndSelect );
    edit( modelIndex );    // let the user type a new name for the WordType
}

void KEduVocWordTypeView::slotDeleteWordType()
{
    QModelIndex selectedIndex = selectionModel()->currentIndex();

    if( selectedIndex.parent() == QModelIndex() ) {
        KMessageBox::information( this, i18n( "The root word type cannot be deleted." ) );
        return;
    }

    KEduVocWordType* WordType = static_cast<KEduVocWordType*>( selectedIndex.internalPointer() );

    int count = WordType->entryCount();

    if( count == 0 ||
            KMessageBox::warningYesNo( this, i18np( "There is a word left with this word type. It will lose its type. Continue?", "There are %1 words left with this word type. They will lose their word type. Continue?", count ) ) == KMessageBox::Yes ) {
        model()->deleteContainer( selectedIndex );
    }
}

void KEduVocWordTypeView::contextMenuEvent( QContextMenuEvent * event )
{
    // check for the root element:
    QModelIndex selectedIndex = selectionModel()->currentIndex();
    d->m_actionRenameWordType->setEnabled( selectedIndex.parent() != QModelIndex() );
    d->m_actionDeleteWordType->setEnabled( selectedIndex.parent() != QModelIndex() );
    d->m_actionSpecialTypeMenu->setEnabled( selectedIndex.parent() != QModelIndex() );

    if( selectedIndex.isValid() ) {
        KEduVocWordFlags t = ( static_cast<KEduVocWordType*>( selectionModel()->currentIndex().internalPointer() )->wordType() );

        if( !t )
            d->m_noneAction->setChecked( true );

        else if( t.testFlag( KEduVocWordFlag::Noun ) && t.testFlag( KEduVocWordFlag::Masculine ) )
            d->m_nounMaleAction->setChecked( true );

        else if( t.testFlag( KEduVocWordFlag::Noun ) && t.testFlag( KEduVocWordFlag::Feminine ) )
            d->m_nounFemaleAction->setChecked( true );

        else if( t.testFlag( KEduVocWordFlag::Noun ) && t.testFlag( KEduVocWordFlag::Neuter ) )
            d->m_nounNeutralAction->setChecked( true );

        else if( t.testFlag( KEduVocWordFlag::Noun ) )
            d->m_nounAction->setChecked( true );

        else if( t.testFlag( KEduVocWordFlag::Adjective ) )
            d->m_adjectiveAction->setChecked( true );

        else if( t.testFlag( KEduVocWordFlag::Adverb ) )
            d->m_adverbAction->setChecked( true );

        else if( t.testFlag( KEduVocWordFlag::Verb ) )
            d->m_verbAction->setChecked( true );

        else if( t.testFlag( KEduVocWordFlag::Conjunction ) )
            d->m_conjunctionAction->setChecked( true );
    }

    QMenu::exec( actions(), event->globalPos() );
}

void KEduVocWordTypeView::setWordTypeNone()
{
    KEduVocWordType* wordType = static_cast<KEduVocWordType*>( selectionModel()->currentIndex().internalPointer() );
    wordType->setWordType( KEduVocWordFlag::NoInformation );
}

void KEduVocWordTypeView::setWordTypeNoun()
{
    KEduVocWordType* wordType = static_cast<KEduVocWordType*>( selectionModel()->currentIndex().internalPointer() );
    wordType->setWordType( KEduVocWordFlag::Noun );
}

void KEduVocWordTypeView::setWordTypeNounMale()
{
    KEduVocWordType* wordType = static_cast<KEduVocWordType*>( selectionModel()->currentIndex().internalPointer() );
    wordType->setWordType( KEduVocWordFlag::Noun | KEduVocWordFlag::Masculine );
}

void KEduVocWordTypeView::setWordTypeNounFemale()
{
    KEduVocWordType* wordType = static_cast<KEduVocWordType*>( selectionModel()->currentIndex().internalPointer() );
    wordType->setWordType( KEduVocWordFlag::Noun | KEduVocWordFlag::Feminine );
}

void KEduVocWordTypeView::setWordTypeNounNeutral()
{
    KEduVocWordType* wordType = static_cast<KEduVocWordType*>( selectionModel()->currentIndex().internalPointer() );
    wordType->setWordType( KEduVocWordFlag::Noun | KEduVocWordFlag::Neuter );
}

void KEduVocWordTypeView::setWordTypeAdjective()
{
    KEduVocWordType* wordType = static_cast<KEduVocWordType*>( selectionModel()->currentIndex().internalPointer() );
    wordType->setWordType( KEduVocWordFlag::Adjective );
}

void KEduVocWordTypeView::setWordTypeAdverb()
{
    KEduVocWordType* wordType = static_cast<KEduVocWordType*>( selectionModel()->currentIndex().internalPointer() );
    wordType->setWordType( KEduVocWordFlag::Adverb );
}

void KEduVocWordTypeView::setWordTypeVerb()
{
    KEduVocWordType* wordType = static_cast<KEduVocWordType*>( selectionModel()->currentIndex().internalPointer() );
    wordType->setWordType( KEduVocWordFlag::Verb );
}


void KEduVocWordTypeView::setWordTypeConjunction()
{
    KEduVocWordType* wordType = static_cast<KEduVocWordType*>( selectionModel()->currentIndex().internalPointer() );
    wordType->setWordType( KEduVocWordFlag::Conjunction );
}
