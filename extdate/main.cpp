#include "testwidget.h"
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>

static const char description[] = I18N_NOOP("ExtDatePicker test program");
static const char notice[] = I18N_NOOP("Compares KDatePicker and ExtDatePicker");

static KCmdLineOptions options[] =
{
	KCmdLineLastOption
};

int main( int argc, char *argv[] )
{
	KAboutData aboutData( "test_extdatepicker", I18N_NOOP("Test ExtDatePicker"),
				"0.1", description, KAboutData::License_GPL,
				I18N_NOOP("(c) 2004, Jason Harris"), notice,
				"http://30doradus.org");
	aboutData.addAuthor("Jason Harris", 0,
				"jharris@30doradus.org", "http://www.30doradus.org");

	KCmdLineArgs::init( argc, argv, &aboutData );

	KApplication a;
	TestWidget *t = new TestWidget(0,0);
	t->show();
	QObject::connect(kapp, SIGNAL(lastWindowClosed()), kapp, SLOT(quit()));
	return a.exec();
}
