/*  This file is part of the KDE libraries
    Copyright (c) 2006 Jacob R Rideout <kde@jacobrideout.net>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef kautosavefiletest_h
#define kautosavefiletest_h

#include <QtCore/QObject>
#include <QtCore/QStringList>

class KAutoSaveFileTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    /** Remove all autosaveFiles for the qtest application */
    void initTestCase();
    /** Remove all autosaveFiles for the qtest application */
    void init();

    void test_readWrite();
    void test_fileStaleFiles();
    void test_applicationStaleFiles();

    /** Check that staleFiles only detects the same filename. */
    void test_crossContaminateFilename();

    /** Checks that staleFiles only detects the same appname. */
    void test_crossContaminateAppname();

    /** Checks that staleFiles are unlocked but not removed on releaseLock. */
    void test_freeStaleOnRelease();

    /** Checks that stateFiles are unlocked on setManaged, when appropriate. */
    void test_freeStaleOnSetManaged();

    /** Checks that file locks are unlocked but not removed when the autosave is destroyed. */
    void test_freeStaleOnDestroy();

    /** Checks that multiple autosaves on same file have different names and indepedent locks. */
    void test_multipleAutoSavesSameFile();

    /** Checks that unlocking and then removing the autosave file cleans up everything. */
    void test_requireExplicitRemoval();

    /** Checks that an abandoned lock can be recovered. */
    void test_stealAbandonedLock();

    void test_locking();

    /** Checks that the example code works. */
    void test_exampleCode();

    /** Check that autosave, kalock and lockfiles in inappropriate combinations are removed. */
    /* void test_housekeeping(); */


    /* Implementation specific tests*/
    /** Checks deletion of directory when last autosave is removed**/
    void test_deleteDirectory();

    void cleanupTestCase();

private:
    QString mainappname, appname1, appname2;    ///< names of the fake applications
    QStringList filesToRemove;
};

#endif
