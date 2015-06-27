/***************************************************************************

    Copyright 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "keduvoctranslator.h"

#include <QDebug>
#include <QMessageBox>

#include <keduvocvocabularymodel.h>
#include <keduvocdocument.h>
#include <keduvoclesson.h>
#include <keduvocexpression.h>
#include <keduvoctranslation.h>

namespace Scripting
{
    KEduVocDocument::KEduVocDocument( ::KEduVocDocument * doc, KEduVocVocabularyModel * vocabularyModel, QString separator ) 
        : QObject(), 
        m_translator( new KEduVocTranslator( this ) ),
        m_doc( new ::KEduVocDocument( doc ) ),
        m_vocabularyModel( vocabularyModel )
    {
        m_separator = separator;
    }

    KEduVocDocument::~KEduVocDocument()
    {
        delete m_translator;
        delete m_doc;
        delete m_vocabularyModel;
    }

    void KEduVocDocument::callTranslateWord(const QString & word, const QString& fromLanguage, const QString& toLanguage)
    {
        emit translationStarted(word, fromLanguage, toLanguage);
        emit translateWord(word, fromLanguage, toLanguage);
        emit translationFinished(word, fromLanguage, toLanguage);
    }

    void KEduVocDocument::addTranslation(QString word, QString fromLanguage, QString toLanguage, QString translation)
    {
        if (m_translator)
            m_translator->addTranslation(word, fromLanguage, toLanguage, translation);
    }

    QStringList KEduVocDocument::locales()
    {
        /// @todo Change it into a QMap property (Parley.languageCodes)
        return QLocale().uiLanguages();
    }

    QString KEduVocDocument::localeName(QString locale)
    {
        return QLocale( locale ).nativeLanguageName( );
    }

    bool KEduVocDocument::open(QString filename)
    {
        QUrl url( QUrl::fromLocalFile(filename) );
        qDebug() << url;
        if (url.path().isEmpty()) {
            return false;
        }

        close();

        if (m_doc) {
            m_doc->deleteLater();
        }
        m_doc = new ::KEduVocDocument(this);
        emit documentChanged(m_doc);
        m_doc->setCsvDelimiter(m_separator);

        bool isSuccess = false, isError = false;

        ::KEduVocDocument::ErrorCode ret = m_doc->open(url,  ::KEduVocDocument::FileDefaultHandling);
        switch ( ret ) {
            case ::KEduVocDocument::NoError :
                isSuccess = true;
                break;
            case ::KEduVocDocument::FileLocked :
            {
                QMessageBox warningBox;
                warningBox.setIcon( QMessageBox::Warning );
                warningBox.setText( "Take Over Lock" );
                warningBox.setInformativeText( "The vocabulary collection is locked by another process.  You can open the file if you take over the lock, but you will lose any changes from the other process.\n\nDo you want to take over the lock?\n" );
                warningBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
                int exit = warningBox.exec();

                if ( exit == QMessageBox::Yes ) { //attempt to steal lock

                    ret = m_doc->open(url, ::KEduVocDocument::FileIgnoreLock);
                    if ( ret == ::KEduVocDocument::NoError ) {
                        qDebug() << "Lock stolen";
                        isSuccess = true;
                    } else {
                        isError = true;
                    }
                } else { //Don't Steal continue work without saving !!!
                }

                break;
            }
            default:
                isError = true;
        }

        if ( isSuccess ) {
            qDebug() << "Open success.";
            //m_parleyApp->editor()->updateDocument();

        } else {
            if ( isError ) {
                QMessageBox errorBox;
                errorBox.setIcon( QMessageBox::Critical );
                errorBox.setText( "Open Collection" );
                errorBox.setInformativeText( "Opening collection " + m_doc->url().url() + " resulted in an error: " + m_doc->errorDescription( ret ) );
                errorBox.setStandardButtons( QMessageBox::Ok );
                errorBox.exec();
            }
            m_doc->deleteLater();
            emit documentChanged(0);
            m_doc = 0;
        }
        return isSuccess;
    }

    void KEduVocDocument::close()
    {
        if (m_doc) {
            emit documentChanged(0);
            m_doc->deleteLater();
            m_doc = 0;
        }
    }

    //FIXME Fix errors related to conversion of KEduVocLesson* to QObject*
    /*KEduVocLesson* KEduVocDocument::activeLesson()
    {
        return m_vocabularyModel->lesson();
    }*/

    QVariantList KEduVocDocument::selectedEntries( QModelIndexList indexes )
    {
        QVariantList entries;

        //get selected indexes and active lesson

        //get the unique selected entries
        QSet<KEduVocExpression*> kentries;
        foreach(const QModelIndex & index, indexes) {
            //             qDebug() << index.row() << index.data(Qt::DisplayRole);
            KEduVocExpression * expr = qvariant_cast<KEduVocExpression*> (index.data(KEduVocVocabularyModel::EntryRole));
            kentries << expr;
        }

        //convert them to Expression objects and add them to the QVariantList
        foreach(KEduVocExpression * expr, kentries) {
            //             Expression entry(expr);
            //             qDebug() << entry.translationTexts();
            KEduVocExpression * obj = new KEduVocExpression( *expr );
            entries << qVariantFromValue( obj );
        }

        return entries;
    }

    QVariantList KEduVocDocument::selectedTranslations( QModelIndexList indexes )
    {
        QVariantList translations;

        //get selected indexes and active lesson

        //get the unique selected entries
        QSet<KEduVocTranslation*> ktranslations;
        //         const QModelIndex &index;
        foreach(const QModelIndex & index, indexes) {
            if (KEduVocVocabularyModel::columnType(index.column()) == KEduVocVocabularyModel::Translation) {
                KEduVocExpression * expr = qvariant_cast<KEduVocExpression*> (index.data(KEduVocVocabularyModel::EntryRole));
                ktranslations << expr->translation(KEduVocVocabularyModel::translation(index.column()));
            }
            //             qDebug() << index.row() << index.data(Qt::DisplayRole);
        }

        //convert them to Expression objects and add them to the QVariantList
        foreach(KEduVocTranslation * tr, ktranslations) {
            //             KEduVocTranslation transltion(tr);
            //             qDebug() << entry.translationTexts();
            KEduVocTranslation * obj = new KEduVocTranslation( *tr );
            translations << qVariantFromValue(obj);
        }

        return translations;
    }

}





