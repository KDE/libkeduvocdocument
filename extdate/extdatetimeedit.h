/*****  --==>> Commenting out entire class.  
See extdatetimeedit.cpp for details.
******/

#if 0
--- COMMENTING OUT ENTIRE CLASS ---

/****************************************************************************
**
** Copyright (C) 1992-2005 Trolltech AS. All rights reserved.
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef EXTDATETIMEEDIT_H
#define EXTDATETIMEEDIT_H

#include "extdatetime.h"
#include <QtGui/qabstractspinbox.h>

//JH: Comnmented out
//QT_MODULE(Gui)

#ifndef QT_NO_DATETIMEEDIT

class ExtDateTimeEditPrivate;
class ExtDateTimeEdit : public QAbstractSpinBox
{
    Q_OBJECT

/* JH: commenting out designer macros
    Q_ENUMS(Section)
    Q_FLAGS(Sections)
    Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime NOTIFY dateTimeChanged)
    Q_PROPERTY(QDate date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(QTime time READ time WRITE setTime NOTIFY timeChangedu)
    Q_PROPERTY(QDate maximumDate READ maximumDate WRITE setMaximumDate RESET clearMaximumDate)
    Q_PROPERTY(QDate minimumDate READ minimumDate WRITE setMinimumDate RESET clearMinimumDate)
    Q_PROPERTY(QTime maximumTime READ maximumTime WRITE setMaximumTime RESET clearMaximumTime)
    Q_PROPERTY(QTime minimumTime READ minimumTime WRITE setMinimumTime RESET clearMinimumTime)
    Q_PROPERTY(Section currentSection READ currentSection WRITE setCurrentSection)
    Q_PROPERTY(Sections displayedSections READ displayedSections)
    Q_PROPERTY(QString displayFormat READ displayFormat WRITE setDisplayFormat)
*/

public:
    enum Section {
        NoSection = 0x0000,
        AmPmSection = 0x0001,
        MSecSection = 0x0002,
        SecondSection = 0x0004,
        MinuteSection = 0x0008,
        HourSection   = 0x0010,
        DaySection    = 0x0100,
        MonthSection  = 0x0200,
        YearSection   = 0x0400,
        TimeSections_Mask = AmPmSection|MSecSection|SecondSection|MinuteSection|HourSection,
        DateSections_Mask = DaySection|MonthSection|YearSection
    };

    Q_DECLARE_FLAGS(Sections, Section)

    explicit ExtDateTimeEdit(QWidget *parent = 0);
    explicit ExtDateTimeEdit(const ExtDateTime &dt, QWidget *parent = 0);
    explicit ExtDateTimeEdit(const ExtDate &d, QWidget *parent = 0);
    explicit ExtDateTimeEdit(const QTime &t, QWidget *parent = 0);

    ExtDateTime dateTime() const;
    ExtDate date() const;
    QTime time() const;

    ExtDate minimumDate() const;
    void setMinimumDate(const ExtDate &min);
    void clearMinimumDate();

    ExtDate maximumDate() const;
    void setMaximumDate(const ExtDate &max);
    void clearMaximumDate();

    void setDateRange(const ExtDate &min, const ExtDate &max);

    QTime minimumTime() const;
    void setMinimumTime(const QTime &min);
    void clearMinimumTime();

    QTime maximumTime() const;
    void setMaximumTime(const QTime &max);
    void clearMaximumTime();

    void setTimeRange(const QTime &min, const QTime &max);

    Sections displayedSections() const;
    Section currentSection() const;
    void setCurrentSection(Section section);

    QString sectionText(Section s) const;

    QString displayFormat() const;
    void setDisplayFormat(const QString &format);

    QSize sizeHint() const;

    virtual void clear();
    virtual void stepBy(int steps);

    bool event(QEvent *e);
signals:
    void dateTimeChanged(const ExtDateTime &date);
    void timeChanged(const QTime &date);
    void dateChanged(const ExtDate &date);

public slots:
    void setDateTime(const ExtDateTime &dateTime);
    void setDate(const ExtDate &date);
    void setTime(const QTime &time);

protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void wheelEvent(QWheelEvent *e);
    virtual void focusInEvent(QFocusEvent *e);
    virtual bool focusNextPrevChild(bool next);
    virtual QValidator::State validate(QString &input, int &pos) const;
    virtual ExtDateTime dateTimeFromText(const QString &text) const;
    virtual QString textFromDateTime(const ExtDateTime &dt) const;
    virtual StepEnabled stepEnabled() const;

private:
    Q_DECLARE_PRIVATE(ExtDateTimeEdit)
    Q_DISABLE_COPY(ExtDateTimeEdit)
};

//JH: Don't need to re-do QTimeEdit
//class Q_GUI_EXPORT QTimeEdit : public ExtDateTimeEdit
//{
//    Q_OBJECT
//public:
//    QTimeEdit(QWidget *parent = 0);
//    QTimeEdit(const QTime &t, QWidget *parent = 0);
//};

class ExtDateEdit : public ExtDateTimeEdit
{
    Q_OBJECT
public:
    ExtDateEdit(QWidget *parent = 0);
    ExtDateEdit(const ExtDate &t, QWidget *parent = 0);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ExtDateTimeEdit::Sections)


#endif // QT_NO_DATETIMEEDIT
#endif // EXTDATETIMEEDIT_H

#endif //0
