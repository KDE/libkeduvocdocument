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

#include "keduvocvocabularydelegate.h"

#include <keduvocexpression.h>
#include <keduvocwordtype.h>
#include <keduvocvocabularymodel.h>
#include <keduvocreadonlycontainermodel.h>
#include <keduvocvocabularyfilter.h>

#include <KLocalizedString>
#include <KComboBox>
#include <QDebug>
#include <QLineEdit>
#include <QTreeView>
#include <QHeaderView>
#include <QDBusInterface>
#include <QKeyEvent>
#include <QtGui>
#include <QToolTip>

using namespace Editor;

class KEduVocVocabularyDelegate::Private
{
public:
    Private( bool automaticTranslation );

    KEduVocDocument *m_doc;
    KEduVocTranslator * m_translator;

    /** Replacement for Prefs::automaticTranslation */
    bool m_automaticTranslation;

    /** Replacement for LanguageSettings::keyboardLayout() */
    QString m_keyboardLayout;

    // for the word type combo
    class WordTypeBasicModel;
};

KEduVocVocabularyDelegate::Private::Private( bool automaticTranslation )
    : m_doc(0),
      m_translator(0)
{
    m_automaticTranslation = automaticTranslation;
}

class KEduVocVocabularyDelegate::Private::WordTypeBasicModel : public KEduVocReadonlyContainerModel
{
public:
    explicit WordTypeBasicModel( QObject *parent = 0 );
protected:
    KEduVocContainer * rootContainer() const;
};

KEduVocVocabularyDelegate::KEduVocVocabularyDelegate( bool automaticTranslation, QObject *parent )
    : QItemDelegate(parent),
      d( new Private( automaticTranslation ) )
{
}

KEduVocVocabularyDelegate::~KEduVocVocabularyDelegate()
{
    delete d;
}

void KEduVocVocabularyDelegate::setAutomaticTranslation( bool v )
{
    d->m_automaticTranslation = v;
}

bool KEduVocVocabularyDelegate::automaticTranslation()
{
    return d->m_automaticTranslation;
}

void KEduVocVocabularyDelegate::setKeyboardLayout( QString keyboardLayout )
{
    d->m_keyboardLayout = keyboardLayout;
}

QString KEduVocVocabularyDelegate::keyboardLayout()
{
    return d->m_keyboardLayout;
}

QSet<QString> KEduVocVocabularyDelegate::getTranslations( const QModelIndex & index ) const
{
    if( d->m_automaticTranslation == false ) return QSet<QString>();

    QSet<QString> translations; //translations of this column from all the other languages

    int language = index.column() / KEduVocVocabularyModel::EntryColumnsMAX;
    QString toLanguage = d->m_doc->identifier( language ).locale();

    //iterate through all the Translation columns
    for( int i = 0; i < index.model()->columnCount( index.parent() ); i++ ) {
        if( KEduVocVocabularyModel::columnType( i ) == KEduVocVocabularyModel::Translation ) {    //translation column
            QString fromLanguage = d->m_doc->identifier( KEduVocVocabularyModel::translation( i ) ).locale();
            QString word = index.model()->index( index.row(), i, QModelIndex() ).data().toString();

            if( fromLanguage != toLanguage ) {
//              qDebug() << fromLanguage << toLanguage << word;
                //get the word translations and add them to the translations set
                QSet<QString> * tr = d->m_translator->getTranslation( word, fromLanguage, toLanguage );
                if( tr )
                    translations.unite( *(tr) );
            }
        }
    }

    return translations;
}

