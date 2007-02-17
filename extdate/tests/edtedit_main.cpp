#include "edtedit_widget.h"
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>

static const char description[] = I18N_NOOP("ExtDateTimeEdit test program");
static const char notice[] = I18N_NOOP("Displays an ExtDateTimeEdit widget to test");

int main( int argc, char *argv[] )
{
	KAboutData aboutData( "test_extdatetimeedit", I18N_NOOP("Test ExtDateTimeEdit"),
				"0.1", description, KAboutData::License_GPL,
				I18N_NOOP("(c) 2005, Jason Harris"), notice,
				"http://edu.kde.org/kstars");
	aboutData.addAuthor("Jason Harris", 0,
				"kstars@30doradus.org", "http://edu.kde.org/kstars");

	KCmdLineArgs::init( argc, argv, &aboutData );

	KApplication a;
	EDTEdit *edte = new EDTEdit(0);
	edte->show();
	QObject::connect(kapp, SIGNAL(lastWindowClosed()), kapp, SLOT(quit()));
	return a.exec();
}
