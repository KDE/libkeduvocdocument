/*  This file is part of the KDE libraries
    Copyright (C) 2001 Waldo Bastian (bastian@kde.org)

    Modified to use ExtDate instead of QDate.  Modifications
    Copyright (C) 2004 Jason Harris (jharris@30doradus.org)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2 as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/


#include <QComboBox>
#include <QHBoxLayout>
#include <QSpinBox>

#include <kdialog.h>

#include "extdatewidget.h"
#include "extcalendarsystemgregorian.h"

class ExtDateWidgetSpinBox : public QSpinBox
{
public:
  ExtDateWidgetSpinBox(int min, int max, QWidget *parent)
    : QSpinBox(parent)
  {
     setMinimum(min);
     setMaximum(max);
     setSingleStep(1);
     setAlignment(Qt::AlignRight);
  }
};

class ExtDateWidget::ExtDateWidgetPrivate
{
public:
   ExtDateWidgetPrivate(ExtDateWidget *qq)
     : q( qq ), calendar( new ExtCalendarSystemGregorian() )
   {
   }

   ~ExtDateWidgetPrivate()
   {
     delete calendar;
   }

   // slots
   void dateChanged();

   ExtDateWidget *q;
   ExtDateWidgetSpinBox *m_day;
   QComboBox *m_month;
   ExtDateWidgetSpinBox *m_year;
   ExtDate m_dat;
   ExtCalendarSystemGregorian *calendar;
};


ExtDateWidget::ExtDateWidget( QWidget *parent )
  : QWidget( parent ), d(new ExtDateWidgetPrivate(this))
{
  init(ExtDate::currentDate());
  setDate(ExtDate());
}

ExtDateWidget::ExtDateWidget( const ExtDate &date, QWidget *parent )
  : QWidget( parent ), d(new ExtDateWidgetPrivate(this))
{
  init(date);
  setDate(date);
}

void ExtDateWidget::init(const ExtDate& date)
{
  //KLocale *locale = KGlobal::locale();
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(0);
  layout->setSpacing(KDialog::spacingHint());
  d->m_day = new ExtDateWidgetSpinBox(1, 1, this);
  layout->addWidget(d->m_day);
  d->m_month = new QComboBox(this);
  layout->addWidget(d->m_month);
  d->m_month->setEditable(false);
  for (int i = 1; ; ++i)
  {
    QString str = d->calendar->monthName(i,
       d->calendar->year(date));
    if (str.isNull()) break;
    d->m_month->addItem(str);
  }

  d->m_year = new ExtDateWidgetSpinBox(d->calendar->minValidYear(),
				     d->calendar->maxValidYear(), this);
  layout->addWidget(d->m_year);

  connect(d->m_day, SIGNAL(valueChanged(int)), this, SLOT(dateChanged()));
  connect(d->m_month, SIGNAL(activated(int)), this, SLOT(dateChanged()));
  connect(d->m_year, SIGNAL(valueChanged(int)), this, SLOT(dateChanged()));
}

ExtDateWidget::~ExtDateWidget()
{
  delete d;
}

void ExtDateWidget::setDate( const ExtDate &date )
{
//  const KCalendarSystem * calendar = KGlobal::locale()->calendar();

  d->m_day->blockSignals(true);
  d->m_month->blockSignals(true);
  d->m_year->blockSignals(true);

  d->m_day->setMaximum(d->calendar->daysInMonth(date));
  d->m_day->setValue(d->calendar->day(date));
  d->m_month->setCurrentIndex(d->calendar->month(date)-1);
  d->m_year->setValue(d->calendar->year(date));

  d->m_day->blockSignals(false);
  d->m_month->blockSignals(false);
  d->m_year->blockSignals(false);

  d->m_dat = date;
  emit changed(d->m_dat);
}

ExtDate ExtDateWidget::date() const
{
  return d->m_dat;
}

void ExtDateWidget::ExtDateWidgetPrivate::dateChanged()
{
//  const KCalendarSystem * calendar = KGlobal::locale()->calendar();

  ExtDate date;
  int y,m,day;

  y = m_year->value();
  y = qMin(qMax(y, calendar->minValidYear()), calendar->maxValidYear());

  calendar->setYMD(date, y, 1, 1);
  m = m_month->currentIndex()+1;
  m = qMin(qMax(m,1), calendar->monthsInYear(date));

  calendar->setYMD(date, y, m, 1);
  day = m_day->value();
  day = qMin(qMax(day,1), calendar->daysInMonth(date));

  calendar->setYMD(date, y, m, day);
  q->setDate(date);
}

#include "extdatewidget.moc"
