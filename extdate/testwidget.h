#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <kmainwindow.h>

class KDatePicker;
class KLineEdit;
class KDateWidget;
class ExtDatePicker;
class ExtDateWidget;
class QGridLayout;
class QDate;
class ExtDate;

class TestWidget : public KMainWindow {
	Q_OBJECT
	public:
		TestWidget( QWidget *parent, const char *name );
		~TestWidget() {}

	public slots:
		void slotKDateChanged(QDate);
		void slotExtDateChanged(const ExtDate&);

	private:
		QGridLayout *glay;
		KDatePicker *kdp;
		ExtDatePicker *edp;
		ExtDateWidget *edw;
		KDateWidget *kdw;
		KLineEdit *kdpEdit, *edpEdit;
};

#endif //ifndef TESTWIDGET_H
