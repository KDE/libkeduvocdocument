/***************************************************************************

    C++ Interface: keduvocwordtype

    -----------------------------------------------------------------------

    begin         : Mi Aug 22 2007

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#ifndef KEDUVOCWORDTYPE_H
#define KEDUVOCWORDTYPE_H

#include "libkeduvocdocument_export.h"

#include <QtCore/QStringList>
#include <QtCore/QMap>

/**
    Word type handling including subtypes (noun - male/female) etc.
    Special types: To let KVocTrain decide which word type is a verb for example the
    special tag is used.
 @author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class KEDUVOCDOCUMENT_EXPORT KEduVocWordType
{

public:
    /** default constructor */
    explicit KEduVocWordType();

    /** copy constructor for d-pointer safe copying */
    KEduVocWordType( const KEduVocWordType& other );

    /** destructor */
    ~KEduVocWordType();

    /** assignment operator */
    KEduVocWordType& operator= ( const KEduVocWordType& other );

    void createDefaultWordTypes();

    /**
     * Create a new word type in the list of known types
     * @param typeName Name of the word type
     * @param specialType Name of the special type - this is used internally to identify which types are use for special queries - verb query needs special == "verb" for example.
     * @param specialTypeExplanation An explanation which can be shown to the user. Since the type can be renamed if it's special, but not deleted this is necessary.
     */
    void addType( const QString& typeName, const QString& specialType = QString(), const QString& specialTypeExplanation = QString() );


    /**
     * Same as addType but for a sub word type (male/female/nutral for noun for example)
     * @param mainType The word type to which the subtype belongs.
     * @param typeName Sub type name
     * @param specialType See above
     * @param specialTypeExplanation See above
     */
    void addSubType( const QString& mainType, const QString& typeName, const QString& specialType = QString(), const QString& specialTypeExplanation = QString() );

    /**
     * Get a list of all known main word types.
     * This can be for example: noun, verb, adjective...
     * @return List of type names
     */
    QStringList typeNameList() const;

    /**
     * Same as typeNameList for subtypes.
     * Could be male, female, nutral for nouns.
     * @param mainType The type whos subtypes are requested.
     * @return The subtypes.
     */
    QStringList subTypeNameList( const QString& mainType ) const;

    /**
     * Rename a type.
     * @param oldTypeName Old name
     * @param newTypeName New name
     */
    void renameType( const QString& oldTypeName, const QString& newTypeName );
    /**
     * Rename a subtype.
     * @param mainTypeName Main type
     * @param oldTypeName Old name
     * @param newTypeName New name
     */
    void renameSubType( const QString& mainTypeName, const QString& oldTypeName, const QString& newTypeName );

    /**
     * Delete a type.
     * Special types cannot be deleted.
     * @param typeName name
     * @return true if it was possible to delete the type
     */
    bool removeType( const QString& typeName );
    /**
     * removeType for subtypes.
     * @param mainTypeName main type
     * @param typeName type name
     * @return true if it was possible to delete the type
     */
    bool removeSubType( const QString& mainTypeName, const QString& typeName );

    /**
     * Get the special type, if any.
     * @param typeName Name whos special type is requested
     * @return the special type or an empty string.
     */
    QString specialType( const QString& typeName );
    /**
     * Same as above for a subtype
     * @param typeName Main type name
     * @param typeName Sub type name
     * @return the special type or an empty string.
     */
    QString specialSubType( const QString& mainTypeName, const QString& subTypeName );

    void printDebugWordTypes();

    KDE_DEPRECATED QString typeOfSpecialType( const QString& specialType ) const;

    QString specialTypeNoun() const;
    QString specialTypeNounMale() const;
    QString specialTypeNounFemale() const;
    QString specialTypeNounNeutral() const;

    QString specialTypeVerb() const;
    QString specialTypeAdjective() const;
    QString specialTypeAdverb() const;

private:


    QString mainTypeName( int index ) const;
    int mainTypeIndex( const QString& name ) const;
    int subTypeIndex( const QString& mainTypeName, const QString& subTypeName ) const;

    class Private;
    Private * const d;
};

#endif