QWidget * KEduVocVocabularyDelegate::createEditor( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index, QString keyboardLayout ) const
{
    Q_UNUSED(option);    /// as long as it's unused

    if( !index.isValid() ) {
        return 0;
    }

    switch( KEduVocVocabularyModel::columnType( index.column() ) ) {
    case KEduVocVocabularyModel::WordClass: {
        if (!d->m_doc) return 0;
        QComboBox *wordTypeCombo = new QComboBox( parent );

        Private::WordTypeBasicModel *basicWordTypeModel = new Private::WordTypeBasicModel( parent );
        wordTypeCombo->setModel( basicWordTypeModel );
        QTreeView *view = new QTreeView( parent );

        view->setModel( basicWordTypeModel );
        wordTypeCombo->setView( view );

        view->header()->setVisible( false );
        view->setRootIsDecorated( true );

        basicWordTypeModel->setDocument( d->m_doc );
        view->expandAll();

        qDebug() << "index data" << index.data().toString();
        //view->setCurrentItem();

        return wordTypeCombo;
    }

    case KEduVocVocabularyModel::Translation: {
        if( !d->m_doc || !d->m_translator) return 0;

        if( KEduVocVocabularyModel::columnType( index.column() ) == KEduVocVocabularyModel::Translation ) {
            //get the translations of this word (fetch only with the help of scripts, if enabled)
            QSet<QString> translations = getTranslations( index );

            //create combo box
            //if there is only one word and that is the suggestion word (in translations) then don't create the combobox
            if( !translations.isEmpty() && !( translations.size() == 1 && ( *translations.begin() ) == index.model()->data( index, Qt::DisplayRole ).toString() ) ) {
                KComboBox *translationCombo = new KComboBox( parent );
                translationCombo->setFrame( false );
                translationCombo->addItems( translations.toList() );
                translationCombo->setEditable( true );
                translationCombo->setFont( index.model()->data( index, Qt::FontRole ).value<QFont>() );
                translationCombo->setEditText( index.model()->data( index, Qt::DisplayRole ).toString() );
                translationCombo->completionObject()->setItems(translations.toList());
                return translationCombo;
            }
        }
        // no break - we fall back to a line edit if there are not multiple translations fetched online
    }
    default: {
        QLineEdit *editor = new QLineEdit( parent );
        editor->setFrame( false );
        editor->setFont( index.model()->data( index, Qt::FontRole ).value<QFont>() );
        editor->setText( index.model()->data( index, Qt::DisplayRole ).toString() );

        QString locale = index.model()->data( index, KEduVocVocabularyModel::LocaleRole ).toString();
        if( !locale.isEmpty() ) {
            /*FIXME Remove Parley dependancy*/
            d->m_keyboardLayout = keyboardLayout;
            if( !d->m_keyboardLayout.isEmpty() ) {
                QDBusInterface kxkb("org.kde.keyboard", "/Layouts", "org.kde.KeyboardLayouts");
                if (kxkb.isValid()) {
                    kxkb.call("setLayout", keyboardLayout);
                }
            }
        }
        return editor;
    }
    }
}

bool KEduVocVocabularyDelegate::helpEvent( QHelpEvent *event, QAbstractItemView *view,
                                   const QStyleOptionViewItem &option, const QModelIndex &index )
{
    Q_UNUSED(view);

    if ( event->type() == QEvent::ToolTip ) {
        QPainterPath audioPainterPath;
        QPainterPath imagePainterPath;
        audioPainterPath.addPolygon( audioPolygon( option ) );
        imagePainterPath.addPolygon( imagePolygon( option ) );

        int column = columnType( index.column() );

        if( audioPainterPath.contains( event->pos() ) && hasAudio( index ) && ( column == Translation || column == Pronunciation ) ) {
            QToolTip::showText( event->globalPos(), i18n( "Sound file selected: %1", audioUrl( index ) ) );
        } else if( imagePainterPath.contains( event->pos() ) && hasImage( index ) && ( column == Translation || column == Pronunciation ) ) {
            QToolTip::showText( event->globalPos(), i18n( "Image file selected: %1", imageUrl( index ) ) );
        } else {
            QToolTip::hideText();
            event->ignore();
        }
        return true;
    }
    return false;
}

QPolygon KEduVocVocabularyDelegate::audioPolygon( const QStyleOptionViewItem &option ) const
{
    QRect rect = option.rect;
    QPolygon polygon;
    polygon << QPoint( rect.x() + rect.width() - 10, rect.y() );
    polygon << QPoint( rect.x() + rect.width(), rect.y() );
    polygon << QPoint( rect.x() + rect.width(), rect.y() + 10 );
    return polygon;
}

QPolygon KEduVocVocabularyDelegate::imagePolygon( const QStyleOptionViewItem &option ) const
{
    QRect rect = option.rect;
    QPolygon polygon;
    polygon << QPoint( rect.x() + rect.width() - 10, rect.y() + rect.height() );
    polygon << QPoint( rect.x() + rect.width(), rect.y() + rect.height() );
    polygon << QPoint( rect.x() + rect.width(), rect.y() + rect.height() - 10 );
    return polygon;
}

bool KEduVocVocabularyDelegate::hasAudio( const QModelIndex &index ) const
{
    return !audioUrl( index ).isEmpty();
}

bool KEduVocVocabularyDelegate::hasImage( const QModelIndex &index ) const
{
    return !imageUrl( index ).isEmpty();
}

QString KEduVocVocabularyDelegate::audioUrl( const QModelIndex &index ) const
{
    QVariant audioVar = index.data( KEduVocVocabularyModel::AudioRole );
    QString audioUrl = audioVar.toString();
    return audioUrl;
}

QString KEduVocVocabularyDelegate::imageUrl( const QModelIndex &index ) const
{
    QVariant imageVar = index.data( KEduVocVocabularyModel::ImageRole );
    QString imageUrl = imageVar.toString();
    return imageUrl;
}

int KEduVocVocabularyDelegate::columnType( int column )
{
    return column % EntryColumnsMAX;
}

void KEduVocVocabularyDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    QItemDelegate::paint( painter, option, index );
    painter->save();

    int column = columnType( index.column() );

    if( hasAudio(index) == true && ( column == Translation || column == Pronunciation ) ) {
        painter->setPen( QPen( Qt::red ) );
        painter->setBrush( QBrush( Qt::red ) );
        painter->drawPolygon( audioPolygon( option ) );
    }

    if( hasImage( index ) == true && ( column == Translation || column == Pronunciation ) ) {
        painter->setPen( QPen( Qt::blue ) );
        painter->setBrush( QBrush( Qt::blue ) );
        painter->drawPolygon( imagePolygon( option ) );
    }

    painter->restore();
}

void KEduVocVocabularyDelegate::setEditorData( QWidget * editor, const QModelIndex & index ) const
{
    if( !index.isValid() ) {
        return;
    }

    switch( KEduVocVocabularyModel::columnType( index.column() ) ) {
        case( KEduVocVocabularyModel::Translation ) : {
            QString value = index.model()->data( index, Qt::DisplayRole ).toString();
            KComboBox * translationCombo = qobject_cast<KComboBox*> ( editor );
            if( translationCombo ) {
                translationCombo->setEditText( value );
                if( value.isEmpty() ) {
                    // show the translations that were fetched as popup
                    translationCombo->showPopup();
                }
                break;
            }
        }
        default: {
            QString value = index.model()->data( index, Qt::DisplayRole ).toString();

            QLineEdit *lineEdit = qobject_cast<QLineEdit*> ( editor );
            if( lineEdit ) {
                lineEdit->setText( value );
            }
        }
    }
}

void KEduVocVocabularyDelegate::setModelData( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
    if( !index.isValid() ) {
        return;
    }

    switch( KEduVocVocabularyModel::columnType( index.column() ) ) {
    case( KEduVocVocabularyModel::WordClass ) : {
        qDebug() << "word type editor";
        QComboBox *combo = qobject_cast<QComboBox*> ( editor );
        if( !combo ) {
            return;
        }
        qDebug() << "combo" << combo->currentText();
        QModelIndex comboIndex = combo->view()->currentIndex();
        KEduVocWordType* wordType = static_cast<KEduVocWordType*>( comboIndex.internalPointer() );

        // the root is the same as no word type
        if( wordType && wordType->parent() == 0 ) {
            wordType = 0;
        }

        KEduVocVocabularyFilter *filter = qobject_cast<KEduVocVocabularyFilter*> ( model );
        KEduVocVocabularyModel *vocModel = qobject_cast<KEduVocVocabularyModel*> ( (filter)->sourceModel() );
        Q_ASSERT( vocModel );
        QVariant data = vocModel->data( filter->mapToSource( index ), KEduVocVocabularyModel::EntryRole );

        KEduVocExpression *expression = data.value<KEduVocExpression*>();
        Q_ASSERT( expression );
        int translationId = KEduVocVocabularyModel::translation( index.column() );

        expression->translation( translationId )->setWordType( wordType );

    }
    case( KEduVocVocabularyModel::Translation ) : {
        QComboBox * translationCombo = qobject_cast<QComboBox*> ( editor );
        if( translationCombo ) {
            model->setData( index, translationCombo->currentText() );
            break;
        }
    }
    default: {
        QLineEdit *lineEdit = qobject_cast<QLineEdit*> ( editor );
        if( lineEdit ) {
            model->setData( index, lineEdit->text() );
        }
    }
    }
}

void KEduVocVocabularyDelegate::setDocument( KEduVocDocument * doc )
{
    d->m_doc = doc;
}

/*
QPair< QString, QString > VocabularyDelegate::guessWordType(const QString & entry, int language) const
{
    qDebug() << "guessing word type for: " << entry;

    QString article = entry.section(" ", 0, 0);
    if ( article.length() < entry.length() ) {
        if ( article == ->identifier(language).articles().article(KEduVocWordFlag::Singular| KEduVocWordFlag::Definite| KEduVocWordFlag::Masculine) ) {
            qDebug() << "Noun masculine";
            return qMakePair(m_doc->wordTypes().specialTypeNoun(), m_doc->wordTypes().specialTypeNounMale());
        }

    }
    return qMakePair(QString(), QString());
}
*/


KEduVocVocabularyDelegate::Private::WordTypeBasicModel::WordTypeBasicModel( QObject * parent )
    : KEduVocReadonlyContainerModel( KEduVocContainer::WordType, parent )
{
}

KEduVocContainer * KEduVocVocabularyDelegate::Private::WordTypeBasicModel::rootContainer() const
{
    if( !document() ) {
        return 0;
    }
    return document()->wordTypeContainer();
}

/**
 * Sets the member variable m_translator to a KEduVocTranslator object
 * @param translator KEduVocTranslator Object to be used for retreiving word translations
 */
void KEduVocVocabularyDelegate::setTranslator( KEduVocTranslator* translator )
{
    d->m_translator = translator;
}