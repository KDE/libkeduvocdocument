#include <kdatepicker.h>
#include <klineedit.h>
#include <qdatetime.h>
#include <qlayout.h>
#include <qlabel.h>

#include "extdatepicker.h"
#include "extdatetime.h"
#include "testwidget.h"

TestWidget::TestWidget( QWidget *p=0, const char *name=0 ) : KMainWindow( p, name ) {
	QWidget *w = new QWidget(this);

	glay = new QGridLayout(w, 3, 2);

	QLabel *kdpLabel = new QLabel( QString("KDatePicker"), w );
	QLabel *edpLabel = new QLabel( QString("ExtDatePicker"), w );
	kdp = new KDatePicker(w);
	edp = new ExtDatePicker(w);
	kdpEdit = new KLineEdit(w);
	kdpEdit->setReadOnly( TRUE );
	edpEdit = new KLineEdit(w);
	edpEdit->setReadOnly( TRUE );

	glay->addWidget( kdpLabel, 0, 0 );
	glay->addWidget( edpLabel, 0, 1 );
	glay->addWidget( kdp, 1, 0 );
	glay->addWidget( edp, 1, 1 );
	glay->addWidget( kdpEdit, 2, 0 );
	glay->addWidget( edpEdit, 2, 1 );

	setCentralWidget(w);

	connect( kdp, SIGNAL( dateChanged(QDate) ), this, SLOT( slotKDateChanged(QDate) ) );
	connect( edp, SIGNAL( dateChanged(const ExtDate&) ), this, SLOT( slotExtDateChanged(const ExtDate&) ) );
}

void TestWidget::slotKDateChanged(QDate d) {
	kdpEdit->setText( d.toString() );
}

void TestWidget::slotExtDateChanged(const ExtDate &d) {
	edpEdit->setText( d.toString() );
}

#include "testwidget.moc"
