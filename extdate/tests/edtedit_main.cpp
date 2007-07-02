#include "edtedit_widget.h"
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>

static const char description[] = I18N_NOOP("ExtDateTimeEdit test program");
static const char notice[] = I18N_NOOP("Displays an ExtDateTimeEdit widget to test");

int main( int argc, char *argv[] )
{
	KAboutData aboutData( "test_extdatetimeedit", 0, ki18n("Test ExtDateTimeEdit"),
				"0.1", ki18n(description), KAboutData::License_GPL,
				ki18n("(c) 2005, Jason Harris"), ki18n(notice),
				"http://edu.kde.org/kstars");
	aboutData.addAuthor(ki18n("Jason Harris"), KLocalizedString(),
				"kstars@30doradus.org", "http://edu.kde.org/kstars");

	KCmdLineArgs::init( argc, argv, &aboutData );

	KApplication a;
	EDTEdit *edte = new EDTEdit(0);
	edte->show();
	QObject::connect(kapp, SIGNAL(lastWindowClosed()), kapp, SLOT(quit()));
	return a.exec();
}
