/***************************************************************************

                      options to enable/disable columns for the languages

    -----------------------------------------------------------------------
    copyright     :(C) 2008 Javier goday <jgoday@gmail.com>
    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCVOCABULARYCOLUMNSDIALOG_H
#define KEDUVOCVOCABULARYCOLUMNSDIALOG_H

#include <QMap>
#include <QDialog>
#include <QGridLayout>

#include <keduvocdocument_export.h>
#include <keduvocdocument.h>

class QGridLayout;
class QStandardItem;
class QStandardItemModel;
class QVBoxLayout;
class KEduVocDocument;
class KEduVocVocabularyModel;

namespace Editor
{

/**
* Show options to enable/disable the columns for each language of the current document
*/
class KEDUVOCDOCUMENT_EXPORT KEduVocVocabularyColumnsDialog : public QDialog
{
    Q_OBJECT
public:
    enum Roles {
        ColumnRole = 0x12,
        LanguageRole = 0x18
    };

    /**
     * @param visibleColumns Represents DocumentSettings::visibleColumns()
     * Example usage: Use doc->url().url() for this parameter, where doc is the KEduVocDocument object passed to this function as @param doc
     * Save the settings using DocumentSettings::save()
     */
    KEduVocVocabularyColumnsDialog(KEduVocDocument *doc, QList <int> visibleColumns, QWidget *parent);
    ~KEduVocVocabularyColumnsDialog();

    void accept();

    /**
     * Initialize the gridlayout of the widget
     * with one treeview for each language showing the possible columns
     */
    void createLanguagesLayout();

    /**
     * Adds a language to the main gridlayout
     */
    void addLanguage(int language, QVBoxLayout *parent);

    /**
     * Save the visible columns for each language in the document settings
     */
    void saveVisibleColumns();

    /**
     * Set the Grid Layout
     */
    void setBox( QGridLayout * box );

    /**
     * Return the current Grid Layout
     */
    QGridLayout* box();

    /**
     * Set the KEduVocDocument
     */
    void setDoc( KEduVocDocument * doc );

    /**
     * Return the current KEduVocDocument
     */
    KEduVocDocument* doc();

    /**
     * Set the list of ItemModels
     * @param visibleColumns Represents DocumentSettings::visibleColumns()
     * Example usage: Use doc->url().url() for this parameter, where doc is the KEduVocDocument object passed to this function as @param doc
     * Save the settings using DocumentSettings::save()
     */
    void setModels( QMap <int, QStandardItemModel *> models );

    /**
     * Return the current list of ItemModels
     */
    QMap <int, QStandardItemModel *> models();

    /**
     * Set the visibleColumns list
     */
    void setVisibleColumns( QList <int> visibleColumns );

    /**
     * Return the current visibleColumns list
     */
    QList <int> visibleColumns();

private:
    class Private;
    Private * const d;
};
}

#endif