class KEduVocTranslator::Private
{
public:
    Private( QObject * parent );

    QMap<QString, QSet<QString>*> m_translations;
    QObject * m_parent;
};

KEduVocTranslator::Private::Private( QObject * parent = 0 )
    : m_parent( parent )
{
}

KEduVocTranslator::KEduVocTranslator()
    : d( new Private )
{
}

KEduVocTranslator::KEduVocTranslator(QObject * parent)
    : d( new Private( parent ) )
{
}

KEduVocTranslator::~KEduVocTranslator()
{
    delete d;
}

void KEduVocTranslator::addTranslation(QString word, QString fromLanguage, QString toLanguage, QString translation)
{
    if (word.trimmed() == "") return;

    QString t = word + fromLanguage + toLanguage;
    qDebug() << "Translation for " << word << "in cache: " << d->m_translations.contains(t);
    if (!d->m_translations.contains(t)) {
        d->m_translations[t] = new QSet<QString>();
    }
    d->m_translations[t]->insert(translation.simplified());
}

QSet<QString>* KEduVocTranslator::getTranslation(QString word, QString fromLanguage, QString toLanguage)
{
    if (word.isEmpty() || fromLanguage.isEmpty() || toLanguage.isEmpty()) return 0;

    QString t = word + fromLanguage + toLanguage;
    qDebug() << "Fetch translation " << word << "(" << fromLanguage << "to" << toLanguage << ")"
             << "already in cache:" << d->m_translations.contains(t);
    /*if (!d->m_translations.contains(t)) {
        //FIXME Remove Parley dependancy and reimplement the commented code below
        Scripting::KEduVocDocument * p = dynamic_cast<Scripting::KEduVocDocument*>(d->m_parent);
        if (p) {
            p->callTranslateWord(word, fromLanguage, toLanguage);
        }
    }*/

    if (d->m_translations.contains(t))
        return d->m_translations.value(t);
    else
        return 0;
}
