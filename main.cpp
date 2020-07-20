#ifdef SssS_USE_GUI
#include <QApplication>
#else
#include <QCoreApplication>
#endif

#include <QTimer>

#include "AppController.h"

int main(int iCountArgs, char *aArguments[]) {

#ifdef SssS_USE_GUI
	QApplication oApp(iCountArgs, aArguments);
#else
	QCoreApplication oApp(iCountArgs, aArguments);
#endif

	QCoreApplication::setOrganizationName("SwissalpS");
	QCoreApplication::setOrganizationDomain("biz.SwissalpS.QtSssSircBot");
	QCoreApplication::setApplicationName(SssS_APP_NAME);
	QCoreApplication::setApplicationVersion(SssS_APP_VERSION_DOTTED);

	SwissalpS::QtSssSircBot::AppController *pAC
			= SwissalpS::QtSssSircBot::AppController::pAppController();

	// This will cause the application to exit when
	// AppController signals done.
	QObject::connect(pAC, SIGNAL(done()), &oApp, SLOT(quit()));

	// This will run the app-controller from the application event loop.
	// This guarantees that all classes are prepared and instances created.
	QTimer::singleShot(0, pAC, SLOT(run()));

	return oApp.exec();

} // main
