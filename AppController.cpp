#include "AppController.h"
#include "AppSettings.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QHostInfo>
#include <QJsonArray>
#include <QMutex>
#include <QProcess>
#include <QStandardPaths>
#include <QTimer>
#include <iostream>



namespace SwissalpS { namespace QtSssSircBot {


AppController *AppController::pSingelton = 0;

AppController::AppController(QObject *pParent) :
	QObject(pParent),
	pClient(0) {

} // construct


AppController::~AppController() {

} // dealloc


void AppController::drop() {

	static QMutex oMutex;

	oMutex.lock();

	delete pSingelton;
	pSingelton = 0;

	oMutex.unlock();

} // drop


AppController *AppController::pAppController() {

	///
	/// \brief oMutex
	///
	static QMutex oMutex;

	///////////////////////////////
	/* NOTE from: http://www.qtcentre.org/wiki/index.php?title=Singleton_pattern
	 * Be aware that you should check for a std::bad_alloc exception
	 * to unlock the mutex in case new fails. We are also using
	 * double-checked locking as locking and unlocking a mutex
	 * is expensive and would significantly slow down a call to
	 *  Singleton::instance().
	 */
	if (!AppController::pSingelton) {

		oMutex.lock();

		if (!pSingelton) {

			pSingelton = new AppController();

		} // if first call

		oMutex.unlock();

	} // if first call

	return pSingelton;

} // singelton access


void AppController::connectErrorMessages() {

	//AppSettings *pAS = AppSettings::pAppSettings();

} // connectErrorMessages


void AppController::debugMessage(const QString &sMessage) {

	std::cout << sMessage.toStdString();
	std::cout << std::endl;

	Q_EMIT debugMessageReceived(sMessage);

} // debugMessage


void AppController::initSettings() {

	this->pAS = AppSettings::pAppSettings();

	connect(pAS, SIGNAL(debugMessage(QString)),
			this, SLOT(debugMessage(QString)));

	this->pAS->init();

} // initSettings


void AppController::onIRCclientLoggedIn(const QString &sNick) {
	Q_UNUSED(sNick)

	const QStringList aChannels = this->pAS->getChannels();
	for (int i = 0; i < aChannels.count(); ++i) {

		this->pClient->sendJoin(aChannels.at(i));

	} // loop

} // onIRCclientLoggedIn


void AppController::onIRCclientQuit(const qint16 &iR) {
	Q_UNUSED(iR)

	this->quit();

} // onIRCclientQuit


void AppController::quit() {

	AppSettings::pAppSettings()->getSettings()->sync();

	if (this->pClient) {
		this->disconnect(this->pClient);
		this->pClient->sendQuit();
		this->pClient->disconnect();
	}

	qApp->quit();

} // quit


void AppController::run() {

	if (this->pClient) return;

	std::cout << "run" << std::endl;

	// init settings
	this->initSettings();

	this->writePID();

	// init 'error' messaging system to route all through app controller
	this->connectErrorMessages();

	this->pClient = new IRCclient(this->pAS->get(AppSettings::sSettingIRCremoteHost).toString(),
								  this->pAS->get(AppSettings::sSettingIRCremoteNick).toString(),
								  this->pAS->get(AppSettings::sSettingIRCremotePort).toUInt(),
								  this->pAS->get(AppSettings::sSettingIRCremoteRealname).toString(),
								  this->pAS->get(AppSettings::sSettingIRCremotePassword).toString(),
								  this);

	connect(this->pClient, SIGNAL(debugMessage(QString)),
			this, SLOT(debugMessage(QString)));

	connect(this->pClient, SIGNAL(loggedIn(QString)),
			this, SLOT(onIRCclientLoggedIn(QString)));

	connect(this->pClient, SIGNAL(quit(qint16)),
			this, SLOT(onIRCclientQuit(qint16)));

	this->pClient->connectEncrypted();

} // run


void AppController::writePID() {

	QString sPathPID = AppSettings::pAppSettings()->get(AppSettings::sSettingPIDpathFile).toString();
	QString sPID = QString::number(QCoreApplication::applicationPid());

	QFile oPID(sPathPID);
	if (!oPID.open(QIODevice::WriteOnly)) {

		std::cout << (tr("AppController::KO:Cannot write to pid-file: ")
					  + sPathPID).toStdString();
		std::cout << std::endl;

		return;

	} // if failed to open pid-file

	oPID.write(sPID.toUtf8());
	oPID.close();

} // writePID



}	} // namespace SwissalpS::QtSssSircBot

